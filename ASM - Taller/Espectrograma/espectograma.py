#Codigo tomado de https://www.kaggle.com/code/joeportilla/c-mo-graficar-espectrogramas-de-audios-en-python

import matplotlib

import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
from scipy.fft import fftshift


fs, audioClean = wavfile.read('ASM - Taller\Espectrograma\download.wav')
fs, audioNoise = wavfile.read('ASM - Taller\Espectrograma\download.wav')


samples = len(audioClean)
t = np.arange(0, samples/fs, 1/fs)


fig, ax = plt.subplots(2, 1, figsize=(12, 6))
fig.patch.set_facecolor('white')

Pxx, freqs, bins, im = ax[0].specgram(audioClean, NFFT=1024, Fs=fs, noverlap=512)
ax[0].set_title('Audio Limpio. Espectograma')
ax[0].set_xlabel('Tiempo [s]')
ax[0].set_ylabel('Amplitud [Hz]')
ax[0].grid(False)

Pxx, freqs, bins, im = ax[1].specgram(audioNoise, NFFT=1024, Fs=fs, noverlap=512)
ax[1].set_title('Audio Ruidoso. Espectograma')
ax[1].set_xlabel('Tiempo [s]')
ax[1].set_ylabel('Amplitud [Hz]')
ax[1].grid(False)

plt.tight_layout()
plt.show()


