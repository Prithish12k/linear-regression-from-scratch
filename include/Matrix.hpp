#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cmath>

class Matrix {
    private:
        std::vector<std::vector<double>> A;
        size_t rows;
        size_t cols;

    public:

        Matrix(const size_t rows, const size_t cols);
        Matrix(const std::vector<std::vector<double>>& input);

        double& operator()(size_t row, size_t col);
        double operator()(size_t row, size_t col) const;
        size_t nRows() const {return rows;}
        size_t nCols() const {return cols;}

        std::vector<double> getColumn(size_t c) const;
        void setColumn(std::vector<double>& b, size_t k);

        Matrix T() const;
        Matrix operator*(const Matrix& other) const;
        std::vector<double> operator*(const std::vector<double>& b) const;
        Matrix operator*(double d) const;
        Matrix operator+(const Matrix& other) const;
        Matrix operator-(const Matrix& other) const;
        void swapRows(size_t i, size_t j);

        std::vector<double> back_sub(const std::vector<double>& b) const;
        std::vector<double> for_sub(const std::vector<double>& b) const;
        std::vector<double> solveQR(const std::vector<double>& b) const;
        std::vector<double> solveLU(const std::vector<double>& b) const;
        

        double determinant() const;
        Matrix inverse() const;
};

std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator*(const std::vector<double>& v1, double d);
double dot(const std::vector<double>& v1, const std::vector<double>& v2);

double norm(const std::vector<double>& v);
