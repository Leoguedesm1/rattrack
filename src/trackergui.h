#ifndef TRACKERGUI_H
#define TRACKERGUI_H

//QT Libraries
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QSignalMapper>
#include <QSlider>
#include <QLineEdit>

//Project Classes
class Tracker;
#include "video.h"
#include "readerxml.h"
#include "writerxml.h"

namespace Ui {
class TrackerGUI;
}

enum {CAMERA, HOMOGRAPHY};

class TrackerGUI : public QWidget
{
    Q_OBJECT

public:
    explicit TrackerGUI(QWidget *parent = 0);
    explicit TrackerGUI(QWidget *parent = 0, int mode = 0, vector<Video *> videos = vector<Video *> ());
    static TrackerGUI* getInstance();
    ~TrackerGUI();
    void restartProgram();
    int getIndexVideo();
    void setIndexVideo(int index);
    void setStatus(QString status);

    //Functions for NormalMode
    void setFilename(QString name);
    void showImage(Mat showFrame);
    Mat getImage();
    void saveSettings();

    //Functions for FastMode
    void setResults(string path);
    void blockCancelButton();
    void setProgressBar();
    void updateProgressBar(int value);
    template<typename T>
    void removeFromVector(vector<T *> *v, int index);
    int* getThreshValue();
    int* getMaxArea();
    int* getMinArea();
    int* getThreshThick();
    int* getScreenMode();

private slots:

    //Functions for NormalMode
    void btPlayPause_clicked();
    void btReset_clicked();
    void btSnapshot_clicked();
    void btAdvancedSettings_clicked();
    void btCancelTest_clicked();
    void createSettingsWindow();
    //-----Functions for settings
    void btCameraVision_clicked();
    void btHomographyVision_clicked();
    void setThreshValue(int value);
    void setMaxArea(int value);
    void setMinArea(int value);
    void setThreshThick(int value);
    void loadSettings();
    void startNormalModeSettings();

    //Functions for FastMode
    void cancelTestButtonClicked(int indexButton);
    void cancelAllButtonClicked();

private:
    Ui::TrackerGUI *ui;
    static TrackerGUI *instance;
    int mode, countVideos;
    Tracker *rattrack;
    int indexVideo;

    //GUI for FastMode
    QSignalMapper *mapper;
    vector<QVBoxLayout *> statusVerticalLayout;
    vector<QLabel *> lbTests, lbStatus, lbResults;
    vector<QProgressBar *> barProgress;
    vector<QPushButton *> btCancelTest;
    QTableWidget *tableWidget;
    QPushButton *btCancel;

    //GUI for NormalMode
    QLabel *lb2File, *lbFrame, *lb2Status;
    QPushButton *btPlayPause, *btReset, *btAdvancedSettings, *btSnapshot;
    //Settings widget
    QWidget *settingsWidget;
    QLabel *lbVisionMode, *lbThreshValue, *lbMaxArea, *lbMinArea, *lbThreshThick, *lbThreshValue1, *lbMaxArea1, *lbMinArea1, *lbThreshThick1;
    QPushButton *btCameraVision, *btHomographyVision;
    QSlider *sliderThreshValue, *sliderMaxArea, *sliderMinArea, *sliderThreshThick;

    //Advanced Settings
    int thresholdValue, minArea, maxArea, thresholdThick, screen;
    WriterInterface *writerSettings;
    ReaderInterface *readerSettings;
};

#endif // TRACKERGUI_H
