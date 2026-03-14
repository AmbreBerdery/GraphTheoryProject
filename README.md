# GraphTheoryProject
Finding shortest paths using the Floyd–Warshall algorithm

## Build

```bash
make
```

This produces the executable `floyd_warshall` in the project root.

## Run

```bash
./floyd_warshall
```

The program must be launched from the project root so that it can find the
graph files in the `graphs/` directory.

## Graph file format

Graphs are stored in `graphs/graph<N>.txt` and selected by their number at
run time. The file format is:

```
<number of vertices n>
<number of arcs m>
<source> <destination> <weight>   ← one line per arc
…
```

- Vertices are integers `0 … n-1`.
- Weights are integers (negative values and `0` are allowed).
- At most one arc from vertex `x` to vertex `y`.

### Provided graphs

| File | Description |
|------|-------------|
| `graphs/graph1.txt` | 4-vertex directed weighted graph — no absorbing circuit |
| `graphs/graph2.txt` | 3-vertex graph containing a **negative (absorbing) circuit** |
| `graphs/graph3.txt` | 6-node delivery-network example (real-world application) |

## Features

1. **Main loop** – the program processes multiple graphs in a single run;
   restart is never required to switch graphs.
2. **Graph loading** – the graph is read from a text file and held entirely
   in memory; the file is never accessed again.
3. **Weight matrix display** – the graph is printed as a labelled matrix with
   aligned columns (`INF` denotes the absence of an arc).
4. **Floyd–Warshall algorithm** – all intermediate L and P matrices (after
   each pivot step) are displayed on screen.
5. **Absorbing-circuit detection** – a negative diagonal entry in the final
   L matrix indicates a negative cycle.
6. **Shortest-path queries** – when no absorbing circuit is present the user
   can repeatedly query the shortest path between any two vertices.

## Real-world application example (graph 3)

The graph models a logistics network of six warehouses:

| Vertex | Location   |
|--------|------------|
| 0      | Paris      |
| 1      | Lyon       |
| 2      | Bordeaux   |
| 3      | Marseille  |
| 4      | Toulouse   |
| 5      | Nice       |

Arc weights represent shipping costs (in hundreds of euros). The arc
`Bordeaux → Toulouse` carries a negative weight (−1) reflecting a
government subsidy. The graph contains no absorbing circuit, so the
Floyd–Warshall algorithm correctly computes minimum-cost routes between
every pair of warehouses.
