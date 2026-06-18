#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Estructura para almacenar las opciones ya procesadas (prefijos acumulados)
struct OpcionPrefijo {
    int tiempo;
    int energia;
    long long satisfaccion;
};

struct Anime {
    string nombre;
    vector<OpcionPrefijo> opciones;
};

// Variables globales para facilitar el acceso en la recursión
long long max_satisfaccion_global = 0;
int n_global, M_global, E_global;
vector<Anime> catalogo;

// Función recursiva de backtracking (Fuerza Bruta)
void brute_force(int idx, int tiempo_actual, int energia_actual, long long satisfaccion_actual) {
    // Caso base: hemos evaluado todos los animes
    if (idx == n_global) {
        if (satisfaccion_actual > max_satisfaccion_global) {
            max_satisfaccion_global = satisfaccion_actual;
        }
        return;
    }

    // Explorar todas las opciones (prefijos) para el anime actual
    for (const auto& opcion : catalogo[idx].opciones) {
        int nuevo_tiempo = tiempo_actual + opcion.tiempo;
        int nueva_energia = energia_actual + opcion.energia;

        // Poda (Pruning): solo continuamos explorando esta rama si no excedemos los límites 
        if (nuevo_tiempo <= M_global && nueva_energia <= E_global) {
            brute_force(idx + 1, nuevo_tiempo, nueva_energia, satisfaccion_actual + opcion.satisfaccion);
        }
    }
}

int main() {
    // Optimización de lectura en C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Lectura de datos iniciales
    if (!(cin >> n_global >> M_global >> E_global)) return 0;

    catalogo.resize(n_global);

    for (int i = 0; i < n_global; ++i) {
        int q;
        long long bono;
        cin >> catalogo[i].nombre >> q >> bono;

        // La opción 0 es no ver ningún capítulo de este anime
        catalogo[i].opciones.push_back({0, 0, 0});

        int t_acum = 0;
        int c_acum = 0;
        long long v_acum = 0;

        // Leer y acumular los capítulos para generar los prefijos
        for (int j = 1; j <= q; ++j) {
            int t, c;
            long long v;
            cin >> t >> c >> v;

            t_acum += t;
            c_acum += c;
            v_acum += v;

            long long satisfaccion_total = v_acum;
            
            // Si es el último capítulo (prefijo completo), sumamos el bono 
            if (j == q) {
                satisfaccion_total += bono;
            }

            catalogo[i].opciones.push_back({t_acum, c_acum, satisfaccion_total});
        }
    }

    // Iniciar la búsqueda exhaustiva desde el anime 0 
    brute_force(0, 0, 0, 0);

    // Imprimir el resultado en el formato exacto que pide el enunciado
    cout << max_satisfaccion_global << "\n";

    return 0;
}