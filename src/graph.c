#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

/* ---------- memory management ------------------------------------------- */

Graph *graph_create(int n)
{
    Graph *g = malloc(sizeof(Graph));
    if (!g) { perror("failed to allocate graph structure"); exit(EXIT_FAILURE); }
    g->n = n;

    g->w = malloc(n * sizeof(int *));
    if (!g->w) { perror("failed to allocate graph row pointers"); exit(EXIT_FAILURE); }

    for (int i = 0; i < n; i++) {
        g->w[i] = malloc(n * sizeof(int));
        if (!g->w[i]) { perror("failed to allocate graph row"); exit(EXIT_FAILURE); }
        for (int j = 0; j < n; j++)
            g->w[i][j] = (i == j) ? 0 : INF;
    }
    return g;
}

void graph_free(Graph *g)
{
    if (!g) return;
    for (int i = 0; i < g->n; i++)
        free(g->w[i]);
    free(g->w);
    free(g);
}

/* ---------- file I/O ----------------------------------------------------- */

Graph *graph_load(int graph_number)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "graphs/graph%d.txt", graph_number);

    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: cannot open '%s'\n", filename);
        return NULL;
    }

    int n, m;
    if (fscanf(f, "%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Error: invalid number of vertices in '%s'\n", filename);
        fclose(f); return NULL;
    }
    if (fscanf(f, "%d", &m) != 1 || m < 0) {
        fprintf(stderr, "Error: invalid number of arcs in '%s'\n", filename);
        fclose(f); return NULL;
    }

    Graph *g = graph_create(n);

    for (int e = 0; e < m; e++) {
        int src, dst, weight;
        if (fscanf(f, "%d %d %d", &src, &dst, &weight) != 3) {
            fprintf(stderr, "Error: malformed arc #%d in '%s'\n", e + 1, filename);
            graph_free(g); fclose(f); return NULL;
        }
        if (src < 0 || src >= n || dst < 0 || dst >= n) {
            fprintf(stderr,
                "Error: arc #%d references out-of-range vertex in '%s'\n",
                e + 1, filename);
            graph_free(g); fclose(f); return NULL;
        }
        g->w[src][dst] = weight;
    }

    fclose(f);
    return g;
}

/* ---------- display ------------------------------------------------------- */

/* Number of characters needed to print value v (for column-width calculation). */
static int cell_width_val(int v)
{
    if (v >= INF) return 3;          /* "INF" */
    char buf[32];
    return (int)snprintf(buf, sizeof(buf), "%d", v);
}

void graph_display(const Graph *g)
{
    int n = g->n;

    /* Determine the widest cell (vertex label or matrix value). */
    int col_w = 3;                   /* minimum: "INF" */
    for (int i = 0; i < n; i++) {
        int lw = cell_width_val(i);
        if (lw > col_w) col_w = lw;
        for (int j = 0; j < n; j++) {
            int vw = cell_width_val(g->w[i][j]);
            if (vw > col_w) col_w = vw;
        }
    }
    col_w++;                         /* one extra space of padding */

    /* Header row */
    printf("%*s |", col_w, "");
    for (int j = 0; j < n; j++) printf("%*d", col_w, j);
    printf("\n");

    /* Separator */
    for (int c = 0; c < col_w; c++) printf("-");
    printf("-+");
    for (int c = 0; c < n * col_w; c++) printf("-");
    printf("\n");

    /* Data rows */
    for (int i = 0; i < n; i++) {
        printf("%*d |", col_w, i);
        for (int j = 0; j < n; j++) {
            if (g->w[i][j] >= INF)
                printf("%*s", col_w, "INF");
            else
                printf("%*d", col_w, g->w[i][j]);
        }
        printf("\n");
    }
}
