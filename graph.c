#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"

/* ============================================
   CREACIÓN DEL GRAFO
   ============================================ */

Graph* create_graph(int num_vertices, int is_directed) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) return NULL;
    
    graph->num_vertices = num_vertices;
    graph->is_directed = is_directed;
    
    graph->vertex_names = (char**)malloc(num_vertices * sizeof(char*));
    for (int i = 0; i < num_vertices; i++) {
        graph->vertex_names[i] = (char*)malloc(MAX_NAME * sizeof(char));
        sprintf(graph->vertex_names[i], "Estacion_%d", i);
    }
    
    graph->array = (AdjList*)malloc(num_vertices * sizeof(AdjList));
    for (int i = 0; i < num_vertices; i++) {
        graph->array[i].head = NULL;
    }
    
    return graph;
}

void set_vertex_name(Graph* graph, int vertex, const char* name) {
    if (vertex < 0 || vertex >= graph->num_vertices) return;
    strcpy(graph->vertex_names[vertex], name);
}

AdjNode* create_adj_node(int vertex, int weight, int cost, const char* line,
                         int start_h, int end_h, int freq) {
    AdjNode* node = (AdjNode*)malloc(sizeof(AdjNode));
    if (node == NULL) return NULL;
    
    node->vertex = vertex;
    node->weight = weight;
    node->cost = cost;
    strcpy(node->line, line);
    node->schedule.start_hour = start_h;
    node->schedule.end_hour = end_h;
    node->schedule.frequency_min = freq;
    node->next = NULL;
    
    return node;
}

void add_edge_with_cost(Graph* graph, int src, int dest, int time, int cost, const char* line) {
    add_edge_with_schedule(graph, src, dest, time, cost, line, 5, 23, 10);
}

void add_edge_with_schedule(Graph* graph, int src, int dest, int time, int cost,
                           const char* line, int start_h, int end_h, int freq) {
    if (src < 0 || src >= graph->num_vertices || 
        dest < 0 || dest >= graph->num_vertices) {
        return;
    }
    
    AdjNode* node = create_adj_node(dest, time, cost, line, start_h, end_h, freq);
    node->next = graph->array[src].head;
    graph->array[src].head = node;
    
    if (!graph->is_directed) {
        node = create_adj_node(src, time, cost, line, start_h, end_h, freq);
        node->next = graph->array[dest].head;
        graph->array[dest].head = node;
    }
}

void print_graph(Graph* graph) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║          RED COMPLETA MEGABÚS PEREIRA 2025                ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    printf("Tipo: %s\n", graph->is_directed ? "Dirigido" : "No Dirigido");
    printf("Total estaciones: %d\n\n", graph->num_vertices);
    
    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode* current = graph->array[i].head;
        printf("[%2d] %-30s -> ", i, graph->vertex_names[i]);
        
        if (current == NULL) {
            printf("(sin conexiones)\n");
            continue;
        }
        
        int count = 0;
        while (current != NULL) {
            if (count > 0) printf("                                      ");
            printf("%s (%d min, $%d, %s)\n",
                   graph->vertex_names[current->vertex],
                   current->weight,
                   current->cost,
                   current->line);
            current = current->next;
            count++;
        }
    }
    printf("\n════════════════════════════════════════════════════════════\n");
}

void load_pereira_megabus(Graph* graph) {
    // Asignar nombres de estaciones reales de Pereira
    set_vertex_name(graph, 0, "Terminal de Transporte");
    set_vertex_name(graph, 1, "Plaza de Bolivar");
    set_vertex_name(graph, 2, "Cuba");
    set_vertex_name(graph, 3, "Universidad Tecnologica");
    set_vertex_name(graph, 4, "Hospital San Jorge");
    set_vertex_name(graph, 5, "Dosquebradas Centro");
    set_vertex_name(graph, 6, "Aeropuerto Matecana");
    set_vertex_name(graph, 7, "Cerritos");
    set_vertex_name(graph, 8, "Estadio Hernan Ramirez");
    set_vertex_name(graph, 9, "La Popa");
    set_vertex_name(graph, 10, "Parque Arboleda");
    set_vertex_name(graph, 11, "Alamos");
    set_vertex_name(graph, 12, "CC Palo de Agua");
    set_vertex_name(graph, 13, "Unicentro");
    set_vertex_name(graph, 14, "El Poblado");
    
    // Línea 1 (Terminal - Cuba - Centro - Aeropuerto)
    add_edge_with_schedule(graph, 0, 1, 8, 2700, "Linea 1", 5, 23, 8);
    add_edge_with_schedule(graph, 1, 2, 10, 2700, "Linea 1", 5, 23, 8);
    add_edge_with_schedule(graph, 2, 6, 15, 2700, "Linea 1", 5, 23, 8);
    
    // Línea 2 (Dosquebradas - Plaza - UTP - Hospital)
    add_edge_with_schedule(graph, 5, 1, 12, 2700, "Linea 2", 5, 23, 8);
    add_edge_with_schedule(graph, 1, 3, 10, 2700, "Linea 2", 5, 23, 8);
    add_edge_with_schedule(graph, 3, 4, 8, 2700, "Linea 2", 5, 23, 8);
    
    // Ruta A (Cuba - Álamos - Estadio)
    add_edge_with_schedule(graph, 2, 11, 7, 1800, "Ruta A", 6, 22, 15);
    add_edge_with_schedule(graph, 11, 8, 6, 1800, "Ruta A", 6, 22, 15);
    
    // Ruta B (Centro - Parque Arboleda - Cerritos)
    add_edge_with_schedule(graph, 1, 10, 9, 1800, "Ruta B", 6, 22, 15);
    add_edge_with_schedule(graph, 10, 7, 8, 1800, "Ruta B", 6, 22, 15);
    
    // Ruta C (La Popa - Palo de Agua - Unicentro)
    add_edge_with_schedule(graph, 9, 12, 10, 1800, "Ruta C", 6, 22, 15);
    add_edge_with_schedule(graph, 12, 13, 12, 1800, "Ruta C", 6, 22, 15);
    
    // Conexiones adicionales para mayor conectividad
    add_edge_with_schedule(graph, 2, 3, 12, 2700, "Linea 3", 5, 23, 10);
    add_edge_with_schedule(graph, 8, 9, 8, 1800, "Ruta D", 6, 22, 15);
    add_edge_with_schedule(graph, 4, 9, 10, 1800, "Ruta E", 6, 22, 15);
    add_edge_with_schedule(graph, 7, 13, 11, 1800, "Ruta F", 6, 22, 15);
    add_edge_with_schedule(graph, 13, 14, 9, 1800, "Ruta G", 6, 22, 15);
    add_edge_with_schedule(graph, 10, 14, 15, 1800, "Ruta H", 6, 22, 15);
    add_edge_with_schedule(graph, 6, 7, 18, 2700, "Linea 4", 5, 23, 10);
    add_edge_with_schedule(graph, 0, 5, 20, 2700, "Linea 5", 5, 23, 10);
    add_edge_with_schedule(graph, 3, 11, 9, 1800, "Ruta I", 6, 22, 15);
}

int find_station_by_name(Graph* graph, const char* name) {
    for (int i = 0; i < graph->num_vertices; i++) {
        if (strstr(graph->vertex_names[i], name) != NULL) {
            return i;
        }
    }
    return -1;
}

void free_graph(Graph* graph) {
    if (graph == NULL) return;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode* current = graph->array[i].head;
        while (current != NULL) {
            AdjNode* temp = current;
            current = current->next;
            free(temp);
        }
        free(graph->vertex_names[i]);
    }
    
    free(graph->vertex_names);
    free(graph->array);
    free(graph);
}
