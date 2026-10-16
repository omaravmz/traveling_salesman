# TSP: NN vs Cheapest-link vs MST

Comparación de tres heurísticas para el TSP (Traveling Salesman Problem) sobre subconjuntos aleatorios de `dsj1000.tsp`.

## Estructura

```
tsp-comparison/
├── dsj1000.tsp      # instancia (copiar aquí)
├── tsp_common.h     # lectura, distancia CEIL_2D, muestreo, costo, experimentos
├── main_NN.cpp      # Nearest Neighbor
├── main_CL.cpp      # Cheapest-link (Greedy Edge)
├── main_MST.cpp     # Minimum Spanning Tree + recorrido en preorden
└── results/         # CSV generados
```

## Compilar y ejecutar (Linux)

```bash
g++ -O2 -std=c++17 main_NN.cpp  -o nn
g++ -O2 -std=c++17 main_CL.cpp  -o cl
g++ -O2 -std=c++17 main_MST.cpp -o mst

./nn dsj1000.tsp
```

Para guardar la tabla en un archivo:

```bash
mkdir -p results
./nn  dsj1000.tsp > results/nn.csv
./cl  dsj1000.tsp > results/cl.csv
./mst dsj1000.tsp > results/mst.csv
```

## Metodología

- N ∈ {10, 50, 100, 200, 400, 600, 800, 1000}, ciudades elegidas al azar sin repetir.
- 3 semillas (1, 2, 3). Para cada semilla, los tres algoritmos usan **el mismo subconjunto** de ciudades.
- Se reporta promedio y desviación estándar muestral del costo del tour.
- Para N = 1000 se reporta el gap contra el óptimo (18,659,688): `(costo - óptimo) / óptimo × 100`.

## Salida

Una fila CSV por N: `N,mean,std,gap_pct,avg_ms`
