import numpy as np
import matplotlib.pyplot as plt

N = 40  # Mida de la taula que vas posar al .orc
taula = np.sin(2 * np.pi * np.arange(N) / N)

step = 2.5 
fases = np.arange(0, N, step)
senyal_generada = []

for f in fases:
    index = int(f + 0.5) % N
    senyal_generada.append(taula[index])

# Creació del gràfic
plt.figure(figsize=(10, 5))

# 1. Dibuixem la taula com a referència (línia suau)
plt.plot(np.arange(N), taula, '--', color='gray', alpha=0.5, label='Contingut de la Taula')

# 2. Dibuixem els valors de la taula (punts buits)
plt.scatter(np.arange(N), taula, facecolors='none', edgecolors='blue', label='Valors de la Taula')

# 3. Dibuixem la senyal generada (LAS PELOTITAS)
plt.scatter(fases, senyal_generada, color='red', s=50, label='Senyal Generada')

plt.title("Visualització de la Síntesi per Taula (InstrumentSeno)")
plt.xlabel("Índex de la Taula / Temps")
plt.ylabel("Amplitud")
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()