import os
import random

# Dominio de los valores y restricciones del problema
MAX_Q_TOTAL = 700
MAX_M = 3000
MAX_E = 500
MAX_T = 300
MAX_C = 100
MAX_V = 10**9
MAX_B = 10**9

def generate_testcase(n, identifier, output_dir):
    # Generar M y E aleatorios pero dentro de los límites
    M = random.randint(max(1, n * 5), MAX_M)
    E = random.randint(max(1, n * 2), MAX_E)
    
    filename = f"testcases_{n}_{identifier}.txt"
    filepath = os.path.join(output_dir, filename)
    
    with open(filepath, 'w') as f:
        # Primera línea: n M E
        f.write(f"{n} {M} {E}\n")
        
        q_remaining = MAX_Q_TOTAL
        animes_remaining = n
        
        for i in range(1, n + 1):
            # Calcular un 'q' válido para no pasarse de 700 capítulos en total ni de 30 por anime
            max_q_for_this = min(30, q_remaining - (animes_remaining - 1))
            max_q_for_this = max(1, max_q_for_this)
            
            q = random.randint(1, max_q_for_this)
            q_remaining -= q
            animes_remaining -= 1
            
            b = random.randint(0, MAX_B)
            anime_name = f"anime_{i}"
            
            # Línea de cabecera del anime: nombre q b
            f.write(f"{anime_name} {q} {b}\n")
            
            # Líneas de los capítulos: t c v
            for _ in range(q):
                t = random.randint(1, MAX_T)
                c = random.randint(1, MAX_C)
                v = random.randint(1, MAX_V)
                f.write(f"{t} {c} {v}\n")

def main():
    # Asegurar que la ruta exista relativa a donde está el script
    # Se guarda en la carpeta ../data/inputs/ 
    script_dir = os.path.dirname(os.path.abspath(__file__))
    output_dir = os.path.join(script_dir, "..", "data", "inputs")
    os.makedirs(output_dir, exist_ok=True)
    
    # Categorías de casos de prueba
    small_cases = [3, 5, 8]
    medium_cases = [20, 40, 80]
    large_cases = [100, 150, 200]
    
    all_n_values = small_cases + medium_cases + large_cases
    
    # Generar 3 casos de prueba distintos (i) por cada tamaño (n)
    for n in all_n_values:
        for i in range(1, 4):
            generate_testcase(n, i, output_dir)
            
    print(f"Casos de prueba generados exitosamente en: {output_dir}")

if __name__ == "__main__":
    main()