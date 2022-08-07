// Include guard, one should be in all headers
#ifndef VECCROSS_H
#define VECCROSS_H

// Include the Eigen pacakge that provides vec classes
#include <Eigen/Core>

// Calculate the cross product between two 3 dimensional vectors
//  @param     a First vector
//  @param     b Second vectors
//  @return    Cross product of the vectors
Eigen::Vector3f cross_product(Eigen::Vector3f a, Eigen::Vector3f b);

#endif // VECCROSS_H
