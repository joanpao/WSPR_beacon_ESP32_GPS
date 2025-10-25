/*
Programa original de WSPR Beacon by Roel Kroes
para  microcontrolador AVR (como el ATmega328P),disponible en
https://github.com/RoelKroes/wsprbeacon/blob/main/WSPR_beacon_arduino.ino
Modificado por EA5JTT Juanpa 
V1 20250902
- Adaptacion a Lilygo ESP32 LoRa GPS 433MHz (T-Beam), compilar en Arduino IDE para placa lilyGo T-Display
- Inclusion de reset para GPS por los cuelgues
- Inclusion de mensajes en pantalla OLED para poder controlarla sin PC 
- Inclusion de de la correccion el Si5351 especifico en la tabla de frecuencias y no uno generico para todas
V1 202510025
- Redondeo de longitud y latitud para evitar Locator erratico
_ incluir el LOCATOR por defecto para evitar Locator erratico
- Flexibilizar el tamaño de la tabla de frecuencias, puede ser cualquier numero de elementos


El Si5351 saca unos 8 dBm y genera una señal de calidad para las bandas de HF de 40, 30, 20, 18, 15, 12 y 10m
no se ha probado para VHF
Se quiere probar con un amplificador de potencia y filtro pasabajos

*/

#include <si5351.h>
#include <TinyGPS++.h>
#include <JTEncode.h>
#include "Wire.h"
#include "settings.h"
#include "esp32-hal-timer.h"
// para pantalla
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include "Fixed8x16.h"

// Definiciones WSPR
#define WSPR_TONE_SPACING 146
#define WSPR_DELAY        683
//#define SYMBOL_PERIOD_US  (WSPR_CTC * 64)  // Ej: 64 µs ticks
// PARA ESP32
#define SYMBOL_PERIOD_US  683000UL

// OLED pins for for ESP32 433 GPS T-BEAM V1.2
#define OLED_RST NOT_A_PIN  // IO16 -- RESET
#define OLED_SDA 21         // IO21 -- SDA for TTGO Lora32 board
#define OLED_SCL 22         // IO22 -- SCL
Adafruit_SSD1306 display(OLED_RST);

Si5351 si5351;
JTEncode jtencode;
TinyGPSPlus gps;
HardwareSerial GPSserial(1);

uint8_t tx_buffer[255];
uint8_t symbol_count = WSPR_SYMBOL_COUNT;
uint16_t tone_delay = WSPR_DELAY, tone_spacing = WSPR_TONE_SPACING;
unsigned long long freq = 0ULL;
volatile bool proceed = false;

hw_timer_t* timer = nullptr;

uint8_t currentFreqIndex = 0;  // Índice de frecuencia actual

// Datos GPS
// Luz parpadeante roja junto al chip del GPS
// Si no funciona el GPS no funciona el dispositivo
struct TGPS {
  bool sendMsg1 = false;
  char MH_1[5] = {'A','A','0','0',0};  // Locator Maidenhead para WSPR
  uint8_t dbm_1 = 23;                 // Potencia en dBm (ejemplo)

  // Campos necesarios para GPS
  int Hours = 0;
  int Minutes = 0;
  int Seconds = 0;
  int Satellites = 0;

  double Latitude = 0;
  double Longitude = 0;

  bool GPS_valid = false;
};

// Variable global
TGPS UGPS;

// Maidenhead como global también (si se accede desde múltiples funciones)
//char MaidenHead[7] = "AA00aa";
char MaidenHead[7] = "IM99UL";

// ISR del temporizador
void IRAM_ATTR onTimer() {
  proceed = true;
}
/*
SETUP
Inicializa comunicación serie con el GPS.
Envía un comando UBX para reinicio cold start del GPS (opcional, aunque útil si se cuelga).
Inicializa el Si5351 y configura el PLL, potencia de salida y corrección.
Configura el temporizador hardware con un periodo de SYMBOL_PERIOD_US = 683000 µs (~0,683 s), necesario para WSPR.
Inicializa la pantalla OLED y muestra mensajes de estado.
*/
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[WSPR Beacon] Inicializando...");

  GPSserial.begin(GPSBaud, SERIAL_8N1, Rx, Tx);
  Serial.println("[GPS] Puerto serie inicializado.");
  
  // A veces se cuelga el GPS por lo que de esta manera al reinicar el programa se resetea
  // Comando UBX para reinicio por software (Cold Start)
  //Un Cold Start (inicio en frío) fuerza al módulo GPS a borrar completamente su memoria interna, incluyendo:
  // - Posición anterior
  // - Datos de tiempo
  // - Información orbital (almanaque y efemérides)
  // - Satélites previamente visibles
  // Si mo tiene problemas de cuelgues comente hasta final de RESET GPS
   uint8_t resetCommand[] = {
    0xB5, 0x62, // UBX header
    0x06, 0x04, // CFG-RST
    0x04, 0x00, // Length
    0xFF, 0xFF, // navBbrMask = 0xFFFF → cold start
    0x00, 0x00, // resetMode = hardware reset
    0x0E, 0x64  // Checksum
    };
   for (int i = 0; i < sizeof(resetCommand); i++) {
    GPSserial.write(resetCommand[i]);  // Enviamos por Serial1 (GPS)
    }
   Serial.println("Comando de reset enviado al GPS.");
  // FIN RESET GPS
  if (!si5351.init(SI5351_CRYSTAL_LOAD_10PF, SI5351FREQ, 0)) {
    Serial.println("[ERROR] No se detectó el Si5351.");
    while (1);
  }
  Serial.println("[Si5351] Inicializado correctamente.");

  si5351.set_correction(SI5351_CORRECTION, SI5351_PLL_INPUT_XO);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  // Aqui configura la salida CLK1
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA);
  si5351.output_enable(SI5351_CLK1, 0);

  timer = timerBegin(1000000);  // 1 MHz (1 us)
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, SYMBOL_PERIOD_US, true, 0);
  timerStart(timer);
  Serial.println("[Timer] Configurado e iniciado.");

  // Initialize OLED display with the I2C address 0x3C.
  // OLED_SDA, OLED_SCL son las variables ya inicializadas con los pines usados para OLED, el 21 y 22 en este caso
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, true, OLED_SDA, OLED_SCL);
  display.clearDisplay();
  display.display();
  display.setFont(&Fixed8x16);
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  display.println("WSPR TX - EA5JTT");
  display.setCursor(0, 27);
  display.println("Esperando GPS ..");
  display.display();
  delay(2000);
  Serial.println("✅ OLED inicializada, mostrando mensaje...");
}
/*
LOOP
- Llama periódicamente a smartDelay(950) (probablemente en otra parte del código) para permitir que el GPS procese los datos.
- Llama a CheckGPS() de GPS.ino
- Si UGPS.sendMsg1 == true y freq está definida, inicia la transmisión WSPR.
*/
void loop() {
  smartDelay(950);
  CheckGPS();

  if (UGPS.sendMsg1) {
    freq = freqArray[currentFreqIndex];
    Serial.printf("[TX] Usando frecuencia #%d: %llu Hz\n", currentFreqIndex, freq);

    display.clearDisplay();
    display.setCursor(0, 12);
    display.println("WSPR TX - EA5JTT");
    display.setCursor(0, 27);
    display.print(F("Frec: "));
    display.println(freq);
    display.display();

    encode();

    // ✅ Avanzar al siguiente índice de frecuencia
    currentFreqIndex++;
    if (currentFreqIndex >= NUM_FREQS) currentFreqIndex = 0;  // volver al inicio

    UGPS.sendMsg1 = false;
  }
}
/*
ENCODE
- Llama a set_tx_buffer() para codificar el mensaje WSPR.
- Enciende el Si5351 (CLK1 habilitado). Modificar CLK1 por el CLK que se use 
- Recorre los símbolos codificados (tx_buffer) y los transmite cambiando la frecuencia del Si5351 cada ~0,683 s.
- Usa el flag proceed (actualizado por la ISR del temporizador onTimer) para mantener el timing exacto.
- Al terminar, apaga la salida del Si5351 y muestra “Fin TX” en el OLED.
*/
void encode() {
  set_tx_buffer();
  Serial.println("[TX] Iniciando transmisión WSPR...");
  display.setCursor(0,39);          
  display.print(F("Inicio TX"));
  display.display();
  si5351.output_enable(SI5351_CLK1, 1);
  for (uint8_t i = 0; i < symbol_count; i++) {
    unsigned long long tx_freq = (freq * 100ULL) + (tx_buffer[i] * tone_spacing);
    si5351.set_freq(tx_freq, SI5351_CLK1);
    Serial.print("[TX] Símbolo "); Serial.print(i); Serial.print(" → Frecuencia: "); printull(tx_freq);
    display.setCursor(i,51);          
    display.print(F("."));
    display.display();
    proceed = false;
    while (!proceed);
  }
  si5351.output_enable(SI5351_CLK1, 0);
  Serial.println("[TX] Transmisión finalizada.");
  display.setCursor(0,63);          
  display.print(F("Fin TX"));
  display.display();
}
/*
SET_TX_BUFFER
- Usa la librería JTEncode para generar los 162 símbolos de WSPR a partir de:
MYCALL → tu indicativo (debe estar definido en settings.h)
UGPS.MH_1 → el locator Maidenhead
UGPS.dbm_1 → potencia en dBm
*/
void set_tx_buffer() {
  memset(tx_buffer, 0, 255);
  if (UGPS.sendMsg1) {
    Serial.print("[ENCODE] Codificando mensaje: ");
    Serial.print(MYCALL); Serial.print(" ");
    Serial.print(UGPS.MH_1); Serial.print(" ");
    Serial.print(UGPS.dbm_1); Serial.println(" dBm");
    jtencode.wspr_encode(MYCALL, UGPS.MH_1, UGPS.dbm_1, tx_buffer);
  }
}
