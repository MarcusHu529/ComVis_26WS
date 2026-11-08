// TU Dresden, Inf, CV1 Ex1, Jannik Presberger
//-----------------------------------------------------------------------------
#include "opencv2/opencv.hpp"
#include <iostream>

#define KEYCODE_ESC 27
#define KEYCODE_SPACE 32

enum Status_Code{
    STATUS_CONTINUE,
    STATUS_QUIT_APPLICATION
};

void invert_image(const cv::Mat& img);

void calculate_histogram(const cv::Mat& input_img, cv::Mat& histogram);
void display_histogram(const cv::Mat& histogram);

void calculate_histogram_equilibration(const cv::Mat& img,const cv::Mat& histogram);
void display_CDC(const cv::Mat& cdc);


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

void info()
{
	std::cout << "\nTU Dresden, Inf, CV1 Ex1" << std::endl;
    std::cout << "Press Space to continue" << std::endl;
    std::cout << "Press Esc to quit the application" << std::endl;
}

void invert_image(const cv::Mat& image){

    if(image.empty()){
        return;
    }

    // Task 2 

    cv::Mat inverted_img = cv::Mat::zeros(image.rows, image.cols, CV_8U);

    // Loop over the image and apply the color inversion point operator.


    cv::namedWindow("Inverted Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Inverted Image", inverted_img);

}

void calculate_histogram(const cv::Mat& input_img, cv::Mat& histogram){

    if(input_img.empty()){
        return;
    }

    // Task (3.1)
    // initialize the cv::Mat used to store the histogram with zeros. Make sure you use an appropriate datatype.


    // loop over the input image and calculate the histogram



    display_histogram(histogram);

}

void display_histogram(const cv::Mat& histogram){

    if(histogram.empty()){
        return;
    }

    // OpenCV provides numerous functions that can be used to draw shapes on images. 
    // You can find further details in the OpenCV documentation: https://docs.opencv.org/4.x/d6/d6e/group__imgproc__draw.html
    // Use the drawing functions provided by OpenCV to visualize the histogram.

}

void display_CDC(const cv::Mat& cdc){

    if(cdc.empty()){
        return;
    }

    // Visualize the cumulative distribution of colors by using OpenCV functions of your choosing.

}


void calculate_histogram_equilibration(const cv::Mat& image,const cv::Mat& histogram){

    if(image.empty() || histogram.empty()){
        return;
    }

    // Task (3.2)

    cv::Mat cdc;

    // Calculate the cumulative distribution of colors using the histogram. 

    display_CDC(cdc);

    // Task (3.3)
    // Apply the histogram equilibration to the input image

}




int main( int argc, char** argv )
{
	info();
 
    // Task (1)
    // Load fruits_grayscale.jpg and store it in cv::Mat image

    cv::Mat image = cv::Mat::zeros(256,256,CV_8U);

    // Task (2)
    invert_image(image);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;


    // Task (3)
    // Load fruits_low_contrast.jpg and store it in cv::Mat low_contrast_image

    cv::Mat low_contrast_image;
    cv::Mat histogram;

    calculate_histogram(low_contrast_image,histogram);


    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    calculate_histogram_equilibration(low_contrast_image,histogram);


    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    // Task (4)

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;


    return 0;

}