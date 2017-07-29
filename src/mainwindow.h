#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QFileDialog>
#include <QMainWindow>
#include "video.h"
#include <vector>
#include <algorithm>

class Tracker;
class CalibrationGUI;

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    static MainWindow* getInstance();
    ~MainWindow();
    int getValueThreshold();
    void setValueThreshold(int value);
    int getMinValueArea();
    void setMinValueArea(int value);
    int getMaxValueArea();
    void setMaxValueArea(int value);
    int getThicknessThreshold();
    void setThicknessThreshold(int value);
    bool getScreen();
    void setScreen(bool value);

    void showImage(Mat image);
    void setStatus(string status);
    void setButtonPlay(bool status);

    bool getCheckedPathButton();
    bool getCheckedIAButton();

    void closeTest();

private slots:

    void addVideo(Video *v);
    void removeVideo();
    void readDirectoryVideos();
    void readVideo(Video* v);

    void createButtonTela();
    void activePerspective();
    void activeOriginal();

    void on_tbThresh_valueChanged(int value);
    void on_tbMin_valueChanged(int value);
    void on_tbMax_valueChanged(int value);
    void on_tbTrack_valueChanged(int value);

    void on_btFile_clicked();
    void on_btDir_clicked();
    void on_btIniciar_clicked();
    void on_btReset_clicked();
    void on_btPlay_clicked();

    void setFileName();
    void resetInterface();
    void inputNameTeste();

    void on_btCamConfig_clicked();
    void on_btSair_clicked();

    void on_btSnap_clicked();

private:

    QString animal;
    QString teste;

    Ui::MainWindow* ui;
    static MainWindow* instance;
    QLineEdit *animalInput;
    QLineEdit *testeInput;
    QAction *showPerspective;
    QAction *showOriginal;
    QMenu *screenMenu;

    vector<Video*> captureVideos;
    int valueThreshold, minValueArea, maxValueArea, thicknessTrack;
    bool screen;

    CalibrationGUI* camCalibration;
    Tracker* tracker;
};

#endif // MAINWINDOW_H
