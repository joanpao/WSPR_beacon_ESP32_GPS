/***********************************************************************************
* Use serial console Output for debugging and development
*  
* Change if needed
************************************************************************************/
// Comment out if you do not want Serial console output
// Comment out DEVMODE in a production environment as it will degrade performance!
#define DEVMODE

// Your own HAM call. Change it
// Ponga aquí su indicativo de radioaficionado
#define MYCALL "XXXXXX"

// The power of your transmission in dBm. 
// for the si5351 this should be set to 10 (=10 milliwatts).
// Potencia teórica que entrega Si5351 en dBm
#define DBMPOWER 10


// If you have a valid GPS lock, the software will make this HIGH twice for 20ms
// You could connect a LED to it.
// Set LED_PIN to 0 if you do not want to use a pin

#define LED_PIN 25       

// Frequency of the SI5351 Oscillator in Hertz
// for example #define SI5351FREQ 26000000 if you have a 26MHz XO
// Use 0 if you have a 25MHz Oscillator
// My SI5351 xomprado en Aliexpress funciona a 25 MHz así es que pongo 0

#define SI5351FREQ 0


/***********************************************************************************
* All Si5351 chips require frequency calibration, and to do so, the adjustment value must be calculated:
* - using the sample calibration program
* - using a frequency meter
* - using a receiver
*
* If calibrated with hundredths of a Hz, set to 0 because it is preferred to perform the adjustment on each band in the array.
*
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
* WSPR 
/* Actual center frequencies of the 200 Hz band assigned to WSPR
/* Frecuencias de centro real de las bandas (BW=200 Hz) asignada a WSPR en HF 
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
************************************************************************************/
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

/***********************************************************************************
* GPS SETTINGS
*  
* Change if needed
************************************************************************************/
//Para Lilygo  T-Beam V1.2 la velocidad es 9600 y los pines 12-TX 34-RX  
static const int Rx = 34, Tx = 12; 
static const uint32_t GPSBaud = 9600;  
                         
