import matplotlib.pyplot as plt

# Datos proporcionados
k_values = [5, 15, 25, 50, 75,100,200]
knn_times = [51.312, 105.805, 155.662, 289.308, 403.365,523.87,1040.39 ]
brute_times = [67.34, 145.695, 211.365, 370.036, 496.641,657.562 ,1250.76]

# Crear el gráfico
plt.plot(k_values, knn_times, marker='o', label='knn')
plt.plot(k_values, brute_times, marker='o', label='brute')

# Configuración del gráfico
plt.title('Tiempo de compilación en función de K')
plt.xlabel('Número de clusters (K)')
plt.ylabel('Tiempo (s)')
plt.legend()
plt.grid(True)

# Mostrar el gráfico
plt.show()
