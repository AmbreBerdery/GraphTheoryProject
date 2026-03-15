#fichier pour lire les graph.xtx et coder l'algorithme de Floyd Warshall
import math
import numpy as np


def read_graph_file(file):
    with open(file, 'r') as file:
        lines = file.readlines();

        ver = line(0)
