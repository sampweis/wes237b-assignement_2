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
bool test_my_mult();

#endif
