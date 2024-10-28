//modulacion.ino
#include "driver/ledc.h"

const int pinFSK = 13; // Pin de salida para la señal FSK
const int freqF1_mic = 1000; // Frecuencia para bit 1 del micrófono (1000 Hz)
const int freqF2_mic = 500;  // Frecuencia para bit 0 del micrófono (500 Hz)
const int freqF1_audio = 2000; // Frecuencia para bit 1 del audio en bucle (2000 Hz)
const int freqF2_audio = 1500; // Frecuencia para bit 0 del audio en bucle (1500 Hz)

const int resolution = 8; // Resolución de 8 bits para el PWM
const int dutyCycle = 128; // Ciclo de trabajo (50% de 256)

void setupModulacion() {
  Serial.begin(9600);
  pinMode(pinFSK, OUTPUT);

  // Configurar canal PWM para el pin de salida
  ledcAttach(pinFSK, freqF1_mic, resolution);
  ledcWrite(pinFSK, dutyCycle);

  Serial.println("Modulación configurada correctamente.");
}

void modularMuestraSecuencial(uint16_t muestraMic, uint16_t muestraAudio) {
  Serial.print("Modulando muestras secuenciales: ");
  Serial.print("Micrófono: ");
  Serial.print(muestraMic);
  Serial.print(" | Audio: ");
  Serial.println(muestraAudio);

  // Modular todos los bits de la muestra del micrófono
  for (int i = 11; i >= 0; i--) {
    uint8_t bitMic = (muestraMic >> i) & 1;
    int freqMic = (bitMic == 1) ? freqF1_mic : freqF2_mic;

    // Establecer la frecuencia para el bit del micrófono
    ledcWriteTone(pinFSK, freqMic);

    // Imprimir información del bit y la frecuencia
    Serial.print("Bit ");
    Serial.print(i);
    Serial.print(" (Mic): ");
    Serial.print(bitMic);
    Serial.print(" -> Frecuencia: ");
    Serial.println(freqMic);
  }

  // Modular todos los bits de la muestra del audio en bucle
  for (int i = 11; i >= 0; i--) {
    uint8_t bitAudio = (muestraAudio >> i) & 1;
    int freqAudio = (bitAudio == 1) ? freqF1_audio : freqF2_audio;

    // Establecer la frecuencia para el bit del audio
    ledcWriteTone(pinFSK, freqAudio);

    // Imprimir información del bit y la frecuencia
    Serial.print("Bit ");
    Serial.print(i);
    Serial.print(" (Audio): ");
    Serial.print(bitAudio);
    Serial.print(" -> Frecuencia: ");
    Serial.println(freqAudio);
  }

  // Detener la señal PWM después de modular todos los bits
  ledcWriteTone(pinFSK, 0);
  Serial.println("Finalización de la modulación para las muestras secuenciales.");
}


