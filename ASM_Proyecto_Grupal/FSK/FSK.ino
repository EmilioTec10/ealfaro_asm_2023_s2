const int samplingFrequency = 10000; // Frecuencia de muestreo en Hz (10 kHz)
unsigned long previousMicros = 0; // Tiempo en microsegundos del último muestreo
const unsigned long interval = 1000000 / samplingFrequency; // Intervalo en microsegundos para la frecuencia de muestreo

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 baudios
}

void loop() {
  unsigned long currentMicros = micros(); // Obtén el tiempo actual en microsegundos

  // Verifica si ha pasado el intervalo de tiempo para el siguiente muestreo
  if (currentMicros - previousMicros >= interval) {
    previousMicros = currentMicros; // Actualiza el tiempo del último muestreo

    uint16_t valorADC = analogRead(34); // Lee el valor del ADC (12 bits)

    // Imprime el valor del ADC en formato binario de 12 bits
    for (int i = 11; i >= 0; i--) {
      Serial.print((valorADC >> i) & 1); // Extrae cada bit desde el más significativo al menos significativo
    }
    Serial.println(); // Salto de línea después de imprimir los 12 bits
  }
}
