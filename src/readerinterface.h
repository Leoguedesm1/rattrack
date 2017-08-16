#ifndef READERINTERFACE
#define READERINTERFACE

#include <iostream>
#include <cv.h>

using namespace std;
using namespace cv;

class ReaderInterface {

public:

    virtual void readFile(const string& path) = 0;

    //Over written
    virtual void getInFile(const char* name, Mat* data) = 0;
    virtual void getInFile(const char* name, Point2d* data) = 0;
    virtual void getInFile(const char* name, double* data) = 0;

    virtual void closeFile() = 0;

    virtual bool exists(const string& path) = 0;
};

#endif // READERINTERFACE

