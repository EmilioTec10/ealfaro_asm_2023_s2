import numpy as np
import matplotlib.pyplot as plt
import audio_parameters
import os
import moviepy.editor as mp

def convert_to_positive_integers(arr):
    # Convertir los valores a su valor absoluto para eliminar negativos
    positive_arr = np.abs(arr)
    
    # Convertir los valores a enteros redondeando
    integer_arr = np.round(positive_arr).astype(int)
    
    return integer_arr

audio_path = 'ASM - Taller/Espectrograma/download.wav'

iteraciones, exp, silly, rest_time, xn = audio_parameters.analyze_audio_in_windows(audio_path)
iteraciones = convert_to_positive_integers(iteraciones)

# Imprimir el número de ventanas analizadas y ejemplo de valores
print(f"Ejemplo de magnitudes en dB: {iteraciones}")
print(f"Ejemplo de cambio de fase en grados: {exp}")
print(f"Ejemplo de pitch: {xn}")

def lyapunov_fractal(seq, x_min, x_max, y_min, y_max, width, height, exp, xn_, iterations):

    # Crear la cuadrícula de coordenadas
    x = np.linspace(x_min, x_max, width)
    y = np.linspace(y_min, y_max, height)
    lyap = np.zeros((height, width))

    # Iterar sobre la cuadrícula
    for i in range(height):
        for j in range(width):
            rA = x[j]
            rB = y[i]
            r = [rA if s == 'A' else rB for s in seq]
            xn = xn_
            lyapunov_exp = exp
            for k in range(iterations):
                xn = r[k % len(r)] * xn * (1 - xn)
                lyapunov_exp += np.log(abs(r[k % len(r)] * (1 - 2 * xn)))
            lyap[i, j] = lyapunov_exp / iterations

    # Normalizar los valores del exponente de Lyapunov para la visualización
    lyap = np.where(lyap > 0, 1, 0)
    return lyap

# Parámetros
seq = "AB"  # Secuencia de crecimiento
x_min, x_max = 2.5, 4.0
y_min, y_max = 2.5, 4.0
width, height = 600, 600
iterations = 10

output_dir = "ASM - Taller/Result2"  # Ruta de salida corregida
os.makedirs(output_dir, exist_ok=True)  # Crear el directorio de salida si no existe

for frame in range(len(iteraciones)):
    lyap = lyapunov_fractal(seq, x_min, x_max, y_min, y_max, width, height,
                            round(abs(exp[frame % len(iteraciones)]), 2),
                            xn[frame % len(iteraciones)],
                            iteraciones[frame % len(iteraciones)])
    
    # Crear la figura y guardar la imagen sin bordes
    plt.imshow(lyap, cmap='hot', extent=[x_min, x_max, y_min, y_max])
    plt.axis('off')
    output_image = os.path.join(output_dir, f'fractal_{frame:03d}.png')
    plt.savefig(output_image, bbox_inches='tight', pad_inches=0)
    plt.close()

# Crear una lista de archivos de imagen
image_files = [os.path.join(output_dir, f'fractal_{i:03d}.png') for i in range(len(iteraciones))]

# Crear el videoclip a partir de las imágenes
clip = mp.ImageSequenceClip(image_files, fps=1/rest_time)

# Guardar el video usando parámetros adicionales
output_video = os.path.join(output_dir, 'fractal_animation.mp4')
clip.write_videofile(output_video, codec='libx264', audio_codec='libmp3lame', audio="ASM - Taller/Espectrograma/download.mp3")

