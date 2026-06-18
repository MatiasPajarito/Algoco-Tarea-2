# Compilador y banderas de optimización
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3

# Nombres de los ejecutables
BRUTE = brute-force
DP = dynamic-programming
GREEDY1 = greedy1
GREEDY2 = greedy2
GENERAL = general

# Directorio de los códigos fuente de los algoritmos
SRC_DIR = algorithms

# Objetivos a compilar
all: $(BRUTE) $(DP) $(GREEDY1) $(GREEDY2) $(GENERAL)

$(BRUTE): $(SRC_DIR)/brute-force.cpp
	$(CXX) $(CXXFLAGS) -o $(BRUTE) $(SRC_DIR)/brute-force.cpp

$(DP): $(SRC_DIR)/dynamic-programming.cpp
	$(CXX) $(CXXFLAGS) -o $(DP) $(SRC_DIR)/dynamic-programming.cpp

$(GREEDY1): $(SRC_DIR)/greedy1.cpp
	$(CXX) $(CXXFLAGS) -o $(GREEDY1) $(SRC_DIR)/greedy1.cpp

$(GREEDY2): $(SRC_DIR)/greedy2.cpp
	$(CXX) $(CXXFLAGS) -o $(GREEDY2) $(SRC_DIR)/greedy2.cpp

$(GENERAL): general.cpp
	$(CXX) $(CXXFLAGS) -o $(GENERAL) general.cpp

# Regla para limpiar los ejecutables compilados
clean:
	rm -f $(BRUTE) $(DP) $(GREEDY1) $(GREEDY2) $(GENERAL)