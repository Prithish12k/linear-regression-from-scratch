#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <utility>
#include "Matrix.hpp"

class CSVReader {
public:
    static std::vector<std::vector<double>> readMatrix(const std::string& filename, bool hasHeader);
    static std::pair<std::vector<std::vector<double>>, std::vector<double>> readMatrixWithTarget(const std::string& filename, const std::string& target);
};
