#include "LinearRegression.hpp"
#include "Matrix.hpp"

void LinearRegression::fit(const std::vector<std::vector<double>>& X, const std::vector<double>& y) {
    auto X_copy = X;
    auto [Q, R] = QR_decomp(X_copy);
    beta_ = qr_solve(Q, R, y);
}

std::vector<double> LinearRegression::predict(const std::vector<std::vector<double>>& X_new) const {
    std::vector<double> pred;

    for (const auto& row : X_new) {
        double val = 0.0;

        for (size_t i {}; i < row.size(); i++) {
            val += row[i]*beta_[i];
        }
        pred.push_back(val);
    }

    return preds;
}

const std::vector<double>& LinearRegression::coefficients() const {
    return beta_;
}

