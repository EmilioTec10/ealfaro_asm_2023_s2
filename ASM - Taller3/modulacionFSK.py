#Codigo tomado de https://www.rfwireless-world.com/source-code/Python/FSK-modulation-python-code.html

import numpy as np
import matplotlib.pyplot as plt

# Parameters
Fs = 1000  # Sampling frequency
fc = 30  # Carrier frequency for binary 0
f1 = 45  # Frequency for binary 1
T = 1  # Time period
t = np.arange(0, T, 1/Fs)

# Binary signal generation
def generate_binary_data(n_bits):
    return np.random.randint(0, 2, n_bits)

# FSK Modulation
def fsk_modulate(binary_data, fc, f1, Fs, T):
    t = np.arange(0, T, 1/Fs)
    fsk_signal = np.zeros(len(t))
    
    for i, bit in enumerate(binary_data):
        f = f1 if bit == 1 else fc
        fsk_signal += np.sin(2 * np.pi * f * t)
    
    return fsk_signal

# Binary Data
binary_data = generate_binary_data(10)

# Modulated Signal
fsk_signal = fsk_modulate(binary_data, fc, f1, Fs, T)

# Plotting
plt.subplot(2, 1, 1)
plt.title('Binary Signal')
plt.plot(binary_data)
plt.subplot(2, 1, 2)
plt.title('FSK Modulated Signal')
plt.plot(fsk_signal)
plt.show()
