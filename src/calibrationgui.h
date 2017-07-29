#ifndef CALIBRATIONGUI_H
#define CALIBRATIONGUI_H

#include <QWidget>

namespace Ui {
class CalibrationGUI;
}

class CalibrationGUI : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrationGUI(QWidget *parent = 0);
    ~CalibrationGUI();

private:
    Ui::CalibrationGUI *ui;
};

#endif // CALIBRATIONGUI_H
