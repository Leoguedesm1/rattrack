#ifndef OUTCSV_H
#define OUTCSV_H

#include <iostream>
#include <cv.h>
#include <fstream>

using namespace std;
using namespace cv;

class outCSV {

public:

    void startWrite(const char* name, ofstream& file) {
        file.open(name);
    }

    template<typename T>
    void writeInFile(T data, ofstream& file) {
        file << data;
    }

    void addColumn(ofstream& file) {
        file << ", ";
    }

    void endLine(ofstream& file) {
        file << endl;
    }
};

#endif // OUTCSV_H
