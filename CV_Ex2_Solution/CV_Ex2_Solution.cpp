// TU Dresden, Inf, CV1 Ex2, Jannik Presberger
//-----------------------------------------------------------------------------
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <functional>
#include <queue>

#define KEYCODE_ESC 27
#define KEYCODE_SPACE 32

enum Status_Code{
    STATUS_CONTINUE,
    STATUS_QUIT_APPLICATION
};

bool print_time_measurement = false;

using namespace cv;

template <typename Input_Mat_Datatype,typename Output_Mat_Datatype, typename Filter_Mat_Datatype> void apply_filter(cv::Mat& input_mat, cv::Mat& output_img, cv::Mat filter);
void combine_sobel_x_y_dir(cv::Mat& sobel_x, cv::Mat& sobel_y);

Status_Code wait_for_and_process_key_input(){

    while(true){
        int keycode = cv::waitKey(0) & 0xEFFFFF;

        switch (keycode)
        {
        case KEYCODE_ESC:
            return STATUS_QUIT_APPLICATION;
        
        case KEYCODE_SPACE:
            return STATUS_CONTINUE;

        default:
            continue;
        }
    }

}

void help()
{
	std::cout << "\nTU Dresden, Inf, CV1 Ex2, Jannik Presberger" << std::endl;
    std::cout << "Press Space to continue" << std::endl;
    std::cout << "Press Esc to quit the application" << std::endl;
}

void calculate_and_display_sobel_using_opencv(cv::Mat& input_img){

    cv::Mat grad;
    Mat grad_x, grad_y;
    cv::Sobel(input_img, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    cv::Sobel(input_img, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);

    Mat abs_grad_x, abs_grad_y;
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    cv::imshow("Sobel Result", grad);

}

void calculate_and_display_sobel(cv::Mat& input_img){

    cv::Mat sobel_x_output = cv::Mat::zeros(input_img.rows,input_img.cols,CV_16S);
    cv::Mat sobel_x_filter = cv::Mat::zeros(3,3,CV_8S);

    sobel_x_filter.at<int8_t>(0,0) = -1;
    sobel_x_filter.at<int8_t>(1,0) = -2;
    sobel_x_filter.at<int8_t>(2,0) = -1;

    sobel_x_filter.at<int8_t>(0,2) = 1;
    sobel_x_filter.at<int8_t>(1,2) = 2;
    sobel_x_filter.at<int8_t>(2,2) = 1;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    apply_filter<uint8_t,int16_t,int8_t>(input_img,sobel_x_output,sobel_x_filter);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    if(print_time_measurement){
        std::cout << "sobel x filter took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " [µs] to calculate" << std::endl;
    }

    cv::namedWindow("Sobel Filter X Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Sobel Filter X Image", sobel_x_output);

    cv::Mat sobel_y_output = cv::Mat::zeros(input_img.rows,input_img.cols,CV_16S);
    cv::Mat sobel_y_filter = cv::Mat::zeros(3,3,CV_8S);

    sobel_y_filter.at<int8_t>(0,0) = -1;
    sobel_y_filter.at<int8_t>(0,1) = -2;
    sobel_y_filter.at<int8_t>(0,2) = -1;

    sobel_y_filter.at<int8_t>(2,0) = 1;
    sobel_y_filter.at<int8_t>(2,1) = 2;
    sobel_y_filter.at<int8_t>(2,2) = 1;

    apply_filter<uint8_t,int16_t,int8_t>(input_img,sobel_y_output,sobel_y_filter);

    cv::namedWindow("Sobel Filter Y Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Sobel Filter Y Image", sobel_y_output);

    cv::Mat sobel_x_as_uint8t;
    sobel_x_output.convertTo(sobel_x_as_uint8t,CV_8U,1.0f/256.0f,(1 << 7));

    cv::Mat sobel_y_as_uint8t;
    sobel_y_output.convertTo(sobel_y_as_uint8t,CV_8U,1.0f/256.0f,(1 << 7));

    combine_sobel_x_y_dir(sobel_x_output,sobel_y_output);

}

template <typename Input_Mat_Datatype,typename Output_Mat_Datatype, typename Filter_Mat_Datatype> void apply_filter(cv::Mat& input_mat, cv::Mat& output_img, cv::Mat filter){

    int filter_col_offset = floorf(((float)filter.cols - 1.0f) / (float)2.0f);
    int filter_row_offset = floorf(((float)filter.rows - 1.0f) / (float)2.0f);

    float rescale_factor = (float)(1 << (8*output_img.elemSize1())) / (float)(1<<(8*input_mat.elemSize1()));

    for(int row = 0; row < input_mat.rows;row++){
        for(int col = 0; col < input_mat.cols;col++){

            int new_pixel_value = 0;
            float filter_sum = 0;

            for(int filter_row = 0; filter_row < filter.rows;filter_row++){
                for(int filter_col = 0; filter_col < filter.cols;filter_col++){

                    int filter_val = filter.at<Filter_Mat_Datatype>(filter_row,filter_col);

                    int final_row = row + filter_row - filter_row_offset;
                    int final_col = col + filter_col - filter_col_offset;

                    if(final_row < 0){
                        final_row = abs(final_row) - 1;
                    }

                    if(final_row >= input_mat.rows){
                        final_row = (input_mat.rows - 1) - (final_row - input_mat.rows);
                    }

                    if(final_col < 0){
                        final_col = abs(final_col) - 1;
                    }

                    if(final_col >= input_mat.cols){
                        final_col = (input_mat.cols - 1) - (final_col - input_mat.cols);
                    }


                    if(final_row >= 0 && final_row < input_mat.rows && final_col >= 0 && final_col < input_mat.cols){

                        filter_sum += fabs(filter_val);
                        new_pixel_value += filter_val * input_mat.at<Input_Mat_Datatype>(final_row,final_col);
                    }
                
                }   
            }

            output_img.at<Output_Mat_Datatype>(row,col) = new_pixel_value / filter_sum * rescale_factor;
        
        }   
    }

}



void combine_sobel_x_y_dir(cv::Mat& sobel_x, cv::Mat& sobel_y){

    cv::Mat grad_img = cv::Mat::zeros(sobel_x.rows,sobel_x.cols,CV_16U);

    cv::Mat grad_abs_img = cv::Mat::zeros(sobel_x.rows,sobel_x.cols,CV_16U);

    for(int row = 0; row < sobel_x.rows; row++){
        for(int col = 0; col < sobel_x.cols; col++){
            int sobel_val_x = sobel_x.at<int16_t>(row,col);
            int sobel_val_y = sobel_y.at<int16_t>(row,col);

            int grad = sqrt(sobel_val_x * sobel_val_x + sobel_val_y * sobel_val_y);

            grad_img.at<uint16_t>(row,col) = grad;

            grad_abs_img.at<uint16_t>(row,col) = (abs(sobel_val_x)) + (abs(sobel_val_y)); 
        }
    }

    cv::namedWindow("Grad",cv::WINDOW_KEEPRATIO);
	cv::imshow("Grad", grad_img);
}



int main( int argc, char** argv )
{
	help();
    

    cv::Mat noisy_img = cv::imread("../images/fruits_noisy.jpg", cv::IMREAD_GRAYSCALE);

    cv::namedWindow("Noisy Fruits",cv::WINDOW_KEEPRATIO);
    cv::imshow("Noisy Fruits",noisy_img);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    cv::Mat box_filter_output = cv::Mat::zeros(noisy_img.rows,noisy_img.cols,CV_8U);
    cv::Mat box_filter = cv::Mat::ones(3,3,CV_8S);
    apply_filter<uint8_t,uint8_t,int8_t>(noisy_img,box_filter_output,box_filter);

    cv::namedWindow("Box Filter Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Box Filter Image", box_filter_output);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    cv::destroyWindow("Box Filter Image");

    cv::Mat gauss_filter_output = cv::Mat::zeros(noisy_img.rows,noisy_img.cols,CV_8U);
    cv::Mat gauss_filter = cv::Mat::ones(3,3,CV_8U);

    gauss_filter.at<uint8_t>(0,1) = 2;
    gauss_filter.at<uint8_t>(1,0) = 2;
    gauss_filter.at<uint8_t>(1,2) = 2;
    gauss_filter.at<uint8_t>(2,1) = 2;
    gauss_filter.at<uint8_t>(1,1) = 4;

    apply_filter<uint8_t,uint8_t,uint8_t>(noisy_img,gauss_filter_output,gauss_filter);

    cv::namedWindow("Gauss Filter Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Gauss Filter Image", gauss_filter_output);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    cv::destroyWindow("Gauss Filter Image");

    calculate_and_display_sobel(noisy_img);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    print_time_measurement = true;

    calculate_and_display_sobel(box_filter_output);
	
    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    return 0;

}