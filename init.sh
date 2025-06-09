#!/bin/bash

# Skrypt do automatycznego tworzenia struktury katalogów i plików
# dla biblioteki MeteoNumerical.

echo "--- Tworzenie struktury projektu MeteoNumerical ---"

# 1. Tworzenie podstawowych katalogów
DIRS=("include" "src" "examples")
for dir in "${DIRS[@]}"; do
    if [ ! -d "$dir" ]; then
        echo "Tworzenie katalogu: $dir/"
        mkdir "$dir"
    else
        echo "Katalog $dir/ już istnieje."
    fi
done

# 2. Definicja modułów
# Moduły, które mają zarówno plik .hpp jak i .cpp
modules_full=(
    "statistics"
    "interpolation"
    "integration"
    "linalg"
    "ode"
    "rootfinding"
    "fileio"
)

# Moduły, które mają tylko plik .hpp (bo są 'inline' lub zawierają tylko definicje typów)
modules_hpp_only=(
    "common"
    "datastructures"
    "conversions"
    "meteocalc"
    "utils"
)

# 3. Tworzenie plików dla modułów .hpp/.cpp
echo ""
echo "-> Tworzenie plików biblioteki..."
for module in "${modules_full[@]}"; do
    hpp_file="include/${module}.hpp"
    cpp_file="src/${module}.cpp"
    touch "$hpp_file"
    touch "$cpp_file"
    echo "   - Utworzono $hpp_file i $cpp_file"
done

for module in "${modules_hpp_only[@]}"; do
    hpp_file="include/${module}.hpp"
    touch "$hpp_file"
    echo "   - Utworzono $hpp_file"
done

# 4. Tworzenie pliku Makefile
echo ""
echo "-> Tworzenie pliku Makefile..."
# Użycie <<'EOF' zapobiega interpretacji znaków specjalnych jak $ przez powłokę
cat > Makefile <<'EOF'
# Makefile dla biblioteki MeteoNumerical

# 1. Zmienne kompilatora
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -O2
CPPFLAGS = -Iinclude

# 2. Nazwy plików i katalogów
TARGET_LIB = libmeteo.a
BUILD_DIR = build
SRC_DIR = src

# 3. Automatyczne znajdowanie plików źródłowych
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# --- GŁÓWNE REGUŁY ---

# Domyślna reguła
all: $(TARGET_LIB) example

# Reguła do budowania biblioteki
$(TARGET_LIB): $(OBJECTS)
	@echo "Tworzenie biblioteki statycznej: $@"
	ar rcs $@ $^

# Reguła do budowania przykładowego programu
example: examples/main.cpp $(TARGET_LIB)
	@echo "Kompilowanie przykładu..."
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@ -L. -lmeteo

# --- REGUŁY WZORCOWE ---

# Reguła wzorcowa do kompilowania plików .cpp na pliki .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "Kompilowanie $< -> $@"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# --- REGUŁY POMOCNICZE ---

# Reguła do czyszczenia projektu
clean:
	@echo "Czyszczenie..."
	rm -rf $(BUILD_DIR) $(TARGET_LIB) example

# Oznaczenie reguł, które nie tworzą plików o tej samej nazwie
.PHONY: all clean example
EOF

# 5. Tworzenie przykładowego pliku main.cpp
echo "-> Tworzenie pliku examples/main.cpp..."
cat > examples/main.cpp <<'EOF'
#include <iostream>
#include "linalg.hpp"      // Dołącz potrzebne nagłówki
#include "conversions.hpp"

int main() {
    std::cout << "Hello from MeteoNumericalLib example!" << std::endl;

    // Przykład użycia modułu linalg
    MeteoNumerical::Common::Matrix A = {{2, 1}, {1, 3}};
    MeteoNumerical::Common::ValueSeries b = {4, 7};

    try {
        MeteoNumerical::Common::ValueSeries x = MeteoNumerical::LinearAlgebra::solveWithLU(A, b);
        std::cout << "\nRozwiazanie ukladu rownan Ax=b:" << std::endl;
        MeteoNumerical::LinearAlgebra::printVector(x); // Oczekiwany wynik: [1, 2]
    } catch (const std::runtime_error& e) {
        std::cerr << "Blad: " << e.what() << std::endl;
    }

    // Przykład użycia modułu conversions
    double temp_c = 25.0;
    double temp_f = MeteoNumerical::Conversions::celsius_to_fahrenheit(temp_c);
    std::cout << "\nKonwersja temperatury:" << std::endl;
    std::cout << temp_c << " C = " << temp_f << " F" << std::endl;

    return 0;
}
EOF
