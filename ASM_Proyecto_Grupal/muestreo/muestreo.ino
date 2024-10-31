const int samplingFrequency = 10000; // Frecuencia de muestreo en Hz (10 kHz)
unsigned long previousMicros = 0; // Tiempo en microsegundos del último muestreo
const unsigned long interval = 1000000 / samplingFrequency; // Intervalo en microsegundos para la frecuencia de muestreo

void setup() {
  Serial.begin(9600);
  setupModulacion();
}

void loop() {
  unsigned long currentMicros = micros();
  
  if (currentMicros - previousMicros >= interval) {
    previousMicros = currentMicros;

    // Leer la muestra del micrófono
    uint16_t valorMic = analogRead(34); // Lee el valor del ADC (12 bits)

    // Leer la muestra del audio en bucle
    uint16_t valorAudio = obtenerMuestraAudio(); // Función que simula leer una muestra del audio en bucle

    // Modula ambas señales secuencialmente en el mismo pin
    modularMuestraSecuencial(valorMic, valorAudio);
  }
}
