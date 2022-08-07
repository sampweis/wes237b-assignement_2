#ifndef VECMULTEST_H
#define VECMULTEST_H

#include <Eigen/Core>
/**
 * Test our custom dot product function against armadillo's built in function
 * @param xin First vector
 * @param yin Second vector
 * @param length Length of the vectors
 * @return True/False if both methods do/do not produce equal outputs
 **/
bool test_dot(Eigen::VectorXf vec1, Eigen::VectorXf vec2, uint32_t length);

/**
 * Test our custom cross product function against armadillo's built in function
 * @param a First vector
 * @param b Second vector
 * @return True/False if both methods do/do not produce equal outputs
 **/
bool test_cross(Eigen::Vector3f a, Eigen::Vector3f b);

#endif
