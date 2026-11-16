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


using namespace cv;

template <typename Input_Mat_Datatype,typename Output_Mat_Datatype, typename Filter_Mat_Datatype> void apply_filter(cv::Mat& input_img, cv::Mat& output_img, cv::Mat filter);

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

template <typename Input_Mat_Datatype,typename Output_Mat_Datatype, typename Filter_Mat_Datatype> void apply_filter(cv::Mat& input_img, cv::Mat& output_img, cv::Mat filter){

    // Task 6

    for(int row = 0; row < input_img.rows;row++){
        for(int col = 0; col < input_img.cols;col++){
            float accumulator = 0;
            for (int filter_row = 0; filter_row < filter.rows; filter_row++) {
                for (int filter_col = 0; filter_col < filter.cols; filter_col++) {
                    int img_row = row + filter_row - filter.rows / 2;
                    int img_col = col + filter_col - filter.cols / 2;
                    if (img_row >= 0 && img_row < input_img.rows && img_col >= 0 && img_col < input_img.cols) {
                        // Accumulate the weighted sum of the neighboring pixels by the filter coefficients
                        accumulator += input_img.at<Input_Mat_Datatype>(img_row, img_col) * filter.at<Filter_Mat_Datatype>(filter_row, filter_col);
                    }
                }
            }
            output_img.at<Output_Mat_Datatype>(row,col) = accumulator;
        }   
    }

}


int main( int argc, char** argv )
{
	help();
    
    cv::Mat noisy_img = cv::imread("../images/fruits_noisy.jpg", cv::IMREAD_GRAYSCALE);
    cv::namedWindow("Noisy Fruits",cv::WINDOW_KEEPRATIO);
    cv::imshow("Noisy Fruits",noisy_img);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    // Task 7

    // Box Filter
    cv::Mat box_filter_output = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_8U);
    cv::Mat box_filter = cv::Mat::ones(3, 3, CV_32F) / 9.0f;
    apply_filter<uint8_t, uint8_t, float>(noisy_img, box_filter_output, box_filter);

    cv::namedWindow("Box Filter Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Box Filter Image", box_filter_output);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;
    cv::destroyWindow("Box Filter Image");

    // Gauss Filter
    cv::Mat gauss_filter_output = cv::Mat::zeros(noisy_img.rows,noisy_img.cols,CV_8U);
    cv::Mat gauss_filter = (cv::Mat_<float>(3, 3) <<1, 2, 1, 2, 4, 2, 1, 2, 1) / 16.0f;
    apply_filter<uint8_t, uint8_t, float>(noisy_img, gauss_filter_output, gauss_filter);

    cv::namedWindow("Gauss Filter Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Gauss Filter Image", gauss_filter_output);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;
    cv::destroyWindow("Gauss Filter Image");

    // Task 8/9

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    return 0;

}