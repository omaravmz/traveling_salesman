// tsp_common.h
// Utilidades compartidas por main_NN.cpp, main_CL.cpp y main_MST.cpp:
// lectura del .tsp, distancia CEIL_2D, muestreo aleatorio, costo del tour,
// validación y el ciclo de experimentos que imprime la tabla.
// Lo único que cambia entre programas es la función que construye el tour.

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <functional>
#include <chrono>
#include <iomanip>
using namespace std;

using Matrix = vector<vector<long long>>;
// Un "solver" recibe la matriz de distancias y un generador aleatorio,
// y regresa el tour como una permutación de índices 0..n-1.
// El regreso a la ciudad inicial NO se repite al final; tourCost lo suma.
using Solver = function<vector<int>(const Matrix&, mt19937&)>;

const long long OPTIMAL_1000 = 18659688;                       // óptimo conocido para las 1000 ciudades
const vector<int> SIZES = {10, 50, 100, 200, 400, 600, 800, 1000};
const vector<unsigned> SEEDS = {1, 2, 3};                       // mismas semillas para los 3 algoritmos

struct City {
    int id;
    double x, y;
};

// Lee el archivo .tsp: ignora el encabezado hasta NODE_COORD_SECTION
// y luego lee tripletas "id x y" hasta encontrar EOF (o fin de archivo).
inline vector<City> readTSP(const string& path) {
    ifstream in(path);
    if (!in) {
        cerr << "No se pudo abrir " << path << "\n";
        exit(1);
    }
    string line;
    while (getline(in, line)) {
        if (line.find("NODE_COORD_SECTION") != string::npos) break;
    }
    vector<City> cities;
    int id;
    double x, y;
    while (in >> id >> x >> y) cities.push_back({id, x, y});  // se detiene solo al leer "EOF"
    return cities;
}

// CEIL_2D: distancia euclidiana redondeada hacia arriba.
inline long long ceil2D(const City& a, const City& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return (long long)ceil(sqrt(dx * dx + dy * dy));
}

// Matriz de distancias n x n (para n = 1000 son ~8 MB, sin problema).
inline Matrix buildMatrix(const vector<City>& c) {
    int n = c.size();
    Matrix D(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            D[i][j] = D[j][i] = ceil2D(c[i], c[j]);
    return D;
}

// Escoge n ciudades distintas al azar (baraja una copia y toma las primeras n).
inline vector<City> sampleCities(const vector<City>& all, int n, unsigned seed) {
    vector<City> copy = all;
    mt19937 rng(seed);
    shuffle(copy.begin(), copy.end(), rng);
    copy.resize(n);
    return copy;
}

// Un tour válido visita cada ciudad 0..n-1 exactamente una vez.
inline bool isValidTour(const vector<int>& t, int n) {
    if ((int)t.size() != n) return false;
    vector<bool> seen(n, false);
    for (int v : t) {
        if (v < 0 || v >= n || seen[v]) return false;
        seen[v] = true;
    }
    return true;
}

// Suma de las aristas del ciclo, incluyendo el regreso a la ciudad inicial.
inline long long tourCost(const vector<int>& t, const Matrix& D) {
    long long s = 0;
    for (size_t i = 0; i < t.size(); i++) s += D[t[i]][t[(i + 1) % t.size()]];
    return s;
}

// Promedio y desviación estándar muestral (divide entre k-1).
inline void meanStd(const vector<double>& v, double& mean, double& sd) {
    mean = accumulate(v.begin(), v.end(), 0.0) / v.size();
    double acc = 0;
    for (double x : v) acc += (x - mean) * (x - mean);
    sd = v.size() > 1 ? sqrt(acc / (v.size() - 1)) : 0.0;
}

// Corre el solver para cada N y cada semilla, e imprime una fila CSV por N:
// N, promedio del costo, desviación estándar, gap vs óptimo (solo N=1000), tiempo promedio en ms.
inline void runExperiments(const string& name, const string& path, Solver solve) {
    vector<City> all = readTSP(path);
    if (all.empty()) {
        cerr << "El archivo no tiene ciudades.\n";
        exit(1);
    }
    cerr << name << ": " << all.size() << " ciudades leídas\n";
    cout << "N,mean,std,gap_pct,avg_ms\n";

    for (int n : SIZES) {
        if (n > (int)all.size()) break;
        vector<double> costs, times;

        for (unsigned seed : SEEDS) {
            vector<City> sub = sampleCities(all, n, seed);
            Matrix D = buildMatrix(sub);
            mt19937 rng(seed);

            auto t0 = chrono::steady_clock::now();
            vector<int> tour = solve(D, rng);
            auto t1 = chrono::steady_clock::now();

            if (!isValidTour(tour, n)) {
                cerr << "[" << name << "] Tour inválido para N=" << n
                     << " (seed " << seed << "). ¿Ya implementaste el algoritmo?\n";
                return;
            }
            costs.push_back((double)tourCost(tour, D));
            times.push_back(chrono::duration<double, milli>(t1 - t0).count());
        }

        double m, s, mt, st;
        meanStd(costs, m, s);
        meanStd(times, mt, st);

        cout << n << "," << fixed << setprecision(1) << m << "," << s << ",";
        if (n == 1000) cout << setprecision(2) << (m - OPTIMAL_1000) * 100.0 / OPTIMAL_1000;
        else cout << "-";
        cout << "," << setprecision(3) << mt << "\n";
    }
}
