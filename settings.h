/***********************************************************************************
* Use serial console Output for debugging and development
*  
* Change if needed
************************************************************************************/
// Comment out if you do not want Serial console output
// Comment out DEVMODE in a production environment as it will degrade performance!
#define DEVMODE

// Your own HAM call. Change it
// Modifico por mi indicativo
#define MYCALL "EA5JTT"

// The power of your transmission in dBm. 
// for the si5351 this should be set to 10 (=10 milliwatts).
// Es la potencia que entrega Si5351
#define DBMPOWER 8


// If you have a valid GPS lock, the software will make this HIGH twice for 20ms
// You could connect a LED to it.
// Set LED_PIN to 0 if you do not want to use a pin
// Pongo el 25 que es el que normalmente usamos 
#define LED_PIN 25       

// Frequency of the SI5351 Oscillator in Hertz
// for example #define SI5351FREQ 26000000 if you have a 26MHz XO
// Use 0 if you have a 25MHz Oscillator
// segun los datos de Aliexpress funciona a 25 MHz asíe s que pongo 0
#define SI5351FREQ 0


/***********************************************************************************
* Todos los chips  Si5351 preceisan calibrar la frecuencia y para ello hay que calcular el valor de ajuste:
* - mediante el progrma de calibraciónd e ejemplo
* - mediante un frecuencimetro
* - mediante un receptor 
*
* Se calibre con centesimas de Hz
* por algun motivo el ajuste de 2660 Hz para 18MHZ se convierte en 1160 ¿?
************************************************************************************/
// #define SI5351_CORRECTION 116000
#define SI5351_CORRECTION 0

/*
WSPR	    f SDR	    Ajuste (Hz)	S SDR (dB)
3570100	  3570600	  50000	  -50
5288700	  5289450	  75000	  -50
7040100	  7041130	  103000	-40
10140200	10141680	148000	-45
13555400	13557380	198000	-40
14097100	14099160	206000	-40
18106100	18108760	266000	-34  CORRECCION REAL (CENTESIAMS HZ) +116000	CORRECCION REAL EN FRECUENCIA -2160 (HZ) => 18103940 HZ
21096100	21099170	307000	-35
24926100	24929730	363000	-40
28126100	28130205	415000	-60
*/
/***********************************************************************************
* WSPR 
************************************************************************************/
/* INTERBATIONAL WSPR BEACON PROJECT https://github.com/HB9VQQ/WSPRBeacon
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
static const unsigned long long freqArray[10] = {
  7039280ULL,  // RPT 0,20,40
  14095400ULL,  // RPT 2,22,42
  21093600ULL,  // RPT 4,24,44
  7039280ULL,  //  ok 40m 6,26,46
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
                         