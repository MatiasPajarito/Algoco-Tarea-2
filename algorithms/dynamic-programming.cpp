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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, M, E;
    if (!(cin >> n >> M >> E)) return 0;

    vector<Anime> catalogo(n);
    for (int i = 0; i < n; ++i) {
        int q; long long bono;
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

    vector<vector<long long>> dp(M + 1, vector<long long>(E + 1, 0));

    for (int i = 0; i < n; ++i) {
        vector<vector<long long>> next_dp = dp;

        for (const auto& op : catalogo[i].opciones) {
            if (op.tiempo == 0 && op.energia == 0) continue;
            for (int m = op.tiempo; m <= M; ++m)
                for (int e = op.energia; e <= E; ++e)
                    next_dp[m][e] = max(next_dp[m][e],
                                        dp[m - op.tiempo][e - op.energia] + op.satisfaccion);
        }
        dp.swap(next_dp);   // O(1) en lugar de copiar 12 MB
    }

    long long resultado = 0;
    for (int m = 0; m <= M; ++m)
        for (int e = 0; e <= E; ++e)
            resultado = max(resultado, dp[m][e]);

    auto t_end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = t_end - t_start;

    cout << resultado << "\n";
    cerr << elapsed.count() << "\n";
    return 0;
}