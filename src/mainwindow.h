#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//QT Libraries
#include <QMainWindow>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>

//Project Classes
#include "readerxml.h"
#include "video.h"
#include "directorycreator.h"
#include "writerxml.h"

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startInterface();
    vector<Video *> getVideos();
    void resetInterface();

private slots:
    void centerWindow();
    bool existsCalibrationFile();
    bool existsSettingsFile();
    void setStatus(string status, int time);
    void createDefaultSettings();

    void on_btFile_clicked();
    void on_btDir_clicked();
    void on_btCancel_clicked();
    void on_btStart_clicked();

    void on_btCamConfig_clicked();

private:
    Ui::MainWindow *ui;
    ReaderInterface *reader;
    vector<Video*> captureVideos;
    DirectoryCreator *dirCreator;
    WriterInterface *writer;
};

#endif // MAINWINDOW_H
