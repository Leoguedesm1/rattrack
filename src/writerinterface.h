#ifndef WRITERINTERFACE
#define WRITERINTERFACE

#include <iostream>
#include <cv.h>

using namespace std;
using namespace cv;

class WriterInterface {

public:

    virtual void startFile(const string& path) = 0;
    virtual void closeFile() = 0;

    //for csv
    virtual void write(string data) = 0;

    //OverWritten (for xml)
    virtual void write(Mat data) = 0;
    virtual void write(vector<Point2f> data) = 0;
    virtual void write(Point2d data) = 0;
    virtual void write(double data) = 0;
    virtual void write(int data) = 0;
};

#endif // WRITERINTERFACE

