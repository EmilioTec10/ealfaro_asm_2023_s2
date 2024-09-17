import numpy as np
import matplotlib.pyplot as plt

def lyapunov_fractal(seq, x_min, x_max, y_min, y_max, width, height, iterations):
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

# Generar el fractal
lyap = lyapunov_fractal(seq, x_min, x_max, y_min, y_max, width, height, iterations)

# Visualizar el fractal
plt.imshow(lyap, cmap='hot', extent=[x_min, x_max, y_min, y_max])
plt.colorbar()
plt.title("Fractal de Lyapunov")
plt.xlabel("rA")
plt.ylabel("rB")
plt.show()
