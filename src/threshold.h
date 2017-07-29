#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "detectorinterface.h"
#include "tracker.h"

using namespace std;
using namespace cv;

#define WHITE 255
#define BLACK 0

class Threshold : public detectorInterface {

public:
    Mat detectRat(Mat& frame, Point2d& center, double& radius, int value) {
        Mat imageThreshold(frame.rows, frame.cols, CV_8U, Scalar(0));

        for(int i = 0; i < frame.rows; i++) {
            for(int j = 0; j < frame.cols; j++) {
                if(((j-center.x)*(j-center.x)) + ((i-center.y)*(i-center.y)) <= radius*radius) {
                    if (frame.at<uchar>(i, j) < value)
                        imageThreshold.at<uchar>(i, j) = BLACK;
                    else
                        imageThreshold.at<uchar>(i, j) = WHITE;
                }else
                    imageThreshold.at<uchar>(i, j) = BLACK;
            }
        }

        medianBlur(imageThreshold, imageThreshold, 5);
        return imageThreshold;
    }
};

#endif // THRESHOLD_H
