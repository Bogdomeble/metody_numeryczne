

# Biblioteka MeteoNumerical

## 1. Opis

`MeteoNumerical` to biblioteka C++ przeznaczona do obliczeń numerycznych, ze szczególnym uwzględnieniem funkcjonalności przydatnych w analizie danych meteorologicznych. Projekt dostarcza zestaw narzędzi do statystyki, interpolacji, całkowania numerycznego, algebry liniowej, rozwiązywania równań różniczkowych zwyczajnych (ODE), a także konwersji jednostek i podstawowych operacji plikowych.

### Kluczowe Cechy

*   **Struktura modułowa:** Kod jest podzielony na logiczne moduły z osobnymi plikami nagłówkowymi (`include/`) i źródłowymi (`src/`), co ułatwia zarządzanie i rozwijanie biblioteki.
*   **Zarządzanie budowaniem przez `Makefile`:** Projekt zawiera w pełni skonfigurowany `Makefile`, który automatyzuje kompilację biblioteki, programów demonstracyjnych oraz testów.
*   **Zintegrowane testy jednostkowe:** Testy napisane przy użyciu frameworka **Google Test** weryfikują poprawność kluczowych funkcjonalności biblioteki.
*   **Podział na przestrzenie nazw:** Wszystkie funkcje są zorganizowane w głównej przestrzeni nazw `MeteoNumerical` oraz dalszych podprzestrzeniach (np. `Statistics`, `LinearAlgebra`), aby zapobiegać kolizjom nazw.
*   **Obsługa błędów:** Biblioteka wykorzystuje mechanizm wyjątków (`std::runtime_error`, `std::out_of_range`) do sygnalizowania krytycznych błędów oraz `std::nan` dla niezdefiniowanych wyników numerycznych.

## 2. Struktura Projektu

```
MeteoNumericalLib/
├── include/            # Pliki nagłówkowe (.hpp) z interfejsem biblioteki
├── src/                # Pliki źródłowe (.cpp) z implementacją funkcji
├── tests/              # Testy jednostkowe (Google Test)
│   ├── lib/googletest  # Kod źródłowy Google Test (jako submoduł Git)
│   └── *.cpp           # Pliki z testami
├── examples/           # Przykładowe programy demonstrujące użycie biblioteki
├── build/              # (Generowany) Katalog z plikami obiektowymi (.o)
├── .gitignore          # Plik z listą ignorowanych plików
├── Makefile            # Plik konfiguracyjny do budowania projektu
└── README.md           # Ta dokumentacja
```

## 3. Jak zacząć?

### Wymagania
*   Kompilator C++ wspierający standard C++17 (np. `g++`)
*   Narzędzie `make`
*   `git` (do sklonowania repozytorium)

### Kompilacja i Uruchomienie

1.  **Sklonuj repozytorium:**
    ```bash
    git clone https://github.com/Bogdomeble/metody_numeryczne.git
    cd MeteoNumericalLib
    ```

2.  **Zainicjuj i pobierz submoduł Google Test:**
    To jednorazowy krok, konieczny do pobrania kodu frameworka testującego.
    ```bash
    git submodule update --init --recursive
    ```

3.  **Skompiluj projekt:**
    Użyj jednej z poniższych komend `make`:
    *   `make` lub `make all` - kompiluje bibliotekę (`libmeteo.a`) oraz oba programy demonstracyjne (`showcase` i `showcase2`).
    *   `make test` - kompiluje i **uruchamia** wszystkie testy jednostkowe.
    *   `make clean` - usuwa wszystkie wygenerowane pliki (katalog `build`, programy, bibliotekę).

4.  **Uruchom programy demonstracyjne:**
    Po wykonaniu `make` w głównym katalogu pojawią się dwa programy:
    ```bash
    # Uruchomienie pierwszego przykładu
    ./showcase

    # Uruchomienie drugiego przykładu
    ./showcase2
    ```
    Programy te wygenerują również pliki `.csv` z wynikami swoich obliczeń.

## 4. Dokumentacja API

Biblioteka jest zorganizowana w kilku podprzestrzeniach nazw wewnątrz `MeteoNumerical`.

#### `MeteoNumerical::Common`
Zawiera wspólne definicje typów i stałe.
- `ValueSeries`: `std::vector<double>`
- `Matrix`: `std::vector<std::vector<double>>`
- `IndexVector`: `std::vector<int>`
- `DEFAULT_EPSILON`: Stała `1e-12` do porównań zmiennoprzecinkowych.
- `evaluatePolynomialHorner(...)`: Oblicza wartość wielomianu metodą Hornera.

#### `MeteoNumerical::DataStructures`
Definiuje struktury danych.
- `struct Measurement`: Przechowuje `temperature_celsius`, `humidity_percent`, `pressure_hpa`, `wind_speed_mps`.
- `MeasurementSeries`: `std::vector<Measurement>`

#### `MeteoNumerical::Statistics`
Podstawowe funkcje statystyczne.
- `calculate_average(...)`
- `find_min(...)`, `find_max(...)`
- `calculate_std_dev(...)`
- `calculate_variance(...)`
- `calculate_mse(...)`

#### `MeteoNumerical::Conversions`
Konwertery jednostek.
- `celsius_to_fahrenheit(...)`, `fahrenheit_to_celsius(...)`
- `mps_to_kmh(...)`, `kmh_to_mps(...)`
- `hpa_to_mmhg(...)`, `mmhg_to_hpa(...)`

#### `MeteoNumerical::MeteoCalculations`
Specyficzne obliczenia meteorologiczne.
- `calculate_dew_point(...)`
- `calculate_wind_chill(...)`

#### `MeteoNumerical::Interpolation`
Metody interpolacji danych.
- `namespace Newton`: `calculateDividedDifferences(...)`, `getNewtonCoefficients(...)`, `newtonInterpolate(...)`.
- `namespace Lagrange`: `lagrangeInterpolate(...)`.
- `selectNodesByStep(...)`: Wybiera co k-ty węzeł z danych.
- `calculateInterpolationMSE(...)`: Oblicza błąd średniokwadratowy dla interpolacji.

#### `MeteoNumerical::Integration`
Metody całkowania numerycznego.
- `IntegrableFunction`: Alias dla `std::function<double(double)>`.
- `rectangleRule(...)`, `trapezoidalRule(...)`, `simpsonRule(...)`.
- `namespace GaussLegendre`: `quadrature(...)` (dla 2-6 punktów), `composite(...)`.

#### `MeteoNumerical::LinearAlgebra`
Algebra liniowa.
- `gaussElimination(...)`: Rozwiązuje Ax=b eliminacją Gaussa.
- `solveWithLU(...)`: Rozwiązuje Ax=b dekompozycją LU.
- `luDecompositionPivoting(...)`, `forwardSubstitution(...)`, `backwardSubstitution(...)`.
- `multiplyMatrices(...)`, `printMatrix(...)`, `printVector(...)`.

#### `MeteoNumerical::ODE`
Rozwiązywanie równań różniczkowych zwyczajnych.
- `ODEFunction`: Alias dla `std::function<double(double t, double y)>`.
- `ODESolution`: Alias dla `std::vector<std::pair<double, double>>`.
- `solve(...)`: Rozwiązuje ODE metodami "euler", "heun", "midpoint", "rk4".
- `getSolutionAtTime(...)`: Odczytuje wartość z rozwiązania w danym czasie.

#### `MeteoNumerical::RootFinding`
Znajdowanie miejsc zerowych funkcji.
- `newton_method_analytic(...)`, `newton_method_numeric(...)`
- `secant_method(...)`
- `bisection_method(...)`
- `regula_falsi_method(...)`

#### `MeteoNumerical::FileIO`
Operacje wejścia/wyjścia.
- `readXYDataFromLines(...)`: Czyta dane X i Y z pliku tekstowego o specyficznym formacie.
- `writeXYDataToCSV(...)`: Zapisuje dane X i Y do pliku CSV.
- `saveRootFindingResultsToCSV(...)`: Zapisuje historię iteracji z metod znajdowania miejsc zerowych do pliku CSV.

#### `MeteoNumerical::Utils`
Narzędzia pomocnicze.
- `extractTemperatures(...)`, `extractHumidities(...)` z serii pomiarów.

## 5. Przykłady Użycia

Praktyczne przykłady użycia biblioteki znajdują się w katalogu `examples/`.
*   **`examples/showcase1.cpp`** (kompilowany do `./showcase`): Demonstruje użycie modułów algebry liniowej, statystyki, interpolacji Lagrange'a, operacji plikowych oraz rozwiązywania równań różniczkowych.
*   **`examples/showcase2.cpp`** (kompilowany do `./showcase2`): Demonstruje znajdowanie miejsc zerowych, całkowanie numeryczne, interpolację Newtona oraz specyficzne obliczenia meteorologiczne.