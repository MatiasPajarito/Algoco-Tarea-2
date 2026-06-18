#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <vector>

using namespace std;

// Función auxiliar para ejecutar un comando en la terminal
void ejecutar_algoritmo(const string& nombre_ejecutable, const string& input_file, const string& output_file, const string& nombre_algoritmo, ofstream& log_tiempos) {
    
    // Comando para redirigir la entrada y salida: ./brute-force < inputs/x.txt > outputs/y.txt
    string comando = "./" + nombre_ejecutable + " < " + input_file + " > " + output_file;
    
    // Iniciar cronómetro
    auto start = chrono::high_resolution_clock::now();
    
    // Ejecutar en la terminal
    int resultado = system(comando.c_str());
    
    // Detener cronómetro
    auto end = chrono::high_resolution_clock::now();
    
    if (resultado != 0) {
        cerr << "Error ejecutando " << nombre_algoritmo << " con " << input_file << "\n";
        return;
    }

    chrono::duration<double, std::milli> duracion = end - start;
    
    // Guardar la medición en el archivo CSV
    log_tiempos << nombre_algoritmo << "," << input_file << "," << duracion.count() << "\n";
    
    cout << "[" << nombre_algoritmo << "] " << input_file << " -> " << duracion.count() << " ms\n";
}

int main() {
    // Archivo de salida para las mediciones de tiempo
    ofstream log_tiempos("data/measurements/tiempos_ejecucion.csv");
    log_tiempos << "Algoritmo,Archivo,Tiempo_ms\n";

    // Tamaños generados por nuestro script de Python
    vector<int> n_values = {3, 5, 8, 20, 40, 80, 100, 150, 200};

    for (int n : n_values) {
        for (int i = 1; i <= 3; ++i) {
            string input_file = "data/inputs/testcases_" + to_string(n) + "_" + to_string(i) + ".txt";
            
            // 1. Ejecutar Greedy 1
            ejecutar_algoritmo("greedy1", input_file, "data/outputs/g1_" + to_string(n) + "_" + to_string(i) + ".txt", "Greedy1", log_tiempos);
            
            // 2. Ejecutar Greedy 2
            ejecutar_algoritmo("greedy2", input_file, "data/outputs/g2_" + to_string(n) + "_" + to_string(i) + ".txt", "Greedy2", log_tiempos);
            
            // 3. Ejecutar Programación Dinámica
            ejecutar_algoritmo("dynamic-programming", input_file, "data/outputs/dp_" + to_string(n) + "_" + to_string(i) + ".txt", "DP", log_tiempos);
            
            // 4. Ejecutar Fuerza Bruta (¡SOLO para casos pequeños para no colgar el PC!)
            if (n <= 8) {
                ejecutar_algoritmo("brute-force", input_file, "data/outputs/bf_" + to_string(n) + "_" + to_string(i) + ".txt", "BruteForce", log_tiempos);
            }
        }
    }

    log_tiempos.close();
    cout << "Todas las ejecuciones terminadas. Tiempos guardados en data/measurements/\n";
    return 0;
}