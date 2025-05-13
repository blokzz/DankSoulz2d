#ifndef LOADLEVELFROMFILE_H
#define LOADLEVELFROMFILE_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream> // Dodajemy <sstream> do rozdzielania liczb

inline std::vector<int> loadLevelFromFile(const std::string& filename, int& width, int& height) {
    std::ifstream file(filename);
    std::vector<int> level;
    std::string line;
    width = 0;
    height = 0;

    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku poziomu: " << filename << std::endl;
        return {};
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Dzielimy linię po spacji i zamieniamy na liczby
        std::istringstream ss(line); // Strumień do rozdzielenia linii
        int tile;
        
        while (ss >> tile) { // Odczytujemy liczby po spacji
            level.push_back(tile); // Dodajemy liczbę do poziomu
        }

        if (width == 0) width = level.size(); // Pierwsza linia ustawia szerokość
        ++height; // Liczymy liczbę wierszy
    }


    return level;
}
#endif // LOAD_LEVEL_FROM_FILE_H