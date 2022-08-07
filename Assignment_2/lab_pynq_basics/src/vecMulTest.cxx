#include <eigen3/Eigen/Dense>

#include <cmath>

#include "vecMulTest.h"
#include "vecDot.h"
#include "vecCross.h"

bool test_dot(Eigen::VectorXf vec1, Eigen::VectorXf vec2, uint32_t length) {
	float eigen_result = vec1.dot(vec2);
	float our_result = dot_product(vec1, vec2, length);

	// Only check precision to four decimal places
	// -> Try me! What happens if we don't round? Why?
    // (it doesn't work due to rounding error in floating point operations)
	eigen_result = floor(eigen_result * pow(10,4) + .5) / pow(10,4);
	our_result = floor(our_result * pow(10,4) + .5) / pow(10,4);

	return eigen_result == our_result;
}

bool test_cross(Eigen::Vector3f a, Eigen::Vector3f b) {
    
    Eigen::Vector3f our_result = cross_product(a, b);
	Eigen::Vector3f eigen_result = a.cross(b);
    
    for(int i = 0; i < 3; i++)
    {
        // Only check precision to four decimal places
        if(floor(eigen_result[i] * pow(10,4) + .5) / pow(10,4) != floor(our_result[i] * pow(10,4) + .5) / pow(10,4))
        {
            return false;
        }
    }
    
    return true;
}