/***********************************************************************************
* Se usa salida serie para debug
* se puede comentar si no se usa
************************************************************************************/
// Comente esta linea en un entorno de producción
#define DEVMODE

// Ponga aquí su indicativo de radioaficionado
#define MYCALL "XXXXXX"

// Potencia teórica que entrega Si5351 en dBm
#define DBMPOWER 10

// LED_PIN  0  se no quiere usar el LED para el GPS (es util)
#define LED_PIN 25       

// My SI5351 Comprado en Aliexpress funciona a 25 MHz así es que pongo 0
#define SI5351FREQ 0


/***********************************************************************************
* Todos los chips  Si5351 preceisan calibrar la frecuencia y para ello hay que calcular el valor de ajuste:
* - mediante el progrma de calibración de ejemplo
* - mediante un frecuencimetro
* - mediante un receptor 
*
* Se calibre con centesimas de Hz, se pone 0 pues se prefiere hacer el ajuste en cada banda en el array
*
************************************************************************************/
// #define SI5351_CORRECTION 116000

#define SI5351_CORRECTION 0

/***********************************************************************************
Frecuencias de centro real de las bandas (BW=200 Hz) asignada a WSPR en HF 
WSPR	 
160m 1838100
80m 3570100	 
60m 5288700	
40m 7040100	
30m 10140200	
20m 14097100	
18m 18106100	
15m 21096100	
12m 24926100	
10m 28126100	
*/
/* INTERNATIONAL WSPR BEACON PROJECT schedul https://github.com/HB9VQQ/WSPRBeacon
     1838100ULL,   // 160m 0,20,40
     3570100ULL,   // 80m 2,22,42
     5288700ULL,   // 60m 4,24,44
     7040100ULL,   // 40m 6,26,46
    10140200ULL,   // 30m 8,28,48
    14097100ULL,   // 20m 10,30,50
    18106100ULL,   // 18m 12,32,52
    21096100ULL,   // 15m 14,34,54
    24926100ULL,   // 12m 16,36,56 
    28126100ULL    //  10m 18,38,58 
*/
// Frecuencia en  Hz con - correcccion si #define SI5351_CORRECTION 0
// 0ULL,        // minute   => no transmission 
// puede aprovechan los slots vacios para repetir bandas      
static const unsigned long long freqArray[10] = {
  0ULL,   // RPT 0,20,40
  0ULL,  // RPT 2,22,42
  0ULL,  // RPT 4,24,44
  7039280ULL,   //  ok 40m 6,26,46
  10139000ULL,  //  ok 30m 8,28,48
  14095400ULL,  //  ok 20m 10,30,50
  18103940ULL,  //  ok 18m 12,32,52 
  21093600ULL,  //  ok 15m 14,34,54 
  24923180ULL,  //  ok 12m 16,36,56 
  28122750ULL,  //  ok 10m 18,38,58 
};

// Calcula automáticamente el tamaño del array
  const uint8_t NUM_FREQS = sizeof(freqArray) / sizeof(freqArray[0]);

/***********************************************************************************
* GPS SETTINGS
*  
* Change if needed
************************************************************************************/
//Para Lilygo  T-Beam V1.2 la velocidad es 9600 y los pines 12-TX 34-RX  
static const int Rx = 34, Tx = 12; 
static const uint32_t GPSBaud = 9600;  
                         
