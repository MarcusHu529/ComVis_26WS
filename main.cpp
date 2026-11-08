#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    cv::Mat image(300, 400, CV_8UC1, cv::Scalar(128));

    cv::imshow("OpenCV Test", image);
    cv::waitKey(0);

    return 0;
}