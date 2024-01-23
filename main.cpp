#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "HoughLines.h"

int main(int argc, char** argv)
{
    // Declare variables
    cv::Mat dst, cdst;

    // const auto* filename = "test.jpg";
    const auto* filename = "road.jpg";



    cv::Mat original_src = cv::imread(filename, cv::IMREAD_COLOR);
    cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE); // Load an image


    cv::resize(src, src, cv::Size(640, 480));
    cv::resize(original_src, original_src, cv::Size(640, 480));

    if (src.empty())
    {
        std::cout << "Unable to open the image\n" << std::endl;
        return -1;
    }

    // blur gaussian
    cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

    // cv::imshow("Blur", src);
    // cv::waitKey();
    cv::Canny(src, dst, 100, 150, 3); // Apply Canny edge detector
    cv::imshow("Canny", dst);
    cv::waitKey();

    std::vector<cv::Vec2f> lines; // Vector to store lines

    // time taken for Hough Transform
    const auto start = std::chrono::high_resolution_clock::now();


    houghLinesParallel(dst, lines, 120, CV_PI / 180, 8);
    // houghLinesSeq(dst, lines, 150, CV_PI / 180);

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken for Hough Transform: " << duration.count() << "ms" << std::endl;

    for(auto&& line : lines)
    {
        // if (a++ > 10)
        //     break;
        const auto rho = line[0], theta = line[1];
        const double a = std::cos(theta);
        const double b = std::sin(theta);
        const double x0 = a * rho;
        const double y0 = b * rho;
        const auto pt1 = cv::Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        const auto pt2 = cv::Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        cv::line(original_src, pt1, pt2, cv::Scalar(0, 0, 255), 1, cv::LINE_AA); // Draw the lines
    }
    cv::imshow("Detected Lines (in red) - Standard Hough Line Transform", original_src);
    cv::waitKey();


    cv::imwrite("output.jpg", original_src);


    return 0;
}
