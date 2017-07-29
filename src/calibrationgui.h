#ifndef CALIBRATIONGUI_H
#define CALIBRATIONGUI_H

#include <QWidget>
#include <cv.h>

class MainWindow;
class Calibration;

using namespace std;
using namespace cv;

namespace Ui {
class CalibrationGUI;
}

class CalibrationGUI : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrationGUI(QWidget *parent = 0);
    ~CalibrationGUI();
    static CalibrationGUI* getInstance();
    void showImage(Mat image);
    void setValueLoadBar(int value);
    void setTotalValueLoadBar(int value);
    void setProgressBar(bool status);
    void setStatus(QString status);
    void showStatus();
    void setTool(bool status);
    void setRadiusMaxValue(int value);
    void setRadiusMinValue(int value);
    int getRadiusMaxValue();
    int getRadiusMinValue();
    void setBtOKRadius(bool status);
    void errorOcurred(QString error);

private slots:
    void createCalibrationDirectory();
    void on_btCancel_clicked();
    void on_btOK_clicked();

    void on_bMaxRadius_valueChanged(int value);
    void on_bMinRadius_valueChanged(int value);

    void on_btOKRadius_clicked();

private:
    Ui::CalibrationGUI *ui;
    static CalibrationGUI* instanceGUI;
    MainWindow* mw;
    Calibration* calibrationCam;
    int radiusMax, radiusMin;
};

#endif // CALIBRATIONGUI_H
