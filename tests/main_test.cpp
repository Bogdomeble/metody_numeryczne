#include "gtest/gtest.h"

// Główna funkcja, która uruchamia wszystkie testy zdefiniowane
// w tym projekcie przy użyciu makr TEST() i TEST_F().
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}