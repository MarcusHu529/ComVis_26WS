// TU Dresden, Inf, CV1 Ex1, Jannik Presberger
//-----------------------------------------------------------------------------
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <cmath>

#define KEYCODE_ESC 27
#define KEYCODE_SPACE 32

enum Status_Code{
    STATUS_CONTINUE,
    STATUS_QUIT_APPLICATION
};

void invert_image(const cv::Mat& img);

void calculate_histogram(const cv::Mat& input_img, cv::Mat& histogram);
void display_histogram(const cv::Mat& histogram, const std::string& window_name);

void calculate_histogram_equilibration(const cv::Mat& img,const cv::Mat& histogram);
void display_CDC(const cv::Mat& cdc, const std::string& window_name);


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

    // Check if the input image is empty
    if(image.empty()){
        return;
    }

    // Task 2 

    cv::Mat inverted_img = cv::Mat::zeros(image.rows, image.cols, CV_8U);

    // Loop over the image and apply the color inversion point operator.
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            uchar pixel = image.at<uchar>(row, col); // uchar: unsigned char, used to represent pixel values in grayscale images (0-255).
            uchar inverted_pixel = 255 - pixel; // Invert the pixel value by subtracting it from 255.
            inverted_img.at<uchar>(row, col) = inverted_pixel; // Assign the inverted pixel value to the corresponding position in the inverted image.
        }
    }

    cv::namedWindow("Inverted Image",cv::WINDOW_KEEPRATIO);
	cv::imshow("Inverted Image", inverted_img);

}

void calculate_histogram(const cv::Mat& input_img, cv::Mat& histogram){

    // Check if the input image is empty
    if(input_img.empty()){
        return;
    }

    // Task (3.1)
    // initialize the cv::Mat used to store the histogram with zeros. Make sure you use an appropriate datatype.
    histogram = cv::Mat::zeros(1, 256, CV_32S); // 32S: Signed 32-bit integer

    // loop over the input image and calculate the histogram
    for (int row = 0; row < input_img.rows; ++row) {
        for (int col = 0; col < input_img.cols; ++col) {
            uchar pixel = input_img.at<uchar>(row, col);
            histogram.at<int>(0, pixel)++;
        }
    }
    // double total_count;
    // cv::Scalar sum_bins = cv::sum(histogram);
    // total_count = sum_bins[0];

}

void calculate_CDC(const cv::Mat& histogram, cv::Mat& cdc) {
    if(histogram.empty()){
        return;
    }

    // Calculate the cumulative distribution of colors using the histogram. 
    cdc = cv::Mat::zeros(1, 256, CV_32S);
    cdc.at<int>(0, 0) = histogram.at<int>(0, 0);
    for (int k = 1; k < 256; ++k) {
        cdc.at<int>(0, k) =cdc.at<int>(0, k - 1) + histogram.at<int>(0, k);
    }
    
}

void display_histogram(const cv::Mat& histogram, const std::string& window_name){

    if(histogram.empty()){
        return;
    }

    // OpenCV provides numerous functions that can be used to draw shapes on images. 
    // You can find further details in the OpenCV documentation: https://docs.opencv.org/4.x/d6/d6e/group__imgproc__draw.html
    // Use the drawing functions provided by OpenCV to visualize the histogram.

    cv::Mat histogram_image = cv::Mat::zeros(400, 512, CV_8UC3);
    int max_frequency = 0;
    for (int k = 0; k < 256; ++k) {
        int frequency = histogram.at<int>(0, k);
        if (frequency > max_frequency) {
            max_frequency = frequency;
        }
    }
    
    if (max_frequency != 0) {
        for (int k = 0; k < 256; ++k){
            int frequency = histogram.at<int>(0, k);
            int bar_height = static_cast<int>(static_cast<float>(frequency) / max_frequency * histogram_image.rows);

            cv::Point top_left = cv::Point(2 * k, histogram_image.rows - bar_height);
            cv::Point bottom_right = cv::Point(2 * k + 1, histogram_image.rows - 1);

            if (bar_height != 0){
                cv::rectangle(histogram_image, top_left, bottom_right, cv::Scalar(255,255,255), cv::FILLED);
            }
        }
    }

    cv::imshow(window_name, histogram_image);

}

void display_CDC(const cv::Mat& cdc, const std::string& window_name){

    if(cdc.empty()){
        return;
    }

    // Visualize the cumulative distribution of colors by using OpenCV functions of your choosing.
    cv::Mat cdc_image = cv::Mat::zeros(400, 512, CV_8UC3);
    int max_cdc = cdc.at<int>(0,255);
    if (max_cdc != 0) {
        for (int k = 1; k < 256; ++k) {
            int bar_height = static_cast<int>(static_cast<float>(cdc.at<int>(0, k))/ max_cdc * cdc_image.rows);
            int previous_bar_height = static_cast<int>(static_cast<float>(cdc.at<int>(0, k - 1))/ max_cdc * cdc_image.rows);

            cv::Point previous_point = cv::Point(2 * k - 2, cdc_image.rows - previous_bar_height);
            cv::Point current_point = cv::Point(2 * k, cdc_image.rows - bar_height);

            cv::line(cdc_image, previous_point, current_point, cv::Scalar(255,255,255), 1);
        }
    }
    cv::imshow(window_name, cdc_image);
}


void calculate_histogram_equilibration(const cv::Mat& image,const cv::Mat& histogram){

    if(image.empty() || histogram.empty()){
        return;
    }

    // Task (3.2)

    cv::Mat cdc;

    // Calculate the cumulative distribution of colors using the histogram. 
    calculate_CDC(histogram, cdc);
    

    display_CDC(cdc, "Original CDC");

    // Task (3.3)
    // Apply the histogram equilibration to the input image
    cv::Mat equalized_image = cv::Mat::zeros(image.rows, image.cols, CV_8U);
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            uchar original_intensity = image.at<uchar>(row, col);
            float new_intensity = 255 * static_cast<float>(cdc.at<int>(0, original_intensity)) / cdc.at<int>(0, 255);
            uchar rounded_new_intensity = static_cast<uchar>(std::round(new_intensity));
            equalized_image.at<uchar>(row, col) = rounded_new_intensity;
        }
    }
    cv::Mat manual_equalized_histogram;
    cv::Mat manual_equalized_cdc;

    calculate_histogram( equalized_image, manual_equalized_histogram);
    calculate_CDC(manual_equalized_histogram, manual_equalized_cdc);

    display_histogram(manual_equalized_histogram, "Manual Equalized Histogram");
    display_CDC(manual_equalized_cdc, "Manual Equalized CDC");

    cv::imshow("Equalized Image", equalized_image);
}


int main( int argc, char** argv )
{
	info();
 
    // Task (1)
    // Load fruits_grayscale.jpg and store it in cv::Mat image

    // cv::imread(filename, flags)
    // filename: The name of the file to be loaded.
    // flags: Flags specifying the color type of a loaded image:
    // Return value: A matrix containing the loaded image.
    cv::Mat image = cv::imread("./images/fruits_grayscale.jpg", cv::IMREAD_GRAYSCALE);
    
    // Check if the image was loaded successfully
    if (image.empty()) {
        std::cout << "Could not find the image file." << std::endl;
        return 1;
    }

    // cv::imshow(windowName, image)
    // windowName: A string identifying the window.
    // image: The cv::Mat object containing the image to be displayed.
    cv::imshow("Original Image", image);

    // Task (2)
    invert_image(image);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;


    // Task (3)
    // Load fruits_low_contrast.jpg and store it in cv::Mat low_contrast_image

    cv::Mat low_contrast_image = cv::imread("./images/fruits_low_contrast.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat histogram;

    // Check if the image was loaded successfully
    if (low_contrast_image.empty()) {
        std::cout << "Could not find the image file." << std::endl;
        return 1;
    }

    cv::imshow("Original Low Contrast Image", low_contrast_image);

    calculate_histogram(low_contrast_image,histogram);
    display_histogram(histogram, "Original Histogram");

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    calculate_histogram_equilibration(low_contrast_image,histogram);


    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;

    // Task (4)
    cv::Mat opencv_equalized_image;
    cv::equalizeHist(low_contrast_image, opencv_equalized_image);
    cv::imshow("OpenCV Equalized Image", opencv_equalized_image);

    if(wait_for_and_process_key_input() == STATUS_QUIT_APPLICATION) return 0;


    return 0;

}