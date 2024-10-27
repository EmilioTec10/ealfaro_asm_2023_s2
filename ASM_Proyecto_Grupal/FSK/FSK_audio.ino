#include <driver/ledc.h>

const int pinPWM = 18; // Pin de salida PWM para la señal modulada
const int freqZero = 100; // Frecuencia para el bit '0'
const int freqOne = 500;  // Frecuencia para el bit '1'

// Configuración del PWM
const int freqPWM = 5000;
const int resolutionPWM = 8;

void setup() {
    Serial.begin(115200);
    Serial.println("Iniciando recepción de datos de audio para FSK");

    // Configuración del PWM
    ledcSetup(0, freqPWM, resolutionPWM);
    ledcAttachPin(pinPWM, 0);
}

void loop() {
    // Verificar si hay datos disponibles en el puerto serie
    if (Serial.available() > 0) {
        char receivedBit = Serial.read(); // Leer el bit recibido

        // Aplicar la modulación FSK según el bit recibido
        if (receivedBit == '0') {
            ledcWriteTone(0, freqZero); // Generar 100 Hz para el bit '0'
        } else if (receivedBit == '1') {
            ledcWriteTone(0, freqOne); // Generar 500 Hz para el bit '1'
        }
    }

    // Añadir un pequeño retardo para evitar saturar el puerto serie
    delay(1);
}
