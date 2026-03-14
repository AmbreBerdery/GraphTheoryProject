#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include "graph.h"

/*
 * Execute the Floyd-Warshall algorithm on graph g.
 *
 * The algorithm prints the L and P matrices after each step (including the
 * initial state) so that the entire progression can be followed.
 *
 * On return:
 *   *L_out[i][j] = length of the shortest path from i to j
 *                  (INF if no path exists, possibly negative if a circuit
 *                  shortens the path)
 *   *P_out[i][j] = predecessor of j on the shortest path from i to j
 *                  (-1 if no path)
 *
 * The caller is responsible for freeing *L_out and *P_out using
 * fw_matrix_free().
 */
void floyd_warshall(const Graph *g, int ***L_out, int ***P_out);

/* Free a matrix allocated by floyd_warshall (L or P). */
void fw_matrix_free(int **mat, int n);

/* Return 1 if L contains at least one negative diagonal entry (absorbing
 * circuit), 0 otherwise. */
int has_absorbing_circuit(int **L, int n);

/* Print the L matrix with label "L^<step>" to stdout. */
void fw_display_L(int **L, int n, int step);

/* Print the P matrix with label "P^<step>" to stdout. */
void fw_display_P(int **P, int n, int step);

/*
 * Print the shortest path from src to dst together with its total length.
 * Prints an error message if no path exists or the vertices are invalid.
 */
void fw_display_path(int **L, int **P, int n, int src, int dst);

#endif /* FLOYD_WARSHALL_H */
