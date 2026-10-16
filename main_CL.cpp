// main_CL.cpp — Cheapest-Link TSP (también llamado Greedy Edge)
// Compilar: g++ -O2 -std=c++17 main_CL.cpp -o cl
// Ejecutar: ./cl dsj1000.tsp

#include "tsp_common.h"

struct Edge {
    long long w;
    int u, v;
};

// DSU (Disjoint Set Union / Union-Find): detecta si dos ciudades
// ya están en el mismo "pedazo" de camino, para no cerrar un ciclo antes de tiempo.
struct DSU {
    vector<int> parent, rnk;
    DSU(int n) : parent(n), rnk(n, 0) { iota(parent.begin(), parent.end(), 0); }

    int find(int x) {
        // TODO: regresa la raíz de x (con compresión de caminos).
        return x;
    }

    bool unite(int a, int b) {
        // TODO: une los conjuntos de a y b; regresa false si ya estaban juntos.
        return false;
    }
};

vector<int> solveCL(const Matrix& D, mt19937& rng) {
    int n = D.size();
    vector<int> tour;

    // TODO:
    // 1. Genera todas las aristas (i, j) con i < j y su peso D[i][j]. Ordénalas de menor a mayor.
    // 2. Recorre las aristas. Acepta (u, v) solo si:
    //      - grado[u] < 2 y grado[v] < 2   (nadie tiene 3 caminos)
    //      - dsu.find(u) != dsu.find(v)    (no cierra un ciclo prematuro)
    //    Al aceptar: guarda la arista en una lista de adyacencia, sube grados, dsu.unite(u, v).
    //    Detente cuando tengas n-1 aristas (ya es un solo camino que pasa por todas).
    // 3. Las dos ciudades con grado 1 son los extremos: únelas para cerrar el ciclo.
    // 4. Convierte la lista de adyacencia en tour: empieza en 0 y camina
    //    siempre al vecino que no sea del que vienes, hasta completar n ciudades.
    // Caso especial: n == 1 → tour = {0}.

    return tour;
}

int main(int argc, char** argv) {
    string path = argc > 1 ? argv[1] : "dsj1000.tsp";
    runExperiments("Cheapest-link-TSP", path, solveCL);
    return 0;
}
