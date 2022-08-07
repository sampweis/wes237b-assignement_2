#include <iostream>
#include <stdlib.h>

#include <eigen3/Eigen/Dense>
// Or possibly just
// #include <Eigen/Dense>

#include "vecDot.h"
#include "vecMulTest.h"
#include "vecCross.h"

//using namespace Eigen;

using namespace std;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::VectorXf;

int main(int argc, const char* argv[]) {
	VectorXf m = VectorXf::Random(3);
	VectorXf v(3);
	v << 1, 2, 3 ;
    
    Eigen::Vector3f a = Eigen::Vector3f::Random(3);
    Eigen::Vector3f b = Eigen::Vector3f::Random(3);

	// Test the methods
    
    if (test_cross(a, b)) {
		printf("Cross test passed\n");
	}
	else {
		printf("Cross test failed\n");
	}
    
	if (test_dot(m, v, 3)) {
		printf("Dot test passed\n");
		return 0;
	}
	else {
		printf("Dot test failed\n");
		return 1;
	}
}