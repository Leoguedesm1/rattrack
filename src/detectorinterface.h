#ifndef DETECTORINTERFACE_H
#define DETECTORINTERFACE_H

#include <iostream>
#include <cv.h>

using namespace std;
using namespace cv;

class DetectorInterface {

public:
    virtual Mat detectRat(Mat& frame, Point2d& center, double& radius, int value) = 0;
};

#endif // DETECTORINTERFACE_H
