#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/highgui/highgui.hpp>
#include <QString>

using namespace std;
using namespace cv;

class Video {

public:
    Video(QString fileName);
    QString getFileName();
    VideoCapture getCaptureVideo();
    double getFPS();
    int getTotalFrames();
    void setCountFrames(int countFrames);
    int getCountFrames();
    int getHeight();
    int getWidth();
    bool isOpen();

private:
    VideoCapture captureVideo;
    double fps;
    int totalFrames;
    int countFrames;
    QString fileName;
    int height, width;
    bool open;
};

#endif // VIDEO_H
