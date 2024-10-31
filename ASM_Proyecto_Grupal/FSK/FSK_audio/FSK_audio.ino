const int pinPWM = 18; // Pin de salida PWM para la señal modulada
const int ledcChannel = 0; // Canal del LEDC (puede ser de 0 a 15)
const int freqZero = 100; // Frecuencia para el bit '0'
const int freqOne = 500;  // Frecuencia para el bit '1'

// Valores de ciclo de trabajo (duty cycle)
const int dutyCycle = 128; // 50% del ciclo para una resolución de 8 bits (0-255)

void setup() {
    Serial.begin(9600);
    Serial.println("Iniciando recepción de datos para FSK");

    // Asocia el canal LEDC al pin especificado
    ledcAttach(pinPWM, ledcChannel, dutyCycle);
}

void loop() {
    // Verificar si hay datos disponibles en el puerto serie
    if (Serial.available() > 0) {
        char receivedBit = Serial.read(); // Leer el bit recibido
        Serial.println("Estoy bellako");
        // Cambiar el ciclo de trabajo para simular la señal de FSK
        if (receivedBit == '0') {
            ledcWrite(ledcChannel, freqZero); // Genera 100 Hz para el bit '0'
            Serial.println("Recibió un 0");
        } else if (receivedBit == '1') {
            ledcWrite(ledcChannel, freqOne); // Genera 500 Hz para el bit '1'
            Serial.println("Recibió un 1");
        }
    }
}