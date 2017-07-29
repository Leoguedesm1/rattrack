#include "calibrationgui.h"
#include "ui_calibrationgui.h"

CalibrationGUI::CalibrationGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrationGUI)
{
    ui->setupUi(this);
}

CalibrationGUI::~CalibrationGUI()
{
    delete ui;
}
