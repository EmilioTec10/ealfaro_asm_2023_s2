import numpy as np
import librosa

def analyze_audio_in_windows(audio_path, window_size=2048, hop_length=512):
    # Cargar el audio
    y, sr = librosa.load(audio_path)

    # Magnitud (espectrograma de magnitud)
    S = np.abs(librosa.stft(y, n_fft=window_size, hop_length=hop_length))
    
    # Convertir la magnitud a decibelios
    S_dB = librosa.amplitude_to_db(S)

    # Fase en radianes
    phase = np.angle(librosa.stft(y, n_fft=window_size, hop_length=hop_length))
    delta_phase = np.diff(phase, axis=1)
    delta_phase_degrees = np.degrees(delta_phase)

    # Pitch tracking en ventanas
    pitches, _ = librosa.core.piptrack(y=y, sr=sr, n_fft=window_size, hop_length=hop_length)
    
    # Filtrar valores no válidos de pitch (donde la magnitud es cero)
    pitches = [p[p > 0] for p in pitches.T]  # Transponemos para iterar sobre frames de tiempo

    # Calcular los valores para cada ventana
    magnitudes_db = np.mean(S_dB, axis=0)  # Magnitud promedio por ventana en dB
    avg_delta_phase_degrees = np.mean(delta_phase_degrees, axis=0)  # Cambio de fase promedio por ventana en grados

    # Ajustar el tamaño de avg_delta_phase_degrees para que coincida con magnitudes_db y avg_pitches_dHz
    min_length = min(len(magnitudes_db), len(avg_delta_phase_degrees), len(pitches))
    magnitudes_db = magnitudes_db[:min_length]
    avg_delta_phase_degrees = avg_delta_phase_degrees[:min_length]

    # Redondear magnitudes_db a 2 decimales
    magnitudes_db = np.round(magnitudes_db, 2)

    # Convertir los pitches promedio a int y manejar el caso 0
    avg_pitches_dHz = []
    for p in pitches[:min_length]:
        if len(p) > 0:
            # Convertir a dHz y redondear al entero más cercano
            avg_pitch_dHz = int(round(np.mean(p) / 10))
            # Si el pitch resultante es 0, asignar el valor 10
            if avg_pitch_dHz == 0:
                avg_pitch_dHz = 1
            avg_pitches_dHz.append(avg_pitch_dHz)
        else:
            # En caso de que no haya valores válidos, asignar 10
            avg_pitches_dHz.append(10)

    rest_time = hop_length / sr
    # Retornar los valores por ventana
    return magnitudes_db, avg_delta_phase_degrees, avg_pitches_dHz, rest_time
