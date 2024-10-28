#include "driver/ledc.h"

const int pinFSK = 13; // Pin de salida para la señal FSK
const int freqF1 = 1000; // Frecuencia para bit 1 (1000 Hz)
const int freqF2 = 500;  // Frecuencia para bit 0 (500 Hz)

const int resolution = 8; // Resolución de 8 bits para el PWM
const int dutyCycle = 128; // Ciclo de trabajo (50% de 256)

void setupModulacion() {
  Serial.begin(9600); // Inicializar la comunicación serial
  pinMode(pinFSK, OUTPUT);

  // Configurar el canal PWM usando la nueva API
  // Se especifica el pin, la frecuencia inicial y la resolución
  ledcAttach(pinFSK, freqF1, resolution); // Configura el pin con la frecuencia y la resolución deseada
  ledcWrite(pinFSK, dutyCycle); // Establece el ciclo de trabajo al 50%

  Serial.println("Modulación configurada correctamente.");
}

void modularMuestra(uint16_t muestra) {
  Serial.print("Modulando muestra: ");
  Serial.println(muestra);

  for (int i = 11; i >= 0; i--) {
    uint8_t bit = (muestra >> i) & 1; // Extraer el bit

    // Establecer la frecuencia según el valor del bit
    int freq = (bit == 1) ? freqF1 : freqF2;

    // Imprimir información del bit y la frecuencia
    Serial.print("Bit ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(bit);
    Serial.print(" -> Frecuencia: ");
    Serial.println(freq);

    // Establecer la frecuencia de salida
    ledcWriteTone(pinFSK, freq);

  }

  // Detener la señal PWM después de modular todos los bits
  ledcWriteTone(pinFSK, 0);
  Serial.println("Finalización de la modulación para la muestra.");

}