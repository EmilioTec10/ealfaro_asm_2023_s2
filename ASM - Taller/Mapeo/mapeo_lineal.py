import numpy as np
import matplotlib.pyplot as plt

def linear_mapping(a, b, xlim=(-2, 2), ylim=(-2, 2), resolution=400):
    """
    Realiza el mapeo lineal w = az + b y grafica el resultado con puntos.
    
    Parámetros:
    - a: constante compleja que multiplica z.
    - b: constante compleja que suma al resultado de az.
    - xlim: tupla con los límites del eje x (parte real).
    - ylim: tupla con los límites del eje y (parte imaginaria).
    - resolution: número de puntos en cada dimensión.
    """
    # Crear una malla de valores en el plano complejo
    real = np.linspace(xlim[0], xlim[1], resolution)
    imag = np.linspace(ylim[0], ylim[1], resolution)
    X, Y = np.meshgrid(real, imag)
    Z = X + 1j * Y  # Z = x + iy, números complejos en la malla

    # Aplicar el mapeo lineal w = az + b
    W = a * Z + b
    
    # Separar la parte real e imaginaria de w
    W_real = np.real(W)
    W_imag = np.imag(W)

    # Crear la gráfica
    plt.figure(figsize=(12, 6))

    # Graficar los puntos del plano complejo original
    plt.subplot(1, 2, 1)
    plt.scatter(X, Y, s=1, color='blue', label="Original")
    plt.title("Plano Complejo Original")
    plt.xlabel("Parte Real de z")
    plt.ylabel("Parte Imaginaria de z")
    plt.grid(True)

    # Graficar los puntos transformados
    plt.subplot(1, 2, 2)
    plt.scatter(W_real, W_imag, s=1, color='red', label="Transformado")
    plt.title("Plano Complejo Transformado")
    plt.xlabel("Parte Real de w")
    plt.ylabel("Parte Imaginaria de w")
    plt.grid(True)

    plt.tight_layout()
    plt.show()

# Ejemplo: Mapeo lineal w = 2z + 1 + i
a = 2 + 1j  # Constante compleja a
b = 1 + 1j  # Constante compleja b

# Llamar a la función con el mapeo lineal
linear_mapping(a, b)
