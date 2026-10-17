// TSP with Cheapest-link
// Compile: g++ -O2 -std=c++17 main_CL.cpp -o cl
// Execution: ./cl dsj1000.tsp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
using namespace std;

// ---------- Helper functions and structures ----------

struct City {
    double x, y;
};

vector<City> readCities(const string& path) {
    ifstream in(path);
    if (!in) {
        cout << "Couldn't be opened " << path << "\n";
        exit(1);
    }
    string line;
    while (getline(in, line)) {
        if (line.find("NODE_COORD_SECTION") != string::npos) break;
    }
    vector<City> cities;
    int id;
    double x, y;
    while (in >> id >> x >> y) cities.push_back({x, y});
    return cities;
}

long long dist(const City& a, const City& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return (long long)ceil(sqrt(dx * dx + dy * dy));
}

vector<vector<long long>> buildMatrix(const vector<City>& c) {
    int n = c.size();
    vector<vector<long long>> D(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            D[i][j] = D[j][i] = dist(c[i], c[j]);
    return D;
}

vector<City> sampleCities(vector<City> cities, int n, int seed) {
    mt19937 gen(seed);
    shuffle(cities.begin(), cities.end(), gen);
    cities.resize(n);
    return cities;
}

long long tourCost(const vector<int>& tour, const vector<vector<long long>>& D) {
    long long total = 0;
    int n = tour.size();
    for (int i = 0; i < n; i++) total += D[tour[i]][tour[(i + 1) % n]];
    return total;
}

// ---------- Algorithm ----------

struct Edge {
    long long w;
    int u, v;
};

// Union-Find (DSU, Disjoint Set Union): dice si dos ciudades ya están
// en el mismo "pedazo" de camino, para no cerrar un ciclo antes de tiempo.
vector<int> parent;

int findRoot(int x) {
    // TODO: sube por parent[] hasta encontrar la raíz (la que es su propio padre).
    return x;
}

void unite(int a, int b) {
    // TODO: haz que la raíz de a apunte a la raíz de b.
}

vector<int> solveCL(const vector<vector<long long>>& D) {
    int n = D.size();
    vector<int> tour;

    parent.assign(n, 0);
    for (int i = 0; i < n; i++) parent[i] = i;   // al inicio cada ciudad es su propio pedazo

    vector<int> degree(n, 0);                    // cuántas conexiones tiene cada ciudad
    vector<vector<int>> adj(n);                  // vecinos de cada ciudad en el recorrido

    // TODO:
    // 1. Crea todas las aristas (i, j) con i < j y peso D[i][j]; ordénalas de menor a mayor.
    // 2. Recorre las aristas y acepta (u, v) solo si:
    //      degree[u] < 2, degree[v] < 2 y findRoot(u) != findRoot(v).
    //    Al aceptar: agrega a adj, sube degree y llama unite(u, v).
    //    Detente al tener n-1 aristas aceptadas.
    // 3. Las 2 ciudades con degree 1 son los extremos: conéctalas en adj para cerrar el ciclo.
    // 4. Arma el tour: empieza en 0 y ve siempre al vecino del que NO vienes, hasta tener n ciudades.
    // Caso especial: si n == 1, el tour es {0}.

    return tour;
}

// ---------- Main (Cases) ----------


int main(int argc, char** argv) {
    string path = argc > 1 ? argv[1] : "dsj1000.tsp";
    vector<City> cities = readCities(path);

    vector<int> sizes = {10, 50, 100, 200, 400, 600, 800, 1000};
    vector<int> seeds = {1, 2, 3};
    const long long OPTIMAL = 18659688;

    cout << "Cheapest-link-TSP  (" << cities.size() << " READ CITIES, SEEDS 1, 2, 3)\n";
    cout << "N\tPromedio\tDesv.Est.\n";

    for (int n : sizes) {
        vector<long long> costs;

        for (int seed : seeds) {
            vector<City> sub = sampleCities(cities, n, seed);
            vector<vector<long long>> D = buildMatrix(sub);
            vector<int> tour = solveCL(D);

            if ((int)tour.size() != n) {
                cout << "Invalid tour at N =" << n << "\n";
                return 1;
            }
            costs.push_back(tourCost(tour, D));
        }

        double mean = 0;
        for (long long c : costs) mean += c;
        mean /= costs.size();

        double var = 0;
        for (long long c : costs) var += (c - mean) * (c - mean);
        double sd = sqrt(var / (costs.size() - 1));

        cout << fixed;
        cout.precision(1);
        cout << n << "\t" << mean << "\t" << sd;
        if (n == 1000) cout << "\t(" << (mean - OPTIMAL) * 100.0 / OPTIMAL << "% arriba del optimo)";
        cout << "\n";
    }
    return 0;
}