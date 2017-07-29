#include "calibrationgui.h"
#include "ui_calibrationgui.h"
#include "mainwindow.h"
#include "calibration.h"

CalibrationGUI* CalibrationGUI::instanceGUI = 0;

CalibrationGUI::CalibrationGUI(QWidget *parent) : QWidget(parent), ui(new Ui::CalibrationGUI) {

    this->mw = MainWindow::getInstance();

    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));

    ui->setupUi(this);
    instanceGUI = this;

    //Full screen and black background
    this->showMaximized();
    ui->lbFrame->setStyleSheet("background-color: rgb(0, 0, 0);");

    //Blocking tools
    ui->frameTool->setEnabled(false);
    ui->lbCalib->hide();
    ui->progressBar->setEnabled(false);
}

CalibrationGUI::~CalibrationGUI() {
    delete ui;
}

CalibrationGUI* CalibrationGUI::getInstance() {

    if(instanceGUI == 0)
        instanceGUI = new CalibrationGUI;
    return instanceGUI;
}

void CalibrationGUI::on_btCancel_clicked() {
    this->~CalibrationGUI();
    this->mw->show();
}

void CalibrationGUI::on_btOK_clicked() {

    //Conditions to initiate calibration
    QString board_h = ui->tbHeight->text();
    QString board_w = ui->tbWidth->text();
    QString n_boards = ui->tbSquad->text();
    QString measure = ui->tbMeasure->text();

    if(board_h.isEmpty() || board_w.isEmpty() || n_boards.isEmpty() || measure.isEmpty())
         QMessageBox::critical(this, tr("Erro"), tr ("Por favor, preencha todos os campos."));
    else{

        //Creating directory
        this->createCalibrationDirectory();

        //Reading video
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Video Files (*.avi)"));

        //Setting GUI
        ui->frameInfos->setEnabled(false);

        //Creating Calibration object
        this->calibrationCam = new Calibration(fileName, board_w.toInt(), board_h.toInt(), n_boards.toInt(),
                                               measure.toFloat());

        //Start calibration
        this->calibrationCam->executeCalibration();
    }
}

void CalibrationGUI::createCalibrationDirectory() {
    QDir dir(QString::fromStdString((string) CALIBRATION_DIR_NAME));
    if(!dir.exists())
        QDir().mkdir(dir.absolutePath());
}

void CalibrationGUI::showImage(Mat image) {
    Mat showFrame;
    cvtColor(image, showFrame, CV_BGR2RGB);
    cv::resize(showFrame, showFrame, Size(704, 480));
    QImage imageShow((const uchar *) showFrame.data, showFrame.cols, showFrame.rows, showFrame.step, QImage::Format_RGB888);
    ui->lbFrame->setPixmap(QPixmap::fromImage(imageShow));
}

void CalibrationGUI::setValueLoadBar(int value) {
    ui->progressBar->setValue(value);
}

void CalibrationGUI::setTotalValueLoadBar(int value) {
    ui->progressBar->setRange(0, value);
}

void CalibrationGUI::setProgressBar(bool status) {
    ui->progressBar->setEnabled(status);
}

void CalibrationGUI::setStatus(QString status) {
    ui->lbCalib->setText(status);
    ui->lbCalib->adjustSize();
}

void CalibrationGUI::showStatus() {
    ui->lbCalib->show();
}

void CalibrationGUI::setTool(bool status) {
    ui->frameTool->setEnabled(status);
}

void CalibrationGUI::setRadiusMaxValue(int value) {
    this->radiusMax = value;
    ui->bMaxRadius->setValue(value);
    ui->lbMax->setText(QString::number(value));
}

void CalibrationGUI::setRadiusMinValue(int value) {
    this->radiusMin = value;
    ui->bMinRadius->setValue(value);
    ui->lbMin->setText(QString::number(value));
}

int CalibrationGUI::getRadiusMaxValue() {
    return this->radiusMax;
}

int CalibrationGUI::getRadiusMinValue() {
    return this->radiusMin;
}

void CalibrationGUI::on_bMaxRadius_valueChanged(int value) {
    this->setRadiusMaxValue(value);
    this->calibrationCam->calcRadius();
}

void CalibrationGUI::on_bMinRadius_valueChanged(int value) {
    this->setRadiusMinValue(value);
    this->calibrationCam->calcRadius();
}

void CalibrationGUI::on_btOKRadius_clicked() {
    //Saving Homography infos
    this->calibrationCam->writeHomographyInfos();

    //Closing Calibration
    QMessageBox::information(this, tr("Calibracao"), tr("A calibracao foi concluida!"));
    this->~CalibrationGUI();
    this->mw->show();
}

void CalibrationGUI::setBtOKRadius(bool status) {
    ui->btOKRadius->setEnabled(status);
}

void CalibrationGUI::errorOcurred(QString error) {
    QMessageBox::critical(this, tr("Calibracao"), tr(error.toAscii().data()));
    this->~CalibrationGUI();
    this->mw->show();
}
