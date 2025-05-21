#ifndef LOADLEVELFROMFILE_H
#define LOADLEVELFROMFILE_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

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

        std::istringstream ss(line);
        int tile;
        
        while (ss >> tile) {
            level.push_back(tile);
        }

        if (width == 0) width = level.size();
        ++height;
    }
    return level;
}
#endif