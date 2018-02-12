#ifndef TRACKER_H
#define TRACKER_H

//QT Libraries
#include <QTimer>

//OpenCV Libraries
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"

//Project Classes
class TrackerGUI;
#include "video.h"
#include "threshold.h"
#include "writercsv.h"
#include "readerxml.h"
#include "directorycreator.h"

using namespace std;
using namespace cv;

enum testMode {NORMAL, FAST};
enum quadrantsMode {ON, OFF};

class Tracker : public QObject {

    Q_OBJECT

public:
    Tracker(QObject* parent = 0);
    Tracker(QObject* parent = 0, vector<Video *> cap = vector<Video *> (), int mode = 0);
    void setTimer(bool status);
    QTimer* getTimer();
    void cancelAnalysis();
    void restartAnalysis();
    void saveSnapshot();
    void removeFromQueue(int index);

private slots:
    void startAnalysis();
    void finishAnalysis();
    void readHomographyFile();
    void readQuadrantsFile();
    void createStatisticsFile();
    void proccessVideo();
    Mat applyHomography(Mat srcFrame);
    void getTrack(Mat imageThreshold, Mat homographyFrame, Mat srcFrame);
    Mat drawPath();
    void calcLearningIndex();
    void calcAndSaveStatistics(bool find);
    int calcQuadrant();
    double calcAngle();
    bool isInside(double superiorAngle, double inferiorAngle, double angle);

private:
    TrackerGUI *interface;
    testMode mode;
    vector <Video *> videos;
    Video * cap;
    QTimer *timer;
    quadrantsMode quadrants;
    ReaderInterface *readerHomography, *readerQuadrants;
    DetectorInterface *detector;
    WriterInterface *writerStatistics, *writerLearningIndex;
    DirectoryCreator *dirCreator;
    int countTracking, countSnapshot;
    Mat imageTracking, homographyMatrix;
    Size warpSize;
    Point2d center, coordBBfore, coordBefore, coordCurrent;
    double pixelRatio, radius;
    vector<double> quadrantAngles;
    vector<Point2d> coordinates;
    int *thresholdValue, *thresholdThick, *minArea, *maxArea, *screen;
};

#endif // TRACKER_H
