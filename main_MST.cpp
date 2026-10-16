// main_MST.cpp — TSP usando Minimum Spanning Tree (árbol de expansión mínima)
// Compilar: g++ -O2 -std=c++17 main_MST.cpp -o mst
// Ejecutar: ./mst dsj1000.tsp

#include "tsp_common.h"

// Prim O(n^2): ideal porque la gráfica es completa (todas las ciudades conectadas).
// Regresa parent[v] = padre de v en el árbol (la raíz tiene parent = -1).
vector<int> primMST(const Matrix& D, int root) {
    int n = D.size();
    vector<int> parent(n, -1);

    // TODO:
    // - key[v] = costo más barato para conectar v al árbol (empieza en infinito; key[root] = 0).
    // - inTree[v] = si v ya está en el árbol.
    // - Repite n veces: toma la v fuera del árbol con menor key, métela,
    //   y para cada u fuera del árbol: si D[v][u] < key[u] → key[u] = D[v][u], parent[u] = v.

    return parent;
}

vector<int> solveMST(const Matrix& D, mt19937& rng) {
    int n = D.size();
    vector<int> tour;

    // TODO:
    // 1. parent = primMST(D, 0).
    // 2. Construye children[p] (lista de hijos) a partir de parent.
    // 3. Haz un DFS en preorden desde la raíz (con stack para no desbordar la recursión).
    //    El orden en que DESCUBRES las ciudades ya es el tour: los "regresos" del DFS
    //    se saltan porque cada ciudad solo se agrega la primera vez (shortcutting).

    return tour;
}

int main(int argc, char** argv) {
    string path = argc > 1 ? argv[1] : "dsj1000.tsp";
    runExperiments("MST-TSP", path, solveMST);
    return 0;
}
