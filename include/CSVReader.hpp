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
    static Matrix readMatrix(const std::string& filename, bool hasHeader);
    static std::pair<Matrix, std::vector<double>> readMatrixWithTarget(const std::string& filename, const std::string& target);
};
