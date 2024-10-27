#include <arduinoFFT.h>

// Definiciones de constantes
const uint16_t SAMPLES = 64;                // Número de muestras para la FFT, debe ser una potencia de 2
const double SAMPLING_FREQUENCY = 8000.0;   // Frecuencia de muestreo en Hz
const uint8_t PWM_PIN = 9;                  // Pin PWM para salida de modulación
const double THRESHOLD_FREQUENCY = 1500.0;  // Frecuencia umbral para determinar el bit modulado (0 o 1)

// Variables para la FFT
double vReal[SAMPLES];
double vImag[SAMPLES];
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

// Variables para la generación de la señal seno
double baseFrequency = 1500.0;              // Frecuencia base en Hz
double modulationAmplitude = 500.0;         // Amplitud de la modulación en Hz
double modulationFrequency = 0.1;           // Frecuencia de modulación en Hz (ajusta este valor según tus necesidades)
double amplitude = 1.0;                     // Amplitud de la señal seno
double phase = 0.0;                         // Fase inicial
double time = 0.0;                          // Tiempo inicial
double timeIncrement = 1.0 / SAMPLING_FREQUENCY; // Incremento de tiempo entre muestras

void setup() {
    Serial.begin(9600);          // Comunicación serial a 9600 baudios
    pinMode(PWM_PIN, OUTPUT);    // Configurar el pin PWM como salida
    analogWriteFrequency(1000);  // Frecuencia inicial del PWM
    analogWrite(PWM_PIN, 128);   // 50% de ciclo de trabajo
}

void loop() {
    // Generar la señal seno con frecuencia modulada y llenar los arreglos vReal[] y vImag[]
    for (int i = 0; i < SAMPLES; i++) {
        // Calcular la frecuencia instantánea usando la función seno
        double signalFrequency = baseFrequency + modulationAmplitude * sin(2 * PI * modulationFrequency * time);
        // Generar la muestra de la señal seno con frecuencia instantánea
        vReal[i] = amplitude * sin(2 * PI * signalFrequency * time + phase);
        vImag[i] = 0.0;  // Parte imaginaria en cero

        time += timeIncrement;
    }

    // Calcular la FFT
    FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);  // Aplicar ventana Hamming
    FFT.compute(FFT_FORWARD);                         // Calcular la FFT
    FFT.complexToMagnitude();                         // Convertir a magnitud

    // Obtener la frecuencia dominante
    double dominantFrequency = FFT.majorPeak();

    // Verificar si la frecuencia dominante es válida
    if (isnan(dominantFrequency)) {
        Serial.println("Frecuencia dominante es NaN. Restableciendo modulación.");
        analogWriteFrequency(1000);  // Frecuencia por defecto
        analogWrite(PWM_PIN, 0);     // Detener la señal PWM
        delay(100);                  // Esperar un momento antes de continuar
        return;                      // Salir del loop actual para evitar la modulación
    }

    // Determinar el bit modulado basado en la frecuencia dominante
    int bitModulado = (dominantFrequency > THRESHOLD_FREQUENCY) ? 1 : 0;

    // Modulación de la frecuencia PWM basada en el bit modulado
    modulatePWM(bitModulado);

    // Imprimir información para depuración
    Serial.print("Frecuencia dominante: ");
    Serial.print(dominantFrequency);
    Serial.print(" Hz, Frecuencia PWM modulada: ");
    Serial.print(bitModulado ? "Alta" : "Baja");
    Serial.print(", Bit modulado: ");
    Serial.println(bitModulado);

    // Esperar un breve momento antes de la siguiente iteración
    delay(100);  // Ajusta este valor según sea necesario
}

// Función para modular la frecuencia PWM según el bit modulado
void modulatePWM(int bit) {
    double pwmFrequency;
    if (bit == 1) {
        pwmFrequency = 2000.0; // Frecuencia alta para bit 1
    } else {
        pwmFrequency = 1000.0; // Frecuencia baja para bit 0
    }

    analogWriteFrequency(pwmFrequency);
    analogWrite(PWM_PIN, 128);  // 50% de ciclo de trabajo
}

// Función para configurar la frecuencia de PWM
void analogWriteFrequency(double frequency) {
    const long clock = 16000000L;  // Frecuencia del reloj del Arduino Uno (16 MHz)
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
    if (top < 1) top = 1;  // Evitar divisiones por cero o valores inválidos

    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (mode & 7);
    TCCR1B |= (prescaler == 1024) ? ((1 << CS12) | (1 << CS10)) :
              (prescaler == 256) ? (1 << CS12) :
              (prescaler == 64)  ? ((1 << CS11) | (1 << CS10)) :
              (prescaler == 8)   ? (1 << CS11) :
                                   (1 << CS10);

    ICR1 = top;
    OCR1A = top / 2;
    OCR1B = top / 2;
}
