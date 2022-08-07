// Include guard, one should be in all headers
#ifndef VECCROSS_H
#define VECCROSS_H

// Include the Eigen pacakge that provides vec classes
#include <Eigen/Core>

// Calculate the dot product between two 3 dimensional vectors
//  @param     a First vector
//  @param     b Second vectors
//  @return        Dot product of the vectors
void mat_mult(Eigen::MatrixXf a, Eigen::MatrixXf b, Eigen::MatrixXf& out);

#endif // VECCROSS_H
