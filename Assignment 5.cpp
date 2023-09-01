// Paul Vautravers 
// Assignment 5 
// Code to develop a matrix class
#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<iomanip>
#include<sstream>

class matrix
{
private:
    size_t m_rows{};
    size_t n_columns{};
    double* matrix_data{nullptr};
public:
    //constructors and destructor
    //default constructor
    matrix() = default;
    //parameterised constructors
    matrix(const size_t rows_in, const size_t columns_in,const double *data_in);
    matrix(const size_t rows_in, const size_t columns_in);
    //copy and move constructors
    matrix(const matrix& other_matrix);
    matrix(matrix&& other_matrix) noexcept ; // noexcept stop windows compiler warning
    //destructor
    ~matrix() { delete[] matrix_data; }
    //standard functions
    size_t length() const { return m_rows * n_columns; }
    matrix delete_row_column( size_t delete_row_i, size_t delete_column_j) const;
    double determinant();
    //overloading operators
    //copy and move assignments
    matrix& operator=(matrix&);
    matrix& operator=(matrix&&) noexcept ; 
    //matrix operations
    matrix operator +(const matrix& other_matrix) const;
    matrix operator -(const matrix& other_matrix) const;
    matrix operator *(const matrix& other_matrix) const;
    //friend functions for iostream overloads
    friend size_t find_max_width(const matrix& other_matrix);
    friend std::ostream& operator<<(std::ostream& out,const matrix& other_matrix);
    friend std::istream& operator>>(std::istream& in,matrix& other_matrix);
};

matrix::matrix(const matrix& other_matrix) 
//copy constructor 
{
    matrix_data = nullptr;
    const size_t size{other_matrix.length()};
    if (size > 0) {
        matrix_data = new double[size];
        m_rows = other_matrix.m_rows;
        n_columns = other_matrix.n_columns;
        for (size_t i{}; i < size; i++) {
            matrix_data[i] = other_matrix.matrix_data[i];
        }
    }
}

matrix::matrix(matrix&& other_matrix) noexcept
// Move constructor
{
    matrix_data = other_matrix.matrix_data;
    m_rows = other_matrix.m_rows;
    n_columns = other_matrix.n_columns;
    other_matrix.matrix_data = nullptr;
    other_matrix.m_rows = 0;
    other_matrix.n_columns = 0;
}

matrix::matrix(const size_t rows_in, const size_t columns_in, const double *data_in)
//Parameterised constructor with data input
{
    if (rows_in < 1 || columns_in < 1 ) {
        std::cerr << "Error: trying to declare a matrix with rows or columns < 1" << std::endl;
        throw("");
    }
    m_rows = rows_in;
    n_columns = columns_in;
    const size_t size{ length() };
    matrix_data = new double[size] {0};
    for(size_t i{}; i < size; i++) {
        matrix_data[i] = data_in[i];
    }
}

matrix::matrix(const size_t rows_in, const size_t columns_in)
//parameterised constructor without data input, values set to zero
{
    if (rows_in < 1 || columns_in < 1){
        std::cerr << "Error: trying to declare a matrix with rows or columns < 1" << std::endl;
        throw("");
    }
    m_rows = rows_in;
    n_columns = columns_in;
    const size_t size{length()};
    matrix_data = new double[size] {0};
}

matrix& matrix::operator=(matrix& other_matrix)
// Assignment operator using deep copying
{
    //self assignment
    if (&other_matrix == this) {
        return *this;
    }
    //delete data and replace with other matrix data
    delete[] matrix_data;
    matrix_data = nullptr;
    m_rows = 0;
    n_columns = 0;
    m_rows = other_matrix.m_rows;
    n_columns = other_matrix.n_columns;
    const size_t size{ length() };
    if (size > 0){
        matrix_data = new double[size];
        for (size_t i{}; i < size; i++) {
            matrix_data[i] = other_matrix.matrix_data[i];
        }
    }
    return *this;
}

matrix& matrix::operator=(matrix&& other_matrix) noexcept
// Move assignment operator
{
    if (this == &other_matrix) {
       return *this;
    }
    delete[] matrix_data;
    matrix_data = other_matrix.matrix_data;
    m_rows = other_matrix.m_rows;
    n_columns = other_matrix.n_columns;
    other_matrix.matrix_data = nullptr;
    other_matrix.m_rows = 0;
    other_matrix.n_columns = 0;
    return *this;
}

matrix matrix::operator +(const matrix& other_matrix) const
//overload for matrix addition
{
    if (m_rows == other_matrix.m_rows && n_columns == other_matrix.n_columns) {
        matrix temporary_matrix{ m_rows,n_columns };
        for (size_t i{}; i < length(); i++) {
            temporary_matrix.matrix_data[i] = matrix_data[i] + other_matrix.matrix_data[i];
        }
        return temporary_matrix;
    } else {
            std::cerr << "Error: trying to subtract incompatible matrices!" << std::endl
                      << "For matrix_x+matrix_y, matrix_x must have the same number of columns and rows as matrix_y." << std::endl
                      << "Default unspecified matrix outputted" << std::endl;
            matrix error_matrix{};
            return error_matrix;
	}
}

matrix matrix::operator -(const matrix& other_matrix) const
//overload for matrix subtraction
{
    if (m_rows == other_matrix.m_rows && n_columns == other_matrix.n_columns) {
        matrix temporary_matrix{m_rows,n_columns};
        for (size_t i{}; i < length(); i++) {
             temporary_matrix.matrix_data[i] = matrix_data[i] - other_matrix.matrix_data[i];
        }
        return temporary_matrix;
    } else {
            std::cerr << "Error: trying to subtract incompatible matrices!" << std::endl
                      << "For matrix_x - matrix_y, matrix_x must have the same number of columns and rows as matrix_y." << std::endl
                      << "Default unspecified matrix outputted" << std::endl;
            matrix error_matrix{};
            return error_matrix;
	}
}

matrix matrix::operator *(const matrix& other_matrix) const
//overload for matrix multiplication
{
    if (n_columns == other_matrix.m_rows) {
        matrix temporary_matrix{m_rows,other_matrix.n_columns};
        for (size_t i{1}; i <= m_rows; i++) {
            for (size_t j{1}; j <= other_matrix.n_columns; j++){
                double temporary_value{};
                for (size_t k{1}; k <= n_columns; k++){
                    temporary_value += matrix_data[(k - 1) + (i - 1) * n_columns] * other_matrix.matrix_data[(j - 1) + (k - 1) * n_columns];
                }
                temporary_matrix.matrix_data[(j - 1) + (i - 1) * n_columns] = temporary_value;
            }
        }
        return temporary_matrix;
    } else{
            std::cerr << "Error: trying to multiply incompatible matrices!" << std::endl
                      << "For matrix_x * matrix_y, matrix_x must have the same number of columns as matrix_y has rows." << std::endl
                      << "Default unspecified matrix outputted" << std::endl;
            matrix error_matrix{};
            return error_matrix;
    }
}

size_t find_max_width(const matrix& other_matrix)
//function to find max required width of elements
{
    double matrix_element{0};
    std::ostringstream string_stream_matrix_element;
    size_t width{};
    for (size_t i{ 1 }; i <= other_matrix.m_rows; i++) {
        for (size_t j{ 1 }; j <= other_matrix.n_columns; j++) {
            matrix_element = other_matrix.matrix_data[(j - 1) + (i - 1) * other_matrix.n_columns];
            string_stream_matrix_element << matrix_element;
            if (string_stream_matrix_element.str().length() > width) {
                width = string_stream_matrix_element.str().length();
                string_stream_matrix_element.str("");
            }
        }
    }
    return width;
}

std::ostream& operator << (std::ostream& out, const matrix& other_matrix)
//overload of ostream to output matrices in an appropriate fashion
{
    double matrix_element{ 0 };
    size_t ideal_width{ find_max_width(other_matrix) };
    for (size_t i{1}; i <= other_matrix.m_rows; i++) {
        out << "|";
        for (size_t j{1}; j <= other_matrix.n_columns; j++){
            matrix_element = other_matrix.matrix_data[(j - 1) + (i - 1) * other_matrix.n_columns];
            out << std::setw(ideal_width) << matrix_element << " ";
        }
        out << "|" << std::endl;
    }
    return out;
}

std::istream& operator >> (std::istream& in, matrix& other_matrix)
//overload of istream to input matrices in an appropriate fashion
{
    std::cout << "Enter the number of rows:";
    in >> other_matrix.m_rows;
    std::cout << "Enter the number of columns:";
    in >> other_matrix.n_columns;
    const size_t size{ other_matrix.length() };
    other_matrix.matrix_data = new double[size];
    for (size_t i{}; i < size; i++) {
        std::cout << "Enter matrix element " << i+1 <<":"<< std::endl;
        in >> other_matrix.matrix_data[i];
    }
    return in;
}

matrix matrix::delete_row_column( size_t deleted_row_i, size_t deleted_column_j) const 
//function to delete a row and column together from a matrix
{
    matrix temporary_matrix{ m_rows - 1, n_columns - 1 };
    for (size_t i{1}; i <= m_rows; i++) {
        for (size_t j{1}; j <= n_columns; j++) {
            if (i != deleted_row_i && j != deleted_column_j) {
                if (i < deleted_row_i && j < deleted_column_j) {
                    (temporary_matrix.matrix_data)[(j - 1) + (i - 1) * temporary_matrix.n_columns]
                    = matrix_data[(j - 1) + (i - 1) * n_columns];
                } else if (i > deleted_row_i && j > deleted_column_j) {
                    (temporary_matrix.matrix_data)[(j - 2) + (i - 2) * temporary_matrix.n_columns]
                    = matrix_data[(j - 1) + (i - 1) * n_columns];
                } else if (i > deleted_row_i && j < deleted_column_j) {
                    (temporary_matrix.matrix_data)[(j - 1) + (i - 2) * temporary_matrix.n_columns]
                    = matrix_data[(j - 1) + (i - 1) * n_columns];
                } else if (i < deleted_row_i && j > deleted_column_j) {
                    (temporary_matrix.matrix_data)[(j - 2) + (i - 1) * temporary_matrix.n_columns]
                    = matrix_data[(j - 1) + (i - 1) * n_columns];
                }
            }
        }
    }
    return temporary_matrix;
}

double matrix::determinant() 
//calculate determinant of a square matrix
{
    double determinant_value{0};
    if (m_rows == n_columns) {
        if (n_columns == 2 && m_rows == 2) {
            determinant_value = matrix_data[0] * matrix_data[3]
                              - matrix_data[1] * matrix_data[2];
        } else {
            for (size_t i{ 1 }; i <= m_rows; i++) {
                determinant_value += pow(-1, i + 1) * (matrix_data[(i - 1) * n_columns]) * 
                                     ((*this).delete_row_column(i, 1).determinant());
            }
        }
        return determinant_value;
    } else {
        std::cerr << "Error: trying to find determinant of non-square matrix" << std::endl
                  << "For a determinant to exist, there must be equal rows and columns" << std::endl
                  << "Null outputted" << std::endl;
        return NULL;
    }
}

void print_line()
{
    //convenient function to break up the console output
    std::cout << "---------------------------------------------------------" << std::endl;
}

int main()
{
    std::cout << "This program will demonstrate the use of matrices in C++." << std::endl;
    print_line();
    //initialise and output test matrices and a 4*4 matrix
    double* data_input_a{ new double[9]{1,2,3,9,8,7,4,2,6} };
    double* data_input_b{ new double[9]{5,5,4,1,2,3,6,9,8} };
    double* data_input_c{ new double[6]{3,4,1,2,5,6} };
    double* data_input_d{ new double[16]{3,4,1,2,5,6,1,2,3,9,8,7,4,2,6,1} };
    matrix matrix_a{ 3,3, data_input_a};
    matrix matrix_b{ 3,3, data_input_b};
    matrix matrix_c{ 2,3, data_input_c};
    matrix matrix_d{ 4,4, data_input_d};
    delete[] data_input_a;
    delete[] data_input_b;
    delete[] data_input_c;
    delete[] data_input_d;

    std::cout << "Outputting matrix_a:" << std::endl;
    std::cout << matrix_a;
    std::cout << "Outputting matrix_b:" << std::endl;
    std::cout << matrix_b;
    std::cout << "Outputting matrix_c:" << std::endl;
    std::cout << matrix_c;
    std::cout << "Outputting matrix_d:" << std::endl;
    std::cout << matrix_d;
    std::cout << "Outputting matrix_a with row 2 and column 2 deleted" << std::endl;
    std::cout << matrix_a.delete_row_column(2, 2);
    std::cout << "Outputting determinant of matrix_a:" << std::endl;
    std::cout << matrix_a.determinant() << std::endl;
    std::cout << "Outputting determinant of matrix_b:" << std::endl;
    std::cout << matrix_b.determinant() << std::endl;
    std::cout << "Outputting determinant of matrix_d:" << std::endl;
    std::cout << matrix_d.determinant() << std::endl;
    std::cout << "Demonstration of impossibility of determinant(matrix_c):" << std::endl;
    std::cout << matrix_c.determinant() << std::endl;

    print_line();
    //Demonstrate operations on test matrices
    std::cout << "Outputting matrix_a + matrix_b:" << std::endl;
    std::cout << matrix_a + matrix_b;
    std::cout << "Outputting matrix_a - matrix_b:" << std::endl;
    std::cout << matrix_a - matrix_b;
    std::cout << "Demonstration of impossibility of matrix_a - matrix_c:" << std::endl;
    std::cout << matrix_a - matrix_c;
    print_line();
    std::cout << "Outputting matrix_c * matrix_b:" << std::endl;
    std::cout << matrix_c * matrix_b;
    std::cout << "Demonstration of impossibility of matrix_b * matrix_c:" << std::endl;
    std::cout << matrix_b * matrix_c;
    print_line();

    //Demonstrate various copy operations on test matrices
    std::cout << "Copying matrix_a by assignment, output of copy:" << std::endl;
    matrix matrix_assigned;
    matrix_assigned = matrix_a;
    std::cout << matrix_assigned;
    std::cout << "Outputting matrix_a:" << std::endl;
    std::cout << matrix_a;
    print_line();
    std::cout << "Copying matrix_b by copy constructor, output of copy:" << std::endl;
    matrix matrix_copy_constructed{ matrix_b };
    std::cout << matrix_copy_constructed;
    std::cout << "Outputting matrix_b:" << std::endl;
    std::cout << matrix_b;
    print_line();
    std::cout << "Copying matrix-c by move constructor, output of copy:" << std::endl;
    matrix matrix_move_constructed{std::move(matrix_c)};
    std::cout << matrix_move_constructed;
    std::cout << "Outputting size of moved-from object matrix_c:" << std::endl;
    std::cout << matrix_c.length() << std::endl;
    print_line();
    std::cout << "Copying matrix_d by move assignment, output of copy:" << std::endl;
    matrix matrix_move_assigned;
    matrix_move_assigned  = std::move(matrix_d);
    std::cout << matrix_move_assigned;
    std::cout << "Outputting size of moved-from object matrix_d:" << std::endl;
    std::cout << matrix_d.length() << std::endl;

    print_line();
    //allow user to input 2 matrices, perform operations on them
    std::cout << "The user may now input two of their own matrices." << std::endl
              << "Square matrices of the same shape are recommended" << std::endl
              << "to find the determinants, sum, difference and product" << std::endl
              << "of the two matrices." << std::endl;
    matrix user_input_1;
    std::cout << " Defining user_matrix_1." << std::endl;
    std::cin >> user_input_1;
    std::cout << "Outputting user_matrix_1:" << std::endl;
    std::cout << user_input_1;
    matrix user_input_2;
    std::cout << "Defining user_matrix_2." << std::endl;
    std::cin >> user_input_2;
    std::cout << "Outputting user_matrix_2:" << std::endl;
    std::cout << user_input_2;
    print_line();
    std::cout << "Outputting determinant of user matrix 1:" << std::endl;
    std::cout << user_input_1.determinant() << std::endl;
    std::cout << "Outputting determinant of user matrix 2:" << std::endl;
    std::cout << user_input_2.determinant() << std::endl;
    std::cout << "Outputting user_matrix_1 + user_matrix_2:" << std::endl;
    std::cout << user_input_1 + user_input_2;
    std::cout << "Outputting user_matrix_1 - user_matrix_2:" << std::endl;
    std::cout << user_input_1 - user_input_2;
    std::cout << "Outputting user_matrix_1 * user_matrix_2:" << std::endl;
    std::cout << user_input_1 * user_input_2;

    return 0;
}
