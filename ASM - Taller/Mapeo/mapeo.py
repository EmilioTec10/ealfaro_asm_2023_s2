import numpy as np
import matplotlib.pyplot as plt
import os
import moviepy.editor as mp
import audio_parameters


audio_path = 'ASM - Taller/Espectrograma/download.wav'

traslaciones, roots, iteraciones, rest_time = audio_parameters.analyze_audio_in_windows(audio_path)

# Imprimir el número de ventanas analizadas y ejemplo de valores
print(f"Ejemplo de magnitudes en dB: {traslaciones[:5]}")
print(f"Ejemplo de cambio de fase en grados: {roots}")
print(f"Ejemplo de pitch: {iteraciones}")



def newton_fractal(x, y, root, traslacion, max_iter=100, threshold=1e-6):
    z = x + 1j * y
    for i in range(max_iter):
        z = z - (z**root + traslacion) / (root * z**(root-1))
        if abs(z**root + traslacion) < threshold:
            return i
    return max_iter

# # # Configuración del plano complejo
x = np.linspace(-2, 2, 800)
y = np.linspace(-2, 2, 800)
X, Y = np.meshgrid(x, y)

# # Directorio donde se guardarán las imágenes
output_dir = "ASM - Taller\Result"
os.makedirs(output_dir, exist_ok=True)

# # Lista de esquemas de colores
cmap = 'inferno'

# Generar y guardar imágenes
for frame in range(len(traslaciones)):
    
    fractal = np.vectorize(newton_fractal)(X, Y, round(abs(roots[frame % len(traslaciones)]), 2), traslaciones[frame % len(traslaciones)],max_iter=iteraciones[frame % len(traslaciones)])

    
#     # Crear la figura y guardar la imagen
    plt.imshow(fractal, cmap=cmap, extent=(-2, 2, -2, 2))
    plt.axis('off')  # No mostrar los ejes
    plt.savefig(f"{output_dir}/fractal_{frame:03d}.png", bbox_inches='tight', pad_inches=0)
    plt.close()

# # Crear el video usando moviepy
# # Crear una lista de archivos de imagen
image_files = [os.path.join(output_dir, f'fractal_{i:03d}.png') for i in range(len(traslaciones))]


# # Crear el videoclip a partir de las imágenes
clip = mp.ImageSequenceClip(image_files, fps= 1/rest_time)

# # Guardar el video usando parámetros adicionales
output_video = os.path.join(output_dir, 'fractal_animation.mp4')
clip.write_videofile(output_video, codec='libx264', audio_codec='libmp3lame', audio="ASM - Taller/Espectrograma/download.mp3")




