#ifndef READERXML
#define READERXML

#include "readerinterface.h"

class ReaderXML :  public ReaderInterface {

public:
    void readFile(const string& path) {
        file = FileStorage(path, FileStorage::READ);
    }

    void getInFile(const char* name, Mat* data) {
        file[name] >> (*data);
    }

    void getInFile(const char *name, Point2d *data) {
        file[name] >> (*data);
    }

    void getInFile(const char *name, double *data) {
        file[name] >> (*data);
    }

    void getInFile(const char *name, int *data) {
        file[name] >> (*data);
    }

    void closeFile() {
        file.release();
    }

    bool exists(const string& path) {
        if(file.isOpened())
            return true;
        else {
            readFile(path);
            if(file.isOpened())
                return true;
        }
        return false;
    }

private:
    FileStorage file;
};

#endif // READERXML

