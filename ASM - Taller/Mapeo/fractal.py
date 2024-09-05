import numpy as np

import matplotlib.pyplot as plt

def newton_fractal(x, y, max_iter=100, threshold=1e-6):
    z = x + 1j * y
    for i in range(max_iter):
        z = z - (z**4 - 1) / (4 * z**3)
        if abs(z**4 - 1) < threshold:
            return i
    return max_iter

#Rangos del plano complejo
x = np.linspace(-2, 2, 1000)
y = np.linspace(-2, 2, 1000)
 
# Create a grid of complex numbers
X, Y = np.meshgrid(x, y)

# Calculo del fractal
fractal = np.vectorize(newton_fractal)(X, Y)

#Mostrado del fractal
plt.imshow(fractal, cmap='hot', extent=(-2, 2, -2, 2))
plt.colorbar()
plt.title("Newton Fractal")
plt.xlabel("Re(c)")
plt.ylabel("Im(c)")
plt.show()