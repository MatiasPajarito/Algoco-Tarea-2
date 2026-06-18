#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

struct OpcionPrefijo {
    int tiempo;
    int energia;
    long long satisfaccion;
};

struct Anime {
    string nombre;
    vector<OpcionPrefijo> opciones;
};

long long max_satisfaccion_global = 0;
int n_global, M_global, E_global;
vector<Anime> catalogo;

void brute_force(int idx, int tiempo_actual, int energia_actual, long long satisfaccion_actual) {
    if (idx == n_global) {
        if (satisfaccion_actual > max_satisfaccion_global)
            max_satisfaccion_global = satisfaccion_actual;
        return;
    }
    for (const auto& opcion : catalogo[idx].opciones) {
        int nuevo_tiempo  = tiempo_actual  + opcion.tiempo;
        int nueva_energia = energia_actual + opcion.energia;
        if (nuevo_tiempo <= M_global && nueva_energia <= E_global)
            brute_force(idx + 1, nuevo_tiempo, nueva_energia,
                        satisfaccion_actual + opcion.satisfaccion);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n_global >> M_global >> E_global)) return 0;
    catalogo.resize(n_global);

    for (int i = 0; i < n_global; ++i) {
        int q;
        long long bono;
        cin >> catalogo[i].nombre >> q >> bono;

        catalogo[i].opciones.push_back({0, 0, 0});
        int t_acum = 0, c_acum = 0;
        long long v_acum = 0;

        for (int j = 1; j <= q; ++j) {
            int t, c; long long v;
            cin >> t >> c >> v;
            t_acum += t; c_acum += c; v_acum += v;
            long long sat = v_acum + (j == q ? bono : 0);
            catalogo[i].opciones.push_back({t_acum, c_acum, sat});
        }
    }

    // ── Cronometría: solo el algoritmo, sin I/O ──
    auto t_start = chrono::high_resolution_clock::now();
    brute_force(0, 0, 0, 0);
    auto t_end   = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed = t_end - t_start;

    cout << max_satisfaccion_global << "\n";   // resultado → stdout
    cerr << elapsed.count() << "\n";           // tiempo    → stderr
    return 0;
}