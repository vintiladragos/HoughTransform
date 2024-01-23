#pragma once


#include <vector>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
/**
@brief Finds lines in a binary image using the standard Hough transform.

@param image: the image to be processed
@param lines: the vector of lines to be filled
@param threshold: the threshold for the accumulator
@param theta: the angle resolution of the accumulator in radians

*/
void houghLinesSeq(cv::Mat& image, std::vector<cv::Vec2f>& lines, int threshold, double theta);
/**
    @param image: the binary image to be processed
    @param lines: the vector of lines to be filled, in (rho, theta) representation
    @param threshold: the threshold for the accumulator (minimum number of intersections)
    @param theta: the angle step resolution in radians
    @param nrThreads: the number of threads to use
 */

void houghLinesParallel(cv::Mat& image, std::vector<cv::Vec2f>& lines, int threshold, double theta, int nrThreads);

