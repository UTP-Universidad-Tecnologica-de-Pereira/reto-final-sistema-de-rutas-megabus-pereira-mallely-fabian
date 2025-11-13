#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

void print_menu() {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║          SISTEMA MEGABÚS PEREIRA 2025                      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  1. 📋 Ver red completa\n");
    printf("  2. ⚡ Ruta más rápida (por tiempo)\n");
    printf("  3. 💰 Ruta más económica (por costo)\n");
    printf("  4. 🔄 Ruta con menos transbordos\n");
    printf("  5. 📊 Estadísticas de red\n");
    printf("  6. 🏆 Top estaciones hub\n");
    printf("  7. 📍 Grados de estaciones\n");
    printf("  8. 🕐 Verificar disponibilidad de ruta\n");
    printf("  9. ℹ️  Listar todas las estaciones\n");
    printf("  0. 🚪 Salir\n");
    printf("\n════════════════════════════════════════════════════════════\n");
    printf("\nSeleccione opción: ");
}

void print_all_stations(Graph* graph) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              LISTADO DE ESTACIONES                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("  [%2d] %s\n", i, graph->vertex_names[i]);
    }
    
    printf("\n════════════════════════════════════════════════════════════\n");
}

void check_route_availability(Graph* graph) {
    int src, dest, hour;
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║         VERIFICAR DISPONIBILIDAD DE RUTA                   ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Ingrese estación origen (0-%d): ", graph->num_vertices - 1);
    if (scanf("%d", &src) != 1 || src < 0 || src >= graph->num_vertices) {
        printf("❌ Estación inválida\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Ingrese estación destino (0-%d): ", graph->num_vertices - 1);
    if (scanf("%d", &dest) != 1 || dest < 0 || dest >= graph->num_vertices) {
        printf("❌ Estación inválida\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Ingrese hora (0-23): ");
    if (scanf("%d", &hour) != 1 || hour < 0 || hour > 23) {
        printf("❌ Hora inválida\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("\n");
    printf("Origen: %s\n", graph->vertex_names[src]);
    printf("Destino: %s\n", graph->vertex_names[dest]);
    printf("Hora: %02d:00\n\n", hour);
    
    if (is_route_available(graph, src, dest, hour)) {
        printf("✅ La ruta está DISPONIBLE a esta hora\n");
        
        // Buscar información de la ruta
        AdjNode* adj = graph->array[src].head;
        while (adj != NULL) {
            if (adj->vertex == dest) {
                printf("\n📋 Detalles:\n");
                printf("   Línea: %s\n", adj->line);
                printf("   Horario: %02d:00 - %02d:00\n", 
                       adj->schedule.start_hour, adj->schedule.end_hour);
                printf("   Frecuencia: cada %d minutos\n", adj->schedule.frequency_min);
                
                int wait = calculate_wait_time(&adj->schedule, hour);
                if (wait >= 0) {
                    printf("   Tiempo de espera estimado: %d minutos\n", wait);
                }
                break;
            }
            adj = adj->next;
        }
    } else {
        printf("❌ La ruta NO está disponible a esta hora\n");
        printf("   (Puede estar fuera del horario de servicio)\n");
    }
    
    printf("\n════════════════════════════════════════════════════════════\n");
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                                                            ║\n");
    printf("║       🚍 SISTEMA DE RUTAS MEGABÚS PEREIRA 2025 🚍          ║\n");
    printf("║                                                            ║\n");
    printf("║              Universidad Tecnológica de Pereira            ║\n");
    printf("║                    Estructuras de Datos                    ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Cargando red de Pereira...\n");
    
    // Crear grafo no dirigido con 15 estaciones
    Graph* megabus = create_graph(15, 0);
    
    // Cargar dataset de Pereira
    load_pereira_megabus(megabus);
    
    printf("✓ 15 estaciones cargadas\n");
    
    // Contar conexiones
    int connections = 0;
    for (int i = 0; i < megabus->num_vertices; i++) {
        AdjNode* adj = megabus->array[i].head;
        while (adj != NULL) {
            connections++;
            adj = adj->next;
        }
    }
    connections /= 2; // Grafo no dirigido
    
    printf("✓ %d conexiones establecidas\n", connections);
    printf("\n¡Sistema listo!\n");
    
    // Menú interactivo
    int opcion;
    int src, dest;
    
    while (1) {
        print_menu();
        
        if (scanf("%d", &opcion) != 1) {
            printf("❌ Opción inválida\n");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }
        
        switch (opcion) {
            case 1:
                print_graph(megabus);
                break;
                
            case 2:
                printf("\n");
                print_all_stations(megabus);
                printf("\nIngrese estación origen (0-%d): ", megabus->num_vertices - 1);
                if (scanf("%d", &src) != 1 || src < 0 || src >= megabus->num_vertices) {
                    printf("❌ Estación inválida\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Ingrese estación destino (0-%d): ", megabus->num_vertices - 1);
                if (scanf("%d", &dest) != 1 || dest < 0 || dest >= megabus->num_vertices) {
                    printf("❌ Estación inválida\n");
                    while (getchar() != '\n');
                    break;
                }
                dijkstra_by_time(megabus, src, dest);
                break;
                
            case 3:
                printf("\n");
                print_all_stations(megabus);
                printf("\nIngrese estación origen (0-%d): ", megabus->num_vertices - 1);
                if (scanf("%d", &src) != 1 || src < 0 || src >= megabus->num_vertices) {
                    printf("❌ Estación inválida\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Ingrese estación destino (0-%d): ", megabus->num_vertices - 1);
                if (scanf("%d", &dest) != 1 || dest < 0 || dest >= megabus->num_vertices) {
                    printf("❌ Estación inválida\n");
                    while (getchar() != '\n');
                    break;
                }
                dijkstra_by_cost(megabus, src, dest);
                break;
                
            case 4:
                printf("\n");
                print_all_stations(megabus);
                printf("\nIngrese estación origen (0-%d): ", megabus->num_vertices - 1);
                if (scanf("%d", &src) != 1 || src < 0 || src >= megabus->num_vertices) {
                    printf("❌ Estación inválida\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Ingrese estación destino (0-%d): ", megabus->num_vertices - 1);
                if (scanf("%d", &dest) != 1 || dest < 0 || dest >= megabus->num_vertices) {
                    printf("❌ Estación inválida\n");
                    while (getchar() != '\n');
                    break;
                }
                bfs_min_transfers(megabus, src, dest);
                break;
                
            case 5:
                print_network_statistics(megabus);
                break;
                
            case 6:
                find_hub_stations(megabus, 5);
                break;
                
            case 7:
                print_station_degrees(megabus);
                break;
                
            case 8:
                check_route_availability(megabus);
                break;
                
            case 9:
                print_all_stations(megabus);
                break;
                
            case 0:
                printf("\n");
                printf("╔════════════════════════════════════════════════════════════╗\n");
                printf("║                                                            ║\n");
                printf("║          ¡Gracias por usar el Sistema Megabús!             ║\n");
                printf("║                                                            ║\n");
                printf("╚════════════════════════════════════════════════════════════╝\n\n");
                free_graph(megabus);
                printf("✓ Sistema finalizado correctamente. Memoria liberada.\n\n");
                return 0;
                
            default:
                printf("\n❌ Opción inválida. Intente nuevamente.\n");
                break;
        }
        
        printf("\nPresione Enter para continuar...");
        while (getchar() != '\n');
        getchar();
    }
    
    return 0;
}
