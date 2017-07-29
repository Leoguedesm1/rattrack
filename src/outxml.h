#ifndef OUTXML_H
#define OUTXML_H

#include <iostream>
#include <cv.h>

using namespace std;
using namespace cv;

class outXML {

public:
    FileStorage startFile(const string& path) {
        FileStorage fs(path, FileStorage::WRITE);
        return fs;
    }

    template <typename T>
    void writeInFile(const char* name, T const& data, FileStorage* file) {
        (*file) << name << data;
    }

    void closeFile(FileStorage* file) {
        (*file).release();
    }
};

#endif // OUTXML_H
