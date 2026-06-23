# AniMarathon — Tarea 2 · INF-221 Algoritmos y Complejidad

> **¿Quién ganará? Greedy vs Dinámico vs Bruto.**

Comparación experimental de cuatro algoritmos para resolver una variante de la mochila
multidimensional con restricción de prefijo.

---

## Tabla de contenidos

- [Descripción del problema](#descripción-del-problema)
- [Estructura del repositorio](#estructura-del-repositorio)
- [Requisitos](#requisitos)
- [Compilación](#compilación)
- [Generación de casos de prueba](#generación-de-casos-de-prueba)
- [Ejecución](#ejecución)
- [Generación de gráficos](#generación-de-gráficos)
- [Algoritmos implementados](#algoritmos-implementados)
- [Formato de entrada y salida](#formato-de-entrada-y-salida)
- [Resultados](#resultados)
- [Autor](#autor)

---

## Descripción del problema

En **AniMarathon**, se dispone de una lista de animes, cada uno con una cantidad fija
de capítulos. Cada capítulo tiene una duración en minutos, un costo de energía y un
puntaje de satisfacción. Además, completar todos los capítulos de un anime otorga un
bono adicional de satisfacción.

**Restricciones clave:**
- Solo se puede ver un **prefijo de capítulos** de cada anime (0, 1, 2, … hasta q_i).
- La suma total de minutos no puede superar **M**.
- La suma total de energía no puede superar **E**.

**Objetivo:** maximizar la satisfacción total.

---

## Estructura del repositorio

```
INF221-2026-1-TAREA-2/
├── code/
│   └── implementation/
│       ├── algorithms/
│       │   ├── brute-force.cpp          # Fuerza bruta con backtracking
│       │   ├── dynamic-programming.cpp  # Programación dinámica 2D
│       │   ├── greedy1.cpp              # Greedy: ratio satisfacción/tiempo
│       │   └── greedy2.cpp              # Greedy: ratio normalizado por M y E
│       ├── data/
│       │   ├── inputs/                  # Casos de prueba generados (.txt)
│       │   ├── outputs/                 # Resultados de cada algoritmo (.txt)
│       │   ├── measurements/            # Tiempos de ejecución (.csv)
│       │   └── plots/                   # Gráficos generados (.png)
│       ├── scripts/
│       │   ├── testcases_generator.py   # Generador de casos de prueba
│       │   └── plot_generator.py        # Generador de gráficos
│       ├── general.cpp                  # Orquestador principal
│       └── makefile                     # Compilación de todos los binarios
└── report/
    ├── report.tex                       # Informe principal en LaTeX
    ├── preamble.tex
    ├── author.tex
    ├── references.bib
    └── sections/
        ├── experiment_intro.tex
        ├── experiment_results.tex
        └── conclusions.tex
```

---

## Requisitos

### C++
- Compilador con soporte C++17: `g++` versión 9 o superior
- Make

### Python
- Python 3.8 o superior
- Librerías: `pandas`, `matplotlib`, `numpy`

```bash
pip3 install pandas matplotlib numpy
```

### LaTeX (solo para compilar el informe)
```bash
sudo apt install texlive-full biber
```

---

## Compilación

Desde la carpeta `code/implementation/`:

```bash
make
```

Esto compila los cuatro algoritmos y el orquestador, generando los ejecutables:
- `brute-force`
- `dynamic-programming`
- `greedy1`
- `greedy2`
- `general`

Para limpiar los ejecutables:

```bash
make clean
```

---

## Generación de casos de prueba

Desde `code/implementation/scripts/`:

```bash
python3 testcases_generator.py
```

Genera **27 archivos** en `data/inputs/` con la nomenclatura `testcases_{n}_{i}.txt`,
donde `n` es la cantidad de animes e `i` el identificador de instancia (1, 2 o 3).

| Categoría | Tamaños de n       | Uso                                          |
|-----------|--------------------|----------------------------------------------|
| Pequeños  | 3, 5, 8            | Validar correctitud (BF == DP)               |
| Medianos  | 20, 40, 80         | Comparar calidad greedy vs óptimo            |
| Grandes   | 100, 150, 200      | Medir rendimiento de algoritmos eficientes   |

---

## Ejecución

### Ejecución completa (todos los algoritmos sobre todos los casos)

Desde `code/implementation/`:

```bash
./general
```

Esto ejecuta automáticamente todos los algoritmos sobre los 27 casos de prueba,
guarda los resultados en `data/outputs/` y los tiempos en
`data/measurements/tiempos_ejecucion.csv`.

> **Nota:** La Fuerza Bruta solo se ejecuta para `n ≤ 8` por su complejidad exponencial.

### Ejecución manual de un algoritmo individual

```bash
./brute-force         < data/inputs/testcases_3_1.txt
./dynamic-programming < data/inputs/testcases_20_1.txt
./greedy1             < data/inputs/testcases_100_1.txt
./greedy2             < data/inputs/testcases_100_1.txt
```

Cada algoritmo imprime el resultado en `stdout` y el tiempo de ejecución en `stderr`:

```bash
# Ver resultado y tiempo por separado
./dynamic-programming < data/inputs/testcases_20_1.txt > resultado.txt 2> tiempo.txt
```

---

## Generación de gráficos

Una vez ejecutado `./general`, desde `code/implementation/scripts/`:

```bash
python3 plot_generator.py
```

Genera 5 gráficos en `data/plots/`:

| Archivo                    | Contenido                                              |
|----------------------------|--------------------------------------------------------|
| `tiempos_log.png`          | Tiempo de todos los algoritmos en escala logarítmica   |
| `tiempos_dp_lineal.png`    | Tiempo de la DP en escala lineal con valores exactos   |
| `comparacion_tiempos.png`  | DP en ms vs Greedy en µs en paneles separados          |
| `calidad_greedy.png`       | Calidad promedio de los Greedy como % del óptimo       |
| `calidad_scatter.png`      | Dispersión de calidad por instancia individual         |

---

## Algoritmos implementados

### Fuerza Bruta (`brute-force.cpp`)
Backtracking recursivo que prueba exhaustivamente todas las combinaciones de prefijos.
Aplica poda inmediata cuando se superan los límites `M` o `E`.
- **Complejidad temporal:** O(∏(q_i + 1))
- **Complejidad espacial:** O(n)
- **Viable para:** n ≤ 8

### Programación Dinámica (`dynamic-programming.cpp`)
Tabla 2D `dp[m][e]` = máxima satisfacción con `m` minutos y `e` energía usados.
Usa tabla auxiliar `next_dp` por anime para garantizar que se elige a lo sumo un
prefijo por serie, evitando contar el mismo anime más de una vez.
- **Complejidad temporal:** O(n · M · E)
- **Complejidad espacial:** O(M · E)
- **Garantiza:** solución óptima

### Greedy 1 (`greedy1.cpp`)
Ordena todas las opciones de prefijo por `satisfacción / tiempo` de mayor a menor.
Selecciona la mejor opción disponible de cada anime que no exceda los recursos.
- **Complejidad temporal:** O(Q log Q), donde Q ≤ 700
- **Limitación:** ignora la restricción de energía al ordenar

### Greedy 2 (`greedy2.cpp`)
Ordena por `satisfacción / (tiempo/M + energía/E)`, normalizando ambos recursos
respecto a sus límites máximos para una comparación equitativa.
- **Complejidad temporal:** O(Q log Q)
- **Mejor que Greedy 1:** considera ambas restricciones simultáneamente

---

## Formato de entrada y salida

### Entrada
```
n M E
nombre_anime_1 q_1 b_1
t_1_1 c_1_1 v_1_1
...
t_1_q1 c_1_q1 v_1_q1
...
nombre_anime_n q_n b_n
t_n_1 c_n_1 v_n_1
...
```

### Salida
```
MaxSatisfaccion
```

### Ejemplo
```
4 240 90
shonen_quest 3 25
45 18 40
50 22 45
55 25 60
romcom_days 2 15
30 10 25
35 12 30
mecha_nova 2 50
60 30 65
70 35 75
slice_cafe 1 5
25 8 18
```
Salida esperada: `260`

---

## Resultados

Resumen de los resultados experimentales obtenidos:

| Algoritmo   | Tiempo en n=200 | Calidad promedio |
|-------------|-----------------|------------------|
| DP          | ~1697 ms        | 100% (óptimo)    |
| Greedy 2    | ~63 µs          | 92.92%           |
| Greedy 1    | ~71 µs          | 58.04%           |
| Fuerza Bruta| solo n ≤ 8      | 100% (óptimo)    |

La DP y la Fuerza Bruta coinciden exactamente en todos los casos pequeños,
validando la correctitud de ambas implementaciones.

---

## Autor

- **Nombre:** Matias Antonio Pajarito Catalan
- **Rol USM:** 202273522-8
- **Curso:** INF-221 Algoritmos y Complejidad — 2026-1
