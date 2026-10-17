// TSP with Nearest Neighbor 
// Compile: g++ -O2 -std=c++17 main_NN.cpp -o nn
// Execution: ./nn dsj1000.tsp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <climits>
using namespace std;

// ---------- Helper functions and structures ----------

struct City {
    double x, y;
};

vector<City> readCities(const string& path) {
    ifstream in(path);
    if (!in) {
        cout << "Could not open " << path << "\n";
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

/*
    Time complexity: O(n^2) where n is the number of cities.
    This is due to the nested loops n steps and n cities verified each time.

    Space complexity: O(n) for the tour and visited arrays.

*/
vector<int> solveNN(const vector<vector<long long>>& D) {
    int n = D.size();
    vector<int> tour;
    tour.reserve(n);
    vector<bool> visited(n, false);

    int current = 0;  
    visited[current] = true;
    tour.push_back(current);

    for (int step = 1; step < n; step++) {
        int best = -1;
        long long bestD = LLONG_MAX;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && D[current][j] < bestD) {
                bestD = D[current][j];
                best = j;
            }
        }
        visited[best] = true;
        tour.push_back(best);
        current = best;
    }
    return tour;
}

// ---------- Main (Cases) ----------

int main(int argc, char** argv) {
    string path = argc > 1 ? argv[1] : "dsj1000.tsp";
    vector<City> cities = readCities(path);

    vector<int> sizes = {10, 50, 100, 200, 400, 600, 800, 1000};
    vector<int> seeds = {1, 2, 3};
    const long long OPTIMAL = 18659688;

    cout << "NN-TSP  (" << cities.size() << " READ CITIES, SEEDS 1, 2, 3)\n";
    cout << "N\tMean\tStd.Dev.\n";

    for (int n : sizes) {
        vector<long long> costs;

        for (int seed : seeds) {
            vector<City> sub = sampleCities(cities, n, seed);
            vector<vector<long long>> D = buildMatrix(sub);
            vector<int> tour = solveNN(D);

            if ((int)tour.size() != n) {
                cout << "Invalid tour at N = " << n << "\n";
                return 1;
            }
            costs.push_back(tourCost(tour, D));
        }

        double mean = 0;
        for (long long c : costs) mean += c;
        mean = mean / costs.size();

        double var = 0;
        for (long long c : costs) var += (c - mean) * (c - mean);
        double sd = sqrt(var / (costs.size() - 1));

        cout << fixed;
        cout.precision(1);
        cout << n << "\t" << mean << "\t" << sd;
        if (n == 1000) cout << "\t(" << (mean - OPTIMAL) * 100.0 / OPTIMAL << "% above optimal)";
        cout << "\n";
    }
    return 0;
}