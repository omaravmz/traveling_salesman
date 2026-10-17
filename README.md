# TSP: NN vs Cheapest-link vs MST

Comparison of three heuristics for the TSP (Traveling Salesman Problem) using random subsets of the `dsj1000.tsp` instance (1000 cities, CEIL_2D distance).

## Files

```
traveling_salesman/
├── dsj1000.tsp     # problem instance
├── main_NN.cpp     # Nearest Neighbor
├── main_CL.cpp     # Cheapest-link (Greedy Edge)
└── main_MST.cpp    # Minimum Spanning Tree (Prim + preorder traversal)
```

Each program is self-contained: it reads the file, runs the experiments and prints its column of the results table.

## Compile and run (Linux)

```bash
g++ -O2 -std=c++17 main_NN.cpp  -o nn
g++ -O2 -std=c++17 main_CL.cpp  -o cl
g++ -O2 -std=c++17 main_MST.cpp -o mst

./nn  dsj1000.tsp
./cl  dsj1000.tsp
./mst dsj1000.tsp
```

## Methodology

- For each N ∈ {10, 50, 100, 200, 400, 600, 800, 1000}, N cities are chosen at random, without repetition, from the 1000 in the file.
- Each case was run with **3 random seeds (1, 2 and 3)**. For a given seed, all three algorithms receive exactly the same cities.
- Each cell reports the **mean ± sample standard deviation** of the tour cost over the 3 runs.
- For N = 1000, the result is compared against the known optimal tour (18,659,688): `gap = (mean − optimal) / optimal × 100`.

## Results

Tour cost (mean ± standard deviation, 3 seeds). The best result in each row is in bold.

| Number of Cities | NN-TSP | Cheapest-link-TSP | MST-TSP |
|---:|---:|---:|---:|
| 10 | **3,107,860.7 ± 264,864.2** | 3,137,122.3 ± 320,481.2 | 3,817,385.3 ± 200,534.8 |
| 50 | **5,725,401.3 ± 98,868.5** | 5,781,286.3 ± 156,427.6 | 6,479,683.3 ± 93,845.7 |
| 100 | 8,302,006.0 ± 1,013,879.2 | **7,773,668.0 ± 422,203.1** | 8,643,501.3 ± 398,199.0 |
| 200 | 11,710,378.3 ± 443,458.8 | **10,583,320.3 ± 273,870.6** | 11,791,399.0 ± 538,600.7 |
| 400 | 15,748,196.0 ± 471,544.2 | **14,347,501.3 ± 459,410.2** | 16,146,136.0 ± 635,620.7 |
| 600 | 18,033,531.7 ± 452,447.2 | **17,718,590.7 ± 1,200,243.5** | 19,798,870.7 ± 498,490.0 |
| 800 | 21,673,959.7 ± 640,361.1 | **19,868,549.3 ± 367,638.0** | 23,057,342.7 ± 454,231.1 |
| 1000 | 24,577,905.3 ± 560,873.2 | **21,706,226.0 ± 0.0** | 25,674,926.3 ± 157,244.6 |

Gap against the optimal tour (N = 1000):

| NN-TSP | Cheapest-link-TSP | MST-TSP |
|---:|---:|---:|
| 31.7% | **16.3%** | 37.6% |

## Complexity

| Algorithm | Time | Extra space |
|---|---|---|
| NN-TSP | O(n²) | O(n) |
| Cheapest-link-TSP | O(n² log n), dominated by sorting the n(n−1)/2 edges | O(n²) |
| MST-TSP | O(n²), Prim on a complete graph | O(n) |

## Analysis

- **Cheapest-link produced the best tours from N = 100 onward**, and at N = 1000 it was only 16.3% above the optimal, about half the gap of the other two. Because it always picks the globally cheapest valid edge, it avoids the long "return" edges that NN is forced to take at the end of its tour.
- **For small instances (N = 10 and 50), NN was slightly better than Cheapest-link** (about 1%), but the difference is smaller than the standard deviation, so the two are effectively tied there.
- **MST was the weakest heuristic in every row** (37.6% gap at N = 1000). This is expected: shortcutting the preorder walk of the tree only guarantees a tour at most twice the optimal, and in practice it adds many long jumps between branches. Its result is still well within that 2× bound.
- **At N = 1000 the standard deviation of Cheapest-link is 0.** With all 1000 cities, every seed uses the same set of cities, only in a different order. Cheapest-link depends only on the sorted distances, so it always builds the same tour. NN and MST start from "city 0", which changes with each seed, so their results vary.
- The cost-quality trade-off: Cheapest-link is the slowest of the three (O(n² log n) versus O(n²)), but its tours were 12% shorter than NN's at N = 1000.