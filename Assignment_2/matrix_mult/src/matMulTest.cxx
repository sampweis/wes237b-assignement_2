#include <cmath>
#include <iostream>

#include "matMult.h"

bool test_my_mult() {
	Eigen::MatrixXf a(3,3);
    Eigen::MatrixXf b(3,2);
    a << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;
    b << 1, 2,
         3, 4,
         5, 6;
    
    Eigen::MatrixXf out(a.rows(), b.cols());
    mat_mult(a, b, out);
    
    
    std::cout << out << std::endl;

	return out == a*b;
}
