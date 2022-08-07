#include "vecCross.h"
#include <iostream>

Eigen::Vector3f cross_product(Eigen::Vector3f a, Eigen::Vector3f b) {
	// Note: This function does not use Eigen; it simply uses the Eigen
	// vector type to keep this example simple. A STL vector would work the
	// same for the implementation approach.

    // dot product is:
    // [a[2]b[3] - a[3]b[2], -(a[1]b[3] -a[3]b[1]), a[1]b[2] - a[2]b[1]]
	Eigen::Vector3f v(3);
	v << (a[1]*b[2] - a[2]*b[1]), -(a[0]*b[2] -a[2]*b[0]), (a[0]*b[1] - a[1]*b[0]) ;
    return v;
}