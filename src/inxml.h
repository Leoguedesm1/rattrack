#ifndef INXML_H
#define INXML_H

#include <iostream>
#include <cv.h>

using namespace std;
using namespace cv;

class inXML {
public:
    FileStorage readFile(const string& path) {
        FileStorage fs(path, FileStorage::READ);
        return fs;
    }

    template<typename T>
    void getInFile(const char* name, FileStorage* file, T* data) {
        (*file)[name] >> (*data);
    }

    /*void getInFile(const char *name, FileStorage *file, Point2d *data) {
        (*data) = (*file)[name];
    }*/

    void closeFile(FileStorage* file) {
        (*file).release();
    }
};

#endif // INXML_H
