#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "floyd_warshall.h"

/* ---------- internal helpers --------------------------------------------- */

static int **matrix_alloc(int n)
{
    int **m = malloc(n * sizeof(int *));
    if (!m) { perror("failed to allocate matrix row pointers"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; i++) {
        m[i] = malloc(n * sizeof(int));
        if (!m[i]) { perror("failed to allocate matrix row"); exit(EXIT_FAILURE); }
    }
    return m;
}

void fw_matrix_free(int **mat, int n)
{
    for (int i = 0; i < n; i++) free(mat[i]);
    free(mat);
}

/*
 * Number of characters needed to render value v.
 * is_pred: non-zero when v is a predecessor index (sentinel -1 printed as "--").
 */
static int cell_w(int v, int is_pred)
{
    if (!is_pred && v >= INF) return 3;   /* "INF" */
    if (is_pred  && v == -1)  return 2;   /* "--"  */
    char buf[32];
    return (int)snprintf(buf, sizeof(buf), "%d", v);
}

/* ---------- matrix display ----------------------------------------------- */

static void display_matrix(int **mat, int n, int step, int is_pred)
{
    /* Build the step label, e.g. "L^0 (initial)", "L^3", "P^0 (initial)" */
    char label[64];
    if (step == 0)
        snprintf(label, sizeof(label),
                 is_pred ? "P^0  (initial)" : "L^0  (initial)");
    else
        snprintf(label, sizeof(label),
                 is_pred ? "P^%d" : "L^%d", step);

    printf("\nMatrix %s:\n", label);

    /* Determine column width from all values and vertex labels. */
    int col_w = is_pred ? 2 : 3;          /* "--" or "INF" */
    for (int i = 0; i < n; i++) {
        int lw = cell_w(i, 0);
        if (lw > col_w) col_w = lw;
        for (int j = 0; j < n; j++) {
            int vw = cell_w(mat[i][j], is_pred);
            if (vw > col_w) col_w = vw;
        }
    }
    col_w++;                               /* one extra padding space */

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
            int v = mat[i][j];
            if (!is_pred && v >= INF)
                printf("%*s", col_w, "INF");
            else if (is_pred && v == -1)
                printf("%*s", col_w, "--");
            else
                printf("%*d", col_w, v);
        }
        printf("\n");
    }
}

void fw_display_L(int **L, int n, int step) { display_matrix(L, n, step, 0); }
void fw_display_P(int **P, int n, int step) { display_matrix(P, n, step, 1); }

/* ---------- Floyd-Warshall algorithm ------------------------------------- */

void floyd_warshall(const Graph *g, int ***L_out, int ***P_out)
{
    int n = g->n;

    int **L = matrix_alloc(n);
    int **P = matrix_alloc(n);

    /* Initialise L from the weight matrix and P from the arc structure. */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            L[i][j] = g->w[i][j];
            if (i == j)
                P[i][j] = i;
            else if (g->w[i][j] < INF)
                P[i][j] = i;   /* direct arc: predecessor of j is i */
            else
                P[i][j] = -1;  /* no path yet */
        }
    }

    /* Display initial matrices (step 0). */
    printf("\n=== Step 0: initial matrices ===");
    fw_display_L(L, n, 0);
    fw_display_P(P, n, 0);

    /* Main Floyd-Warshall loop.
     * After iteration k, L[i][j] is the shortest path from i to j using
     * only vertices 0 … k as possible intermediate vertices. */
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                /* Skip if there is no path from i to k or from k to j. */
                if (L[i][k] >= INF || L[k][j] >= INF)
                    continue;
                if (L[i][k] + L[k][j] < L[i][j]) {
                    L[i][j] = L[i][k] + L[k][j];
                    P[i][j] = P[k][j];   /* predecessor of j via path k→j */
                }
            }
        }

        /* Display matrices after using vertex k as pivot (step k+1). */
        printf("\n=== Step %d: after adding vertex %d as pivot ===", k + 1, k);
        fw_display_L(L, n, k + 1);
        fw_display_P(P, n, k + 1);
    }

    *L_out = L;
    *P_out = P;
}

/* ---------- absorbing-circuit detection ---------------------------------- */

int has_absorbing_circuit(int **L, int n)
{
    for (int i = 0; i < n; i++)
        if (L[i][i] < 0) return 1;
    return 0;
}

/* ---------- path display ------------------------------------------------- */

void fw_display_path(int **L, int **P, int n, int src, int dst)
{
    if (src < 0 || src >= n || dst < 0 || dst >= n) {
        printf("  Error: vertex index out of range (valid: 0 to %d).\n", n - 1);
        return;
    }

    if (src == dst) {
        printf("  Path from %d to %d: %d  (length: 0)\n", src, dst, src);
        return;
    }

    if (L[src][dst] >= INF) {
        printf("  No path from %d to %d exists in this graph.\n", src, dst);
        return;
    }

    /* Reconstruct path by following predecessor links backwards. */
    int *path = malloc(n * sizeof(int));
    if (!path) { perror("failed to allocate path array"); exit(EXIT_FAILURE); }

    int len = 0;
    path[len++] = dst;
    int cur = dst;

    while (cur != src) {
        cur = P[src][cur];
        if (cur == -1 || len >= n) {
            /* Should not happen when there is no absorbing circuit. */
            printf("  Error: path reconstruction failed.\n");
            free(path);
            return;
        }
        path[len++] = cur;
    }

    /* Print in forward order. */
    printf("  Shortest path from %d to %d: ", src, dst);
    for (int i = len - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0) printf(" -> ");
    }
    printf("  (total length: %d)\n", L[src][dst]);

    free(path);
}
