#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

    int n_global, M_global, E_global;
    if (!(cin >> n_global >> M_global >> E_global)) return 0;

    vector<Anime> catalogo(n_global);

    for (int i = 0; i < n_global; ++i) {
        int q;
        long long bono;
        cin >> catalogo[i].nombre >> q >> bono;

        catalogo[i].opciones.push_back({0, 0, 0});
        int t_acum = 0, c_acum = 0;
        long long v_acum = 0;

        for (int j = 1; j <= q; ++j) {
            int t, c;
            long long v;
            cin >> t >> c >> v;
            t_acum += t;
            c_acum += c;
            v_acum += v;
            
            long long satisfaccion_total = v_acum;
            if (j == q) satisfaccion_total += bono; // Bono de completación
            
            catalogo[i].opciones.push_back({t_acum, c_acum, satisfaccion_total});
        }
    }

    // dp[m][e] = max satisfacción usando 'm' minutos y 'e' energía
    vector<vector<long long>> dp(M_global + 1, vector<long long>(E_global + 1, 0));

    for (int i = 0; i < n_global; ++i) {
        vector<vector<long long>> next_dp = dp; 
        
        for (const auto& opcion : catalogo[i].opciones) {
            if (opcion.tiempo == 0 && opcion.energia == 0) continue;
            
            for (int m = opcion.tiempo; m <= M_global; ++m) {
                for (int e = opcion.energia; e <= E_global; ++e) {
                    next_dp[m][e] = max(next_dp[m][e], dp[m - opcion.tiempo][e - opcion.energia] + opcion.satisfaccion);
                }
            }
        }
        dp = next_dp;
    }

    long long max_satisfaccion = 0;
    for (int m = 0; m <= M_global; ++m) {
        for (int e = 0; e <= E_global; ++e) {
            max_satisfaccion = max(max_satisfaccion, dp[m][e]);
        }
    }

    cout << max_satisfaccion << "\n";
    return 0;
}