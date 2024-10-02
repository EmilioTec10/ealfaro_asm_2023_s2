import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from scipy.signal import hilbert, butter, filtfilt

# 1. Lectura del archivo de audio
AudioName = "wind-chimes.wav"  # Ruta de tu archivo de audio
fs, Audiodata = wavfile.read(AudioName)   # fs: frecuencia de muestreo, Audiodata: señal de audio

# Verificar si el audio es mono o estéreo
if Audiodata.ndim == 1:
    x = Audiodata
else:
    x = Audiodata[:, 0]  # Usar el canal izquierdo si es estéreo

# Convertir a tipo float y normalizar entre -1 y 1
x = x.astype(np.float32)
max_val = np.max(np.abs(x))
if max_val > 0:
    x = x / max_val

# Vector de tiempo
t = np.arange(len(x)) / fs

# Visualización de la señal original
plt.figure(figsize=(12, 4))
plt.plot(t, x)
plt.title('Señal de Audio Original')
plt.xlabel('Tiempo [s]')
plt.ylabel('Amplitud')
plt.show()

# 2. Modulación AM con portadora completa
# Verificar la frecuencia de muestreo
print(f"Frecuencia de muestreo del archivo: {fs} Hz")

# Parámetros de la portadora
fc = 10000  # Frecuencia de la portadora ajustada a 10 kHz

# Verificar que la frecuencia de la portadora sea válida
if fc >= fs / 2:
    raise ValueError("La frecuencia de la portadora debe ser menor que la mitad de la frecuencia de muestreo (fs/2).")

A = 1       # Amplitud de la portadora

# Generar la señal portadora
carrier = np.cos(2 * np.pi * fc * t)

# Modulación AM: (A + x) * carrier
modulated_signal = (A + x) * carrier

# Visualización de la señal modulada
plt.figure(figsize=(12, 4))
plt.plot(t, modulated_signal)
plt.title('Señal Modulada en Amplitud (AM)')
plt.xlabel('Tiempo [s]')
plt.ylabel('Amplitud')
plt.show()

# 3. Demodulación de la señal AM

# Obtener la envolvente de la señal modulada usando la Transformada de Hilbert
analytic_signal = hilbert(modulated_signal)
envelope = np.abs(analytic_signal)

# Filtro pasa bajos para eliminar componentes de alta frecuencia
def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs  # Frecuencia de Nyquist
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

# Parámetros del filtro pasa bajos
cutoff = 5000  # Ajusta esta frecuencia según el contenido de tu señal
order = 6      # Orden del filtro

# Aplicar el filtro pasa bajos a la envolvente
b, a = butter_lowpass(cutoff, fs, order)
demodulated_signal = filtfilt(b, a, envelope)

# Visualización de la señal demodulada
plt.figure(figsize=(12, 4))
plt.plot(t, demodulated_signal)
plt.title('Señal Demodulada')
plt.xlabel('Tiempo [s]')
plt.ylabel('Amplitud')
plt.show()

# 4. Comparación de las señales en el dominio del tiempo

plt.figure(figsize=(12, 8))

plt.subplot(3, 1, 1)
plt.plot(t, x)
plt.title('Señal Original')
plt.xlabel('Tiempo [s]')
plt.ylabel('Amplitud')

plt.subplot(3, 1, 2)
plt.plot(t, modulated_signal)
plt.title('Señal Modulada (AM)')
plt.xlabel('Tiempo [s]')
plt.ylabel('Amplitud')

plt.subplot(3, 1, 3)
plt.plot(t, demodulated_signal)
plt.title('Señal Demodulada')
plt.xlabel('Tiempo [s]')
plt.ylabel('Amplitud')

plt.tight_layout()
plt.show()

# 5. Comparación de las señales en el dominio de la frecuencia (usando FFT)

# Función para calcular y graficar el espectro de frecuencia
def plot_frequency_spectrum(signal, fs, title):
    N = len(signal)
    yf = np.fft.rfft(signal)
    xf = np.fft.rfftfreq(N, 1 / fs)
    plt.figure(figsize=(12, 4))
    plt.plot(xf, np.abs(yf))
    plt.title(title)
    plt.xlabel('Frecuencia [Hz]')
    plt.ylabel('Magnitud')
    plt.xlim(0, fs / 2)
    plt.show()

# Espectro de la señal original
plot_frequency_spectrum(x, fs, 'Espectro de Frecuencia de la Señal Original')

# Espectro de la señal modulada
plot_frequency_spectrum(modulated_signal, fs, 'Espectro de Frecuencia de la Señal Modulada')

# Espectro de la señal demodulada
plot_frequency_spectrum(demodulated_signal, fs, 'Espectro de Frecuencia de la Señal Demodulada')
