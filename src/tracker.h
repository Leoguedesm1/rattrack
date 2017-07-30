#ifndef TRACKER_H
#define TRACKER_H

#include <QTimer>
#include <QObject>
#include <QDir>

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

class MainWindow;
#include "video.h"
#include "detectorinterface.h"
#include "inxml.h"
#include "outcsv.h"

using namespace std;
using namespace cv;

const string CALIBRATION_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Calibration";
const string HOMOGRAPHY_FILE_NAME = "/homography.yml";
const string TESTES_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Testes";
const string STATISTICS_FILE_NAME = "/statistics.csv";
const string LEARNING_FILE_NAME = "/learning.csv";

class Tracker : public QObject {

    Q_OBJECT

public:

    Tracker(QObject* parent = 0);
    Tracker(QObject* parent = 0, QString animal = "", QString teste = "", Video *captureVideo = 0);
    void resetCaptureVideo();
    void pauseVideo();
    void saveSnapshot();
    Video* getCaptureVideo();
    void executeTracker();
    QTimer* getTimer();
    Mat getPerspectiveFrame();
    Point2d getCenter();
    double getRadius();
    MainWindow* getGUI();

private slots:

    void readHomographyFile();
    void createTestDirectory();
    void createSnapshotDirectory();
    void startWriteStatistics();
    void processVideo();
    void applyingHomography();
    void getTrack(Mat imageThreshold);
    void addCordinates(Point2d coord);
    Mat drawPath();
    void calcAndSaveStatistics(bool find);
    void endVideo();
    void calcLearningIndex();

private:

    detectorInterface* detector;
    MainWindow* mw;
    QTimer* tmrTimer;

    QString animal, teste;

    Video* captureVideo;

    int snapshot;

    int countTracking;
    Mat imageTracking;

    Size warpSize;
    Mat imageTransform;
    Mat homographyWarp;
    Point2d center;
    double pixelRatio;
    double radius;

    Mat srcFrame, perspectiveFrame;

    Point2d coordBBfore, coordBefore, coordCurrent;

    ofstream statisticsFile;

    vector<Point2d> coordinates;

    outCSV* writerCSV;
};

#endif // TRACKER_H
