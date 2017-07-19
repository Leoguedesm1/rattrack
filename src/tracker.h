#ifndef TRACKER_H
#define TRACKER_H

#include <QTimer>
#include <QObject>
#include <fstream>

//Bibliotecas C
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <string.h>
#include <time.h>
#include <cctype>

//Bibliotecas OPENCV
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>

#include "mainwindow.h"
#include "video.h"

using namespace std;
using namespace cv;

//Values used in threshold
#define WHITE 255
#define BLACK 0

//const string HOMOGRAPHY_FILE_NAME = "/homography.yml";
const string TESTES_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Testes";
const string STATISTICS_FILE_NAME = "/statistics.csv";

class Tracker {
public:
    Tracker();
    Tracker(QString animal, QString teste, Video *captureVideo);
    void resetCaptureVideo();
private slots:

    void setAnimal(QString animal);
    void setTeste(QString teste);
    void setCaptureVideo(Video* captureVideo);

    void pauseVideo();
    void executeTracker();
    void readHomographyFile();
    void createTestDirectory();
    void startWriteStatistics();
    void processVideo();
    void getPerspectiveFrame();
    Mat getThreshold();
    void getTrack(Mat imageThreshold);
    void addCordinates(Point2d coord);
    Mat drawPath();
    void calcAndSaveStatistics(bool find);
    void endVideo();
    Mat getImageIA();

private:

    MainWindow* mw;
    QTimer* tmrTimer;

    QString animal, teste;

    Video* captureVideo;

    int countTracking;
    Mat imageTracking;

    Size warpSize;
    Mat imageTransform;
    Mat homographyWarp;
    Point2d center;
    double pixelRatio;
    double radius;

    Mat imageScreen;
    Mat srcFrame, perspectiveFrame;

    Point2d coordBBfore, coordBefore, coordCurrent;

    ofstream statisticsFile;

    vector<Point2d> coordinates;
};

#endif // TRACKER_H
