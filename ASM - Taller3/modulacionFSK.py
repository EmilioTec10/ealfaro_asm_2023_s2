#Codigo tomado de https://www.rfwireless-world.com/source-code/Python/FSK-modulation-python-code.html

import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter

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

# Filtro paso banda
def butter_bandpass(lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    return b, a

def bandpass_filter(data, lowcut, highcut, fs, order=5):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = lfilter(b, a, data)
    return y

# FSK Demodulación
def fsk_demodulate(fsk_signal, fc, f1, Fs, T, n_bits):
    t = np.arange(0, T, 1/Fs)
    
    # Filtrar para detectar la frecuencia fc (0) y f1 (1)
    signal_fc = bandpass_filter(fsk_signal, fc-5, fc+5, Fs)
    signal_f1 = bandpass_filter(fsk_signal, f1-5, f1+5, Fs)
    
    # Comparar la energía de ambas señales
    demodulated_signal = []
    bit_duration = len(t) // n_bits  # Duración de cada bit
    
    for i in range(0, len(fsk_signal), bit_duration):
        # Sumar la energía en cada segmento filtrado
        energy_fc = np.sum(signal_fc[i:i+bit_duration]**2)
        energy_f1 = np.sum(signal_f1[i:i+bit_duration]**2)
        
        # Si la energía de f1 es mayor, es un '1', si no es '0'
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

# 3. Demodular la señal FSK
demodulated_signal = fsk_demodulate(fsk_signal, fc, f1, Fs, T, n_bits)

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
plt.title('Señal Demodulada')
plt.plot(np.repeat(demodulated_signal, len(t)//n_bits), 'g')
plt.ylim(-0.5, 1.5)

plt.tight_layout()
plt.show()

print("Datos Binarios Originales:", binary_data)
print("Datos Demodulados:", demodulated_signal)
