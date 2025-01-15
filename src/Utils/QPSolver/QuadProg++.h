/** 
 * @class QuadProg++
 * @brief This class provides a C++ interface to the Quadratic Programming solver QuadProg++.
 * @details File $Id: QuadProg++.hh 232 2007-06-21 12:29:00Z digasper $ 
 
 The quadprog_solve() function implements the algorithm of Goldfarb and Idnani 
 for the solution of a (convex) Quadratic Programming problem
 by means of an active-set dual method.
	 
The problem is in the form:

min 0.5 * x G x + g0 x
s.t.
    CE^T x + ce0 = 0
    CI^T x + ci0 >= 0
	 
 The matrix and vectors dimensions are as follows:
     G: n * n
		g0: n
				
		CE: n * p
	 ce0: p
				
	  CI: n * m
   ci0: m

     x: n
 
 The function will return the cost of the solution written in the x vector or
 std::numeric_limits::infinity() if the problem is infeasible. In the latter case
 the value of the x vector is not correct.
 
 References: D. Goldfarb, A. Idnani. A numerically stable dual method for solving
             strictly convex quadratic programs. Mathematical Programming 27 (1983) pp. 1-33.

 Notes:
  1. pay attention in setting up the vectors ce0 and ci0. 
	   If the constraints of your problem are specified in the form 
	   A^T x = b and C^T x >= d, then you should set ce0 = -b and ci0 = -d.
  2. The matrices have column dimension equal to MATRIX_DIM, 
     a constant set to 20 in this file (by means of a #define macro). 
     If the matrices are bigger than 20 x 20 the limit could be
		 increased by means of a -DMATRIX_DIM=n on the compiler command line.
  3. The matrix G is modified within the function since it is used to compute
     the G = L^T L cholesky factorization for further computations inside the function. 
     If you need the original matrix G you should make a copy of it and pass the copy
     to the function.
    
 Author: Luca Di Gaspero
  			 DIEGM - University of Udine, Italy
				 luca.digaspero@uniud.it
				 http://www.diegm.uniud.it/digaspero/
 
 The author will be grateful if the researchers using this software will
 acknowledge the contribution of this function in their research papers.
 
 Copyright (c) 2007-2016 Luca Di Gaspero
 
 This software may be modified and distributed under the terms
 of the MIT license.  See the LICENSE file for details.
*/

#ifndef _QUADPROGPP
#define _QUADPROGPP

#include "Array.h"

/**
 * @brief Solves a quadratic programming problem using the Goldfarb-Idnani method.
 * This function solves the quadratic programming problem:
 * 
 * Minimize: 0.5 * x^T * G * x + g0^T * x
 * Subject to:
 * CE^T * x + ce0 = 0
 * CI^T * x + ci0 >= 0
 * 
 * @param G The symmetric positive-definite matrix for the quadratic term.
 * @param g0 The vector for the linear term in the objective function.
 * @param CE The matrix for equality constraints.
 * @param ce0 The vector for equality constraints.
 * @param CI The matrix for inequality constraints.
 * @param ci0 The vector for inequality constraints.
 * @param x The vector to store the solution of the optimization problem.
 * 
 * @return The minimum value of the quadratic objective function.
 * 
 * @throws std::logic_error If the dimensions of input matrices and vectors are inconsistent.
 * @throws std::runtime_error If the equality constraints are linearly dependent.
 */
double solve_quadprog(Matrix<double>& G, Vector<double>& g0, 
                      const Matrix<double>& CE, const Vector<double>& ce0,  
                      const Matrix<double>& CI, const Vector<double>& ci0, 
                      Vector<double>& x);

/**
 * @brief Solves a quadratic programming problem using MATLAB-style input format.
 * This function provides a wrapper for `solve_quadprog` with MATLAB-style input,
 * including lower and upper bounds on the solution.
 * 
 * @param G The symmetric positive-definite matrix for the quadratic term.
 * @param g0 The vector for the linear term in the objective function.
 * @param CE The matrix for equality constraints.
 * @param ce0 The vector for equality constraints.
 * @param CI The matrix for inequality constraints.
 * @param ci0 The vector for inequality constraints.
 * @param lb The vector for lower bounds.
 * @param ub The vector for upper bounds.
 * @param x The vector to store the solution of the optimization problem.
 * 
 * @throws std::logic_error If input dimensions are inconsistent.
 *
 * @see solve_quadprog
 */
void solve_quadprog_matlab_syntax(Matrix<double> &G, Vector<double> &g0,
                                  const Matrix<double> &CE, const Vector<double> &ce0,
                                  const Matrix<double> &CI, const Vector<double> &ci0,
                                  const Vector<double> &lb, const Vector<double> &ub,
                                  Vector<double> &x);

/**
 * @brief Prints the contents of a matrix to the console in a formatted style.
 * This function formats and prints a matrix along with its name, for debugging
 * or informational purposes.
 * 
 * @param name The name of the matrix to display as a label.
 * @param A The matrix to be printed.
 * @param n The number of rows to print. If -1, all rows are printed.
 * @param m The number of columns to print. If -1, all columns are printed.
 * 
 * @throws None
 */
void print_matrix(char *name, const Matrix<double> &A, int n = -1, int m = -1);

/**
 * @brief Prints the contents of a vector to the console in a formatted style.
 * This function formats and prints a vector along with its name, for debugging
 * or informational purposes.
 * 
 * @param name The name of the vector to display as a label.
 * @param v The vector to be printed.
 * @param n The number of elements to print. If -1, all elements are printed.
 * 
 */
void print_vector(char *name, const Vector<double> &v, int n = -1);




#endif
