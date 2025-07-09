#pragma once
#include <vector>

class LinearRegression {
public:

    void fit(const std::vector<std::vector<double>>& X, const std::vector<double>& y);
    std::vector<double> predict(const std::vector<std::vector<double>>& X_new) const;
    const std::vector<double>& coefficients() const;

private:

    std::vector<double> beta_;
};