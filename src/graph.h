#ifndef GRAPH_H
#define GRAPH_H

/*
 * Sentinel value representing "no arc" (infinity) in the weight matrix.
 * Large enough to avoid overflow when two INF values are compared after
 * addition, but small enough to fit comfortably in a signed int.
 */
#define INF 1000000

/* Directed weighted graph stored as an adjacency matrix. */
typedef struct {
    int  n;    /* number of vertices (labelled 0 … n-1) */
    int **w;   /* w[i][j] = weight of arc i->j, or INF if no such arc */
} Graph;

/* Allocate an n-vertex graph with no arcs (all INF, diagonal 0). */
Graph *graph_create(int n);

/* Release all memory owned by g. */
void graph_free(Graph *g);

/*
 * Load a graph from the file  graphs/graph<number>.txt
 * File format:
 *   n          (number of vertices)
 *   m          (number of arcs)
 *   src dst w  (one arc per line, repeated m times)
 * Returns NULL on error.
 */
Graph *graph_load(int graph_number);

/* Print the weight matrix to stdout with aligned columns. */
void graph_display(const Graph *g);

#endif /* GRAPH_H */
