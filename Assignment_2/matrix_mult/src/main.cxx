#include <iostream>
#include <stdlib.h>

#include <Eigen/Core>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/eigen.hpp>

#include "matMult.h"
#include "cycletime.h"

using namespace std;

// controls if program should print to cout
int print = 1;

bool matCompare(const Eigen::MatrixXf& a, const Eigen::MatrixXf& b)
{
    if(a.rows() != b.rows()) return false;
    if(a.cols() != b.cols()) return false;
    for(int x = 0; x < a.rows(); x++)
    {
        for(int y = 0; y < a.cols(); y++)
        {
            if(floor(b(x,y) * 10 + .5) / 10 !=
               floor(a(x,y) * 10 + .5) / 10)
            {
                if(print)
                {
                    cout << (floor(b(x,y) * 10 + .5) / 10) <<  " != " << (floor(a(x,y) * 10 + .5) / 10) << endl;
                }
                return false;
            }
        }
    }
    return true;
}

int main(int argc, const char* argv[]) {
    
    Eigen::MatrixXf a;
    Eigen::MatrixXf b;
    
    // args: [mode] [print results] [width/height] [cycle counting]
    
    // modes:
    // 0: my mult
    // 1: eigen
    // 2: open cv
    // 3: test
    int mode = 0;
    
    if(argc >= 3)
    {
        // arg 1, multiplication method
        mode = atoi(argv[1]);
        
        // arg 2, print results on/off
        print = atoi(argv[2]);
    }
    
    if(argc < 4)
    {
        a = Eigen::MatrixXf(3,3);
        b = Eigen::MatrixXf(3,3);

        a << 1, 2, 3,
             4, 5, 6,
             7, 8, 9;
        b << 10, 11, 12,
             13, 14, 15,
             16, 17, 18;
    }
    else
    {
        int WIDTH  = 64;
        int HEIGHT = 64;
        
        // 3rd argument on command line: WIDTH = HEIGHT = arg
        if(argc >= 4)
        {
            WIDTH = atoi(argv[3]);
            HEIGHT = WIDTH;
        }
        
        a = Eigen::MatrixXf::Random(WIDTH, HEIGHT);
        b = Eigen::MatrixXf::Random(WIDTH, HEIGHT);
    }
    
    int time = 0;
    unsigned int cpu_before, cpu_after, cpu_diff;
    
    if(argc >= 5)
    {
        // arg 4, cycle counting on/off
        time = atoi(argv[4]);
    }
    
    
    cv::Mat_<float> _a;
    cv::Mat_<float> _b;
    
    cv::eigen2cv(a, _a);
    cv::eigen2cv(b, _b);
    
    if(time)
    {
        init_counters(true, false);
        cpu_before = get_cyclecount();
    }
    
	switch(mode)
    {
    case 0:
        {
            // my mult
            Eigen::MatrixXf out(a.rows(), b.cols());
            mat_mult(a, b, out);
            if(print)
            {
                cout << out << endl;
            }
            break;
        }
    case 1:
        {  
            // eigen mult
            Eigen::MatrixXf eigen_out = a*b;
            if(print)
            {
                cout << eigen_out << endl;
            }
            break;
        }
    case 2:
        {  
            // opencv mult
            cv::Mat_<float>cv_out = _a*_b;
            if(print)
            {
                cout << cv_out << endl;
            }
            break;
        }
    case 3:
    default:
        {
            // test
            Eigen::MatrixXf out(a.rows(), b.cols());
            mat_mult(a, b, out);
            
            // compare to eigen
            Eigen::MatrixXf eigen_out = a*b;
            if(!matCompare(out, eigen_out))
            {
                if(print)
                {
                    cout << "my mult != eigen" << endl;
                }
                return 0;
            }
            
            // compare to opencv
            cv::Mat_<float>cv_out = _a*_b;
            Eigen::MatrixXf cv_out_eigen;
            cv2eigen(cv_out, cv_out_eigen);
            if(!matCompare(out, cv_out_eigen))
            {
                if(print)
                {
                    cout << "my mult != cv" << endl;
                }
                return 0;
            }
            
            if(print)
            {
                cout << "tests passed!" << endl;
            }
            break;
        }
    }
    
    if(time)
    {
        cpu_after = get_cyclecount();

        cpu_diff = cpu_after - cpu_before;

        cout << "Before: " << cpu_before << " After: " << cpu_after << endl;
        cout << "Timer: " << cpu_diff << endl;
    }
    
    return 1;
}