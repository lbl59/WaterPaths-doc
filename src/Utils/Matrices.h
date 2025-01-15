/**
 * @class Matrices 
 * 
 * @brief This class defines a 2D and 3D matrix classes that are used to store data in the model.
 * This is a header-only file.
 * 
 * Created by bernardoct on 6/7/17.
 */
#ifndef TRIANGLEMODEL_MATRICES_H
#define TRIANGLEMODEL_MATRICES_H



#include <iostream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <memory>

template<typename T>
class Matrix2D {
private:
    /**
     * @brief The number of rows in the matrix.
     */
    int di_;
    /**
     * @brief The number of columns in the matrix.
     */
    int dj_;
    /**
     * @brief The data of the matrix.
     */
    unique_ptr<T[]> data_;
    /**
     * @brief A flag to indicate if the matrix has been initialized.
     */
    bool not_initialized = true;
public:
    Matrix2D();

    Matrix2D(int di, int dj);

    T &operator()(int i, int j);        // Subscript operators often come in pairs
    T operator()(int i, int j) const;  // Subscript operators often come in pairs
    // ...
    ~Matrix2D();                              // Destructor
    Matrix2D(const Matrix2D &m);               // Copy constructor
    Matrix2D<T> &operator=(const Matrix2D<T> &m);   // Assignment operator
    Matrix2D<T> &operator+=(const Matrix2D<T> &m);

    Matrix2D<T> &operator/(const double m);

    void reset(T value);

    void print(int i) const;

    int get_i();

    int get_j();

    bool empty();

    void setPartialData(int i, T *data, unsigned long length);

    void setData(T *data, int length);

    T *getPointerToElement(int i, int j) const;

    void add_to_position(int i, int j, T *data, int length);

    const vector<vector<T>> get_vector() const;
};

/**
 * @brief Construct a new Matrix 2 D< T>:: Matrix 2 D object with specified dimensions.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param di The number of rows in the matrix.
 * @param dj The number of columns in the matrix.
 * 
 * @throw length_error If either di or dj are 0.
 */
template<typename T>
Matrix2D<T>::Matrix2D(int di, int dj) : di_(di), dj_(dj) {
    if (di == 0 || dj == 0)
        throw length_error("Matrix2D constructor has 0 size");
    data_ = unique_ptr<T[]>(new T[di * dj]);
    fill_n(data_.get(), di_ * dj_, 0);
}

/**
 * @brief Copy constructor for a new Matrix 2 D< T>:: Matrix 2 D object by copying another matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param m The matrix to be copied.
 * 
 * @throw length_error If either di or dj of the original matrix are 0.
 */
template<typename T>
Matrix2D<T>::Matrix2D(const Matrix2D<T> &m) : di_(m.di_), dj_(m.dj_), not_initialized(m.not_initialized) {
    if (di_ == 0 || dj_ == 0)
        throw length_error("Matrix2D dimensions has 0 size");
    data_ = unique_ptr<T[]>(new T[di_ * dj_]);
    std::copy(m.data_.get(), m.data_.get() + di_ * dj_, data_.get());
}

/**
 * @brief Default constructor for a new Matrix 2 D< T>:: Matrix 2 D object.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 */
template<typename T>
Matrix2D<T>::Matrix2D() {}

/**
 * @brief Destructor for a Matrix 2 D< T>:: Matrix 2 D object.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 */
template<typename T>
Matrix2D<T>::~Matrix2D() {}

/**
 * @brief Assignment operator for a Matrix 2 D< T>:: Matrix 2 D object.
 * This operator copies the data from another matrix to this matrix, assigning the values in the original matrix to this matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param m The matrix to be copied.
 * @return Matrix2D<T>& 
 */
template<typename T>
Matrix2D<T> &Matrix2D<T>::operator=(const Matrix2D<T> &m) {
    di_ = m.di_;
    dj_ = m.dj_;
    if (di_ == 0 || dj_ == 0)
        throw length_error("Matrix2D dimensions has 0 size");
    data_ = unique_ptr<T[]>(new T[di_ * dj_]);
    std::copy(m.data_.get(), m.data_.get() + di_ * dj_, data_.get());
    return *this;
}

/**
 * @brief In-place addition operator that adds each element in the matrix to the corresponding element in another matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * 
 * @param m The matrix to be added to the current matrix.
 * @return Matrix2D<T>& 
 * 
 * @throw length_error If the dimensions of the two matrices are different.
 */
template<typename T>
Matrix2D<T> &Matrix2D<T>::operator+=(const Matrix2D<T> &m) {

    if (m.di_ != di_ || m.dj_ != dj_)
        throw length_error("Matrixes of different sizes cannot be added.");

    for (int i = 0; i < di_ * dj_; ++i) {
        data_[i] += m.data_[i];
    }
    return *this;
}

/**
 * @brief Division operator that divides each element in the matrix by a scalar.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param n The constant used to divide each element in the matrix.
 * @return Matrix2D<T>& 
 */
template<typename T>
Matrix2D<T> &Matrix2D<T>::operator/(const double n) {

    for (int i = 0; i < di_ * dj_; ++i) {
        data_[i] /= n;
    }
    return *this;
}

/**
 * @brief  a subscript operator that returns a reference to the element at position (i, j) in the matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param i The row index.
 * @param j The column index.
 * @return T& 
 * 
 * @throw length_error If the indices are out of bounds.
 */
template<typename T>
T &Matrix2D<T>::operator()(int i, int j) {
    if (i >= di_ || j >= dj_) {
        string error_message = "Matrix2D subscript out of bounds.\ni=" +
                               to_string(i) + " (>=" + to_string(di_) + "?)\nj=" +
                               to_string(j) + " (>" + to_string(dj_) + "?)";
        std::throw_with_nested(std::length_error(error_message.c_str()));
    }
    return data_[dj_ * i + j];
}

/**
 * @brief  a subscript operator that returns a reference to the element at position (i, j) in the matrix 
 * without changing the original matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param i The row index.
 * @param j The column index.
 * @return T& 
 * 
 * @throw length_error If the indices are out of bounds.
 */
template<typename T>
T Matrix2D<T>::operator()(int i, int j) const {
    if (i >= di_ || j >= dj_) {
        string error_message = "Matrix2D subscript out of bounds.\ni=" +
                               to_string(i) + " (>=" + to_string(di_) + "?)\nj=" +
                               to_string(j) + " (>" + to_string(dj_) + "?)";
        std::throw_with_nested(std::length_error(error_message.c_str()));
    }
    return data_[dj_ * i + j];
}

/**
 * @brief Print the i-th row of the matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param i The row index.
 * 
 * @return void
 */
template<typename T>
void Matrix2D<T>::print(int i) const {
    for (int j = 0; j < dj_; ++j) {
        printf("%0.2f ", data_[dj_ * i + j]);
    }
    printf("\n");
}

/**
 * @brief Set all elements in the matrix to specific values provided by the data.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param value The value to which all elements in the matrix will be set.
 * 
 * @return void
 * 
 * @throw throw_with_nested If the size of the data does not match the size of the matrix.
 */
template<typename T>
void Matrix2D<T>::setData(T *data, int length) {
    if (length != di_ * dj_) {
        string er = "Size of data does not match that of matrix: " +
                    to_string(length) + " vs. " + to_string(di_*dj_);
        throw_with_nested(invalid_argument(er.c_str()));
    }
    memcpy(data_.get(), data, length * sizeof(T));
}

/**
 * @brief Set a portion of the matrix to specific values provided by the data.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param i The row index where the data will be set.
 * @param data The data to be set.
 * @param length The length of the data.
 * 
 * @return void
 * 
 * @throw length_error If the indices are out of bounds.
 */
template<typename T>
void Matrix2D<T>::setPartialData(int i, T *data, unsigned long length) {
    if (i >= di_ + length)
        throw length_error("Matrix2D subscript out of bounds or negative");
    memcpy(data_.get() + i * dj_, data, length * sizeof(T));
}

/**
 * @brief Get the memory address of the element at position (i, j) in the matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param i The row index.
 * @param j The column index.
 * @return T* 
 */
template<typename T>
T *Matrix2D<T>::getPointerToElement(int i, int j) const {
    return data_.get() + i * dj_ + j;
}

/**
 * @brief Set all elements in the matrix to a specific value.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param value The value to which all elements in the matrix will be set.
 * 
 * @return void
 */
template<typename T>
void Matrix2D<T>::reset(T value) {
    fill_n(data_.get(), di_ * dj_, value);
}

/**
 * @brief Get the number of rows in the matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @return int 
 */
template<typename T>
int Matrix2D<T>::get_i() {
    return di_;
}

/**
 * @brief Get the number of columns in the matrix.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @return int 
 */
template<typename T>
int Matrix2D<T>::get_j() {
    return dj_;
}

/**
 * @brief A flag to indicate if the matrix has been initialized.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @return True if the matrix has not been initialized, false otherwise.
 */ 
template<typename T>
bool Matrix2D<T>::empty() {
    return not_initialized;
}

/**
 * @brief Adds a sequence of values to a specified position in a 2D matrix. 
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @param i The row index.
 * @param j The column index.
 * @param data A pointer to the array of values to be added.
 * @param length The length of the data array.
 */
template<typename T>
void Matrix2D<T>::add_to_position(int i, int j, T *data,
                                  int length) {
    int pos0 = i * dj_ + j;
    for (int p = 0; p < length; ++p) {
        data_[pos0 + p] += data[p];
    }
}

/**
 * @brief Converts the 2D matrix into a nested vector.
 * 
 * @tparam T The datatype stored in the 2D matrix.
 * @return const vector<vector<T>> 
 */
template<typename T>
const vector<vector<T>> Matrix2D<T>::get_vector() const {
    vector<vector<T>> vector_matrix;
    for (int i = 0; i < di_; ++i) {
        vector<T> row;
        for (int j = 0; j < dj_; ++j) {
            // Append row elements to the temporary row vector.
            row.push_back(data_[dj_ * i + j]);
        }
        // Append temporary row vector to the matrix vector.
        vector_matrix.push_back(row);
    }
    return vector_matrix;
}

template<typename T>
class Matrix3D {
private:
    /**
     * @brief The number of rows in the matrix.
     */
    int di_ = NON_INITIALIZED;
    /**
     * @brief The number of columns in the matrix.
     */
    int dj_ = NON_INITIALIZED; 
    /**
     * @brief The number of slices in the matrix.
     */
    int dk_ = NON_INITIALIZED;
    unique_ptr<T[]> data_;
public:
    Matrix3D();

    Matrix3D(int di, int dj, int dk);

    T &operator()(int i, int j, int k);        
    T operator()(int di, int dj, int dk) const;  

    ~Matrix3D();                              
    Matrix3D(const Matrix3D &m);               
    Matrix3D<T> &operator=(const Matrix3D<T> &m);  
    Matrix3D<T> &operator+=(const Matrix3D<T> &m);

    Matrix3D<T> &operator/(const double m);

    Matrix2D<T> get2D(int ijk, char dim);

    void add_to_position(int i, int j, int k, T* data, int length);

    void setPartialData(int i, int j, T *data, int length);

    T* getPointerToElement(int i, int j, int k) const;

    void reset(T value);

    void print(int i) const;

    void setData(T *data, int length);

    int get_i() const;

    int get_j() const;

    int get_k() const;

    bool empty() const;
};

/**
 * @brief Construct a new Matrix 3D< T>:: Matrix 3D object with specified dimensions.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param di The number of rows in the matrix.
 * @param dj The number of columns in the matrix.
 * @param dk The number of slices in the matrix.
 * 
 * @throw length_error If either di, dj, or dk are 0.
 */
template<typename T>
Matrix3D<T>::Matrix3D(int di, int dj, int dk) : di_(di), dj_(dj), dk_(dk)
{
    if (di == 0 || dj == 0 || dk == 0)
        throw length_error("Matrix3D dimensions has 0 size");
    data_ = unique_ptr<T[]>(new T[di * dj * dk]);
    fill_n(data_.get(), di_ * dj_ * dk_, 0);
}

/**
 * @brief Copy constructor for a new Matrix 3D< T>:: Matrix 3D object
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param m The Matrix3D object to be copied.
 * 
 * @throw length_error If either di, dj, or dk of the original matrix are 0.
 */
template<typename T>
Matrix3D<T>::Matrix3D(const Matrix3D<T> &m) : di_(m.di_), dj_(m.dj_), dk_(m.dk_) {
    if (di_ == 0 || dj_ == 0 || dk_ == 0)
        throw length_error("Matrix3D dimensions has 0 size");
    data_ = unique_ptr<T[]>(new T[di_ * dj_ * dk_]);
    std::copy(m.data_.get(), m.data_.get() + di_ * dj_ * dk_, data_.get());
}

/**
 * @brief A default constructor for a new Matrix 3D< T>:: Matrix 3D object.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 */
template<typename T>
Matrix3D<T>::Matrix3D() {}

/**
 * @brief Destroy the Matrix 3D< T>:: Matrix 3D object
 * 
 * @tparam T The datatype stored in the 3D matrix.
 */
template<typename T>
Matrix3D<T>::~Matrix3D() {}

/**
 * @brief Assignment operator for a Matrix 3D< T>:: Matrix 3D object.
 * This operator copies the data from another matrix to this matrix, assigning the values in the original matrix to this matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param m The matrix to be copied.
 * 
 * @return Matrix3D<T>& 
 */
template<typename T>
Matrix3D<T> &Matrix3D<T>::operator=(const Matrix3D<T> &m) {
    di_ = m.di_;
    dj_ = m.dj_;
    dk_ = m.dk_;
    if (di_ == 0 || dj_ == 0 || dk_ == 0)
        throw length_error("Matrix3D dimensions has 0 size");
    data_ = unique_ptr<T[]>(new T[di_ * dj_ * dk_]);
    std::copy(m.data_.get(), m.data_.get() + di_ * dj_ * dk_, data_.get());
    return *this;
}

/**
 * @brief In-place addition operator that adds each element in the matrix to the corresponding element in another matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param m The matrix to be added to the current matrix.
 * 
 * @return Matrix3D<T>& 
 * 
 * @throw length_error If the dimensions of the two matrices are different.
 */
template<typename T>
Matrix3D<T> &Matrix3D<T>::operator+=(const Matrix3D<T> &m) {

    if (m.di_ != di_ || m.dj_ != dj_ || m.dk_ != dk_)
        throw length_error("Matrixes of different sizes cannot be added.");

    for (int i = 0; i < di_ * dj_ * dk_; ++i) {
        data_[i] += m.data_[i];
    }
    return *this;
}

/**
 * @brief Division operator that divides each element in the matrix by a scalar.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param n The constant used to divide each element in the matrix.
 * 
 * @return Matrix3D<T>& 
 */
template<typename T>
Matrix3D<T> &Matrix3D<T>::operator/(const double n) {

    for (int i = 0; i < di_ * dj_ * dk_; ++i) {
        data_[i] /= n;
    }
    return *this;
}

/**
 * @brief A subscript operator that returns a reference to the element at position (i, j, k) in the matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param i the row index.
 * @param j The column index.
 * @param k The slice index.
 * 
 * @return T& 
 * 
 * @throw length_error If the indices are out of bounds.
 */
template<typename T>
T &Matrix3D<T>::operator()(int i, int j, int k) {
    if (i >= di_ || j >= dj_ || k >= dk_) {
        string error_message = "Matrix3D subscript out of bounds.\ni=" +
                to_string(i) + " (>=" + to_string(di_) + "?)\nj=" +
                to_string(j) + " (>" + to_string(dj_) + "?)\nk=" +
                to_string(k) + " (>=" + to_string(dk_) + "?)";
        std::throw_with_nested(std::length_error(error_message.c_str()));
    }
    return data_[dj_ * dk_ * i + dk_ * j + k];
}

/**
 * @brief A subscript operator that returns a reference to the element at position (i, j, k) in the matrix without changing the original matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param i the row index.
 * @param j The column index.
 * @param k The slice index.
 * 
 * @return T
 * 
 * @throw length_error If the indices are out of bounds.
 */
template<typename T>
T Matrix3D<T>::operator()(int i, int j, int k) const {
    if (i >= di_ || j >= dj_ || k >= dk_) {
        string error_message = "Matrix3D subscript out of bounds.\ni=" +
                to_string(i) + " (>=" + to_string(di_) + "?)\nj=" +
                to_string(j) + " (>" + to_string(dj_) + "?)\nk=" +
                to_string(k) + " (>=" + to_string(dk_) + "?)";
        std::throw_with_nested(std::length_error(error_message.c_str()));
    }
    return data_[dj_ * dk_ * i + dk_ * j + k];
}

/**
 * @brief Set all elements in the matrix to specific values provided by the data.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param value The value to which all elements in the matrix will be set.
 * 
 * @return void
 * 
 * @throw throw_with_nested If the size of the data does not match the size of the matrix.
 */
template<typename T>
void Matrix3D<T>::setData(T *data, int length) {
    if (length != di_*dj_*dk_) {
        string er = "Size of data does not match that of matrix: " +
                to_string(length) + " vs. " + to_string(di_*dj_*dk_);
        throw_with_nested(invalid_argument(er.c_str()));
    }
    memcpy(data_.get(), data, length* sizeof(T));
}

/**
 * @brief Set a subset of data at a specific position in the matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param i The row index where the data will be set.
 * @param data The data to be set.
 * @param length The length of the data.
 * 
 * @return void
 * 
 * @throw length_error If the indices are out of bounds.
 */
template<typename T>
void Matrix3D<T>::setPartialData(int i, int j, T *data, int length) {
    if (i >= di_ || j >= dj_ || i * j < 0)
        throw length_error("Matrix3D subscript out of bounds or negative");
    memcpy(data_.get() + i * dj_ * dk_ + j * dk_, data, length * sizeof(T));
}

/**
 * @brief Check if the matrix has been initialized.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @return True if the matrix has not been initialized, false otherwise.
 */
template<typename T>
bool Matrix3D<T>::empty() const {
    return di_ == NON_INITIALIZED;
}

/**
 * @brief Resets all elements in the matrix to a specific value.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param value The value to which all elements in the matrix will be set.
 * 
 * @return void
 */
template<typename T>
void Matrix3D<T>::reset(T value) {
    fill_n(data_.get(), di_ * dj_ * dk_, value);
}

/**
 * @brief Print the contents of the i-th slice of the matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param i The row index.
 * 
 * @return void
 */
template<typename T>
void Matrix3D<T>::print(int i) const {
    for (int j = 0; j < dj_; ++j) {
        for (int k = 0; k < dk_; ++k) {
            std::cout << data_[dj_ * dk_ * i + dk_ * j + k] << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Extract a 2D slice from the 3D matrix along the specified dimension.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param ijk The index along the selected dimension from which the slice is taken. 
 * @param dim The dimension along which the slice is extracted ('i', 'j', or 'k').
 * 
 * @return Matrix2D<T> 
 * 
 * @see setData
 */
template<typename T>
Matrix2D<T> Matrix3D<T>::get2D(int ijk, char dim) {
    T *data2D;
    Matrix2D<T> m;

    // Check the value of `dim` to determine the slicing dimension
    if (dim == 'k') {
        data2D = new T[di_ * dj_];
        for (int i = 0; i < di_; ++i) {
            for (int j = 0; j < dj_; ++j) {
                data2D[dj_ * i + j] = data_[dj_ * dk_ * i + dk_ * j + ijk];
            }
        }

        m = Matrix2D<T>(dj_, dk_);
        m.setData(data2D);

    } else if (dim == 'i') {
        data2D = new T[dj_ * dk_];
        for (int j = 0; j < dj_; ++j) {
            for (int k = 0; k < dk_; ++k) {
                data2D[dk_ * j + k] = data_[dj_ * dk_ * ijk + dk_ * j + k];
            }
        }

        m = Matrix2D<T>(di_, dk_);
        m.setData(data2D);

    } else if (dim == 'j') {
        data2D = new T[dj_ * dk_];
        for (int i = 0; i < di_; ++i) {
            for (int k = 0; k < dk_; ++k) {
                data2D[dk_ * i + +k] = data_[dj_ * dk_ * i + dk_ * ijk + k];
            }
        }

        m = Matrix2D<T>(di_, dj_);
        m.setData(data2D);
    } else

    return m;
}

/**
 * @brief Gets the number of slices in the matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @return int 
 */
template<typename T>
int Matrix3D<T>::get_i() const {
    return di_;
}

/**
 * @brief Gets the number of rows in the matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @return int 
 */
template<typename T>
int Matrix3D<T>::get_j() const {
    return dj_;
}

/**
 * @brief Gets the number of columns in the matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @return int 
 */
template<typename T>
int Matrix3D<T>::get_k() const {
    return dk_;
}

/**
 * @brief Gets the memory adddress of the element at position (i, j, k) in the matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param i The row index.
 * @param j The column index.
 * @param k The slice index.
 * 
 * @return T* 
 */
template<typename T>
T *Matrix3D<T>::getPointerToElement(int i, int j, int k) const {
    return data_.get() + i * dj_ * dk_ + j * dk_ + k;
}

/**
 * @brief Adds a sequence of values to a specified position in a 3D matrix.
 * 
 * @tparam T The datatype stored in the 3D matrix.
 * @param i The row index.
 * @param j The column index.
 * @param k The slice index.
 * @param data A pointer to the array of values to be added.
 * @param length The number of elements in the `data` array.
 * 
 * @return void
 */
template<typename T>
void Matrix3D<T>::add_to_position(int i, int j, int k, T *data,
                                  int length) {
    int pos0 = i * dj_ * dk_ + j * dk_ + k;
    for (int p = 0; p < length; ++p) {
        data_[pos0 + p] += data[p];
    }
}


#endif //TRIANGLEMODEL_MATRICES_H
