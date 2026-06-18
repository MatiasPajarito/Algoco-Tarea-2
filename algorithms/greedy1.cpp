#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct OpcionGreedy {
    int id_anime;
    int tiempo;
    int energia;
    long long satisfaccion;
    double ratio;
};

// Ordenar de mayor a menor ratio
bool compararOpciones(const OpcionGreedy& a, const OpcionGreedy& b) {
    return a.ratio > b.ratio;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, M, E;
    if (!(cin >> n >> M >> E)) return 0;

    vector<OpcionGreedy> todas_opciones;

    for (int i = 0; i < n; ++i) {
        string nombre;
        int q;
        long long bono;
        cin >> nombre >> q >> bono;

        int t_acum = 0, c_acum = 0;
        long long v_acum = 0;

        for (int j = 1; j <= q; ++j) {
            int t, c;
            long long v;
            cin >> t >> c >> v;
            t_acum += t;
            c_acum += c;
            v_acum += v;

            long long satisfaccion_total = v_acum + (j == q ? bono : 0);
            double ratio = (double)satisfaccion_total / t_acum; // Criterio Greedy 1

            todas_opciones.push_back({i, t_acum, c_acum, satisfaccion_total, ratio});
        }
    }

    sort(todas_opciones.begin(), todas_opciones.end(), compararOpciones);

    vector<bool> anime_visto(n, false);
    int tiempo_gastado = 0;
    int energia_gastada = 0;
    long long satisfaccion_total = 0;

    for (const auto& op : todas_opciones) {
        if (!anime_visto[op.id_anime] && 
            tiempo_gastado + op.tiempo <= M && 
            energia_gastada + op.energia <= E) {
            
            anime_visto[op.id_anime] = true;
            tiempo_gastado += op.tiempo;
            energia_gastada += op.energia;
            satisfaccion_total += op.satisfaccion;
        }
    }

    cout << satisfaccion_total << "\n";
    return 0;
}