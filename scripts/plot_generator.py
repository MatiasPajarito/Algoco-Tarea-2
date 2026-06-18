import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import os

# ── Configuración ──────────────────────────────────────────────────────────────
CSV_PATH    = "../data/measurements/tiempos_ejecucion.csv"
OUTPUTS_DIR = "../data/outputs/"
PLOTS_DIR   = "../data/plots/"
os.makedirs(PLOTS_DIR, exist_ok=True)

COLORES = {
    "BruteForce": "#e74c3c",
    "DP":         "#2ecc71",
    "Greedy1":    "#3498db",
    "Greedy2":    "#f39c12",
}

N_VALUES = [3, 5, 8, 20, 40, 80, 100, 150, 200]

# ── Función auxiliar para formatear el eje X igual en todos los gráficos ──────
def formatear_eje_x(ax):
    ax.set_xscale("linear")
    ax.set_xticks(N_VALUES)
    # Los valores 3, 5, 8 van en línea inferior para evitar que se amontonen
    etiquetas = []
    for n in N_VALUES:
        if n <= 8:
            etiquetas.append(f"\n{n}")
        else:
            etiquetas.append(str(n))
    ax.set_xticklabels(etiquetas, rotation=0)
    ax.set_xlim(-5, 210)

# ── Carga de datos de tiempo ───────────────────────────────────────────────────
df = pd.read_csv(CSV_PATH)
df["n"] = df["Archivo"].str.extract(r"testcases_(\d+)_\d+").astype(int)

df_mean = (df.groupby(["Algoritmo", "n"])["Tiempo_ms"]
             .mean()
             .reset_index()
             .rename(columns={"Tiempo_ms": "Tiempo_mean_ms"}))

# ── Carga de resultados para calidad ──────────────────────────────────────────
def leer_resultado(prefijo, n, i):
    path = os.path.join(OUTPUTS_DIR, f"{prefijo}_{n}_{i}.txt")
    try:
        with open(path) as f:
            return int(f.read().strip())
    except Exception:
        return None

filas_calidad = []
for n in N_VALUES:
    for i in range(1, 4):
        dp_val = leer_resultado("dp", n, i)
        if dp_val is None or dp_val == 0:
            continue
        for algo, prefijo in [("Greedy1", "g1"), ("Greedy2", "g2")]:
            val = leer_resultado(prefijo, n, i)
            if val is not None:
                filas_calidad.append({
                    "Algoritmo": algo,
                    "n":         n,
                    "instancia": i,
                    "ratio":     val / dp_val * 100
                })

df_calidad      = pd.DataFrame(filas_calidad)
df_calidad_mean = (df_calidad.groupby(["Algoritmo", "n"])["ratio"]
                              .mean()
                              .reset_index())

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 1 — Tiempo de ejecución (escala logarítmica) vs n
# ══════════════════════════════════════════════════════════════════════════════
fig, ax = plt.subplots(figsize=(10, 6))

for algo, grupo in df_mean.groupby("Algoritmo"):
    grupo = grupo.sort_values("n")
    ax.plot(grupo["n"], grupo["Tiempo_mean_ms"],
            marker="o", linewidth=2, label=algo, color=COLORES[algo])

# Anotación explicando por qué BruteForce se corta
bf_n8 = df_mean[(df_mean["Algoritmo"] == "BruteForce") &
                (df_mean["n"] == 8)]["Tiempo_mean_ms"].values
if len(bf_n8) > 0:
    ax.annotate("BruteForce solo\nejecutado hasta n=8",
                xy=(8, bf_n8[0]),
                xytext=(40, 0.05),
                arrowprops=dict(arrowstyle="->", color="gray"),
                fontsize=9, color="gray")

ax.set_yscale("log")
ax.set_xlabel("Número de animes (n)", fontsize=13)
ax.set_ylabel("Tiempo promedio (ms) — escala log", fontsize=13)
ax.set_title("Tiempo de ejecución por algoritmo vs tamaño de entrada", fontsize=14)
formatear_eje_x(ax)
ax.legend(fontsize=11)
ax.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "tiempos_log.png"), dpi=150)
plt.close()
print("✓ tiempos_log.png")

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 2 — Tiempo de ejecución solo DP (escala lineal)
# ══════════════════════════════════════════════════════════════════════════════
fig, ax = plt.subplots(figsize=(10, 6))

dp_data = df_mean[df_mean["Algoritmo"] == "DP"].sort_values("n")
ax.plot(dp_data["n"], dp_data["Tiempo_mean_ms"],
        marker="s", linewidth=2.5, color=COLORES["DP"], label="DP")

# Anotar cada punto con su valor exacto
for _, row in dp_data.iterrows():
    ax.annotate(f'{row["Tiempo_mean_ms"]:.0f} ms',
                xy=(row["n"], row["Tiempo_mean_ms"]),
                xytext=(5, 8), textcoords="offset points",
                fontsize=8, color="#1a8a50")

ax.set_xlabel("Número de animes (n)", fontsize=13)
ax.set_ylabel("Tiempo promedio (ms)", fontsize=13)
ax.set_title("Tiempo de la DP vs n (escala lineal)\n"
             "La varianza entre casos del mismo n refleja diferencias en M y E",
             fontsize=13)
formatear_eje_x(ax)
ax.legend(fontsize=11)
ax.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "tiempos_dp_lineal.png"), dpi=150)
plt.close()
print("✓ tiempos_dp_lineal.png")

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 3 — Calidad de los Greedy como % del óptimo (DP)
# ══════════════════════════════════════════════════════════════════════════════
fig, ax = plt.subplots(figsize=(10, 6))

for algo, grupo in df_calidad_mean.groupby("Algoritmo"):
    grupo = grupo.sort_values("n")
    ax.plot(grupo["n"], grupo["ratio"],
            marker="o", linewidth=2, label=algo, color=COLORES[algo])

ax.axhline(100, color="gray", linestyle="--", linewidth=1.2,
           label="Óptimo (DP = 100%)")
ax.set_ylim(0, 115)
ax.set_xlabel("Número de animes (n)", fontsize=13)
ax.set_ylabel("Calidad relativa (% del óptimo)", fontsize=13)
ax.set_title("Calidad de los Greedy relativa a la solución óptima (DP)", fontsize=14)
formatear_eje_x(ax)
ax.legend(fontsize=11)
ax.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "calidad_greedy.png"), dpi=150)
plt.close()
print("✓ calidad_greedy.png")

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 4 — Dispersión de calidad por instancia individual (scatter)
# ══════════════════════════════════════════════════════════════════════════════
fig, ax = plt.subplots(figsize=(10, 6))

# Offset horizontal para que los puntos de G1 y G2 no se superpongan
offsets = {"Greedy1": -2, "Greedy2": 2}

for algo, grupo in df_calidad.groupby("Algoritmo"):
    x_jitter = grupo["n"] + offsets.get(algo, 0)
    ax.scatter(x_jitter, grupo["ratio"],
               alpha=0.75, s=70, label=algo, color=COLORES[algo],
               edgecolors="white", linewidths=0.5)

ax.axhline(100, color="gray", linestyle="--", linewidth=1.2, label="Óptimo")
ax.set_ylim(0, 115)
ax.set_xlabel("Número de animes (n)", fontsize=13)
ax.set_ylabel("Calidad (% del óptimo)", fontsize=13)
ax.set_title("Dispersión de calidad por instancia individual", fontsize=14)
formatear_eje_x(ax)
ax.legend(fontsize=11)
ax.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "calidad_scatter.png"), dpi=150)
plt.close()
print("✓ calidad_scatter.png")

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 5 — Comparación DP (ms) vs Greedy (µs) en paneles separados
# ══════════════════════════════════════════════════════════════════════════════
fig, axes = plt.subplots(1, 2, figsize=(14, 5))

# Panel izquierdo: DP
dp_data = df_mean[df_mean["Algoritmo"] == "DP"].sort_values("n")
axes[0].plot(dp_data["n"], dp_data["Tiempo_mean_ms"],
             marker="s", color=COLORES["DP"], linewidth=2, label="DP")
axes[0].set_title("Programación Dinámica", fontsize=13)
axes[0].set_xlabel("Número de animes (n)", fontsize=11)
axes[0].set_ylabel("Tiempo promedio (ms)", fontsize=11)
formatear_eje_x(axes[0])
axes[0].grid(True, linestyle="--", alpha=0.5)
axes[0].legend(fontsize=10)

# Panel derecho: ambos Greedy en microsegundos
for algo in ["Greedy1", "Greedy2"]:
    g_data = df_mean[df_mean["Algoritmo"] == algo].sort_values("n")
    axes[1].plot(g_data["n"], g_data["Tiempo_mean_ms"] * 1000,
                 marker="o", color=COLORES[algo], linewidth=2, label=algo)

axes[1].set_title("Heurísticas Greedy", fontsize=13)
axes[1].set_xlabel("Número de animes (n)", fontsize=11)
axes[1].set_ylabel("Tiempo promedio (µs)", fontsize=11)
formatear_eje_x(axes[1])
axes[1].legend(fontsize=10)
axes[1].grid(True, linestyle="--", alpha=0.5)

fig.suptitle("Comparación de tiempos: DP (ms) vs Greedy (µs)", fontsize=14)
plt.tight_layout()
plt.savefig(os.path.join(PLOTS_DIR, "comparacion_tiempos.png"), dpi=150)
plt.close()
print("✓ comparacion_tiempos.png")

# ══════════════════════════════════════════════════════════════════════════════
# RESUMEN EN CONSOLA
# ══════════════════════════════════════════════════════════════════════════════
print("\n── Resumen de calidad promedio por algoritmo ──")
resumen = df_calidad.groupby("Algoritmo")["ratio"].agg(["mean", "min", "max"])
resumen.columns = ["Promedio %", "Mínimo %", "Máximo %"]
print(resumen.round(2).to_string())

print("\n── Tiempos promedio por n (todos los algoritmos) ──")
pivot = df_mean.pivot(index="n", columns="Algoritmo", values="Tiempo_mean_ms")
print(pivot.round(4).to_string())