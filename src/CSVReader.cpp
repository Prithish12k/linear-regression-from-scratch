#include "CSVReader.hpp"

std::vector<std::vector<double>> CSVReader::readMatrix(const std::string& filename, bool hasHeader) {
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file.");
    }

    std::string line;

    if (hasHeader)
    {
        std::getline(file, line);
    }

    std::vector<std::vector<double>> X_cpy;

    while (std::getline(file, line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<double> row;
        
        while (std::getline(lineStream, cell, ','))
        {
            double val = std::stod(cell);
            row.push_back(val);
        }

        X_cpy.push_back(row);
    }

    return X_cpy;
}

std::pair<std::vector<std::vector<double>>, std::vector<double>> CSVReader::readMatrixWithTarget(const std::string& filename, const std::string& targetCol) {
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file.");
    }

    std::string line;

    if (!getline(file, line))
    {
        throw std::runtime_error("file is empty.");
    }

    std::vector<std::string> headers;
    std::stringstream headerstream(line);
    std::string header;

    while (std::getline(headerstream, header, ',')) {
        headers.push_back(header);
    }

    size_t targetInd = headers.size();
    for (size_t i {}; i < headers.size(); i++)
    {
        if (headers[i] == targetCol) {
            targetInd = i;
            break;
        }
    }

    if (targetInd == headers.size()) {
        throw std::invalid_argument("Target column not found.");
    }

    std::vector<std::vector<double>> X_data;
    std::vector<double> y_data;

    while (std::getline(file, line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        size_t colInd {};
        double targetValue = 0.0;
        std::vector<double> features;
        features.push_back(1.0);
        
        while (std::getline(lineStream, cell, ','))
        {
            double val = std::stod(cell);
            if (colInd == targetInd)
            {
                targetValue = val;
            }

            else
            {
                features.push_back(val);
            }
            colInd++;
        }

        if (colInd != headers.size()) {
            throw std::runtime_error("Row has unexpected number of elements.");
        }

        X_data.push_back(features);
        y_data.push_back(targetValue);
    }

    return {X_data, y_data};
}