#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "floyd_warshall.h"

/* Clear any leftover characters from the current input line. */
static void flush_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(void)
{
    printf("=======================================================\n");
    printf("   Floyd-Warshall Shortest Path Algorithm\n");
    printf("=======================================================\n");
    printf("Graphs are stored in files  graphs/graph<N>.txt\n");
    printf("Enter 0 to quit.\n");

    while (1) {
        /* ---- select a graph -------------------------------------------- */
        printf("\n-------------------------------------------------------\n");
        printf("Graph number (0 to quit): ");
        fflush(stdout);

        int graph_num;
        if (scanf("%d", &graph_num) != 1) {
            printf("Invalid input — please enter a number.\n");
            flush_input();
            continue;
        }
        flush_input();

        if (graph_num == 0) {
            printf("Goodbye!\n");
            break;
        }

        /* ---- load graph ------------------------------------------------- */
        Graph *g = graph_load(graph_num);
        if (!g) {
            printf("Could not load graph %d. Please try another number.\n",
                   graph_num);
            continue;
        }
        printf("\nGraph %d loaded successfully (%d vertices).\n",
               graph_num, g->n);

        /* ---- display weight matrix -------------------------------------- */
        printf("\n--- Weight matrix (graph %d) ---\n", graph_num);
        graph_display(g);

        /* ---- run Floyd-Warshall ----------------------------------------- */
        printf("\n--- Floyd-Warshall algorithm — intermediate matrices ---\n");
        int **L, **P;
        floyd_warshall(g, &L, &P);

        /* ---- absorbing-circuit check ------------------------------------ */
        printf("\n--- Analysis ---\n");
        if (has_absorbing_circuit(L, g->n)) {
            printf("The graph contains at least one absorbing circuit "
                   "(negative cycle).\n");
            printf("Floyd-Warshall shortest-path values are not meaningful.\n");
        } else {
            printf("No absorbing circuit detected.\n");

            printf("\n--- Final shortest-path matrix L^%d ---\n", g->n);
            fw_display_L(L, g->n, g->n);

            printf("\n--- Final predecessor matrix P^%d ---\n", g->n);
            fw_display_P(P, g->n, g->n);

            /* ---- path query loop --------------------------------------- */
            while (1) {
                printf("\nPath? (y/n): ");
                fflush(stdout);
                char choice;
                if (scanf(" %c", &choice) != 1) { flush_input(); break; }
                flush_input();

                if (choice != 'y' && choice != 'Y') break;

                int src, dst;
                printf("Starting vertex? ");
                fflush(stdout);
                if (scanf("%d", &src) != 1) { flush_input(); break; }
                flush_input();

                printf("Ending vertex? ");
                fflush(stdout);
                if (scanf("%d", &dst) != 1) { flush_input(); break; }
                flush_input();

                fw_display_path(L, P, g->n, src, dst);

                printf("Restart? (y/n): ");
                fflush(stdout);
                if (scanf(" %c", &choice) != 1) { flush_input(); break; }
                flush_input();

                if (choice != 'y' && choice != 'Y') break;
            }
        }

        /* ---- clean up -------------------------------------------------- */
        fw_matrix_free(L, g->n);
        fw_matrix_free(P, g->n);
        graph_free(g);
    }

    return 0;
}
