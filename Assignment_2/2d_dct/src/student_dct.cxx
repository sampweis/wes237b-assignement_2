#include "main.h"


using namespace cv;


Mat LUT_w;
Mat LUT_h;


// Helper function
float sf(int in){
	if (in == 0)
		return 0.70710678118; // = 1 / sqrt(2)
	return 1.;
}

// Initialize LUT
void initDCT(int WIDTH, int HEIGHT)
{
    LUT_w = Mat(WIDTH, WIDTH, CV_32FC1);
    LUT_h = Mat(HEIGHT, HEIGHT, CV_32FC1);
    
    float* w_ptr  = LUT_w.ptr<float>();
    float* h_ptr  = LUT_h.ptr<float>();
    
    float scale = sqrt(2./sqrt(HEIGHT*WIDTH));
    
	for(int x = 0; x < WIDTH; x++)
    {
        for(int i = 0; i < WIDTH; i++)
        {
            w_ptr[x * WIDTH + i] = scale * sf(x)
                * cos(M_PI/((float)WIDTH)*(i+1./2.)*(float)x);
        }
    }
    
    for(int y = 0; y < HEIGHT; y++)
    {
        for(int j = 0; j < HEIGHT; j++)
        {
            h_ptr[y * WIDTH + j] = scale * sf(y)
                * cos(M_PI/((float)HEIGHT)*(j+1./2.)*(float)y);
        }
    }   
}

// simple matrix multiplication
void mat_mult(Mat a, Mat b, Mat& out) {
    const int HEIGHT = a.rows;
	const int WIDTH  = b.cols;
    
    float* out_ptr  = out.ptr<float>();
    float* a_ptr  = a.ptr<float>();
    float* b_ptr  = b.ptr<float>();
    
    // select column vector of b
    for(int b_col = 0; b_col < b.cols; b_col++)
    {
        // perform matrix-vector mult
        for(int a_row = 0; a_row < a.rows; a_row++)
        {
            //out_ptr[a_row * WIDTH + b_col] = 0;
            for(int i = 0; i < a.cols; i++)
            {
                out_ptr[a_row * WIDTH + b_col] += 
                    a_ptr[a_row * WIDTH + i] *
                    b_ptr[i * WIDTH + b_col];
            }
        }
    }
}

// performs an entire block-based multiplication
void matrix_mult_block(Mat a, Mat b, Mat out, int block_size)
{
    const int HEIGHT = a.rows;
	const int WIDTH  = b.cols;
    
    float* out_ptr  = out.ptr<float>();
    float* a_ptr  = a.ptr<float>();
    float* b_ptr  = b.ptr<float>();
    
    for(int a_row_start = 0; a_row_start < WIDTH; a_row_start+=block_size)
    {
        for(int b_col_start = 0; b_col_start < HEIGHT; b_col_start+=block_size)
        {
            for(int inner_start = 0; inner_start < WIDTH; inner_start+=block_size)
            {
                int a_row_end = (a_row_start + block_size) < WIDTH ? (a_row_start + block_size) : WIDTH;
                int b_col_end = (b_col_start + block_size) < HEIGHT ? (b_col_start + block_size) : HEIGHT;
                int inner_end = (inner_start + block_size) < WIDTH ? (inner_start + block_size) : WIDTH;
                for(int a_row = a_row_start; a_row < a_row_end; a_row++)
                {
                    for(int b_col = b_col_start; b_col < b_col_end; b_col++)
                    {
                        for(int i = inner_start; i < inner_end; i++)
                        {
                            out_ptr[a_row * WIDTH + b_col] += 
                                a_ptr[a_row * WIDTH + i] *
                                b_ptr[i * WIDTH + b_col];
                        }
                    }
                }
            }
        }
    }
    
    
}

// Naive DCT
// Baseline: O(N^4)
Mat student_dct_naive(Mat input, bool lookup)
{
	const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;

	float scale = 2./sqrt(HEIGHT*WIDTH);

	Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);

	// Note: Using pointers is faster than Mat.at<float>(x,y)
	// Try to use pointers for your LUT as well
	float* result_ptr = result.ptr<float>();
	float* input_ptr  = input.ptr<float>();
    
    float* w_ptr  = LUT_w.ptr<float>();
    float* h_ptr  = LUT_h.ptr<float>();

	for(int x = 0; x < HEIGHT; x++)
	{
		for(int y = 0; y < WIDTH; y++)
		{
			float value = 0.f;

			for(int i = 0; i < HEIGHT; i++)
			{
				for(int j = 0; j < WIDTH; j++)
				{
                    if(lookup)
                    {
                        value += input_ptr[i * WIDTH + j]
                            * w_ptr[x * WIDTH + i]
                            * h_ptr[y * WIDTH + j];
                    }
                    else
                    {
                        value += input_ptr[i * WIDTH + j]
                            * cos(M_PI/((float)HEIGHT)*(i+1./2.)*(float)x)
                            * cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)y);
                    }
				}
			}
            if(!lookup)
            {
                value = scale * sf(x) * sf(y) * value;
            }

			result_ptr[x * WIDTH + y] = value;
		}
	}

	return result;
}

// 1D separable DCT
Mat student_dct_separable(Mat input, bool lookup){
	const int HEIGHT = input.rows;
	const int WIDTH = input.cols;

	float scale = 2./sqrt(HEIGHT*WIDTH);

	// Create the result matrix of the correct datatype
	Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);
	Mat result_row = Mat(HEIGHT, WIDTH, CV_32FC1);

    float* result_row_ptr = result_row.ptr<float>();
	float* result_ptr = result.ptr<float>();
	float* input_ptr = input.ptr<float>();
    
    float* w_ptr  = LUT_w.ptr<float>();
    float* h_ptr  = LUT_h.ptr<float>();

	// Less naive implementation.
	// Perform 2 1D DCTs, one for the rows and one for the columns
	float value;
	for(int k=0; k<HEIGHT; k++) {
		for(int i=0; i<WIDTH; i++) {
			value = 0.0;
			for(int j=0; j<WIDTH; j++) {
                if(lookup)
                {   
                    value += input_ptr[k * WIDTH + j] * w_ptr[i * WIDTH + j];
                }
                else
                {   
                    value += input_ptr[k * WIDTH + j] * cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)i);
                }
			}
            if(lookup)
            {
                result_row_ptr[k * WIDTH + i] = value;
            }
            else
            {
                result_row_ptr[k * WIDTH + i] = value * sf(i);
            }
		}
	}

	// Now perform the column transformation
	for(int k=0; k<WIDTH; k++) {
		for(int i=0; i<HEIGHT; i++) {
			value = 0.0;
			for (int j=0; j<HEIGHT; j++) {
                if(lookup)
                {   
                    value += result_row_ptr[j * WIDTH + k] * h_ptr[i * WIDTH + j];
                }
                else
                {   
                    value += result_row_ptr[j * WIDTH + k] * cos(M_PI/((float)HEIGHT)*(j+1./2.)*(float)i);
                }
			}
            if(lookup)
            {
                result_ptr[i * WIDTH + k] = value;
            }
            else
            {
                result_ptr[i * WIDTH + k] = value*sf(i)*scale;
            }
		}
	}

	return result;
}

// matrix multiply DCT
Mat student_dct_mat(Mat input)
{
	// -- Works only for WIDTH == HEIGHT
	assert(input.rows == input.cols);
	
    const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;

	Mat output1 = Mat(HEIGHT, WIDTH, CV_32FC1, Scalar(0));
    Mat output2 = Mat(HEIGHT, WIDTH, CV_32FC1, Scalar(0));
    
    mat_mult(input, LUT_w.t(), output1);
    mat_mult(LUT_w, output1, output2);

	return output2;
}

// block matrix multiply DCT
Mat student_dct_block(Mat input, int block_size)
{   
	// -- Works only for WIDTH == HEIGHT
	assert(input.rows == input.cols);

    const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;

	Mat output1 = Mat(HEIGHT, WIDTH, CV_32FC1, Scalar(0));
    Mat output2 = Mat(HEIGHT, WIDTH, CV_32FC1, Scalar(0));
    
    matrix_mult_block(input, LUT_w.t(), output1, block_size);
    matrix_mult_block(LUT_w, output1, output2, block_size);

	return output2;
}
