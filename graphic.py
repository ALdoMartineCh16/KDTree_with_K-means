import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from itertools import cycle
from matplotlib.legend_handler import HandlerPathCollection

# Configura el estilo de seaborn
sns.set(style='whitegrid', palette='Paired')  # Cambiado a la paleta 'Paired'

# Lee el archivo y carga los datos de los clusters
clusters = []
centroids = []
radii = []  # Lista para almacenar los radios de los círculos
current_cluster = None

with open('pruebas.txt', 'r') as file:
    for line in file:
        line = line.strip()
        if line.startswith('clusters_'):
            if current_cluster is not None:
                clusters.append(current_cluster)
                # Calcula el centroide y agrega a la lista de centroides
                centroid_x = sum(x for x, _ in current_cluster) / len(current_cluster)
                centroid_y = sum(y for _, y in current_cluster) / len(current_cluster)
                centroids.append((centroid_x, centroid_y))
                # Calcula el radio del círculo como la máxima distancia desde el centroide
                radii.append(max(np.linalg.norm(np.array(point) - np.array([centroid_x, centroid_y])) for point in current_cluster))
            current_cluster = []
        elif line and line.startswith('cluster_'):
            pass  # Ignora las líneas que comienzan con 'cluster_'
        elif line:
            x, y = map(float, line.split(','))
            if current_cluster is not None:  # Verifica que current_cluster esté inicializado
                current_cluster.append((x, y))

# Agrega el último cluster
if current_cluster is not None:
    clusters.append(current_cluster)
    # Calcula el centroide y agrega a la lista de centroides
    centroid_x = sum(x for x, _ in current_cluster) / len(current_cluster)
    centroid_y = sum(y for _, y in current_cluster) / len(current_cluster)
    centroids.append((centroid_x, centroid_y))
    # Calcula el radio del círculo como la máxima distancia desde el centroide
    radii.append(max(np.linalg.norm(np.array(point) - np.array([centroid_x, centroid_y])) for point in current_cluster))

# Tamaño más pequeño de los puntos
size = 10

# Grafica cada cluster con un color diferente y tamaño más pequeño
scatter_objects = []
for cluster, color in zip(clusters, cycle(sns.color_palette())):
    cluster = list(zip(*cluster))  # Transpone los puntos para separar x y y
    scatter = plt.scatter(*cluster, c=color, s=size, label="")  # Etiqueta vacía para la leyenda
    scatter_objects.append(scatter)

# Grafica los centroides con un marcador especial
centroids_x, centroids_y = zip(*centroids)
plt.scatter(centroids_x, centroids_y, c='black', marker='X', s=100, label='Centroides')

# Grafica círculos alrededor de cada cluster
for (x, y), radius in zip(centroids, radii):
    circle = plt.Circle((x, y), radius, color='black', fill=False, linestyle='dashed', linewidth=2)
    plt.gca().add_artist(circle)

# Configura el gráfico
plt.xlabel('Eje X')
plt.ylabel('Eje Y')
plt.title('Clusters obtenidos con KMeans')

# Crea una leyenda personalizada
legend_labels = [f'Cluster {i}' for i in range(len(clusters))] + ['Centroides']
legend = plt.legend(legend_labels, loc='center left', bbox_to_anchor=(1, 0.5))

# Función para manejar eventos de clic en la leyenda
def on_legend_click(event):
    if event.artist is not None and isinstance(event.artist, HandlerPathCollection):
        # Encuentra el índice del punto haciendo clic en la leyenda
        index = legend_labels.index(event.artist.get_label())
        # Aumenta el tamaño del punto en el índice correspondiente
        if index < len(clusters):
            scatter_objects[index].set_sizes([4 * size])
        else:
            # Si se hace clic en 'Centroides', aumenta todos los tamaños de puntos de los clusters
            for scatter in scatter_objects:
                scatter.set_sizes([4 * size])

# Conecta el manejador de eventos
plt.gcf().canvas.mpl_connect('pick_event', on_legend_click)

plt.show()
