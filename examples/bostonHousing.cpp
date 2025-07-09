#include <iostream>
#include "../include/CSVReader.hpp"
#include "../include/LinearRegression.hpp"

int main(void)
{
    auto [X, y] = CSVReader::readMatrixWithTarget("../data/BostonHousing.csv", "MEDV");

    LinearRegression lr;
    lr.fit(X, y);

}
