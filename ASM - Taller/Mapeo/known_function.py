import numpy as np
import matplotlib.pyplot as plt

def lyapunov_fractal_sine(seq, x_min, x_max, y_min, y_max, width, height, iterations):
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
            
            xn = 0.5
            lyapunov_exp = 0
            for k in range(iterations):
                # Usar función seno en lugar de la logística
                xn = np.sin(r[k % len(r)] * np.pi * xn)
                # Derivada de la función seno para el exponente de Lyapunov
                deriv = r[k % len(r)] * np.pi * np.cos(r[k % len(r)] * np.pi * xn)
                # Evitar problemas de log(0) o valores negativos
                if deriv != 0:
                    lyapunov_exp += np.log(abs(deriv))

            lyap[i, j] = lyapunov_exp / iterations

    # Normalizar los valores para la visualización
    # Ajustar para que el rango esté entre -2 y 2 (puedes experimentar con estos límites)
    lyap = np.clip(lyap, -2, 2)
    
    # Normalizar entre 0 y 1 para la visualización
    lyap_norm = (lyap - lyap.min()) / (lyap.max() - lyap.min())
    return lyap_norm

# Parámetros
seq = "AB"  # Secuencia de crecimiento
x_min, x_max = 2.5, 4.0
y_min, y_max = 2.5, 4.0
width, height = 600, 600
iterations = 100  # Aumentar el número de iteraciones para más detalles

# Generar el fractal
lyap = lyapunov_fractal_sine(seq, x_min, x_max, y_min, y_max, width, height, iterations)

# Visualizar el fractal
plt.imshow(lyap, cmap='inferno', extent=[x_min, x_max, y_min, y_max])  # Usar un mapa de color más adecuado
plt.colorbar()
plt.title("Fractal de Lyapunov con Seno")
plt.xlabel("rA")
plt.ylabel("rB")
plt.show()
