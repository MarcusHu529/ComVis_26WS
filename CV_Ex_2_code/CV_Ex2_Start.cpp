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
#include <algorithm>

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

    // Check if the image was loaded successfully
    if (noisy_img.empty()) {
        std::cout << "Could not find the image file." << std::endl;
        return 1;
    }

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
    cv::Mat gauss_filter_output = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_8U);
    cv::Mat gauss_filter = (cv::Mat_<float>(3, 3) << 1, 2, 1, 2, 4, 2, 1, 2, 1) / 16.0f;
    apply_filter<uint8_t, uint8_t, float>(noisy_img, gauss_filter_output, gauss_filter);

    cv::namedWindow("Gauss Filter Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Gauss Filter Image", gauss_filter_output);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;
    cv::destroyWindow("Gauss Filter Image");

    // Task 8/9
  
    // Sobel Filter for X direction
    cv::Mat sobel_x_output = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_32F);
    cv::Mat sobel_x_filter_reversed = (cv::Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1) / 8.0f;
    apply_filter<uint8_t, float, float>(noisy_img, sobel_x_output, sobel_x_filter_reversed);
    
    // Sobel Filter for Y direction
    cv::Mat sobel_y_output = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_32F);
    cv::Mat sobel_y_filter_reversed = (cv::Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1) / 8.0f;
    apply_filter<uint8_t, float, float>(noisy_img, sobel_y_output, sobel_y_filter_reversed);

    // Display the Sobel X output image
    cv::Mat sobel_x_display = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_8U);
    for (int row = 0; row < sobel_x_output.rows; row++) {
        for (int col = 0; col < sobel_x_output.cols; col++) {
            float temp = sobel_x_output.at<float>(row, col) + 128.0f;
            float clamped_value = std::max(0.0f, std::min(255.0f, temp));
            sobel_x_display.at<uint8_t>(row, col) = static_cast<uint8_t>(clamped_value); 
        }
    }
    cv::imshow("Sobel X Filter Image", sobel_x_display);

    // Display the Sobel Y output image
    cv::Mat sobel_y_display = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_8U);
    for (int row = 0; row < sobel_y_output.rows; row++) {
        for (int col = 0; col < sobel_y_output.cols; col++) {
            float temp = sobel_y_output.at<float>(row, col) + 128.0f;
            float clamped_value = std::max(0.0f, std::min(255.0f, temp));
            sobel_y_display.at<uint8_t>(row, col) = static_cast<uint8_t>(clamped_value); 
        }
    }
    cv::imshow("Sobel Y Filter Image", sobel_y_display);

    // Gradient Magnitude Calculation
    cv::Mat gradient_magnitude = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_32F);
    for (int row = 0; row < gradient_magnitude.rows; row++) {
        for (int col = 0; col < gradient_magnitude.cols; col++) {
            float x = sobel_x_output.at<float>(row, col);
            float y = sobel_y_output.at<float>(row, col);
            gradient_magnitude.at<float>(row, col) = std::sqrt(x * x + y * y);
        }
    }

    // Display the Gradient Magnitude image
    cv::Mat gradient_magnitude_display = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_8U);
    for (int row = 0; row < gradient_magnitude.rows; row++) {
        for (int col = 0; col < gradient_magnitude.cols; col++) {
            float clamped_value = std::max(0.0f, std::min(255.0f, gradient_magnitude.at<float>(row, col)));
            gradient_magnitude_display.at<uint8_t>(row, col) = static_cast<uint8_t>(clamped_value);
        }
    }
    cv::imshow("Gradient Magnitude Image", gradient_magnitude_display);

    // Sobel after Gaussian Filter
    cv::Mat sobel_x_gauss_output = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_32F);
    cv::Mat sobel_y_gauss_output = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_32F);
    apply_filter<uint8_t, float, float>(gauss_filter_output, sobel_x_gauss_output, sobel_x_filter_reversed);
    apply_filter<uint8_t, float, float>(gauss_filter_output, sobel_y_gauss_output, sobel_y_filter_reversed);

    // Magnitude Calculation for sobel after Gaussian Filter
    cv::Mat gradient_gauss_magnitude = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_32F);
    for (int row = 0; row < gradient_gauss_magnitude.rows; row++) {
        for (int col = 0; col < gradient_gauss_magnitude.cols; col++) {
            float x = sobel_x_gauss_output.at<float>(row, col);
            float y = sobel_y_gauss_output.at<float>(row, col);
            gradient_gauss_magnitude.at<float>(row, col) = std::sqrt(x * x + y * y);
        }
    }

    // Display the Gradient Magnitude after Gaussian Filter image
    cv::Mat gradient_gauss_magnitude_display = cv::Mat::zeros(noisy_img.rows, noisy_img.cols, CV_8U);
    for (int row = 0; row < gradient_gauss_magnitude.rows; row++) {
        for (int col = 0; col < gradient_gauss_magnitude.cols; col++) {
            float clamped_value = std::max(0.0f, std::min(255.0f, gradient_gauss_magnitude.at<float>(row, col)));
            gradient_gauss_magnitude_display.at<uint8_t>(row, col) = static_cast<uint8_t>(clamped_value);
        }
    }
    cv::imshow("Gradient Magnitude after Gaussian Filter Image", gradient_gauss_magnitude_display);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    return 0;

}