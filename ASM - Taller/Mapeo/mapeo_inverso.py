import numpy as np
import matplotlib.pyplot as plt

#Se tomo ayuda de https://chatgpt.com/
def inverse_mapping(Z):
    """
    Realiza el mapeo inverso w = 1 / z y grafica el resultado con puntos.
    
    Parámetros:
    - xlim: tupla con los límites del eje x (parte real).
    - ylim: tupla con los límites del eje y (parte imaginaria).
    - resolution: número de puntos en cada dimensión.
    """

    # Evitar la división por cero en el punto (0, 0)
    Z[Z == 0] = 1e-10
    
    # Aplicar el mapeo inverso w = 1 / z
    W = 1 / Z
    
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
    plt.title("Plano Complejo Transformado (Inverso)")
    plt.xlabel("Parte Real de w")
    plt.ylabel("Parte Imaginaria de w")
    plt.grid(True)

    plt.tight_layout()
    plt.show()

# Crear una malla de valores en el plano complej
xlim=(-2, 2) 
ylim=(-2, 2)
resolution=400

real = np.linspace(xlim[0], xlim[1], resolution)
imag = np.linspace(ylim[0], ylim[1], resolution)
X, Y = np.meshgrid(real, imag)
Z = X + 1j * Y  # Z = x + iy, números complejos en la malla

# Llamar a la función para graficar el mapeo inverso
inverse_mapping(Z)
