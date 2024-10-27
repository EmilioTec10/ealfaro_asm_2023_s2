#Codigo tomado de https://www.rfwireless-world.com/source-code/Python/FSK-modulation-python-code.html

import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft

# Generar datos binarios aleatorios
def generate_binary_data(n_bits):
    return np.random.randint(0, 2, n_bits)

# FSK Modulación
def fsk_modulate(binary_data, fc, f1, Fs, T):
    t = np.arange(0, T, 1/Fs)
    fsk_signal = np.zeros(len(t))
    bit_duration = len(t) // len(binary_data)  # Duración de cada bit

    for i, bit in enumerate(binary_data):
        f = f1 if bit == 1 else fc
        fsk_signal[i * bit_duration:(i + 1) * bit_duration] = np.sin(2 * np.pi * f * t[i * bit_duration:(i + 1) * bit_duration])
    
    return fsk_signal

# FFT para demodulación
def fsk_demodulate_fft(fsk_signal, fc, f1, Fs, T, n_bits):
    bit_duration = len(fsk_signal) // n_bits
    demodulated_signal = []

    for i in range(0, len(fsk_signal), bit_duration):
        # Obtener la FFT del segmento de la señal
        segment = fsk_signal[i:i + bit_duration]
        fft_result = fft(segment)
        fft_magnitude = np.abs(fft_result)

        # Obtener las frecuencias correspondientes
        freqs = np.fft.fftfreq(len(segment), 1/Fs)

        # Verificar qué frecuencia es dominante
        idx_fc = np.where((freqs >= fc - 5) & (freqs <= fc + 5))[0]
        idx_f1 = np.where((freqs >= f1 - 5) & (freqs <= f1 + 5))[0]

        # Energía en ambas frecuencias
        energy_fc = np.sum(fft_magnitude[idx_fc])
        energy_f1 = np.sum(fft_magnitude[idx_f1])

        # Decidir si es 0 o 1 según la energía dominante
        if energy_f1 > energy_fc:
            demodulated_signal.append(1)
        else:
            demodulated_signal.append(0)
    
    return demodulated_signal

# Parámetros
Fs = 1000  # Frecuencia de muestreo
fc = 30  # Frecuencia de portadora para 0
f1 = 45  # Frecuencia de portadora para 1
T = 1  # Tiempo total de simulación
n_bits = 10  # Número de bits

# 1. Generar datos binarios
binary_data = generate_binary_data(n_bits)

# 2. Modulación FSK
fsk_signal = fsk_modulate(binary_data, fc, f1, Fs, T)

# 3. Demodular usando FFT
demodulated_signal = fsk_demodulate_fft(fsk_signal, fc, f1, Fs, T, n_bits)

# 4. Graficar las señales
t = np.arange(0, T, 1/Fs)

plt.figure(figsize=(12, 8))

# Señal Binaria Original
plt.subplot(3, 1, 1)
plt.title('Señal Binaria de Entrada')
plt.plot(np.repeat(binary_data, len(t)//n_bits), 'b')
plt.ylim(-0.5, 1.5)

# Señal FSK Modulada
plt.subplot(3, 1, 2)
plt.title('Señal FSK Modulada')
plt.plot(t, fsk_signal, 'r')

# Señal FSK Demodulada
plt.subplot(3, 1, 3)
plt.title('Señal Demodulada usando FFT')
plt.plot(np.repeat(demodulated_signal, len(t)//n_bits), 'g')
plt.ylim(-0.5, 1.5)

plt.tight_layout()
plt.show()

print("Datos Binarios Originales:", binary_data)
print("Datos Demodulados (usando FFT):", demodulated_signal)
