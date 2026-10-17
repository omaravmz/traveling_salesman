// TSP with Minimum Spanning Tree
// Compile: g++ -O2 -std=c++17 main_MST.cpp -o mst
// Execution: ./mst dsj1000.tsp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <stack>
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
    This is due to the n iterations, each one scanning the n cities twice
    (once to pick the closest one outside the tree and once to update the keys).

    Space complexity: O(n) for the parent, key and inTree arrays.

*/
vector<int> primMST(const vector<vector<long long>>& D) {
    int n = D.size();
    vector<int> parent(n, -1);
    vector<long long> key(n, LLONG_MAX);
    vector<bool> inTree(n, false);
    key[0] = 0;

    for (int it = 0; it < n; it++) {
        int v = -1;
        for (int u = 0; u < n; u++)
            if (!inTree[u] && (v == -1 || key[u] < key[v])) v = u;
        inTree[v] = true;

        for (int u = 0; u < n; u++) {
            if (!inTree[u] && D[v][u] < key[u]) {
                key[u] = D[v][u];
                parent[u] = v;
            }
        }
    }

    return parent;
}

/*
    Time complexity: O(n^2) where n is the number of cities.
    Prim takes O(n^2); building the children lists and the preorder traversal
    visit each city and each tree edge once, which is O(n).

    Space complexity: O(n) for the parent, children, stack and tour arrays
    (the tree has n - 1 edges).

*/
vector<int> solveMST(const vector<vector<long long>>& D) {
    int n = D.size();
    vector<int> tour;
    tour.reserve(n);

    vector<int> parent = primMST(D);
    vector<vector<int>> children(n);
    for (int v = 0; v < n; v++)
        if (parent[v] != -1) children[parent[v]].push_back(v);

    stack<int> st;
    st.push(0);
    while (!st.empty()) {
        int v = st.top();
        st.pop();
        tour.push_back(v);
        for (int c : children[v]) st.push(c);
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

    cout << "MST-TSP  (" << cities.size() << " READ CITIES, SEEDS 1, 2, 3)\n";
    cout << "N\tMean\tStd.Dev.\n";

    for (int n : sizes) {
        vector<long long> costs;

        for (int seed : seeds) {
            vector<City> sub = sampleCities(cities, n, seed);
            vector<vector<long long>> D = buildMatrix(sub);
            vector<int> tour = solveMST(D);

            if ((int)tour.size() != n) {
                cout << "Invalid tour at N = " << n << "\n";
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
        if (n == 1000) cout << "\t(" << (mean - OPTIMAL) * 100.0 / OPTIMAL << "% above optimal)";
        cout << "\n";
    }
    return 0;
}