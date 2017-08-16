#ifndef CALIBRATIONGUI_H
#define CALIBRATIONGUI_H

#include <QWidget>
#include <cv.h>
#include <QMouseEvent>

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
    void setBtOKRadius(bool status);
    void errorOcurred(QString error);
    bool eventFilter(QObject * watched, QEvent * event);
    Point2d getCenter();
    double getRadius();

private slots:
    void createCalibrationDirectory();
    void on_btCancel_clicked();
    void on_btStart_clicked();

    void on_btFinish_clicked();
    void on_btLoadFile_clicked();
    void on_btEditCircle_clicked();

    void on_btCancel2_clicked();

private:
    Ui::CalibrationGUI *ui;
    static CalibrationGUI* instanceGUI;
    MainWindow* mw;
    Calibration* calibrationCam;
    double radius;
    QString fileName;
    Point2d centerPoint;
    bool editCenterPoint, editRadius;
};

#endif // CALIBRATIONGUI_H
