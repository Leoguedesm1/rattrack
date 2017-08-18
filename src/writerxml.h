#ifndef WRITERXML
#define WRITERXML

#include "writerinterface.h"

class WriterXML : public WriterInterface {

public:
    void startFile(const string& path) {
        file = FileStorage(path.c_str(), FileStorage::WRITE);
    }

    void closeFile() {
        file.release();
    }

    void write(string data) {
        file << data;
    }

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
    FileStorage file;
};

#endif // WRITERXML

