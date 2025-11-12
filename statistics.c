#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"

/* ============================================
   OPCIÓN A: SISTEMA DE HORARIOS
   ============================================ */

int is_route_available(Graph* graph, int src, int dest, int hour) {
    if (src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) return 0;
    
    AdjNode* adj = graph->array[src].head;
    while (adj != NULL) {
        if (adj->vertex == dest) {
            if (hour >= adj->schedule.start_hour && 
                hour <= adj->schedule.end_hour) {
                return 1;
            }
            return 0;
        }
        adj = adj->next;
    }
    return 0;
}

int calculate_wait_time(Schedule* schedule, int current_hour) {
    if (current_hour < schedule->start_hour) {
        return (schedule->start_hour - current_hour) * 60;
    }
    if (current_hour > schedule->end_hour) {
        return -1; // Fuera de servicio
    }
    return schedule->frequency_min / 2; // Tiempo promedio de espera
}

/* ============================================
   OPCIÓN C: ESTADÍSTICAS DE RED
   ============================================ */

void print_station_degrees(Graph* graph) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              GRADOS DE LAS ESTACIONES                      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    for (int i = 0; i < graph->num_vertices; i++) {
        int degree = 0;
        AdjNode* adj = graph->array[i].head;
        while (adj != NULL) {
            degree++;
            adj = adj->next;
        }
        printf("%-35s: %d conexiones\n", graph->vertex_names[i], degree);
    }
    
    printf("\n════════════════════════════════════════════════════════════\n");
}

void find_hub_stations(Graph* graph, int top_n) {
    int* degrees = (int*)calloc(graph->num_vertices, sizeof(int));
    
    // Calcular grados
    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode* adj = graph->array[i].head;
        while (adj != NULL) {
            degrees[i]++;
            adj = adj->next;
        }
    }
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              TOP %d ESTACIONES HUB                          ║\n", top_n);
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Encontrar top N
    for (int rank = 0; rank < top_n && rank < graph->num_vertices; rank++) {
        int max_degree = -1;
        int max_index = -1;
        
        for (int i = 0; i < graph->num_vertices; i++) {
            if (degrees[i] > max_degree) {
                max_degree = degrees[i];
                max_index = i;
            }
        }
        
        if (max_index != -1) {
            printf("   %d. %-35s: %d conexiones\n", 
                   rank + 1, graph->vertex_names[max_index], max_degree);
            degrees[max_index] = -1; // Marcar como usado
        }
    }
    
    printf("\n════════════════════════════════════════════════════════════\n");
    
    free(degrees);
}

void dfs_util(Graph* graph, int vertex, int* visited) {
    visited[vertex] = 1;
    
    AdjNode* neighbor = graph->array[vertex].head;
    while (neighbor != NULL) {
        if (!visited[neighbor->vertex]) {
            dfs_util(graph, neighbor->vertex, visited);
        }
        neighbor = neighbor->next;
    }
}

int is_network_connected(Graph* graph) {
    if (graph->is_directed) {
        return 0; // No verificamos conectividad en grafos dirigidos
    }
    
    int* visited = (int*)calloc(graph->num_vertices, sizeof(int));
    dfs_util(graph, 0, visited);
    
    int connected = 1;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (!visited[i]) {
            connected = 0;
            break;
        }
    }
    
    free(visited);
    return connected;
}

float average_network_distance(Graph* graph) {
    int total_distance = 0;
    int total_pairs = 0;
    
    // BFS desde cada vértice
    for (int src = 0; src < graph->num_vertices; src++) {
        int* visited = (int*)calloc(graph->num_vertices, sizeof(int));
        int* distance = (int*)malloc(graph->num_vertices * sizeof(int));
        
        for (int i = 0; i < graph->num_vertices; i++) {
            distance[i] = -1;
        }
        
        // Cola simple
        int queue[MAX_VERTICES];
        int front = 0, rear = 0;
        
        visited[src] = 1;
        distance[src] = 0;
        queue[rear++] = src;
        
        while (front < rear) {
            int current = queue[front++];
            
            AdjNode* neighbor = graph->array[current].head;
            while (neighbor != NULL) {
                if (!visited[neighbor->vertex]) {
                    visited[neighbor->vertex] = 1;
                    distance[neighbor->vertex] = distance[current] + 1;
                    queue[rear++] = neighbor->vertex;
                }
                neighbor = neighbor->next;
            }
        }
        
        // Sumar distancias
        for (int dest = 0; dest < graph->num_vertices; dest++) {
            if (dest != src && distance[dest] != -1) {
                total_distance += distance[dest];
                total_pairs++;
            }
        }
        
        free(visited);
        free(distance);
    }
    
    if (total_pairs == 0) return 0.0;
    return (float)total_distance / total_pairs;
}

void print_network_statistics(Graph* graph) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              ESTADÍSTICAS DE LA RED                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Total de estaciones
    printf("📍 Total de estaciones: %d\n", graph->num_vertices);
    
    // Total de conexiones
    int total_connections = 0;
    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode* adj = graph->array[i].head;
        while (adj != NULL) {
            total_connections++;
            adj = adj->next;
        }
    }
    if (!graph->is_directed) {
        total_connections /= 2; // Contar cada arista una vez
    }
    printf("🔗 Total de conexiones: %d\n\n", total_connections);
    
    // Top 3 hubs
    int* degrees = (int*)calloc(graph->num_vertices, sizeof(int));
    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode* adj = graph->array[i].head;
        while (adj != NULL) {
            degrees[i]++;
            adj = adj->next;
        }
    }
    
    printf("🏆 Top 3 Estaciones Hub:\n");
    for (int rank = 0; rank < 3 && rank < graph->num_vertices; rank++) {
        int max_degree = -1;
        int max_index = -1;
        
        for (int i = 0; i < graph->num_vertices; i++) {
            if (degrees[i] > max_degree) {
                max_degree = degrees[i];
                max_index = i;
            }
        }
        
        if (max_index != -1) {
            printf("   %d. %s: %d conexiones\n", 
                   rank + 1, graph->vertex_names[max_index], max_degree);
            degrees[max_index] = -1;
        }
    }
    free(degrees);
    
    // Conectividad
    printf("\n🔄 Red completamente conectada: %s\n", 
           is_network_connected(graph) ? "SÍ" : "NO");
    
    // Distancia promedio
    float avg_dist = average_network_distance(graph);
    printf("📏 Distancia promedio: %.1f estaciones\n", avg_dist);
    
    // Tiempo promedio (estimado)
    printf("⏱️  Tiempo promedio: %.0f minutos\n", avg_dist * 9); // ~9 min por estación
    
    printf("\n════════════════════════════════════════════════════════════\n");
}
