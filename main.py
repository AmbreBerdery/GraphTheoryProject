#menu
from sympy import true

while true:
    print("Which graph do you want to analyse?")
    print("1. Graph 1")
    print("1. Graph 2")
    print("1. Graph 3")
    print("1. Graph 4")
    print("1. Graph 5")
    print("1. Graph 6")
    print("1. Graph 7")
    print("1. Graph 8")
    print("1. Graph 9")
    print("1. Graph 10")
    print("1. Graph 11")
    print("1. Graph 12")
    print("1. Graph 13")

    graph_choice = input("Enter your choice:")
    while (graph_choice >= 1 and graph_choice <= 13):
        print("You have chosen graph " + graph_choice)
        #display la matrice du graphe choisi
        #analyser la matrice = executer floyd warshall
        #dire si y'a un absorbing cycle ou pas
        #afficher le résultat sous forme txt ou matrice
        break


