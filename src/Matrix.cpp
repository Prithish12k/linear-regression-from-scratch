#include "Matrix.hpp"

Matrix::Matrix(const size_t row, const size_t col) : rows(row), cols(col), A(row, std::vector<double>(col, 0.0)) {}
Matrix::Matrix(const std::vector<std::vector<double>>& input)
    : rows(input.size()),
      cols(input.empty() ? 0 : input[0].size()),
      A(input)
{
    for (size_t i = 0; i < rows; ++i) {
        if (input[i].size() != cols) {
            throw std::invalid_argument("Matrix constructor: inconsistent row sizes.");
        }
    }
}

double& Matrix::operator()(size_t row, size_t col) {
    return A[row][col];
}

double Matrix::operator()(size_t row, size_t col) const {
    return A[row][col];
}

Matrix Matrix::T() const {
    int r = nRows();
    int c = nCols();

    Matrix t(c, r);

    for (size_t i {}; i < c; i++)
    {
        for (size_t j {}; j < r; j++)
        {
            t(i, j) = A[j][i];
        }
    }

    return t;
}

Matrix Matrix::operator*(const Matrix& other) const {
    size_t r1 = nRows();
    size_t c1 = nCols();

    size_t r2 = other.nRows();
    size_t c2 = other.nCols();

    if (c1 != r2)
    {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication.");
    }

    Matrix m(r1, c2);

    for (size_t i {}; i < r1; i++)
    {
        for (size_t j {}; j < c2; j++)
        {
            for (size_t k {}; k < c1; k++)
            {
                m(i, j) += A[i][k]*other(k, j);
            }
        }
    }

    return m;
}

std::vector<double> Matrix::operator*(const std::vector<double>& b) const {
    size_t n = b.size();
    size_t r = nRows();

    if (n != nCols())
    {
        throw std::invalid_argument("Matrix, Vector dimensions incompatible for multiplication.");
    }

    std::vector<double> res(r);

    for (size_t i {}; i < r; i++)
    {
        for (size_t j {}; j < n; j++)
        {
            res[i] += A[i][j]*b[j];
        }
    }

    return res;
}

Matrix Matrix::operator*(double d) const {
    Matrix m(*this);

    const size_t r = nRows();
    const size_t c = nCols();

    for (size_t i {}; i < r; i++)
    {
        for (size_t j {}; j < c; j++)
        {
            m(i, j) *= d;
        }
    }

    return m;
}

Matrix Matrix::operator+(const Matrix& other) const {
    size_t r1 = nRows();
    size_t c1 = nCols();

    size_t r2 = other.nRows();
    size_t c2 = other.nCols();

    if ((r1 != r2) || (c1 != c2))
    {
        throw std::invalid_argument("Matrix dimensions incompatible for addition.");
    }

    Matrix m(r1, c1);

    for (size_t i {}; i < r1; i++)
    {
        for (size_t j {}; j < c1; j++)
        {
            m(i, j) = A[i][j] + other(i, j);
        }
    }

    return m;
}

Matrix Matrix::operator-(const Matrix& other) const {
    size_t r1 = nRows();
    size_t c1 = nCols();

    size_t r2 = other.nRows();
    size_t c2 = other.nCols();

    if ((r1 != r2) || (c1 != c2))
    {
        throw std::invalid_argument("Matrix dimensions incompatible for subtraction.");
    }

    Matrix m(r1, c1);

    for (size_t i {}; i < r1; i++)
    {
        for (size_t j {}; j < c1; j++)
        {
            m(i, j) = A[i][j] - other(i, j);
        }
    }

    return m;
}

std::vector<double> Matrix::getColumn(size_t k) const {
    if (k < 0 || k >= nCols())
    {
        throw std::invalid_argument("Index out of bounds!");
    }

    const size_t n = nRows();    
    std::vector<double> v(n);

    for (size_t i {}; i < n; i++)
    {
        v[i] = A[i][k];
    }

    return v;
}

void Matrix::swapRows(size_t i, size_t j) {
        
    if (i >= nRows() || j >= nRows()) {
        throw std::out_of_range("Row index out of bounds in swapRows.");
    }

    std::swap(A[i], A[j]);
}

std::vector<double> Matrix::back_sub(const std::vector<double>& b) const {
    const size_t n = b.size();

    if (n != nRows())
    {
        throw std::invalid_argument("invalid: length of input and output dont match.");
    }

    std::vector<double> x(n);
    x[n-1] = b[n-1]/A[n-1][n-1];

    for (int i = n-2; i >= 0; i--)
    {
        if (std::fabs(A[i][i]) < 1e-12) {
            throw std::runtime_error("Zero pivot encountered in back substitution.");
        }

        x[i] = b[i];

        for (int j {i+1}; j < n; j++)
        {
            x[i] -= A[i][j]*x[j];
        }

        x[i] = x[i]/A[i][i];
    }

    return x;
}

std::vector<double> Matrix::for_sub(const std::vector<double>& b) const {
    const size_t n = b.size();

    if (n != nRows())
    {
        throw std::invalid_argument("invalid, length of input and output dont match.");
    }

    std::vector<double> x(n);
    x[0] = b[0]/A[0][0];

    for (size_t i {1}; i < n; i++)
    {
        if (std::fabs(A[i][i]) < 1e-12) {
            throw std::runtime_error("Zero pivot encountered in forward substitution.");
        }

        x[i] = b[i];

        for (size_t j {}; j < i; j++)
        {
            x[i] -= A[i][j]*x[j];
        }

        x[i] = x[i]/A[i][i];
    }

    return x;
}

std::vector<double> Matrix::solveQR(const std::vector<double>& b) const {
    if (b.size() != nRows())
    {
        throw std::invalid_argument("#rows(A) != length(b).");
    }
    
    const size_t n = b.size();
    const size_t m = nCols();

    Matrix Q(n, m);
    Matrix R(m, m);

    for (size_t j {}; j < m; j++)
    {
        std::vector<double> aj = getColumn(j);

        for (size_t i {}; i < j; i++)
        {
            std::vector<double> qi = Q.getColumn(i);
            double inner = dot(aj, qi);
            R(i, j) = inner;
            aj = aj-(qi*inner);
        }

        double norm_aj = norm(aj);
        R(j, j) = norm_aj;

        aj = aj*(1.0/norm_aj);
        Q.setColumn(aj, j);
    }

    Matrix QT = Q.T();

    std::vector<double> QT_y = QT*b;

    return R.back_sub(QT_y);
}

void Matrix::setColumn(std::vector<double>& b, size_t k) {
    if (k < 0 || k >= nCols())
    {
        throw std::invalid_argument("invalid index.");
    }

    if (b.size() != nRows())
    {
        throw std::invalid_argument("different row size.");
    }

    const size_t n = b.size();

    for (size_t i {}; i < n; i++)
    {
        A[i][k] = b[i];
    }
}

std::vector<double> Matrix::solveLU(const std::vector<double>& b) const {
    
    const size_t n = nCols();

    Matrix X_cpy(*this);
    std::vector<double> b_cpy = b;

    Matrix X_cpyT = X_cpy.T();
    Matrix XT_X = X_cpyT*X_cpy;

    b_cpy = X_cpyT*b_cpy;

    Matrix L(n, n);
    for (size_t i {}; i < n; i++)
    {
        L(i, i) = 1.0;
    }

    for (size_t i {}; i < n; i++)
    {
        int mInd = i;
        for (size_t j {i+1}; j < n; j++)
        {
            mInd = (fabs(XT_X(mInd, i)) < fabs(XT_X(j, i)) ? j : mInd);
        }

        if (mInd != i)
        {
            XT_X.swapRows(i, mInd);
            std::swap(b_cpy[i], b_cpy[mInd]);
            
            for (size_t k {}; k < i; k++)
            {
                std::swap(L(i, k), L(mInd, k));
            }
        }

        if (fabs(XT_X(i, i)) < 1e-12)
        {
            throw std::runtime_error("zero-pivot.\n");
        }

        for (size_t j {i+1}; j < n; j++)
        {
            L(j, i) = XT_X(j, i)/XT_X(i, i);

            for (size_t k {i}; k < n; k++)
            {
                XT_X(j, k) -= L(j, i)*XT_X(i, k);
            }
        }
    }

    std::vector<double> y = L.for_sub(b_cpy);
    return XT_X.back_sub(y);
}

std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2) {
    if (v1.size() != v2.size())
    {
        throw std::invalid_argument("different size");
    }

    const size_t n = v1.size();

    std::vector<double> res(n);

    for (size_t i {}; i < n; i++)
    {
        res[i] = v1[i] + v2[i];
    }

    return res;
}

std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2) {
    if (v1.size() != v2.size())
    {
        throw std::invalid_argument("different size");
    }

    const size_t n = v1.size();

    std::vector<double> res(n);

    for (size_t i {}; i < n; i++)
    {
        res[i] = v1[i] - v2[i];
    }

    return res;
}

std::vector<double> operator*(const std::vector<double>& v1, double d) {
    const size_t n = v1.size();

    std::vector<double> res(n);

    for (size_t i {}; i < n; i++)
    {
        res[i] = d*v1[i];
    }

    return res;
}

double dot(const std::vector<double>& v1, const std::vector<double>& v2) {
    if (v1.size() != v2.size())
    {
        throw std::invalid_argument("Invalid: length of vectors is different.");
    }

    const size_t n = v1.size();

    double res {0.0};

    for (size_t i {}; i < n; i++)
    {
        res += v1[i]*v2[i];
    }

    return res;
}

double norm(const std::vector<double>& v)
{
    return std::sqrt(dot(v, v));
}

