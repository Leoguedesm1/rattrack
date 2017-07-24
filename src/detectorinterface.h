#ifndef DETECTORINTERFACE_H
#define DETECTORINTERFACE_H

#include <iostream>
#include <cv.h>
class Tracker;

using namespace std;
using namespace cv;

class detectorInterface {

public:
    virtual Mat detectRat(Mat& frame, Point2d& center, double& radius, int value) = 0;
};

#endif // DETECTORINTERFACE_H
