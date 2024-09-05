import numpy as np
import matplotlib.pyplot as plt

def mapeo_cuadratico(z):

    # Aplicar el mapeo w = z^2
    w = z ** 2

    # Graficar los puntos antes y después del mapeo
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6))

    # Graficar el plano complejo original
    ax1.contour(x, y, np.real(z), levels=30, colors='blue', linestyles='dotted')
    ax1.contour(x, y, np.imag(z), levels=30, colors='red', linestyles='dotted')
    ax1.set_title('Plano Complejo Z')
    ax1.set_xlabel('Re(z)')
    ax1.set_ylabel('Im(z)')
    ax1.grid(True)

    # Graficar el plano transformado w = z^2
    ax2.contour(np.real(w), np.imag(w), np.real(w), levels=30, colors='blue', linestyles='dotted')
    ax2.contour(np.real(w), np.imag(w), np.imag(w), levels=30, colors='red', linestyles='dotted')
    ax2.set_title('Plano Transformado W = Z^2')
    ax2.set_xlabel('Re(w)')
    ax2.set_ylabel('Im(w)')
    ax2.grid(True)

    plt.tight_layout()
    plt.show()

# Definir el rango para el plano complejo
real = np.linspace(-2, 2, 400)
imag = np.linspace(-2, 2, 400)
# Crear una cuadrícula de números complejos z = x + iy
x, y = np.meshgrid(real, imag)
z = x + 1j * y 
mapeo_cuadratico(z);