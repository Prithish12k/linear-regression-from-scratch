# Linear Regression in C++ using QR Decomposition

This project is a from-scratch C++ implementation of Multiple Linear Regression. It is designed to be a robust, efficient, and educational tool that demonstrates how to solve linear least squares problems using numerically stable matrix decompositions, specifically **QR Decomposition**.

The library includes a powerful `Matrix` class that supports a wide range of operations and a `CSVReader` utility for easy data loading.

## Core Components

1.  **`LinearRegression` Class:** The main interface for the regression model. It orchestrates the training and prediction process.
2.  **`Matrix` Class:** A comprehensive matrix manipulation library. It provides the fundamental building blocks for linear algebra, including matrix-matrix/matrix-vector operations, and, most importantly, linear system solvers based on **LU** and **QR** decompositions.
3.  **`CSVReader` Class:** A helper utility to read and parse `.csv` files into feature matrices (`X`) and target vectors (`y`). It can automatically identify the target column by its header name and adds a bias term (a column of ones) to the feature matrix.

## The Mathematical Model

Linear regression aims to model the relationship between a dependent variable $y$ and one or more independent variables $X$ by fitting a linear equation. The goal is to find the vector of coefficients, $\beta$, that minimizes the sum of the squared differences between the observed values and the values predicted by the model.

The objective is to solve for $\beta$ in:
$$\underset{\beta}{\text{argmin}} || y - X\beta ||^2$$

While this can be solved with the normal equation $\beta = (X^T X)^{-1} X^T y$, such an approach can be prone to numerical instability if the matrix $X^T X$ is ill-conditioned.

This implementation uses a more robust method: **QR Decomposition**. The feature matrix $X$ is decomposed into the product of an orthogonal matrix $Q$ (where $Q^T Q = I$) and an upper triangular matrix $R$.

1.  **Decomposition**: $X = QR$
2.  The minimization problem becomes: $\underset{\beta}{\text{argmin}} || y - QR\beta ||^2$
3.  Since multiplying by an orthogonal matrix doesn't change the norm, we can multiply by $Q^T$: $\underset{\beta}{\text{argmin}} || Q^T y - R\beta ||^2$
4.  This simplifies to solving the much simpler upper triangular system for $\beta$:
    $$R\beta = Q^T y$$
This system is solved efficiently using **back substitution**. This project implements the Gram-Schmidt process to find Q and R.

## How to Build and Use

### 1. Building the Project

The project uses CMake for cross-platform builds. You will need a C++ compiler (like GCC, Clang, or MSVC) and CMake installed.

```bash
# Clone the repository
git clone [https://github.com/Prithish12k/linear-regression-from-scratch.git](https://github.com/Prithish12k/linear-regression-from-scratch.git)
cd linear-regression-from-scratch

# Create a build directory
mkdir build
cd build

# Configure and build the project
cmake ..
make

# The executables (e.g., from the 'examples' folder) will be in the build/bin directory
# You can run the example like this:
# ./bin/example
```

### 2. Usage Example

The following demonstrates a complete workflow: loading data from a CSV file, training the model, and making a prediction.

```cpp
#include "CSVReader.hpp"
#include "LinearRegression.hpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
    // 1. Load Data using CSVReader
    CSVReader reader;
    std::string filename = "../data/your_data.csv"; // <-- Make sure you have a CSV file here
    std::string target_col = "Target";           // <-- Change to your target column name

    try {
        // 'readMatrixWithTarget' returns a pair containing the feature matrix X and target vector y.
        // It automatically adds the bias term (a column of 1s) to X.
        auto data = reader.readMatrixWithTarget(filename, target_col);
        std::vector<std::vector<double>> X_train = data.first;
        std::vector<double> y_train = data.second;

        // 2. Initialize and Train the Model
        LinearRegression model;

        // The 'fit' method calculates the beta coefficients using QR decomposition.
        model.fit(X_train, y_train);

        // 3. View the Results
        // Retrieve and print the learned coefficients (beta_0, beta_1, ...)
        const std::vector<double>& betas = model.coefficients();
        std::cout << "Learned Coefficients (beta):" << std::endl;
        for (size_t i = 0; i < betas.size(); ++i) {
            // beta_0 is the intercept, the rest are coefficients for the features
            std::cout << "  beta_" << i << ": " << betas[i] << std::endl;
        }

        // 4. Make Predictions
        // Create new data for prediction.
        // IMPORTANT: You must manually add the bias term (1.0) as the first element.
        std::vector<std::vector<double>> X_test = {{1.0, 2.5, 3.8}}; // {bias, feature1, feature2}
        
        std::vector<double> predictions = model.predict(X_test);
        std::cout << "\nPrediction for new data point {2.5, 3.8}: " << predictions[0] << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

## API Reference

CSVReader
| Method | Description |
|---|---|
| readMatrixWithTarget(file, target) | Reads a CSV, separates features and target, adds a bias term to features, and returns {X, y}. |
| readMatrix(file, hasHeader) | Reads an entire CSV into a single matrix without separating a target. |
LinearRegression
| Method | Description |
|---|---|
| fit(X, y) | Trains the model by solving for coefficients using QR decomposition. |
| predict(X_new) | Returns a vector of predicted values for new input data. |
| coefficients() | Returns a constant reference to the learned beta coefficients vector. |

## Project Structure
```
.
├── CMakeLists.txt      # Build script for CMake
├── README.md           # This file
├── data/               # Directory for .csv datasets
├── examples/           # Example code showing how to use the library
│   └── main.cpp
├── include/            # Header files for the library
│   ├── CSVReader.hpp
│   ├── LinearRegression.hpp
│   └── Matrix.hpp
└── src/                # Source file implementations
    ├── CSVReader.cpp
    ├── LinearRegression.cpp
    └── Matrix.cpp
```
