#include "video.h"

Video::Video(QString fileName) {
    this->fileName = fileName;
    this->countFrames = 0;

    //Open Video
    this->captureVideo.open(this->fileName.toAscii().data());

    //Verifying if video has opened
    if(!this->captureVideo.isOpened())
        open = false;
    else{
        open = true;
        this->fps = this->captureVideo.get(CAP_PROP_FPS);
        this->totalFrames = (int) this->captureVideo.get(CAP_PROP_FRAME_COUNT);
        this->height = this->captureVideo.get(CAP_PROP_FRAME_HEIGHT);
        this->width = this->captureVideo.get(CAP_PROP_FRAME_WIDTH);
    }
}

QString Video::getFileName() {
    return this->fileName;
}

VideoCapture Video::getCaptureVideo() {
    return this->captureVideo;
}

double Video::getFPS() {
    return this->fps;
}

int Video::getTotalFrames() {
    return this->totalFrames;
}

void Video::setCountFrames(int countFrames) {
    this->countFrames = countFrames;
}

int Video::getCountFrames() {
    return this->countFrames;
}

int Video::getHeight() {
    return this->height;
}

int Video::getWidth() {
    return this->width;
}

bool Video::isOpen() {
    return open;
}
