#include <arduinoFFT.h>

// Definiciones de constantes
const uint16_t SAMPLES = 64;                // Número de muestras para la FFT, debe ser una potencia de 2
const double SAMPLING_FREQUENCY = 8000.0;   // Frecuencia de muestreo en Hz
const double THRESHOLD_FREQUENCY = 1500.0;  // Frecuencia umbral para determinar el bit modulado (0 o 1)

// Variables para la FFT
double vReal[SAMPLES];
double vImag[SAMPLES];
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

// Variables para la generación de la señal seno
double baseFrequency = 1500.0;              // Frecuencia base en Hz
double modulationAmplitude = 500.0;         // Amplitud de la modulación en Hz
double modulationFrequency = 0.1;           // Frecuencia de modulación en Hz
double amplitude = 1.0;                     // Amplitud de la señal seno
double phase = 0.0;                         // Fase inicial
double time = 0.0;                          // Tiempo inicial
double timeIncrement = 1.0 / SAMPLING_FREQUENCY; // Incremento de tiempo entre muestras

void setup() {
    Serial.begin(9600);  // Comunicación serial a 9600 baudios
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

    // Encontrar la amplitud máxima en el arreglo de magnitudes
    double maxAmplitude = 0.0;
    for (int i = 0; i < SAMPLES / 2; i++) {
        if (vReal[i] > maxAmplitude) {
            maxAmplitude = vReal[i];
        }
    }

    // Imprimir la amplitud máxima detectada
    Serial.print("Amplitud máxima: ");
    Serial.println(maxAmplitude);

    // Esperar un breve momento antes de la siguiente iteración
    delay(100);  // Ajusta este valor según sea necesario
}