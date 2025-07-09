#include "../include/LinearRegression.hpp"
#include "../include/Matrix.hpp"

void LinearRegression::fit(const std::vector<std::vector<double>>& X, const std::vector<double>& y) {
    Matrix X_cpy(X);
    beta_ = X_cpy.solveQR(y);
}

std::vector<double> LinearRegression::predict(const std::vector<std::vector<double>>& X_new) const {
    std::vector<double> pred;

    for (const auto& row : X_new) {
        pred.push_back(dot(row, beta_));
    }

    return pred;
}

const std::vector<double>& LinearRegression::coefficients() const {
    return beta_;
}

