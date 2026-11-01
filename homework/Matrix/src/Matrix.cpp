#include "Matrix.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>

// ========== 1.构造函数 ==========
Matrix::Matrix(int rows, int cols)
{
    if (rows <= 0 || cols <= 0)
    {
        throw std::invalid_argument("行列必须大于0");
    }
    this->rows = rows;
    this->cols = cols;
    data = new double*[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new double[cols]{0.0};
    }
}

// 拷贝构造函数
Matrix::Matrix(const Matrix &other)
{
    rows = other.rows;
    cols = other.cols;
    data = new double*[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new double[cols];
        for (int j = 0; j < cols; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }
}

// 移动构造函数
Matrix::Matrix(Matrix &&other)
{
    rows = other.rows;
    cols = other.cols;
    data = other.data;
    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
}

// 析构函数，释放二维数组内存
Matrix::~Matrix()
{
    if (data != nullptr)
    {
        for (int i = 0; i < rows; i++)
        {
            delete[] data[i];
        }
        delete[] data;
        data = nullptr;
    }
}

// ========== 2. set / get 基础函数 ==========
Matrix &Matrix::set(int row, int col, double value)
{
    if (row < 1 || row > rows || col < 1 || col > cols)
        throw std::out_of_range("下标越界");
    data[row - 1][col - 1] = value;
    return *this;
}

double Matrix::get(int row, int col) const
{
    if (row < 1 || row > rows || col < 1 || col > cols)
        throw std::out_of_range("下标越界");
    return data[row - 1][col - 1];
}

double *Matrix::getRow(int row) const
{
    if (row < 1 || row > rows)
        throw std::out_of_range("行下标越界");
    return data[row - 1];
}

Matrix &Matrix::setRow(int row, double *values)
{
    if (row < 1 || row > rows)
        throw std::out_of_range("行下标越界");
    for (int j = 0; j < cols; j++)
    {
        data[row - 1][j] = values[j];
    }
    return *this;
}

double *Matrix::getCol(int col) const
{
    if (col < 1 || col > cols)
        throw std::out_of_range("列下标越界");
    double* colData = new double[rows];
    for (int i = 0; i < rows; i++)
    {
        colData[i] = data[i][col - 1];
    }
    return colData;
}

Matrix &Matrix::setCol(int col, double *values)
{
    if (col < 1 || col > cols)
        throw std::out_of_range("列下标越界");
    for (int i = 0; i < rows; i++)
    {
        data[i][col - 1] = values[i];
    }
    return *this;
}

// ========== getRows / getCols 获取行列（重点！名字是getRows！） ==========
int Matrix::getRows() const
{
    return rows;
}

int Matrix::getCols() const
{
    return cols;
}

double **Matrix::getData() const
{
    return data;
}

// ========== print打印 ==========
Matrix &Matrix::print() const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << data[i][j] << "\t";
        }
        std::cout << "\n";
    }
    return const_cast<Matrix&>(*this);
}

// ========== add 矩阵加法函数 ==========
Matrix Matrix::add(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("add：两个矩阵行列必须相同");
    Matrix res(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            res.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return res;
}

// ========== multiply矩阵乘法 ==========
Matrix Matrix::multiply(const Matrix &other) const
{
    if (cols != other.rows)
        throw std::invalid_argument("multiply：左矩阵列数必须等于右矩阵行数");
    Matrix res(rows, other.cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < other.cols; j++)
        {
            double sum = 0;
            for (int k = 0; k < cols; k++)
            {
                sum += data[i][k] * other.data[k][j];
            }
            res.data[i][j] = sum;
        }
    }
    return res;
}

// ========== transpose转置 ==========
Matrix Matrix::transpose() const
{
    Matrix res(cols, rows);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            res.data[j][i] = data[i][j];
        }
    }
    return res;
}

// ========== inverse求逆（仅支持2阶方阵） ==========
Matrix Matrix::inverse() const
{
    if (!isSquare())
        throw std::invalid_argument("inverse：必须是方阵");
    if (rows != 2)
        throw std::invalid_argument("inverse：只支持2阶矩阵");
    double det = determinant();
    if (det == 0)
        throw std::invalid_argument("矩阵奇异，不存在逆矩阵");
    Matrix res(2,2);
    res.data[0][0] = data[1][1]/det;
    res.data[0][1] = -data[0][1]/det;
    res.data[1][0] = -data[1][0]/det;
    res.data[1][1] = data[0][0]/det;
    return res;
}

// ========== determinant行列式（仅2阶） ==========
double Matrix::determinant() const
{
    if (!isSquare())
        throw std::invalid_argument("determinant：必须是方阵");
    if (rows != 2)
        throw std::invalid_argument("determinant：仅支持2阶");
    return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

// ========== isSquare 判断是否方阵 ==========
bool Matrix::isSquare() const
{
    return rows == cols;
}

// ========== 运算符重载 ==========
Matrix Matrix::operator+(const Matrix &other) const
{
    return this->add(other);
}

Matrix Matrix::operator-(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("operator-：行列不一致");
    Matrix res(rows, cols);
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            res.data[i][j] = data[i][j] - other.data[i][j];
    return res;
}

//一元负号 -m
Matrix Matrix::operator-() const
{
    Matrix res(rows, cols);
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            res.data[i][j] = -data[i][j];
    return res;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    return this->multiply(other);
}

Matrix Matrix::operator/(const Matrix &other) const
{
    if(!other.isSquare())
        throw std::invalid_argument("/：除数必须是方阵");
    Matrix inv = other.inverse();
    return (*this)*inv;
}

//赋值重载 =
Matrix &Matrix::operator=(const Matrix &other)
{
    if(this == &other) return *this;
    //释放旧内存
    for(int i=0;i<rows;i++) delete[] data[i];
    delete[] data;

    rows = other.rows;
    cols = other.cols;
    data = new double*[rows];
    for(int i=0;i<rows;i++){
        data[i] = new double[cols];
        for(int j=0;j<cols;j++)
            data[i][j] = other.data[i][j];
    }
    return *this;
}

//移动赋值
Matrix &Matrix::operator=(Matrix &&other)
{
    if(this == &other) return *this;
    for(int i=0;i<rows;i++) delete[] data[i];
    delete[] data;

    rows = other.rows;
    cols = other.cols;
    data = other.data;
    other.data = nullptr;
    other.rows=0; other.cols=0;
    return *this;
}

bool Matrix::operator==(const Matrix &other) const
{
    if(rows != other.rows || cols != other.cols) return false;
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
        {
            if(data[i][j] - other.data[i][j] > 1e-9 || data[i][j] - other.data[i][j] < -1e-9)
                return false;
        }
    return true;
}

bool Matrix::operator!=(const Matrix &other) const
{
    return !((*this) == other);
}

//括号运算符重载 () 下标从1开始
int Matrix::operator()(int row, int col) const
{
    if(row<1||row>rows||col<1||col>cols)
        throw std::out_of_range("下标越界");
    return static_cast<int>(data[row-1][col-1]);
}

int &Matrix::operator()(int row, int col)
{
    if(row<1||row>rows||col<1||col>cols)
        throw std::out_of_range("下标越界");
    return reinterpret_cast<int&>(data[row-1][col-1]);
}

Matrix &Matrix::operator<<(double value)
{
    //这个是hpp里的 operator<<(double val)，不是流！
    static int cnt = 0;
    int r = cnt / cols;
    int c = cnt % cols;
    if(r >= rows)
    {
        cnt = 0;
        r = 0; c =0;
    }
    data[r][c] = value;
    cnt++;
    return *this;
}

// ========== 友元 流输出 operator<< （hpp里面已经声明为friend） ==========
std::ostream& operator<<(std::ostream &os, const Matrix &mat)
{
    for(int i=0; i<mat.rows; i++)
    {
        for(int j=0; j<mat.cols; j++)
        {
            os << mat.data[i][j] << "\t";
        }
        os << "\n";
    }
    return os;
}
