#ifndef VIDEO_H
#define VIDEO_H

#include "opencv2/videoio.hpp"
#include <QString>

using namespace std;
using namespace cv;

class Video {

public:
    Video(QString fileName);
    QString getFileName();
    void setCaptureVideo(VideoCapture captureVideo);
    VideoCapture getCaptureVideo();
    void setFPS(double fps);
    double getFPS();
    void setTotalFrames(int totalFrames);
    int getTotalFrames();
    void setCountFrames(int countFrames);
    int getCountFrames();
    void setHeight(int height);
    int getHeight();
    void setWidth(int width);
    int getWidth();

private:
    VideoCapture captureVideo;
    double fps;
    int totalFrames;
    int countFrames;
    QString fileName;
    int height, width;
};

#endif // VIDEO_H
