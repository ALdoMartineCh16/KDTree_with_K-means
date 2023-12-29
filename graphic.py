import matplotlib.pyplot as plt
import seaborn as sns
from itertools import cycle
from matplotlib.legend_handler import HandlerPathCollection

# Configura el estilo de seaborn
sns.set(style='whitegrid', palette='Paired')  # Cambiado a la paleta 'Paired'

# Lee el archivo y carga los datos de los clusters
clusters = []
current_cluster = None

with open('pruebas.txt', 'r') as file:
    for line in file:
        line = line.strip()
        if line.startswith('clusters_'):
            if current_cluster is not None:
                clusters.append(current_cluster)
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

# Tamaño más pequeño de los puntos
size = 10

# Grafica cada cluster con un color diferente y tamaño más pequeño
scatter_objects = []
for cluster, color in zip(clusters, cycle(sns.color_palette())):
    cluster = list(zip(*cluster))  # Transpone los puntos para separar x y y
    scatter = plt.scatter(*cluster, c=color, s=size, label="")  # Etiqueta vacía para la leyenda
    scatter_objects.append(scatter)

# Configura el gráfico
plt.xlabel('Eje X')
plt.ylabel('Eje Y')
plt.title('Clusters obtenidos con KMeans')

# Crea una leyenda personalizada
legend_labels = [f'Cluster {i}' for i in range(len(clusters))]
legend = plt.legend(legend_labels, loc='center left', bbox_to_anchor=(1, 0.5))

# Función para manejar eventos de clic en la leyenda
def on_legend_click(event):
    if event.artist is not None and isinstance(event.artist, HandlerPathCollection):
        # Encuentra el índice del punto haciendo clic en la leyenda
        index = legend_labels.index(event.artist.get_label())
        # Aumenta el tamaño del punto en el índice correspondiente
        scatter_objects[index].set_sizes([4 * size])

# Conecta el manejador de eventos
plt.gcf().canvas.mpl_connect('pick_event', on_legend_click)

plt.show()
