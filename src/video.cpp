#include "video.h"

Video::Video(QString fileName) {
    this->fileName = fileName;
    this->countFrames = 0;
}

QString Video::getFileName() {
    return this->fileName;
}

void Video::setCaptureVideo(VideoCapture captureVideo) {
    this->captureVideo = captureVideo;
}

VideoCapture Video::getCaptureVideo() {
    return this->captureVideo;
}

void Video::setFPS(double fps) {
    this->fps = fps;
}

double Video::getFPS() {
    return this->fps;
}

void Video::setTotalFrames(int totalFrames) {
    this->totalFrames = totalFrames;
}

int Video::getTotalFrames() {
    return this->totalFrames;
}

void Video::setHeight(int height) {
    this->height = height;
}

int Video::getHeight() {
    return this->height;
}

void Video::setWidth(int width) {
    this->width = width;
}

int Video::getWidth() {
    return this->width;
}
