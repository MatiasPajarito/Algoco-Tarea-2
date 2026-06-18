#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;

// Ejecuta el algoritmo y captura el tiempo que el propio binario imprime en stderr
void ejecutar_algoritmo(const string& ejecutable,
                        const string& input_file,
                        const string& output_file,
                        const string& nombre_algoritmo,
                        ofstream& log_tiempos) {

    // Archivo temporal para capturar stderr (el tiempo interno)
    string tiempo_file = "data/measurements/tmp_tiempo.txt";

    string comando = "./" + ejecutable
                   + " < "  + input_file
                   + " > "  + output_file
                   + " 2> " + tiempo_file;

    int resultado = system(comando.c_str());

    if (resultado != 0) {
        cerr << "Error ejecutando " << nombre_algoritmo
             << " con " << input_file << "\n";
        return;
    }

    // Leer el tiempo que el binario escribió en stderr
    double tiempo_ms = -1.0;
    ifstream tf(tiempo_file);
    if (tf.is_open()) tf >> tiempo_ms;

    log_tiempos << nombre_algoritmo << ","
                << input_file       << ","
                << tiempo_ms        << "\n";

    cout << "[" << nombre_algoritmo << "] "
         << input_file << " -> " << tiempo_ms << " ms\n";
}

int main() {
    ofstream log_tiempos("data/measurements/tiempos_ejecucion.csv");
    log_tiempos << "Algoritmo,Archivo,Tiempo_ms\n";

    vector<int> n_values = {3, 5, 8, 20, 40, 80, 100, 150, 200};

    for (int n : n_values) {
        for (int i = 1; i <= 3; ++i) {
            string input_file = "data/inputs/testcases_"
                              + to_string(n) + "_"
                              + to_string(i) + ".txt";

            ejecutar_algoritmo("greedy1", input_file,
                "data/outputs/g1_" + to_string(n) + "_" + to_string(i) + ".txt",
                "Greedy1", log_tiempos);

            ejecutar_algoritmo("greedy2", input_file,
                "data/outputs/g2_" + to_string(n) + "_" + to_string(i) + ".txt",
                "Greedy2", log_tiempos);

            ejecutar_algoritmo("dynamic-programming", input_file,
                "data/outputs/dp_" + to_string(n) + "_" + to_string(i) + ".txt",
                "DP", log_tiempos);

            if (n <= 8) {
                ejecutar_algoritmo("brute-force", input_file,
                    "data/outputs/bf_" + to_string(n) + "_" + to_string(i) + ".txt",
                    "BruteForce", log_tiempos);
            }
        }
    }

    log_tiempos.close();
    cout << "Listo. Tiempos reales guardados en data/measurements/tiempos_ejecucion.csv\n";
    return 0;
}