#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <limits>

#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include "opencv/cv.h"

using namespace cv;
using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void openControlPainel();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_rbCam_clicked();
    void on_rbFile_clicked();
    void on_bSelectFile_clicked();
    void on_bOK_clicked();
    void le_video_file(VideoCapture& src, string argv);
    void le_video_cam(VideoCapture& src, string argv);

private:
    Ui::MainWindow *ui;
   };

#endif // MAINWINDOW_H

