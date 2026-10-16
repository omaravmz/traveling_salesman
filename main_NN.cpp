// main_NN.cpp — Nearest Neighbor TSP (vecino más cercano)
// Compilar: g++ -O2 -std=c++17 main_NN.cpp -o nn
// Ejecutar: ./nn dsj1000.tsp

#include "tsp_common.h"

vector<int> solveNN(const Matrix& D, mt19937& rng) {
    int n = D.size();
    vector<int> tour;
    tour.reserve(n);

    // TODO (puedes reutilizar tu código del quiz):
    // 1. Elige la ciudad inicial (0, o una aleatoria con uniform_int_distribution y rng).
    // 2. Márcala como visitada y agrégala al tour.
    // 3. Mientras falten ciudades: recorre todas las NO visitadas,
    //    quédate con la de menor D[actual][j], muévete a ella.
    // 4. Regresa el tour (sin repetir la ciudad inicial al final).

    return tour;
}

int main(int argc, char** argv) {
    string path = argc > 1 ? argv[1] : "dsj1000.tsp";
    runExperiments("NN-TSP", path, solveNN);
    return 0;
}
