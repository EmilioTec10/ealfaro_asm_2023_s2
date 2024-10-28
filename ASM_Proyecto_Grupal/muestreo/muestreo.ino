const int samplingFrequency = 10000; // Frecuencia de muestreo en Hz (10 kHz) 
unsigned long previousMicros = 0; // Tiempo en microsegundos del último muestreo 
const unsigned long interval = 1000000 / samplingFrequency; // Intervalo en microsegundos para la frecuencia de muestreo  
void setup() {   
  Serial.begin(9600);   
  setupModulacion(); 
  }  
  void loop() {   
    uint16_t valorADC = analogRead(34); // Lee el valor del ADC (12 bits)   
    modularMuestra(valorADC); 
    } 
