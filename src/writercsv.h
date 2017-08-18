#ifndef WRITERCSV
#define WRITERCSV

#include "writerinterface.h"
#include <fstream>

class WriterCSV : public WriterInterface {

public:
    void startFile(const string& path) {
        file.open(path.c_str());
    }

    void closeFile() {
        file.close();
    }

    void write(string data) {
        file << data;
    }

    //For xml - don't need here
    void write(Mat data) {
        file << data;
    }

    void write(vector<Point2f> data) {
        file << data;
    }

    void write(Point2d data) {
        file << data;
    }

    void write(double data) {
        file << data;
    }

    void write(int data) {
        file << data;
    }

private:
    ofstream file;
};

#endif // WRITERCSV

