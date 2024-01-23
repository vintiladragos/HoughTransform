#include "HoughLines.h"
// multi-threaded hough lines
#include <thread>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>

std::mutex accumulatorMutex;


void houghLinesParallel(cv::Mat& image, std::vector<cv::Vec2f>& lines, int threshold, double theta, int nrThreads) {

    cv::Mat edges = image.clone();

    const double maxRho = sqrt(pow(image.rows, 2) + pow(image.cols, 2));


    cv::Mat accumulator = cv::Mat::zeros(static_cast<int>(maxRho * 2), static_cast<int>(CV_PI / theta), CV_32SC1);

    auto processSection = [&](int startY, int endY) {
        for (int y = startY; y < endY; y++) {
            for (int x = 0; x < edges.cols; x++) {
                if (edges.at<uchar>(y, x) > 0) {
                    for (int tIdx = 0; tIdx < accumulator.cols; tIdx++) {
                        // Calculate rho for each theta
                        double t = tIdx * theta;
                        int rho = static_cast<int>(x * cos(t) + y * sin(t));

                        // Increment the accumulator
                        CV_XADD(&accumulator.at<int>(rho + maxRho, tIdx), 1);

                    }
                }
            }
        }
    };

    std::vector<std::thread> threads;
    int rowsPerThread = edges.rows / nrThreads;
    int startY = 0;
    for (int i = 0; i < nrThreads - 1; i++) {
        int endY = startY + rowsPerThread;
        threads.emplace_back(processSection, startY, endY);
        startY = endY;
    }

    threads.emplace_back(processSection, startY, edges.rows);


    for (auto& thread : threads) {
        thread.join();
    }



    for (int r = 0; r < accumulator.rows; r++) {
        for (int t = 0; t < accumulator.cols; t++) {
            if (accumulator.at<int>(r, t) > threshold) {
                // Convert back to (rho, theta) representation
                double rho = r - maxRho;
                double angle = t * theta;

                lines.push_back(cv::Vec2f(static_cast<float>(rho), static_cast<float>(angle)));
            }
        }
    }
}


#include "HoughLines.h"
// multi-threaded hough lines

void houghLinesSeq(cv::Mat& image, std::vector<cv::Vec2f>& lines, int threshold, double theta) {



    const double maxRho = sqrt(pow(image.rows, 2) + pow(image.cols, 2));


    cv::Mat accumulator = cv::Mat::zeros(static_cast<int>(maxRho * 2), static_cast<int>(CV_PI / theta), CV_32SC1);

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            if (image.at<uchar>(y, x) > 0) {
                for (int tIdx = 0; tIdx < accumulator.cols; tIdx++) {
                    // Calculate rho for each theta
                    double t = tIdx * theta;
                    int rho = static_cast<int>(x * cos(t) + y * sin(t));

                    // Increment the accumulator
                    accumulator.at<int>(rho + maxRho, tIdx) += 1;
                }
            }
        }
    }

    for (int r = 0; r < accumulator.rows; r++) {
        for (int t = 0; t < accumulator.cols; t++) {
            if (accumulator.at<int>(r, t) > threshold) {
                // Convert back to (rho, theta) representation
                double rho = r - maxRho;
                double angle = t * theta;

                lines.push_back(cv::Vec2f(static_cast<float>(rho), static_cast<float>(angle)));
            }
        }
    }
}