# =============================================================================
# Makefile dla biblioteki, przykładów i testów MeteoNumerical (Wersja Poprawiona)
# =============================================================================

# --- Konfiguracja Kompilatora ---
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -O2 -pthread

# --- Konfiguracja Projektu ---
TARGET_LIB = libmeteo.a
EXAMPLE1_EXEC = showcase
EXAMPLE2_EXEC = showcase2
TEST_EXEC = run_tests

BUILD_DIR = build
SRC_DIR = src
EXAMPLE_DIR = examples
TEST_DIR = tests

# --- Konfiguracja Google Test ---
GTEST_DIR = $(TEST_DIR)/lib/googletest/googletest
CPPFLAGS = -Iinclude -I$(GTEST_DIR)/include

# =============================================================================
# Automatyczne Wykrywanie Plików
# =============================================================================

LIB_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
LIB_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/lib/%.o, $(LIB_SOURCES))

TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/test/%.o, $(TEST_SOURCES))

GTEST_SOURCES = $(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJECTS = $(patsubst $(GTEST_DIR)/src/%.cc, $(BUILD_DIR)/gtest/%.o, $(GTEST_SOURCES))


# =============================================================================
# Główne Reguły
# =============================================================================

all: $(TARGET_LIB) $(EXAMPLE1_EXEC) $(EXAMPLE2_EXEC)

test: $(TEST_EXEC)
	@echo "URUCHAMIANIE TESTÓW..."
	./$(TEST_EXEC)

$(TARGET_LIB): $(LIB_OBJECTS)
	@echo "ARCHIWIZACJA: Tworzenie biblioteki $@"
	ar rcs $@ $^

# --- Reguły dla Programów Wykonywalnych (POPRAWIONE) ---

# Zależność od $(TARGET_LIB) zapewnia, że biblioteka jest zbudowana jako pierwsza.
# Ale w komendzie linkera używamy -L. -lmeteo, a nie pliku .a bezpośrednio.
$(EXAMPLE1_EXEC): $(EXAMPLE_DIR)/showcase1.cpp $(TARGET_LIB)
	@echo "LINKOWANIE: $@ (Program #1)"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@ -L. -lmeteo

$(EXAMPLE2_EXEC): $(EXAMPLE_DIR)/showcase2.cpp $(TARGET_LIB)
	@echo "LINKOWANIE: $@ (Program #2)"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@ -L. -lmeteo

# POPRAWKA: Usunęliśmy $(TARGET_LIB) z listy plików dla linkera (zmienna $^).
# Zamiast tego polegamy w 100% na flagach -L i -l.
$(TEST_EXEC): $(TEST_OBJECTS) $(GTEST_OBJECTS) $(TARGET_LIB)
	@echo "LINKOWANIE: $@ (Testy)"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TEST_OBJECTS) $(GTEST_OBJECTS) -o $@ -L. -lmeteo


# =============================================================================
# Reguły Wzorcowe
# =============================================================================

$(BUILD_DIR)/lib/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo "KOMPILACJA [LIB]: $< -> $@"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo "KOMPILACJA [TEST]: $< -> $@"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/gtest/%.o: $(GTEST_DIR)/src/%.cc
	@mkdir -p $(@D)
	@echo "KOMPILACJA [GTEST]: $< -> $@"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I$(GTEST_DIR) -c $< -o $@


# =============================================================================
# Reguły Pomocnicze
# =============================================================================

clean:
	@echo "CZYSZCZENIE: Usuwanie wygenerowanych plików..."
	rm -rf $(BUILD_DIR) $(TARGET_LIB) $(EXAMPLE1_EXEC) $(EXAMPLE2_EXEC) $(TEST_EXEC) *.csv

.PHONY: all test clean