\# Sistema Megabús Pereira 2025



\## Autor

\- \*\*Nombre:\*\* \[FABIAN DAVID BOLIVAR VELEZ - MAYELI]

\- \*\*Código:\*\* \[1.087.551.045]

\- \*\*Universidad Tecnológica de Pereira\*\*

\- \*\*Asignatura:\*\* Estructuras de Datos



\## Descripción



Sistema completo de rutas del Megabús de Pereira implementado en C. El programa modela la red de transporte público con 15 estaciones reales, permitiendo encontrar rutas óptimas por tiempo, costo, o número de transbordos. Incluye funcionalidades adicionales de análisis de red y sistema de horarios.



\## Compilación



\### Requisitos

\- GCC 7.0 o superior

\- Sistema operativo: Linux, macOS, o Windows con WSL

\- Make (opcional pero recomendado)



\### Compilar con Makefile

```bash

make

```



\### Compilar manualmente

```bash

gcc -Wall -Wextra -std=c11 -g main.c graph.c algorithms.c statistics.c -o megabus

```



\### Limpiar archivos compilados

```bash

make clean

```



\## Ejecución

```bash

./megabus

```



\## Funcionalidades Implementadas



\### ✅ Parte 1: Extensiones Básicas (60 puntos)



1\. \*\*Dijkstra por Costo\*\* (20 puntos)

&nbsp;  - Encuentra la ruta más económica entre dos estaciones

&nbsp;  - Considera tarifas diferenciadas (troncal $2,700, alimentador $1,800)

&nbsp;  - Muestra detalles de costos y transbordos gratuitos



2\. \*\*BFS con Menos Transbordos\*\* (20 puntos)

&nbsp;  - Minimiza el número de cambios de línea

&nbsp;  - Identifica claramente los puntos de transbordo

&nbsp;  - Muestra el itinerario completo con líneas utilizadas



3\. \*\*Dataset de Pereira\*\* (20 puntos)

&nbsp;  - 15 estaciones reales del sistema Megabús

&nbsp;  - Tiempos realistas entre estaciones (5-20 minutos)

&nbsp;  - Múltiples líneas: Línea 1-5, Rutas A-I

&nbsp;  - Red completamente conectada



\### ✅ Parte 2: Funcionalidades Adicionales (40 puntos)



4\. \*\*Sistema de Horarios\*\* (20 puntos)

&nbsp;  - Verifica disponibilidad de rutas según hora del día

&nbsp;  - Calcula tiempos de espera promedio

&nbsp;  - Horarios diferenciados por tipo de línea



5\. \*\*Estadísticas de Red\*\* (20 puntos)

&nbsp;  - Análisis completo de la topología de red

&nbsp;  - Identificación de estaciones hub

&nbsp;  - Verificación de conectividad total

&nbsp;  - Cálculo de distancias y tiempos promedios



\## Estructuras de Datos



\### Grafo Principal

```c

typedef struct Graph {

&nbsp;   int num\_vertices;        // 15 estaciones

&nbsp;   int is\_directed;         // 0 = no dirigido

&nbsp;   char\*\* vertex\_names;     // Nombres de estaciones

&nbsp;   AdjList\* array;          // Listas de adyacencia

} Graph;

```



\### Nodo de Adyacencia

```c

typedef struct AdjNode {

&nbsp;   int vertex;              // Estación destino

&nbsp;   int weight;              // Tiempo en minutos

&nbsp;   int cost;                // Costo en pesos

&nbsp;   char line\[MAX\_LINE];     // Línea de transporte

&nbsp;   Schedule schedule;       // Horarios

&nbsp;   struct AdjNode\* next;

} AdjNode;

```



\## Complejidad



| Función | Complejidad Temporal | Complejidad Espacial |

|---------|---------------------|---------------------|

| `dijkstra\_by\_cost` | O((V+E) log V) | O(V) |

| `dijkstra\_by\_time` | O((V+E) log V) | O(V) |

| `bfs\_min\_transfers` | O(V+E) | O(V) |

| `is\_network\_connected` | O(V+E) | O(V) |

| `average\_network\_distance` | O(V(V+E)) | O(V) |

| `find\_hub\_stations` | O(V²) | O(V) |



\*\*Donde:\*\* V = 15 vértices, E = 44 aristas



\## Estaciones del Sistema



0\. Terminal de Transporte

1\. Plaza de Bolívar

2\. Cuba

3\. Universidad Tecnológica

4\. Hospital San Jorge

5\. Dosquebradas Centro

6\. Aeropuerto Matecaña

7\. Cerritos

8\. Estadio Hernán Ramírez

9\. La Popa

10\. Parque Arboleda

11\. Álamos

12\. CC Palo de Agua

13\. Unicentro

14\. El Poblado



\## Ejemplos de Uso



\### Ejemplo 1: Ruta más económica

```

Opción: 3

Origen: 0 (Terminal)

Destino: 4 (Hospital San Jorge)



Resultado:

💰 Costo total: $2,700

⏱️  Tiempo estimado: 38 minutos

🚍 Ruta: Terminal → Plaza → UTP → Hospital

```



\### Ejemplo 2: Menos transbordos

```

Opción: 4

Origen: 6 (Aeropuerto)

Destino: 5 (Dosquebradas)



Resultado:

🔄 Transbordos: 1

📍 Estaciones: 4

```



\## Verificación de Memory Leaks

```bash

make valgrind

```



\*\*Resultado esperado:\*\*

```

All heap blocks were freed -- no leaks are possible

```



\## Limitaciones Conocidas



1\. Sistema limitado a 100 estaciones (MAX\_VERTICES)

2\. Solo implementa la ruta óptima (no 3 alternativas)

3\. No incluye persistencia en archivos CSV

4\. Requiere índices numéricos (0-14) en lugar de nombres



\## Referencias



\- Cormen, T. H., et al. (2009). Introduction to Algorithms

\- Weiss, M. A. (1996). Data Structures and Algorithm Analysis in C



---



\*\*Desarrollado para el curso de Estructuras de Datos - UTP 2025\*\*

