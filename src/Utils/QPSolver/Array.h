/**
 * @class Array
 * @brief This class defines templates for representing a 2D vector.
 * 
 * @details $Id: Array.hh 249 2008-11-20 09:58:23Z schaerf $
 * 
 * This file is part of EasyLocalpp: a C++ Object-Oriented framework aimed at easing the development of Local Search algorithms. 
 * 
 * Copyright (C) 2001--2008 Andrea Schaerf, Luca Di Gaspero. 
 * 
 * This software may be modified and distributed under the terms of the MIT license.  See the LICENSE file for details.
 */

#if !defined(_ARRAY_HH)
#define _ARRAY_HH

#include <set>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

enum MType { DIAG };

template <typename T>
class Vector
{
public:
    Vector();
    Vector(const unsigned int n);
    Vector(const T& a, const unsigned int n); 
    Vector(const T* a, const unsigned int n); 
    Vector(const Vector &rhs);
    ~Vector();

    inline T& operator[](const unsigned int& i);  
    inline const T& operator[](const unsigned int& i) const;
    inline unsigned int size() const;
    inline void resize(const unsigned int n);
    inline void resize(const T& a, const unsigned int n);

    Vector<T>& operator=(const Vector<T>& rhs); //assignment
    Vector<T>& operator=(const T& a);
    inline Vector<T>& operator+=(const Vector<T>& rhs);
    inline Vector<T>& operator-=(const Vector<T>& rhs);
    inline Vector<T>& operator*=(const Vector<T>& rhs);
    inline Vector<T>& operator/=(const Vector<T>& rhs);
    inline Vector<T>& operator^=(const Vector<T>& rhs);
    inline Vector<T>& operator+=(const T& a);
    inline Vector<T>& operator-=(const T& a);
    inline Vector<T>& operator*=(const T& a);
    inline Vector<T>& operator/=(const T& a);
    inline Vector<T>& operator^=(const T& a);

private:
    /**
     * @brief The size of the array. The upper index is `n-1`.
     */
    unsigned int n; 
    /**
     * @brief A pointer to the location in memory that stores the vector (the data).
     */
    T* v; 
};

// Templates enable us to write generic code that works with any datatype.
// Data type can be passed as a parameter in <>
/**
 * @brief Base constructor for a new Vector< T>:: Vector object with data type `T` with size 0 and no data.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 */
template <typename T>
Vector<T>::Vector()
        : n(0), v(0)
{}

/**
 * @brief Construct a new Vector< T>:: Vector object with size `n` and no data.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param n The size of the vector.
 */
template <typename T>
Vector<T>::Vector(const unsigned int n)
        : v(new T[n])
{
    this->n = n;
}

/**
 * @brief Construct a new Vector< T>:: Vector object with size `n` and initialize all elements to the constant value `a`.
 * 
 * This template takes a reference to the constant value `a` and initializes all elements to this value.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to initialize all elements to.
 * @param n The size of the vector
 */
template <typename T>
Vector<T>::Vector(const T& a, const unsigned int n)
        : v(new T[n])
{
    this->n = n;
    for (unsigned int i = 0; i < n; i++)
        v[i] = a;
}

/**
 * @brief Construct a new Vector< T>:: Vector object with size `n` and initialize all elements to the constant value `a`.
 * 
 * This template takes a pointer to the constant value `a`. 
 * It first dereferences the pointer to get the value of `a` and then increments the pointer to get the next value.
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The array to initialize the vector to.
 * @param n The size of the vector.
 */
template <typename T>
Vector<T>::Vector(const T* a, const unsigned int n)
        : v(new T[n])
{
    this->n = n;
    for (unsigned int i = 0; i < n; i++)
        v[i] = *a++;
}

/**
 * @brief Construct a new Vector< T>:: Vector object by copying the vector `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The vector to copy.
 */
template <typename T>
Vector<T>::Vector(const Vector<T>& rhs)
        : v(new T[rhs.n])
{
    this->n = rhs.n;
    for (unsigned int	i = 0; i < n; i++)
        v[i] = rhs[i];
}

/**
 * @brief Destroy the Vector< T>:: Vector object
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 */
template <typename T>
Vector<T>::~Vector()
{
    delete[] v;
}

/**
 * @brief Resize the vector to size `n`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param n The size to resize the vector to.
 */
template <typename T>
void Vector<T>::resize(const unsigned int n)
{
    if (n == this->n)
        return;

    delete[] (v);
    v = new T[n];

    this->n = n;
}

/**
 * @brief Resize the vector to size `n` and initialize all elements to the constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to initialize all elements to.
 * @param n The size to resize the vector to.
 */
template <typename T>
void Vector<T>::resize(const T& a, const unsigned int n)
{
    resize(n);
    for (unsigned int i = 0; i < n; i++)
        v[i] = a;
}

/**
 * @brief Assignment operator that assigns the current vector to the vector `rhs`.
 * 
 * Postcondition: normal assignment via copying has been performed.
 * If vector and rhs were different sizes, vector has been resized to match the size of `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The vector to assign to.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
{
    if (this != &rhs)
    {
        resize(rhs.n);
        for (unsigned int i = 0; i < n; i++)
            v[i] = rhs[i];
    }
    return *this;
}

/**
 * @brief Assignment operator that assigns the constant value `a` to every element of the vector.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to assign to every element.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T> & Vector<T>::operator=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        v[i] = a;
    return *this;
}

/**
 * @brief A subscripting operator to access the `i`-th element of the vector.
 * 
 * @param i The index of the element to access.
 * @return T& A reference to the `i`-th element.
 */
template <typename T>
inline T & Vector<T>::operator[](const unsigned int& i)
{
    return v[i];
}

/**
 * @brief A subscripting operator to access the `i`-th element of the vector without changing the vector itself.
 * 
 * @param i The index of the element to access.
 * @return T& A reference to the `i`-th element.
 */
template <typename T>
inline const T& Vector<T>::operator[](const unsigned int& i) const
{
    return v[i];
}

/**
 * @brief Returns the size of the vector.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @return unsigned int 
 */
template <typename T>
inline unsigned int Vector<T>::size() const
{
    return n;
}

/**
 * @brief Operator that performs an in-place addition of each element in the vector `rhs` to each element in the current vector.
 * 
 * @param rhs The vector to add.
 * @return Vector<T>& 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T>& Vector<T>::operator+=(const Vector<T>& rhs)
{
    if (this->size() != rhs.size())
        throw std::logic_error("Operator+=: vectors have different sizes");
    for (unsigned int i = 0; i < n; i++)
        v[i] += rhs[i];

    return *this;
}

/**
 * @brief Operator that  performs an in-place addition of a constant value `a` to each element in the vector.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to add.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T>& Vector<T>::operator+=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        v[i] += a;

    return *this;
}

/**
 * @brief Operator that returns the positive of the vector `rhs`.
 * 
 * @param rhs The vector to add.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T> operator+(const Vector<T>& rhs)
{
    return rhs;
}

/**
 * @brief Operator that adds each element in vector `rhs` to each element in `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand side vector to add.
 * @param rhs The right-hand sided vector to add. 
 * @return Vector<T> 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T> operator+(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::logic_error("Operator+: vectors have different sizes");
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = lhs[i] + rhs[i];

    return tmp;
}

/**
 * @brief Operator that adds a constant value `a` to each element in `lhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand side vector to add.
 * @param a The constant value to add.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator+(const Vector<T>& lhs, const T& a)
{
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = lhs[i] + a;

    return tmp;
}

/**
 * @brief Operator that adds a constant value `a` to each element in `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to add.
 * @param rhs The right-hand sided vector to add.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator+(const T& a, const Vector<T>& rhs)
{
    Vector<T> tmp(rhs.size());
    for (unsigned int i = 0; i < rhs.size(); i++)
        tmp[i] = a + rhs[i];

    return tmp;
}

/**
 * @brief Operator that performs an in-place subtraction of each element in vector `rhs` from each element in the current vector.
 * 
 * @param rhs The vector to subtract.
 * @return Vector<T>& 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T>& Vector<T>::operator-=(const Vector<T>& rhs)
{
    if (this->size() != rhs.size())
        throw std::logic_error("Operator-=: vectors have different sizes");
    for (unsigned int i = 0; i < n; i++)
        v[i] -= rhs[i];

    return *this;
}

/**
 * @brief Operator that performs an in-place subtraction of a constant value `a` from each element in the vector.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to subtract.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T>& Vector<T>::operator-=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        v[i] -= a;

    return *this;
}

/**
 * @brief Operator that returns the negative of the vector `rhs`.
 * 
 * @param rhs The vector to turn negative.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator-(const Vector<T>& rhs)
{
    return (T)(-1) * rhs;
}

/**
 * @brief Operator that subtracts each element in vector `rhs` from each element in `lhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand side vector to subtract.
 * @param rhs The right-hand sided vector to subtract. 
 * @return Vector<T> 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T> operator-(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::logic_error("Operator-: vectors have different sizes");
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = lhs[i] - rhs[i];

    return tmp;
}

/**
 * @brief Operator that subtracts a constant value `a` from each element in `lhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand side vector to subtract.
 * @param a The constant value to subtract.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator-(const Vector<T>& lhs, const T& a)
{
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = lhs[i] - a;

    return tmp;
}

/**
 * @brief Operator that subtracts each element in `rhs` from a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to subtract.
 * @param rhs The right-hand sided vector to subtract.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator-(const T& a, const Vector<T>& rhs)
{
    Vector<T> tmp(rhs.size());
    for (unsigned int i = 0; i < rhs.size(); i++)
        tmp[i] = a - rhs[i];

    return tmp;
}

/**
 * @brief Operator that multiplies each element in vector `rhs` with each element in the current vector.
 * 
 * @param rhs The vector to multiply.
 * @return Vector<T>& 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T>& Vector<T>::operator*=(const Vector<T>& rhs)
{
    if (this->size() != rhs.size())
        throw std::logic_error("Operator*=: vectors have different sizes");
    for (unsigned int i = 0; i < n; i++)
        v[i] *= rhs[i];

    return *this;
}

/**
 * @brief Operator that multiplies each element in the vector with the constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to multiply.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T>& Vector<T>::operator*=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        v[i] *= a;

    return *this;
}

/**
 * @brief Operator that multiplies each element in vector `lhs` with each element in `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand side vector to multiply.
 * @param rhs The right-hand sided vector to multiply. 
 * @return Vector<T> 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */ 
template <typename T>
inline Vector<T> operator*(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::logic_error("Operator*: vectors have different sizes");
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = lhs[i] * rhs[i];

    return tmp;
}

/**
 * @brief Operator that multiplies a constant value `a` to each element in `lhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand side vector to multiply.
 * @param a The constant value to multiply.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator*(const Vector<T>& lhs, const T& a)
{
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = lhs[i] * a;

    return tmp;
}

/**
 * @brief Operator that multiplies a constant value `a` to each element in `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to multiply.
 * @param rhs The right-hand sided vector to multiply.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator*(const T& a, const Vector<T>& rhs)
{
    Vector<T> tmp(rhs.size());
    for (unsigned int i = 0; i < rhs.size(); i++)
        tmp[i] = a * rhs[i];

    return tmp;
}

/**
 * @brief Operator that divides each element in vector `rhs` with each element in the current vector.
 * 
 * @param rhs The vector to divide.
 * @return Vector<T>& 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T>& Vector<T>::operator/=(const Vector<T>& rhs)
{
    if (this->size() != rhs.size())
        throw std::logic_error("Operator/=: vectors have different sizes");
    for (unsigned int i = 0; i < n; i++)
        v[i] /= rhs[i];

    return *this;
}

/**
 * @brief Operator that divides each element in the vector with the constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to divide.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T>& Vector<T>::operator/=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        v[i] /= a;

    return *this;
}

/**
 * @brief Operator that divides each element in vector `lhs` with each element in `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand side vector to divide.
 * @param rhs The right-hand sided vector to divide. 
 * @return Vector<T> 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T> operator/(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::logic_error("Operator/: vectors have different sizes");
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = lhs[i] / rhs[i];

    return tmp;
}

/**
 * @brief Operator that divides a constant value `a` to each element in `lhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand side vector to divide.
 * @param a The constant value to divide.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator/(const Vector<T>& lhs, const T& a)
{
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = lhs[i] / a;

    return tmp;
}

/**
 * @brief Operator that divides each element in `rhs` from a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to divide.
 * @param rhs The right-hand sided vector to divide.
 * @return Vector<T> 
 */
template <typename T>
inline Vector<T> operator/(const T& a, const Vector<T>& rhs)
{
    Vector<T> tmp(rhs.size());
    for (unsigned int i = 0; i < rhs.size(); i++)
        tmp[i] = a / rhs[i];

    return tmp;
}

/**
 * @brief Operator that raises each element in `lhs` to the power of `rhs`.
 * 
 * @param lhs The base vector .
 * @param rhs The vector to raise to the power of.
 * @return Vector<T>& 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T> operator^(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::logic_error("Operator^: vectors have different sizes");
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = pow(lhs[i], rhs[i]);

    return tmp;
}

/**
 * @brief Operator that raises each element in `lhs` to the power of the constant value `a`.
 * 
 * @param lhs The base vector.
 * @param a The constant value to raise to the power of.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T> operator^(const Vector<T>& lhs, const T& a)
{
    Vector<T> tmp(lhs.size());
    for (unsigned int i = 0; i < lhs.size(); i++)
        tmp[i] = pow(lhs[i], a);

    return tmp;
}

/**
 * @brief Operator that raises a constant value `a` to each element in `rhs`.
 * 
 * @param a The base constant value.
 * @param rhs The right-hand sided vector to raise to the power of.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T> operator^(const T& a, const Vector<T>& rhs)
{
    Vector<T> tmp(rhs.size());
    for (unsigned int i = 0; i < rhs.size(); i++)
        tmp[i] = pow(a, rhs[i]);

    return tmp;
}

/**
 * @brief Operator that performs an in-place raising of each element in the vector to the power each element in `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The vector to raise to the power of.
 * @return Vector<T>& 
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline Vector<T>& Vector<T>::operator^=(const Vector<T>& rhs)
{
    if (this->size() != rhs.size())
        throw std::logic_error("Operator^=: vectors have different sizes");
    for (unsigned int i = 0; i < n; i++)
        v[i] = pow(v[i], rhs[i]);

    return *this;
}

/**
 * @brief Operator that performs an in-place raising of each element in the vector to the power of the constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to raise to the power of.
 * @return Vector<T>& 
 */
template <typename T>
inline Vector<T>& Vector<T>::operator^=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        v[i] = pow(v[i], a);

    return *this;
}

/**
 * @brief Equivalence operator that checks if all elements in two vectors are equal.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param v A vector to compare.
 * @param w A vector to compare.
 * @return True is all values in `v` are equal to the corresponding values in `w`. False otherwise.
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */ 
template <typename T>
inline bool operator==(const Vector<T>& v, const Vector<T>& w)
{
    if (v.size() != w.size())
        throw std::logic_error("Vectors of different size are not confrontable");
    for (unsigned i = 0; i < v.size(); i++)
        if (v[i] != w[i])
            return false;
    return true;
}

/**
 * @brief A non-equivalence operator that checks if any element in two vectors is not equal.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param v A vector to compare.
 * @param w A vector to compare.
 * @return True if any element in `v` is not equal to the corresponding element in `w`. False otherwise.
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline bool operator!=(const Vector<T>& v, const Vector<T>& w)
{
    if (v.size() != w.size())
        throw std::logic_error("Vectors of different size are not confrontable");
    for (unsigned i = 0; i < v.size(); i++)
        if (v[i] != w[i])
            return true;
    return false;
}

/**
 * @brief Operator that checks if all elements in the vector `v` are less than the corresponding elements in the vector `w`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param v A vector to compare.
 * @param w A vector to compare.
 * @return True is all elements in `v` are less than the corresponding elements in `w`. False otherwise.
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline bool operator<(const Vector<T>& v, const Vector<T>& w)
{
    if (v.size() != w.size())
        throw std::logic_error("Vectors of different size are not confrontable");
    for (unsigned i = 0; i < v.size(); i++)
        if (v[i] >= w[i])
            return false;
    return true;
}

/**
 * @brief Operator that checks if all elements in the vector `v` are less than or equal to the corresponding elements in the vector `w`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param v A vector to compare.
 * @param w A vector to compare.
 * @return True if all elements in `v` are less than or equal to the corresponding elements in `w`. False otherwise.
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline bool operator<=(const Vector<T>& v, const Vector<T>& w)
{
    if (v.size() != w.size())
        throw std::logic_error("Vectors of different size are not confrontable");
    for (unsigned i = 0; i < v.size(); i++)
        if (v[i] > w[i])
            return false;
    return true;
}

/**
 * @brief Operator that checks if all elements in the vector `v` are greater than the corresponding elements in the vector `w`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param v A vector to compare.
 * @param w A vector to compare.
 * @return True if all elements in `v` are greater than the corresponding elements in `w`. False otherwise.
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline bool operator>(const Vector<T>& v, const Vector<T>& w)
{
    if (v.size() != w.size())
        throw std::logic_error("Vectors of different size are not confrontable");
    for (unsigned i = 0; i < v.size(); i++)
        if (v[i] <= w[i])
            return false;
    return true;
}

/**
 * @brief Operator that checks if all elements in the vector `v` are greater than or equal to the corresponding elements in the vector `w`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param v A vector to compare.
 * @param w A vector to compare.
 * @return True if all elements in `v` are greater than or equal to the corresponding elements in `w`. False otherwise.
 * 
 * @throw std::logic_error if the vectors are of different sizes.
 */
template <typename T>
inline bool operator>=(const Vector<T>& v, const Vector<T>& w)
{
    if (v.size() != w.size())
        throw std::logic_error("Vectors of different size are not confrontable");
    for (unsigned i = 0; i < v.size(); i++)
        if (v[i] < w[i])
            return false;
    return true;
}

/**
 * @brief Output stream operator that prints the vector to the output stream.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors.
 * @param os The output stream.
 * @param v The vector to print.
 * @return std::ostream& 
 * 
 */
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector<T>& v)
{
    os << std::endl << v.size() << std::endl;
    for (unsigned int i = 0; i < v.size() - 1; i++)
        os << std::setw(20) << std::setprecision(16) << v[i] << ", ";
    os << std::setw(20) << std::setprecision(16) << v[v.size() - 1] << std::endl;

    return os;
}

/**
 * @brief Input stream operator that reads the vector from the input stream.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param is The input stream.
 * @param v The vector to read.
 * @return std::istream& 
 */
template <typename T>
std::istream& operator>>(std::istream& is, Vector<T>& v)
{
    int elements;
    char comma;
    is >> elements;
    v.resize((const unsigned int) elements);
    for (unsigned int i = 0; i < elements; i++)
        is >> v[i] >> comma;

    return is;
}

/**
 * @brief This function partitions a segment of a vector around a pivot for use in quicksort.
 * 
 * The partition operation rearranges elements in the vector such that all elements less than 
 * the pivot are on its left, and all greater elements are on its right.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. The type of elements stored in the vector.
 * @param v The vector to be partitioned.
 * @param begin The starting index of the segment to partition.
 * @param end The ending index of the segment to partition.
 * @return unsigned int The index of the pivot after partitioning.
 * 
 */
template <typename T>
inline unsigned int partition(Vector<T>& v, unsigned int begin, unsigned int end)
{
    unsigned int i = begin + 1, j = begin + 1;

    // Select the first element in the segment as the pivot.
    T pivot = v[begin];
    while (j <= end)
    // Compare each element to the pivot.
    {
        if (v[j] < pivot) {
            // Swap elements as necessary to ensure elements less than the pivot are placed to its left
            std::swap(v[i], v[j]);
            i++;
        }
        j++;
    }
    // Place the pivot in its correct position
    // Return its index.
    v[begin] = v[i - 1];
    v[i - 1] = pivot;
    return i - 2;
}

/**
 * @brief This function performs a quicksort on a vector. 
 * Quicksort is performed using a recursive divide-and-conquer strategy.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param v The vector to be sorted.
 * @param begin The starting index of the segment to sort.
 * @param end The ending index of the segment to sort.
 */
template <typename T>
inline void quicksort(Vector<T>& v, unsigned int begin, unsigned int end)
{
    if (end > begin)
    {
        unsigned int index = partition(v, begin, end);
        quicksort(v, begin, index);
        quicksort(v, index + 2, end);
    }
}

template <typename T>
class Matrix
{
public:
    
    Matrix(); 
    Matrix(const unsigned int n, const unsigned int m); // Construct a n allocations_aux m matrix
    Matrix(const T& a, const unsigned int n, const unsigned int m); // Initialize the content to constant a
    Matrix(MType t, const T& a, const T& o, const unsigned int n, const unsigned int m);
    Matrix(MType t, const Vector<T>& v, const T& o, const unsigned int n, const unsigned int m);
    Matrix(const T* a, const unsigned int n, const unsigned int m); // Initialize to array
    Matrix(const Matrix<T>& rhs); // Copy constructor
    ~Matrix(); // destructor

    inline T* operator[](const unsigned int& i) { return v[i]; } // Subscripting: row i
    inline const T* operator[](const unsigned int& i) const { return v[i]; }; // const subsctipting

    inline void resize(const unsigned int n, const unsigned int m);
    inline void resize(const T& a, const unsigned int n, const unsigned int m);

    inline Vector<T> extractRow(const unsigned int i) const;
    inline Vector<T> extractColumn(const unsigned int j) const;


    inline unsigned int nrows() const { return n; } // number of rows
    inline unsigned int ncols() const { return m; } // number of columns

    inline Matrix<T>& operator=(const Matrix<T>& rhs); // Assignment operator
    inline Matrix<T>& operator=(const T& a); // Assign to every element value a
    inline Matrix<T>& operator+=(const Matrix<T>& rhs);
    inline Matrix<T>& operator-=(const Matrix<T>& rhs);
    inline Matrix<T>& operator*=(const Matrix<T>& rhs);
    inline Matrix<T>& operator/=(const Matrix<T>& rhs);
    inline Matrix<T>& operator^=(const Matrix<T>& rhs);
    inline Matrix<T>& operator+=(const T& a);
    inline Matrix<T>& operator-=(const T& a);
    inline Matrix<T>& operator*=(const T& a);
    inline Matrix<T>& operator/=(const T& a);
    inline Matrix<T>& operator^=(const T& a);
    inline operator Vector<T>();
private:
    /**
     * @brief The number of rows of the matrix.
     */
    unsigned int n;
    /**
     * @brief The number of columns of the matrix.
     */
    unsigned int m;
    /**
     * @brief The pointer to the memory address where the matrix data is stored.
     */
    T **v;
};

/**
 * @brief The default constructor for a new Matrix object.
 * This constructor initializes the matrix to have 0 rows and 0 columns.
 */
template <typename T>
Matrix<T>::Matrix()
        : n(0), m(0), v(0)
{}

/**
 * @brief Construct a new Matrix< T>:: Matrix object with `n` rows and `m` columns.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param n The number of rows in the matrix.
 * @param m The number of columns in the matrix.
 */
template <typename T>
Matrix<T>::Matrix(unsigned int n, unsigned int m)
        : v(new T*[n])
{
    register unsigned int i;
    this->n = n; this->m = m;
    v[0] = new T[m * n];
    for (i = 1; i < n; i++)
        v[i] = v[i - 1] + m;
}

/**
 * @brief Construct a new Matrix< T>:: Matrix object with `n` rows and `m` columns and initialize all elements to the constant value `a`.
 * This constructor initializes all elements in the matrix to a reference to the constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a A reference to the constant value to initialize all elements in the matrix to.
 * @param n The number of rows in the matrix.
 * @param m The number of columns in the matrix.
 */
template <typename T>
Matrix<T>::Matrix(const T& a, unsigned int n, unsigned int m)
        : v(new T*[n])
{
    register unsigned int i, j;
    this->n = n; this->m = m;
    v[0] = new T[m * n];
    for (i = 1; i < n; i++)
        v[i] = v[i - 1] + m;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            v[i][j] = a;
}

/**
 * @brief Construct a new Matrix< T>:: Matrix object with `n` rows and `m` columns and initialize all elements to the constant value `a`.
 * This constructor initializes all elements in the matrix to a pointer to the constant value `a`.
 * It first dereferences the pointer `a` and then assigns the value to each element in the matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a A reference to the constant value to initialize all elements in the matrix to.
 * @param n The number of rows in the matrix.
 * @param m The number of columns in the matrix.
 */
template <class T>
Matrix<T>::Matrix(const T* a, unsigned int n, unsigned int m)
        : v(new T*[n])
{
    register unsigned int i, j;
    this->n = n; this->m = m;
    v[0] = new T[m * n];
    for (i = 1; i < n; i++)
        v[i] = v[i - 1] + m;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            v[i][j] = *a++;
}

/**
 * @brief Constructs a diagonal matrix of specified dimensions and initializes its elements based on the matrix type.
 * 
 * This constructor supports creating a diagonal matrix with a constant value `a` on the diagonal and `o` as the off-diagonal elements.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. The type of elements stored in the matrix.
 * @param t The type of the matrix to create (e.g., DIAG for a diagonal matrix).
 * @param a The value to initialize the diagonal elements (if applicable).
 * @param o The value to initialize the off-diagonal elements (if applicable).
 * @param n The number of rows in the matrix.
 * @param m The number of columns in the matrix.
 * 
 * @throws std::logic_error If the specified matrix type is not supported.
 * 
 */
template <class T>
Matrix<T>::Matrix(MType t, const T& a, const T& o, unsigned int n, unsigned int m)
        : v(new T*[n])
{
    // Allocate memory for the matrix data and set up row pointers.
    register unsigned int i, j;
    this->n = n; this->m = m;
    v[0] = new T[m * n];
    for (i = 1; i < n; i++)
        v[i] = v[i - 1] + m;
    // Check the specified matrix type.
    switch (t)
    {
        // Initialize diagonal elements to `a` and off-diagonal elements to `o`.
        case DIAG:
            for (i = 0; i < n; i++)
                for (j = 0; j < m; j++)
                    if (i != j)
                        v[i][j] = o;
                    else
                        v[i][j] = a;
            break;
        default:
            throw std::logic_error("Matrix type not supported");
    }
}

/**
 * @brief Constructs a diagonal matrix of specified dimensions and initializes its elements based on the matrix type.
 * 
 * This constructor supports creating a diagonal matrix with the input vector `a`'s elements on the diagonal and 
 * a pre-specified value `o` as the off-diagonal elements.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. The type of elements stored in the matrix.
 * @param t The type of the matrix to create (e.g., DIAG for a diagonal matrix).
 * @param a The vector used to initialize the diagonal elements (if applicable).
 * @param o The value to initialize the off-diagonal elements (if applicable).
 * @param n The number of rows in the matrix.
 * @param m The number of columns in the matrix.
 * 
 * @throws std::logic_error If the specified matrix type is not supported.
 * 
 */
template <class T>
Matrix<T>::Matrix(MType t, const Vector<T>& a, const T& o, unsigned int n, unsigned int m)
        : v(new T*[n])
{
    register unsigned int i, j;
    this->n = n; this->m = m;
    v[0] = new T[m * n];
    for (i = 1; i < n; i++)
        v[i] = v[i - 1] + m;
    switch (t)
    {
        case DIAG:
            for (i = 0; i < n; i++)
                for (j = 0; j < m; j++)
                    if (i != j)
                        v[i][j] = o;
                    else
                        v[i][j] = a[i];
            break;
        default:
            throw std::logic_error("Matrix type not supported");
    }
}

/**
 * @brief Constructs a new matrix as a deep copy of an existing matrix.
 * 
 * This copy constructor duplicates the dimensions and contents of the provided matrix into a new matrix instance.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. The type of elements stored in the matrix.
 * @param rhs The matrix to copy.
 */
template <typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
        : v(new T*[rhs.n])
{
    register unsigned int i, j;
    n = rhs.n; m = rhs.m;
    v[0] = new T[m * n];
    for (i = 1; i < n; i++)
        v[i] = v[i - 1] + m;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            v[i][j] = rhs[i][j];
}

/**
 * @brief Destroys the matrix object and frees the memory allocated for the matrix data.
 */
template <typename T>
Matrix<T>::~Matrix()
{
    if (v != 0) {
        delete[] (v[0]);
        delete[] (v);
    }
}

/**
 * @brief Assignment operator that copies the contents of the right-hand side matrix `rhs` into the current matrix.
 * If matrix and rhs were different sizes, matrix has been resized to match the size of `rhs`.
 * 
 * Postcondition: normal assignment via copying has been performed;
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The right-hand side matrix to copy.
 * @return Matrix<T>& 
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator=(const Matrix<T> &rhs)
{
    register unsigned int i, j;
    if (this != &rhs)
    {
        resize(rhs.n, rhs.m);
        for (i = 0; i < n; i++)
            for (j = 0; j < m; j++)
                v[i][j] = rhs[i][j];
    }
    return *this;
}

/**
 * @brief Assignment operator that assigns the constant value `a` to each element in the matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to assign to each element in the matrix.
 * @return Matrix<T>& 
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator=(const T& a)
{
    register unsigned int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            v[i][j] = a;
    return *this;
}

/**
 * @brief Resizes the matrix to have `n` rows and `m` columns.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param n The number of rows of the resized matrix.
 * @param m The number of columns of the resized matrix.
 */
template <typename T>
inline void Matrix<T>::resize(const unsigned int n, const unsigned int m)
{
    register unsigned int i;
    if (n == this->n && m == this->m)
        return;
    if (v != 0)
    {
        delete[] (v[0]);
        delete[] (v);
    }
    this->n = n; this->m = m;
    v = new T*[n];
    v[0] = new T[m * n];
    for (i = 1; i < n; i++)
        v[i] = v[i - 1] + m;
}

/**
 * @brief Resizes the matrix to have `n` rows and `m` columns. Sets all elements to the constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to set all elements in the matrix to.
 * @param n The number of rows of the resized matrix.
 * @param m The number of columns of the resized matrix.
 */
template <typename T>
inline void Matrix<T>::resize(const T& a, const unsigned int n, const unsigned int m)
{
    register unsigned int i, j;
    resize(n, m);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            v[i][j] = a;
}

/**
 * @brief Extracts a specific row from the matrix as a vector.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param i The row index to extract.
 * @return Vector<T> 
 * 
 * @throw std::logic_error if the row index is out of bounds.
 */
template <typename T>
inline Vector<T> Matrix<T>::extractRow(const unsigned int i) const
{
    if (i >= n)
        throw std::logic_error("Error in extractRow: trying to extract a row out of matrix bounds");
    Vector<T> tmp(v[i], m);

    return tmp;
}

/**
 * @brief Extracts a specific column from the matrix as a vector.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param j The column index to extract.
 * @return Vector<T> 
 * 
 * @throw std::logic_error if the column index is out of bounds.
 */
template <typename T>
inline Vector<T> Matrix<T>::extractColumn(const unsigned int j) const
{
    register unsigned int i;
    if (j >= m)
        throw std::logic_error("Error in extractRow: trying to extract a row out of matrix bounds");
    Vector<T> tmp(n);

    for (i = 0; i < n; i++)
        tmp[i] = v[i][j];

    return tmp;
}

/**
 * @brief Converts the matrix to its positive equivalent.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The input matrix.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator+(const Matrix<T>& rhs)
{
    return rhs;
}

/**
 * @brief Operator that adds two matrices of the same dimensions together.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to add.
 * @param rhs The right-hand sided matrix to add.
 * @return Matrix<T> 
 * 
 * @throw std::logic_error if the matrices are of different sizes.
 */
template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    if (lhs.ncols() != rhs.ncols() || lhs.nrows() != rhs.nrows())
        throw std::logic_error("Operator+: matrices have different sizes");
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = lhs[i][j] + rhs[i][j];

    return tmp;
}

/**
 * @brief Operator that adds a constant value `a` to each element in the matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to add.
 * @param a The constant value to add.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const T& a)
{
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = lhs[i][j] + a;

    return tmp;
}

/**
 * @brief Operator that adds a constant value `a` to each element in the matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to add.
 * @param rhs The right-hand sided matrix to add.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator+(const T& a, const Matrix<T>& rhs)
{
    Matrix<T> tmp(rhs.nrows(), rhs.ncols());
    for (unsigned int i = 0; i < rhs.nrows(); i++)
        for (unsigned int j = 0; j < rhs.ncols(); j++)
            tmp[i][j] = a + rhs[i][j];

    return tmp;
}

/**
 * @brief Operator that performs an in-place addition of two matrices with the same dimensions.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The right-hand sided matrix to add.
 * @return Matrix<T>& 
 * 
 * @throw std::logic_error if the matrices are of different sizes.
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs)
{
    if (m != rhs.ncols() || n != rhs.nrows())
        throw std::logic_error("Operator+=: matrices have different sizes");
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] += rhs[i][j];

    return *this;
}

/**
 * @brief Operator that performs an in-place addition of a constant value `a` to each element in the matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to add.
 * @return Matrix<T>& 
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator+=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] += a;

    return *this;
}

/**
 * @brief Converts the matrix to its negative equivalent.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The matrix to convert.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator-(const Matrix<T>& rhs)
{
    return (T)(-1) * rhs;
}

/**
 * @brief Operator that subtracts two matrices of the same dimensions.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to subtract.
 * @param rhs The right-hand sided matrix to subtract.
 * @return Matrix<T> 
 * 
 * @throw std::logic_error if the matrices are of different sizes.
 */
template <typename T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    if (lhs.ncols() != rhs.ncols() || lhs.nrows() != rhs.nrows())
        throw std::logic_error("Operator-: matrices have different sizes");
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = lhs[i][j] - rhs[i][j];

    return tmp;
}

/**
 * @brief Operator that subtracts a constant value `a` from each element in the matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to subtract.
 * @param a The constant value to subtract.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator-(const Matrix<T>& lhs, const T& a)
{
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = lhs[i][j] - a;

    return tmp;
}

/**
 * @brief Operator that subtracts each element in the matrix from a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to subtract from.
 * @param rhs The right-hand sided matrix to subtract.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator-(const T& a, const Matrix<T>& rhs)
{
    Matrix<T> tmp(rhs.nrows(), rhs.ncols());
    for (unsigned int i = 0; i < rhs.nrows(); i++)
        for (unsigned int j = 0; j < rhs.ncols(); j++)
            tmp[i][j] = a - rhs[i][j];

    return tmp;
}

/**
 * @brief Operator that performs an in-place subtraction of two matrices with the same dimensions.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The right-hand sided matrix to subtract.
 * @return Matrix<T>& 
 * 
 * @throw std::logic_error if the matrices are of different sizes.
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs)
{
    if (m != rhs.ncols() || n != rhs.nrows())
        throw std::logic_error("Operator-=: matrices have different sizes");
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] -= rhs[i][j];

    return *this;
}

/**
 * @brief Operator that performs an in-place subtraction of a constant value `a` from each element in the matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to subtract.
 * @return Matrix<T>& 
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator-=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] -= a;

    return *this;
}

/**
 * @brief Operator that multiplies each element of two matrices of the same dimensions together.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to multiply.
 * @param rhs The right-hand sided matrix to multiply.
 * @return Matrix<T> 
 * 
 * @throw std::logic_error if the matrices are of different sizes.
 */
template <typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    if (lhs.ncols() != rhs.ncols() || lhs.nrows() != rhs.nrows())
        throw std::logic_error("Operator*: matrices have different sizes");
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = lhs[i][j] * rhs[i][j];

    return tmp;
}

/**
 * @brief Operator that multiplies each element of the matrix by a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to multiply.
 * @param a The constant value to multiply.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const T& a)
{
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = lhs[i][j] * a;

    return tmp;
}

/**
 * @brief Operator that multiplies each element of the matrix by a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to multiply.
 * @param rhs The right-hand sided matrix to multiply.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator*(const T& a, const Matrix<T>& rhs)
{
    Matrix<T> tmp(rhs.nrows(), rhs.ncols());
    for (unsigned int i = 0; i < rhs.nrows(); i++)
        for (unsigned int j = 0; j < rhs.ncols(); j++)
            tmp[i][j] = a * rhs[i][j];

    return tmp;
}

/**
 * @brief Operator that performs an in-place multiplication of each element of two matrices with the same dimensions.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The right-hand sided matrix to multiply.
 * @return Matrix<T>& 
 * 
 * @throw std::logic_error if the matrices are of different sizes.
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs)
{
    if (m != rhs.ncols() || n != rhs.nrows())
        throw std::logic_error("Operator*=: matrices have different sizes");
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] *= rhs[i][j];

    return *this;
}

/**
 * @brief Operator that performs an in-place multiplication of each element of the matrix by a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to multiply.
 * @return Matrix<T>& 
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator*=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] *= a;

    return *this;
}

/**
 * @brief Operator that divides each element of two matrices of the same dimensions.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to divide.
 * @param rhs The right-hand sided matrix to divide.
 * @return Matrix<T> 
 * 
 * @throw std::logic_error if the matrices are of different sizes.
 */
template <typename T>
Matrix<T> operator/(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    if (lhs.ncols() != rhs.ncols() || lhs.nrows() != rhs.nrows())
        throw std::logic_error("Operator+: matrices have different sizes");
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = lhs[i][j] / rhs[i][j];

    return tmp;
}

/**
 * @brief Operator that divides each element of the matrix by a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to divide.
 * @param a The constant value to divide.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator/(const Matrix<T>& lhs, const T& a)
{
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = lhs[i][j] / a;

    return tmp;
}

/**
 * @brief Operator that divides each element of the matrix by a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to divide.
 * @param rhs The right-hand sided matrix to divide.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator/(const T& a, const Matrix<T>& rhs)
{
    Matrix<T> tmp(rhs.nrows(), rhs.ncols());
    for (unsigned int i = 0; i < rhs.nrows(); i++)
        for (unsigned int j = 0; j < rhs.ncols(); j++)
            tmp[i][j] = a / rhs[i][j];

    return tmp;
}

/**
 * @brief Operator that performs an in-place division of each element of two matrices with the same dimensions.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The right-hand sided matrix to divide.
 * @return Matrix<T>& 
 * 
 * @throw std::logic_error if the matrices are of different sizes.
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator/=(const Matrix<T>& rhs)
{
    if (m != rhs.ncols() || n != rhs.nrows())
        throw std::logic_error("Operator+=: matrices have different sizes");
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] /= rhs[i][j];

    return *this;
}

/**
 * @brief Operator that performs an in-place division of each element of the matrix by a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to divide.
 * @return Matrix<T>& 
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator/=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] /= a;

    return *this;
}

/**
 * @brief Operator that raises each element of the matrix to the power of a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param lhs The left-hand sided matrix to raise to the power of.
 * @param a The constant value to raise each element to the power of.
 * @return Matrix<T> 
 */
template <typename T>
Matrix<T> operator^(const Matrix<T>& lhs, const T& a)
{
    Matrix<T> tmp(lhs.nrows(), lhs.ncols());
    for (unsigned int i = 0; i < lhs.nrows(); i++)
        for (unsigned int j = 0; j < lhs.ncols(); j++)
            tmp[i][j] = pow(lhs[i][j], a);

    return tmp;
}

/**
 * @brief Operator that raises each element of the matrix to the power of each element in `rhs`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param rhs The right-hand sided matrix to raise to the power of.
 * @return Matrix<T> 
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator^=(const Matrix<T>& rhs)
{
    if (m != rhs.ncols() || n != rhs.nrows())
        throw std::logic_error("Operator^=: matrices have different sizes");
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] = pow(v[i][j], rhs[i][j]);

    return *this;
}

/**
 * @brief Operator that performs an in-place operation to raise each element of the matrix to the power of a constant value `a`.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The constant value to raise each element to the power of.
 * @return Matrix<T>& 
 */
template <typename T>
inline Matrix<T>& Matrix<T>::operator^=(const T& a)
{
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int j = 0; j < m; j++)
            v[i][j] = pow(v[i][j], a);

    return *this;
}

/**
 * @brief Converts a matrix to a vector if it has a single row or column.
 * 
 * This operator allows a matrix to be cast to a vector by extracting its single row or column.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. The type of elements stored in the matrix.
 * @return Vector<T> The extracted row or column as a vector.
 * 
 * @throws std::logic_error If the matrix has more than one row and more than one column.
 */
template <typename T>
inline Matrix<T>::operator Vector<T>()
{
    if (n > 1 && m > 1)
        throw std::logic_error("Error matrix cast to vector: trying to cast a multi-dimensional matrix");
    if (n == 1)
        return extractRow(0);
    else
        return extractColumn(0);
}

/**
 * @brief Equality operator that compares two matrices for equality.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a Matrix to compare.
 * @param b Matrix to compare.
 * @return True if all elements in the matrices are equal, false otherwise.
 * 
 * @throw std::logic_error If the matrices are of different sizes.
 */
template <typename T>
inline bool operator==(const Matrix<T>& a, const Matrix<T>& b)
{
    if (a.nrows() != b.nrows() || a.ncols() != b.ncols())
        throw std::logic_error("Matrices of different size are not confrontable");
    for (unsigned i = 0; i < a.nrows(); i++)
        for (unsigned j = 0; j < a.ncols(); j++)
            if (a[i][j] != b[i][j])
                return false;
    return true;
}

/**
 * @brief Inequality operator that compares two matrices for inequality.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a Matrix to compare.
 * @param b Matrix to compare. 
 * @return True if any element in the matrices is not equal, false otherwise. 
 *
 * @throw std::logic_error If the matrices are of different sizes.
 */
template <typename T>
inline bool operator!=(const Matrix<T>& a, const Matrix<T>& b)
{
    if (a.nrows() != b.nrows() || a.ncols() != b.ncols())
        throw std::logic_error("Matrices of different size are not confrontable");
    for (unsigned i = 0; i < a.nrows(); i++)
        for (unsigned j = 0; j < a.ncols(); j++)
            if (a[i][j] != b[i][j])
                return true;
    return false;
}

/**
 * @brief Output stream operator that writes the matrix to the output stream.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param os The output stream to write to.
 * @param m The matrix to write.
 * @return std::ostream& The output stream with the matrix written to it.
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
    os << std::endl << m.nrows() << " " << m.ncols() << std::endl;
    for (unsigned int i = 0; i < m.nrows(); i++)
    {
        for (unsigned int j = 0; j < m.ncols() - 1; j++)
            os << std::setw(20) << std::setprecision(16) << m[i][j] << ", ";
        os << std::setw(20) << std::setprecision(16) << m[i][m.ncols() - 1] << std::endl;
    }

    return os;
}

/**
 * @brief Input stream operator that reads the matrix from the input stream.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param is The input stream to read from.
 * @param m The matrix to read into.
 * @return std::istream& The input stream with the matrix read from it.
 */
template <typename T>
std::istream& operator>>(std::istream& is, Matrix<T>& m)
{
    unsigned int rows, cols;
    char comma;
    is >> rows >> cols;
    m.resize(rows, cols);
    for (unsigned int i = 0; i < rows; i++)
        for (unsigned int j = 0; j < cols; j++)
            is >> m[i][j] >> comma;

    return is;
}

/**
 * @brief Computes the transpose of the matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors. 
 * @param a The matrix to transpose.
 * @return Matrix<T> The transposed matrix.
 */
template <typename T>
Matrix<T> t(const Matrix<T>& a)
{
    Matrix<T> tmp(a.ncols(), a.nrows());
    for (unsigned int i = 0; i < a.nrows(); i++)
        for (unsigned int j = 0; j < a.ncols(); j++)
            tmp[j][i] = a[i][j];

    return tmp;
}

/**
 * @brief Solves a linear system using the Cholesky decomposition of a matrix.
 * 
 * This function solves the system \( LL^T x = b \), where \( L \) is a lower triangular matrix obtained via Cholesky decomposition.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors.
 * @param LL The lower triangular matrix from the Cholesky decomposition.
 * @param x The vector to store the solution of the linear system.
 * @param b The vector representing the right-hand side of the equation.
 */
template <typename T>
void cholesky_solve(const Matrix<T>& LL, Vector<T>& x, const Vector<T>& b)
{
    x = cholesky_solve(LL, b);
}

/**
 * @brief Performs forward elimination to solve a lower triangular system \( Ly = b \).
 * 
 * This function solves the linear system where \( L \) is a lower triangular matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors.
 * @param L The square (lower triangular) matrix.
 * @param y The vector to store the intermediate solution \( y \).
 * @param b The vector representing the right-hand side of the equation.
 * 
 * @throws std::logic_error If the matrix \( L \) is not square or if the dimensions of \( b \) do not match the rows of \( L \).
 * 
 */
template <typename T>
void forward_elimination(const Matrix<T>& L, Vector<T>& y, const Vector<T> b)
{
    // Check that L is a square matrix.
    if (L.ncols() != L.nrows())
        throw std::logic_error("Error in Forward elimination: matrix must be squared (lower triangular)");
    // Check that the size of b matches the number of rows in L.
    if (b.size() != L.nrows())
        throw std::logic_error("Error in Forward elimination: b vector must be of the same dimensions of L matrix");
    register int i, j, n = b.size();
    y.resize((const unsigned int) n);

    y[0] = b[0] / L[0][0];
    // Solve Ly = b iteratively:
    for (i = 1; i < n; i++)
    {
        y[i] = b[i];
        for (j = 0; j < i; j++)
            y[i] -= L[i][j] * y[j];
        y[i] = y[i] / L[i][i];
    }
}

/**
 * @brief Performs backward elimination to solve an upper triangular system \( Ux = y \).
 * 
 * This function solves the linear system where \( U \) is an upper triangular matrix.
 * 
 * @tparam T The type of the elements stored in the matrix and vectors.
 * @param U The square (upper triangular) matrix.
 * @param x The vector to store the solution \( x \).
 * @param y The vector representing the right-hand side of the equation.
 * 
 * @throws std::logic_error If the matrix \( U \) is not square or if the dimensions of \( y \) do not match the rows of \( U \).
 * 
 */
template <typename T>
void backward_elimination(const Matrix<T>& U, Vector<T>& x, const Vector<T>& y)
{
    // Check that U is a square matrix.
    if (U.ncols() != U.nrows())
        throw std::logic_error("Error in Backward elimination: matrix must be squared (upper triangular)");
    // Check that the size of y matches the number of rows in U.
    if (y.size() != U.nrows())
        throw std::logic_error("Error in Backward elimination: b vector must be of the same dimensions of U matrix");
    register int i, j, n = y.size();
    x.resize((const unsigned int) n);

    x[n - 1] = y[n - 1] / U[n - 1][n - 1];
    // Solve Ux = y iteratively
    for (i = n - 2; i >= 0; i--)
    {
        x[i] = y[i];
        for (j = i + 1; j < n; j++)
            x[i] -= U[i][j] * x[j];
        x[i] = x[i] / U[i][i];
    }
}

/**
 * @brief Solves a linear system using the Cholesky decomposition of a matrix.
 * 
 * This function solves the system \( LL^T x = b \), where \( L \) is a lower triangular matrix obtained from Cholesky decomposition.
 * 
 * @tparam T The type of elements stored in the matrix and vectors.
 * @param LL The lower triangular matrix from the Cholesky decomposition.
 * @param b The vector representing the right-hand side of the equation.
 * @return Vector<T> The solution vector \( x \).
 * 
 * @throws std::logic_error If the matrix \( LL \) is not square or if the dimensions of \( b \) do not match the rows of \( LL \).
 * 
 * @details Steps:
 * 1. Validate that \( LL \) is a square matrix and that the size of \( b \) matches the number of rows in \( LL \).
 * 2. Solve \( L y = b \) using forward elimination to compute the intermediate vector \( y \).
 * 3. Solve \( L^T x = y \) using backward elimination to compute the final solution \( x \).
 * 4. Return the solution vector \( x \).
 */
template <typename T>
Vector<T> cholesky_solve(const Matrix<T>& LL, const Vector<T>& b)
{
    // Check that LL is a square matrix
    if (LL.ncols() != LL.nrows())
        throw std::logic_error("Error in Cholesky solve: matrix must be squared");
    unsigned int n = LL.ncols();
    // Check that the size of b matches the number of rows in LL
    if (b.size() != n)
        throw std::logic_error("Error in Cholesky decomposition: b vector must be of the same dimensions of LU matrix");
    Vector<T> x, y;

    //Solve Ly = b using forward elimination to compute the intermediate vector y.
    forward_elimination(LL, y, b);
    // Solve L^T x = y using backward elimination to compute the final solution x.
    backward_elimination(LL, x, y);

    return x;
}

#endif // define _ARRAY_HH_
