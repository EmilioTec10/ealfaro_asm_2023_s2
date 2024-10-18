#include <arduinoFFT.h>

const uint16_t SAMPLES = 64;   // Número de muestras para la FFT, debe ser una potencia de 2
const double SAMPLING_FREQUENCY = 8000;  // Frecuencia de muestreo en Hz
const double FSK_FREQ_LOW = 1000;   // Frecuencia para representar el bit 0 en la modulación PWM
const double FSK_FREQ_HIGH = 2000;  // Frecuencia para representar el bit 1 en la modulación PWM
const double FREQUENCY_THRESHOLD = 1500;   // Umbral para decidir la frecuencia PWM
const uint16_t BAUD_RATE = 2;   // Velocidad de transmisión en bits por segundo (ajustada para la simulación)
const uint8_t PWM_PIN = 9;  // Pin PWM para salida de modulación

double vReal[SAMPLES];
double vImag[SAMPLES];
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

// Variables para la generación de la señal binaria simulada
const uint16_t BIT_DURATION = (1000 / BAUD_RATE);  // Duración de cada bit en milisegundos
unsigned long lastBitChange = 0;
bool currentBit = 0;  // Bit actual ('0' o '1')

void setup() {
    Serial.begin(9600);  // Comunicación serial a 9600 baudios
    pinMode(PWM_PIN, OUTPUT);
}

void loop() {
    // Verificar si es momento de cambiar el bit
    unsigned long currentTime = millis();
    if (currentTime - lastBitChange >= BIT_DURATION) {
        lastBitChange = currentTime;
        currentBit = !currentBit;  // Alternar el bit entre '0' y '1'
    }

    // Generar la señal simulada basada en el bit actual
    double testFrequency = currentBit ? 2500.0 : 500.0;  // 2500 Hz para '1', 500 Hz para '0'

    // Llenar los arrays vReal[] y vImag[] con la señal simulada
    for (int i = 0; i < SAMPLES; i++) {
        double time = (i / SAMPLING_FREQUENCY);
        vReal[i] = 2.0 * sin(2 * PI * testFrequency * time);
        vImag[i] = 0.0;  // Parte imaginaria en cero
    }

    // Calcular la FFT
    FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);  // Aplicar ventana Hamming
    FFT.compute(FFT_FORWARD);  // Calcular la FFT
    FFT.complexToMagnitude();  // Convertir a magnitud

    // Obtener la frecuencia dominante
    double dominantFrequency = FFT.majorPeak();

    // Verificar si la frecuencia dominante es NaN
    if (isnan(dominantFrequency)) {
        Serial.println("Frecuencia dominante es NaN. Restableciendo modulación.");
        analogWriteFrequency(FSK_FREQ_LOW);  // Restablecer a la frecuencia baja
        analogWrite(PWM_PIN, 0);  // Detener la señal PWM
        delay(1000 / BAUD_RATE);  // Esperar un momento antes de continuar
        return;  // Salir del loop actual para evitar la modulación
    }

    // Modulación FSK basada en la frecuencia dominante
    modulateFrequencyToFSK(dominantFrequency);

    // Esperar para el siguiente ciclo, de acuerdo con el baud rate
    delay(100);  // Ajusta este valor según sea necesario
}

// Función para modular la frecuencia PWM según la frecuencia dominante
void modulateFrequencyToFSK(double dominantFreq) {
    Serial.print("Frecuencia dominante: ");
    Serial.println(dominantFreq);

    if (dominantFreq < FREQUENCY_THRESHOLD) {
        analogWriteFrequency(FSK_FREQ_LOW);
        analogWrite(PWM_PIN, 128);  // 50% duty cycle
        Serial.println("Modulando en frecuencia baja");
    } else {
        analogWriteFrequency(FSK_FREQ_HIGH);
        analogWrite(PWM_PIN, 128);  // 50% duty cycle
        Serial.println("Modulando en frecuencia alta");
    }
}

// Función para configurar la frecuencia de PWM
void analogWriteFrequency(double frequency) {
    const long clock = 16000000L; // Frecuencia del reloj del Arduino Uno (16 MHz)
    long prescaler = 1;
    int mode = 0;

    // Seleccionar el prescaler adecuado para la frecuencia deseada
    if (frequency < 31) {
        prescaler = 1024;
        mode = 5;
    } else if (frequency < 245) {
        prescaler = 256;
        mode = 5;
    } else if (frequency < 1000) {
        prescaler = 64;
        mode = 5;
    } else if (frequency < 4000) {
        prescaler = 8;
        mode = 5;
    } else {
        prescaler = 1;
        mode = 5;
    }

    long top = clock / (prescaler * frequency) - 1;
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (mode & 7);
    TCCR1B |= (prescaler == 1024 ? (1 << CS12) | (1 << CS10) :
               prescaler == 256 ? (1 << CS12) :
               prescaler == 64  ? (1 << CS11) | (1 << CS10) :
               prescaler == 8   ? (1 << CS11) :
                                  (1 << CS10));

    ICR1 = top;
    OCR1A = top / 2;
    OCR1B = top / 2;

    Serial.print("Frecuencia PWM modulada: ");
    Serial.print(frequency);
    Serial.println(" Hz");
}
