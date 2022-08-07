#include "matMult.h"
#include <iostream>

void mat_mult(Eigen::MatrixXf a, Eigen::MatrixXf b, Eigen::MatrixXf& out) {
	// Note: This function does not use Eigen; it simply uses the Eigen
	// vector type to keep this example simple.
    
    // select column vector of b
    for(int b_col = 0; b_col < b.cols(); b_col++)
    {
        // perform matrix-vector mult
        for(int a_row = 0; a_row < a.rows(); a_row++)
        {
            out(a_row, b_col) = 0;
            for(int i = 0; i < a.cols(); i++)
            {
                out(a_row, b_col) += a(a_row, i)*b(i, b_col);
            }
        }
    }
}