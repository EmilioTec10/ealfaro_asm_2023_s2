#Codigo tomado de https://deepnote.com/app/marianodr-eb01/Aplicacion-FFT-799b9b0d-217e-4cce-97b6-991a72501dcf

from scipy.io import wavfile # Permite leer y grabar audio
from IPython.display import Audio # Permite reproducir el audio
import numpy as np
import matplotlib.pyplot as plt

AudioName = "ASM-Tarea1/wind-chimes.wav" # Archivo de Audio

fs, Audiodata = wavfile.read(AudioName)

duracion = Audiodata.shape[0]/fs

print(f'Duracion = {duracion} , Frecuencia de Muestreo = {fs} [Muestras/Seg]' \
      f', Wav format = {Audiodata.dtype}')

dt = 1/fs                                    # Tiempo entre muestras
t = np.arange(0, duracion, dt)               # Se genera el vector tiempo
x = Audiodata[:,0]                           # Se crea la señal de sonido

plt.plot(t,x,color='r',lw=1,label='Audio')
plt.xlim(t[0],t[-1])
plt.title('Audio en el tiempo')
plt.xlabel('Tiempo [s]')
plt.xlabel('Amplitud')
plt.legend()
plt.show()

N = len(t)                                           # Cantidad de muestras totales

AUDIO_FFT = np.fft.fft(x,N)                          # Computo de la FFT

mag_AUDIO_FFT = abs(AUDIO_FFT)                       # Magnitud de los coefiecientes
PSD = AUDIO_FFT * np.conj(AUDIO_FFT) / N             # Densidad espectral de potencia (Power spectrum, power per frecuency)

freq = (1/(dt*N)) * np.arange(N)                     # Eje x de frecuencias
L = np.arange(0, np.floor(N/2), dtype='int')         # Para plotear la primera mitad del espectro

plt.plot(freq[L], mag_AUDIO_FFT[L])
plt.title('Espectro del audio')
plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Magnitud FFT')
plt.xlim(freq[L[0]],freq[L[-1]])
plt.show()

plt.plot(freq[L],PSD[L],color='r',lw=2,label='PSD')
plt.title('PSD(Densidad espectral de potencia) del audio')
plt.xlim(freq[L[0]],freq[L[-1]])
plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Potencia')
plt.legend()
plt.show()