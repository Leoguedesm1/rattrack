#ifndef CALIBRATIONGUI_H
#define CALIBRATIONGUI_H

//QT Libraries
#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QTableWidget>
#include <QSignalMapper>

//Project Classes
class Calibration;
#include "video.h"

namespace Ui {
class CalibrationGUI;
}

enum {ON, OFF};

class CalibrationGUI : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrationGUI(QWidget *parent = 0);
    ~CalibrationGUI();
    static CalibrationGUI* getInstance();
    void errorMessage(QString message);
    void startGUILoadFile();
    void startGUICalibration(Mat showFrame);
    void resetProgram();
    void showImage(Mat showFrame);

private slots:

    void clearWindow();
    void clearMainLayout();
    void centerWindow();

    void btFile_clicked();
    void btStart_clicked();
    void btCancel_clicked();

    void btInstructions_clicked();
    void btEditCircle_clicked();
    void btMoveCircle_clicked();
    void btDeleteCircle_clicked();
    void cbQuadrants_clicked(int value);
    void btAddLine_clicked();
    void btEditLine_clicked(int index);
    void btDeleteLine_clicked(int index);
    void removeLine(int index);
    void btSave_clicked();
    void btCancelCalibration_clicked();

    bool eventFilter(QObject * watched, QEvent * event);

private:
    Ui::CalibrationGUI *ui;
    static CalibrationGUI* instanceGUI;
    Calibration *calibrationCam;
    Video* cap;
    QVBoxLayout *mainLayout;

    //Load File GUI
    QLabel *lb2File;
    QLineEdit *leHorCorners, *leVerCorners, *leMeasure;
    QPushButton *btFile, *btStart, *btCancel;

    //Calibration GUI
    QLabel *lbFrame, *lbQuadrants;
    QPushButton *btInstructions, *btEditCircle, *btMoveCircle, *btDeleteCircle, *btSave, *btAddLine;
    QCheckBox *cbQuadrants;
    QTableWidget *tableLines;
    vector<QPushButton *> btEditLine, btDeleteLine;
    QSignalMapper *editLineMapper, *deleteLineMapper;

    int editCircle, moveCircle, quadrants;
    vector<int> lines;
    QPointF before, current;
};

#endif // CALIBRATIONGUI_H
