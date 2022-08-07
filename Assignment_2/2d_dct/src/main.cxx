#include "main.h"
#include "timer.h"
#include "cycletime.h"

#define FRAME_NUMBER 1 //set to 0 or -1 to run while loop

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
	unsigned int c_start;
	unsigned int opencv_c, student_c;

	cout << "WES237B lab 2" << endl;

	VideoCapture cap("input.raw");

	Mat frame, gray, dct_org, dct_student, diff_img;
	char key=0;
	float mse;
	int fps_cnt = 0;

	int WIDTH  = 64;
	int HEIGHT = 64;
    int BLOCK_SIZE = 40;
    
    int mode = 0;
    bool cycle_timing = false;
    unsigned int cpu_before, cpu_after, cpu_diff;

    // args: [mode] [width] [height] [cycle timing setting] [block size]
    
    // 1 argument on command line: method:
    // 0: naive with no lookup
    // 1: naive with lookup
    // 2: separable with no lookup
    // 3: separable with lookup
    // 4: matrix mult
    // 5: block matrix mult
    if(argc >= 2)
    {
        mode = atoi(argv[1]);
        
        switch(mode)
        {
            case 0:
                cout << "naive with no lookup" << endl;
                break;
            case 1:
                cout << "naive with lookup" << endl;
                break;
            case 2:
                cout << "separable with no lookup" << endl;
                break;
            case 3:
                cout << "separable with lookup" << endl;
                break;
            case 4:
                cout << "matrix mult" << endl;
                break;
            case 5:
            default:
                cout << "block matrix mult" << endl;
                break;
        }
    }
	// 2 argument on command line: WIDTH = HEIGHT = arg
	if(argc >= 3)
	{
		WIDTH = atoi(argv[2]);
		HEIGHT = WIDTH;
	}
	// 3 arguments on command line: WIDTH = arg1, HEIGHT = arg2
	if(argc >= 4)
	{
		HEIGHT = atoi(argv[3]);
	}
    // 5 arguments on command line: cycle timing
    if(argc >= 5)
	{
        int time_mode = atoi(argv[4]);
        if(time_mode)
        {
            cycle_timing = true;
            // 2 indicates the counter needs to be divided by 64 to prevent overflow
            if(time_mode == 2)
            {
                cout << "cycle count being divded by 64" << endl;
                init_counters(true, true);
            }
            else
            {
                init_counters(true, false);
            }
        }
    }
    // 5 arguments on command line: BLOCK_SIZE
	if(argc >= 6)
	{
		BLOCK_SIZE = atoi(argv[5]);
        cout << "block size: " << BLOCK_SIZE << endl;
	}
    else if (mode == 5)
    {
        cout << "block size: " << BLOCK_SIZE << endl;
    }
    
    cout << "matrix size: " << WIDTH << "x" << HEIGHT << endl;

	initDCT(WIDTH, HEIGHT);

	float avg_perf = 0.f;
	int count = 0;

#if FRAME_NUMBER <= 0
	while(key != 'q')
#else
    for(int f = 0; f < FRAME_NUMBER; f++)
#endif
	{
		cap >> frame;
		if(frame.empty()){ break; }

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		resize(gray, gray, Size(WIDTH, HEIGHT));
		gray.convertTo(gray, CV_32FC1);

		// OpenCV DCT
		dct(gray, dct_org);

		// Your DCT
		LinuxTimer t;
        
        if(cycle_timing)
        {
            cpu_before = get_cyclecount();
        }
        
        switch(mode)
        {
            case 0:
                dct_student = student_dct_naive(gray, false);
                break;
            case 1:
                dct_student = student_dct_naive(gray, true);
                break;
            case 2:
                dct_student = student_dct_separable(gray, false);
                break;
            case 3:
                dct_student = student_dct_separable(gray, true);
                break;
            case 4:
                dct_student = student_dct_mat(gray);
                break;
            case 5:
            default:
                dct_student = student_dct_block(gray, BLOCK_SIZE);
                break;
        }
        
        if(cycle_timing)
        {
            cpu_after = get_cyclecount();

            cpu_diff = cpu_after - cpu_before;
        }
        
		t.stop();
		float myTimer = t.getElapsed();
        
        if(cycle_timing)
        {
            cout << "Before: " << cpu_before << " After: " << cpu_after << endl;
            cout << "Timer: " << cpu_diff << endl;
        }

		gray.convertTo(gray, CV_8UC1);

		absdiff(dct_org, dct_student, diff_img); 

		/* calculating RMSE */
		diff_img = diff_img.mul(diff_img);
		Scalar se = sum(diff_img);
		mse = se[0]/((float)HEIGHT*WIDTH);

		count++;

		cout <<  "Execute time: "
			<< (double)myTimer/1000000000.0 << endl;
		printf("RMSE: %.4f\n", sqrt(mse));

		Mat inverse;
		idct(dct_student, inverse);
		inverse.convertTo(inverse, CV_8U);

#ifndef __arm__
		imshow("Original", gray);
		imshow("IDCT Output", inverse);
		moveWindow("IDCT Output", 500, 0);
		key = waitKey(10);
#endif
	}

	return 0;
}

