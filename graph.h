#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NAME 50
#define MAX_LINE 20
#define MAX_VERTICES 100
#define INF INT_MAX

/* ============================================
   ESTRUCTURAS DE DATOS
   ============================================ */

// Información de horarios
typedef struct Schedule {
    int start_hour;          // Hora inicio (0-23)
    int end_hour;            // Hora fin (0-23)
    int frequency_min;       // Frecuencia en minutos
} Schedule;

// Nodo en la lista de adyacencia
typedef struct AdjNode {
    int vertex;              // Índice del vértice destino
    int weight;              // Tiempo en minutos
    int cost;                // Costo en pesos colombianos
    char line[MAX_LINE];     // Línea de transporte
    Schedule schedule;       // Horarios de operación
    struct AdjNode* next;
} AdjNode;

// Lista de adyacencia
typedef struct AdjList {
    AdjNode* head;
} AdjList;

// Estructura del grafo
typedef struct Graph {
    int num_vertices;
    int is_directed;
    char** vertex_names;
    AdjList* array;
} Graph;

// Estructura para rutas alternativas
typedef struct Path {
    int* vertices;           // Array de índices de estaciones
    int length;              // Número de estaciones
    int total_time;          // Tiempo total en minutos
    int total_cost;          // Costo total en pesos
    int transfers;           // Número de transbordos
    char** lines;            // Líneas utilizadas
} Path;

/* ============================================
   FUNCIONES DEL GRAFO
   ============================================ */

Graph* create_graph(int num_vertices, int is_directed);
void set_vertex_name(Graph* graph, int vertex, const char* name);
void add_edge_with_cost(Graph* graph, int src, int dest, int time, int cost, const char* line);
void add_edge_with_schedule(Graph* graph, int src, int dest, int time, int cost, 
                           const char* line, int start_h, int end_h, int freq);
void print_graph(Graph* graph);
void free_graph(Graph* graph);
void load_pereira_megabus(Graph* graph);

/* ============================================
   ALGORITMOS
   ============================================ */

void dijkstra_by_cost(Graph* graph, int src, int dest);
void dijkstra_by_time(Graph* graph, int src, int dest);
void bfs_min_transfers(Graph* graph, int src, int dest);

/* ============================================
   FUNCIONALIDADES ADICIONALES
   ============================================ */

// Opción A: Sistema de horarios
int is_route_available(Graph* graph, int src, int dest, int hour);
int calculate_wait_time(Schedule* schedule, int current_hour);

// Opción C: Estadísticas de red
void print_station_degrees(Graph* graph);
void find_hub_stations(Graph* graph, int top_n);
int is_network_connected(Graph* graph);
float average_network_distance(Graph* graph);
void print_network_statistics(Graph* graph);

/* ============================================
   UTILIDADES
   ============================================ */

int find_station_by_name(Graph* graph, const char* name);
void print_menu();

#endif // GRAPH_H
