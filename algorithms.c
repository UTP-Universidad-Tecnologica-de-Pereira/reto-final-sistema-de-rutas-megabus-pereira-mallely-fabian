#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"

/* ============================================
   MIN-HEAP PARA DIJKSTRA
   ============================================ */

typedef struct MinHeapNode {
    int vertex;
    int distance;
} MinHeapNode;

typedef struct MinHeap {
    int size;
    int capacity;
    int* pos;
    MinHeapNode** array;
} MinHeap;

MinHeap* create_min_heap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->pos = (int*)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    return heap;
}

MinHeapNode* create_min_heap_node(int vertex, int distance) {
    MinHeapNode* node = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    node->vertex = vertex;
    node->distance = distance;
    return node;
}

void swap_min_heap_nodes(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < heap->size && 
        heap->array[left]->distance < heap->array[smallest]->distance) {
        smallest = left;
    }
    
    if (right < heap->size && 
        heap->array[right]->distance < heap->array[smallest]->distance) {
        smallest = right;
    }
    
    if (smallest != idx) {
        MinHeapNode* smallest_node = heap->array[smallest];
        MinHeapNode* idx_node = heap->array[idx];
        
        heap->pos[smallest_node->vertex] = idx;
        heap->pos[idx_node->vertex] = smallest;
        
        swap_min_heap_nodes(&heap->array[smallest], &heap->array[idx]);
        min_heapify(heap, smallest);
    }
}

int is_empty_heap(MinHeap* heap) {
    return heap->size == 0;
}

MinHeapNode* extract_min(MinHeap* heap) {
    if (is_empty_heap(heap)) return NULL;
    
    MinHeapNode* root = heap->array[0];
    MinHeapNode* last = heap->array[heap->size - 1];
    heap->array[0] = last;
    
    heap->pos[root->vertex] = heap->size - 1;
    heap->pos[last->vertex] = 0;
    
    --heap->size;
    min_heapify(heap, 0);
    
    return root;
}

void decrease_key(MinHeap* heap, int vertex, int distance) {
    int i = heap->pos[vertex];
    heap->array[i]->distance = distance;
    
    while (i && heap->array[i]->distance < heap->array[(i - 1) / 2]->distance) {
        heap->pos[heap->array[i]->vertex] = (i - 1) / 2;
        heap->pos[heap->array[(i - 1) / 2]->vertex] = i;
        swap_min_heap_nodes(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int is_in_min_heap(MinHeap* heap, int vertex) {
    return heap->pos[vertex] < heap->size;
}

/* ============================================
   DIJKSTRA POR COSTO
   ============================================ */

void print_cost_path(int* parent, int vertex, Graph* graph, int* total_time, 
                     char*** lines, int* line_count) {
    if (parent[vertex] == -1) {
        printf("%s", graph->vertex_names[vertex]);
        return;
    }
    
    print_cost_path(parent, parent[vertex], graph, total_time, lines, line_count);
    
    // Buscar la arista para obtener tiempo y línea
    AdjNode* adj = graph->array[parent[vertex]].head;
    while (adj != NULL) {
        if (adj->vertex == vertex) {
            *total_time += adj->weight;
            
            // Guardar línea
            if (*line_count < 10) {
                (*lines)[*line_count] = (char*)malloc(MAX_LINE * sizeof(char));
                strcpy((*lines)[*line_count], adj->line);
                (*line_count)++;
            }
            
            printf(" → [%s, %d min] →\n     %s", 
                   adj->line, adj->weight, graph->vertex_names[vertex]);
            break;
        }
        adj = adj->next;
    }
}

void dijkstra_by_cost(Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) return;
    
    int V = graph->num_vertices;
    int* dist = (int*)malloc(V * sizeof(int));
    int* parent = (int*)malloc(V * sizeof(int));
    
    MinHeap* heap = create_min_heap(V);
    
    for (int v = 0; v < V; v++) {
        dist[v] = INF;
        parent[v] = -1;
        heap->array[v] = create_min_heap_node(v, dist[v]);
        heap->pos[v] = v;
    }
    
    dist[src] = 0;
    decrease_key(heap, src, dist[src]);
    heap->size = V;
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              RUTA MÁS ECONÓMICA (POR COSTO)               ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    printf("Origen: %s\n", graph->vertex_names[src]);
    printf("Destino: %s\n\n", graph->vertex_names[dest]);
    
    while (!is_empty_heap(heap)) {
        MinHeapNode* min_node = extract_min(heap);
        int u = min_node->vertex;
        free(min_node);
        
        if (u == dest) break;
        
        AdjNode* neighbor = graph->array[u].head;
        while (neighbor != NULL) {
            int v = neighbor->vertex;
            
            if (is_in_min_heap(heap, v) && 
                dist[u] != INF && 
                neighbor->cost + dist[u] < dist[v]) {
                dist[v] = dist[u] + neighbor->cost;
                parent[v] = u;
                decrease_key(heap, v, dist[v]);
            }
            neighbor = neighbor->next;
        }
    }
    
    if (dist[dest] == INF) {
        printf("❌ No existe ruta entre estas estaciones\n");
    } else {
        int total_time = 0;
        char** lines = (char**)malloc(10 * sizeof(char*));
        int line_count = 0;
        
        printf("💰 Costo total: $%d\n", dist[dest]);
        printf("⏱️  Tiempo estimado: ");
        
        // Calcular tiempo
        int temp = dest;
        while (parent[temp] != -1) {
            AdjNode* adj = graph->array[parent[temp]].head;
            while (adj != NULL) {
                if (adj->vertex == temp) {
                    total_time += adj->weight;
                    break;
                }
                adj = adj->next;
            }
            temp = parent[temp];
        }
        printf("%d minutos\n\n", total_time);
        
        printf("🚍 Ruta:\n     ");
        line_count = 0;
        print_cost_path(parent, dest, graph, &total_time, &lines, &line_count);
        printf("\n\n");
        
        // Detalles de costos
        printf("📋 Detalles de costos:\n");
        char prev_line[MAX_LINE] = "";
        int segment_cost = 0;
        int is_first = 1;
        
        for (int i = 0; i < line_count; i++) {
            if (is_first || strcmp(lines[i], prev_line) != 0) {
                if (!is_first) {
                    printf("   - %s: $%d\n", prev_line, segment_cost);
                    if (strcmp(lines[i], prev_line) != 0) {
                        printf("   - Transbordo a %s: $0 (dentro de 1 hora)\n", lines[i]);
                    }
                    segment_cost = 0;
                }
                strcpy(prev_line, lines[i]);
                is_first = 0;
            }
            
            // Sumar costo del segmento
            temp = dest;
            int steps = line_count - i;
            int current_step = 0;
            while (parent[temp] != -1 && current_step < steps) {
                AdjNode* adj = graph->array[parent[temp]].head;
                while (adj != NULL) {
                    if (adj->vertex == temp && current_step == line_count - 1 - i) {
                        segment_cost = adj->cost;
                        break;
                    }
                    adj = adj->next;
                }
                temp = parent[temp];
                current_step++;
            }
        }
        if (line_count > 0) {
            printf("   - %s: $%d\n", prev_line, segment_cost);
        }
        
        for (int i = 0; i < line_count; i++) {
            free(lines[i]);
        }
        free(lines);
    }
    
    printf("\n════════════════════════════════════════════════════════════\n");
    
    for (int i = 0; i < heap->capacity; i++) {
        if (heap->array[i]) free(heap->array[i]);
    }
    free(heap->array);
    free(heap->pos);
    free(heap);
    free(dist);
    free(parent);
}

/* ============================================
   DIJKSTRA POR TIEMPO
   ============================================ */

void dijkstra_by_time(Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) return;
    
    int V = graph->num_vertices;
    int* dist = (int*)malloc(V * sizeof(int));
    int* parent = (int*)malloc(V * sizeof(int));
    
    MinHeap* heap = create_min_heap(V);
    
    for (int v = 0; v < V; v++) {
        dist[v] = INF;
        parent[v] = -1;
        heap->array[v] = create_min_heap_node(v, dist[v]);
        heap->pos[v] = v;
    }
    
    dist[src] = 0;
    decrease_key(heap, src, dist[src]);
    heap->size = V;
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              RUTA MÁS RÁPIDA (POR TIEMPO)                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    printf("Origen: %s\n", graph->vertex_names[src]);
    printf("Destino: %s\n\n", graph->vertex_names[dest]);
    
    while (!is_empty_heap(heap)) {
        MinHeapNode* min_node = extract_min(heap);
        int u = min_node->vertex;
        free(min_node);
        
        if (u == dest) break;
        
        AdjNode* neighbor = graph->array[u].head;
        while (neighbor != NULL) {
            int v = neighbor->vertex;
            
            if (is_in_min_heap(heap, v) && 
                dist[u] != INF && 
                neighbor->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + neighbor->weight;
                parent[v] = u;
                decrease_key(heap, v, dist[v]);
            }
            neighbor = neighbor->next;
        }
    }
    
    if (dist[dest] == INF) {
        printf("❌ No existe ruta entre estas estaciones\n");
    } else {
        int total_cost = 0;
        char** lines = (char**)malloc(10 * sizeof(char*));
        int line_count = 0;
        
        printf("⏱️  Tiempo total: %d minutos\n", dist[dest]);
        printf("💰 Costo estimado: $");
        
        // Calcular costo
        int temp = dest;
        while (parent[temp] != -1) {
            AdjNode* adj = graph->array[parent[temp]].head;
            while (adj != NULL) {
                if (adj->vertex == temp) {
                    total_cost += adj->cost;
                    break;
                }
                adj = adj->next;
            }
            temp = parent[temp];
        }
        printf("%d\n\n", total_cost);
        
        printf("🚍 Ruta:\n     ");
        total_cost = 0;
        print_cost_path(parent, dest, graph, &total_cost, &lines, &line_count);
        printf("\n");
        
        for (int i = 0; i < line_count; i++) {
            free(lines[i]);
        }
        free(lines);
    }
    
    printf("\n════════════════════════════════════════════════════════════\n");
    
    for (int i = 0; i < heap->capacity; i++) {
        if (heap->array[i]) free(heap->array[i]);
    }
    free(heap->array);
    free(heap->pos);
    free(heap);
    free(dist);
    free(parent);
}

/* ============================================
   BFS CON MENOS TRANSBORDOS
   ============================================ */

typedef struct QueueNode {
    int vertex;
    char line[MAX_LINE];
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

int is_empty(Queue* queue) {
    return queue->front == NULL;
}

void enqueue(Queue* queue, int vertex, const char* line) {
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    node->vertex = vertex;
    strcpy(node->line, line);
    node->next = NULL;
    
    if (queue->rear == NULL) {
        queue->front = queue->rear = node;
        return;
    }
    
    queue->rear->next = node;
    queue->rear = node;
}

QueueNode* dequeue(Queue* queue) {
    if (is_empty(queue)) return NULL;
    
    QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    return temp;
}

void bfs_min_transfers(Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) return;
    
    int* visited = (int*)calloc(graph->num_vertices, sizeof(int));
    int* parent = (int*)malloc(graph->num_vertices * sizeof(int));
    char** parent_lines = (char**)malloc(graph->num_vertices * sizeof(char*));
    
    for (int i = 0; i < graph->num_vertices; i++) {
        parent[i] = -1;
        parent_lines[i] = (char*)malloc(MAX_LINE * sizeof(char));
        strcpy(parent_lines[i], "");
    }
    
    Queue* queue = create_queue();
    visited[src] = 1;
    enqueue(queue, src, "Inicio");
    
    while (!is_empty(queue)) {
        QueueNode* current_node = dequeue(queue);
        int current = current_node->vertex;
        
        if (current == dest) {
            free(current_node);
            break;
        }
        
        AdjNode* neighbor = graph->array[current].head;
        while (neighbor != NULL) {
            if (!visited[neighbor->vertex]) {
                visited[neighbor->vertex] = 1;
                parent[neighbor->vertex] = current;
                strcpy(parent_lines[neighbor->vertex], neighbor->line);
                enqueue(queue, neighbor->vertex, neighbor->line);
            }
            neighbor = neighbor->next;
        }
        free(current_node);
    }
    
    // Liberar cola
    while (!is_empty(queue)) {
        QueueNode* node = dequeue(queue);
        free(node);
    }
    free(queue);
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║           RUTA CON MENOS TRANSBORDOS                      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    printf("Origen: %s\n", graph->vertex_names[src]);
    printf("Destino: %s\n\n", graph->vertex_names[dest]);
    
    if (!visited[dest]) {
        printf("❌ No existe ruta entre estas estaciones\n");
    } else {
        // Reconstruir ruta
        int path[MAX_VERTICES];
        int path_length = 0;
        int temp = dest;
        
        while (temp != -1) {
            path[path_length++] = temp;
            temp = parent[temp];
        }
        
        // Contar transbordos
        int transfers = 0;
        char current_line[MAX_LINE] = "";
        
        for (int i = path_length - 1; i >= 0; i--) {
            if (i < path_length - 1) {
                if (strlen(current_line) > 0 && 
                    strcmp(parent_lines[path[i]], current_line) != 0) {
                    transfers++;
                }
                strcpy(current_line, parent_lines[path[i]]);
            }
        }
        
        printf("🔄 Transbordos: %d\n", transfers);
        printf("📍 Estaciones: %d\n\n", path_length);
        printf("🚍 Itinerario:\n");
        
        strcpy(current_line, "");
        for (int i = path_length - 1; i >= 0; i--) {
            if (i == path_length - 1) {
                printf("   %d. %s [Inicio]\n", path_length - i, 
                       graph->vertex_names[path[i]]);
            } else {
                char* line = parent_lines[path[i]];
                if (strlen(current_line) > 0 && strcmp(line, current_line) != 0) {
                    printf("   %d. %s [%s] → ⚠️  TRANSBORDO\n", 
                           path_length - i, graph->vertex_names[path[i]], line);
                } else {
                    printf("   %d. %s [%s]\n", 
                           path_length - i, graph->vertex_names[path[i]], line);
                }
                strcpy(current_line, line);
            }
        }
    }
    
    printf("\n════════════════════════════════════════════════════════════\n");
    
    free(visited);
    free(parent);
    for (int i = 0; i < graph->num_vertices; i++) {
        free(parent_lines[i]);
    }
    free(parent_lines);
}
