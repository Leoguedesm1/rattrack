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

    ui->lbFrame->installEventFilter(this);

    centerPoint.x = -1;
    centerPoint.y = -1;
    radius = -1;
    editCenterPoint = true;
    editRadius = false;

    //Full screen and black background
    this->showMaximized();
    ui->lbFrame->setStyleSheet("background-color: rgb(0, 0, 0);");

    //Blocking tools
    ui->btStart->setEnabled(false);
    ui->frameTool->setEnabled(false);
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

void CalibrationGUI::on_btStart_clicked() {

    //Conditions to initiate calibration
    QString board_h = ui->tbHeight->text();
    QString board_w = ui->tbWidth->text();
    QString measure = ui->tbMeasure->text();

    if(board_h.isEmpty() || board_w.isEmpty() || measure.isEmpty())
         QMessageBox::critical(this, tr("Erro"), tr ("Por favor, preencha todos os campos."));
    else{

        //Creating directory
        this->createCalibrationDirectory();

        //Setting GUI
        ui->frameInfos->setEnabled(false);

        //Creating Calibration object
        this->calibrationCam = new Calibration(fileName, board_w.toInt() - 1, board_h.toInt() - 1, board_w.toInt()*board_h.toInt(),
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
    //cv::resize(showFrame, showFrame, Size(704, 480));
    QImage imageShow((const uchar *) showFrame.data, showFrame.cols, showFrame.rows, showFrame.step, QImage::Format_RGB888);
    ui->lbFrame->setPixmap(QPixmap::fromImage(imageShow));
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

void CalibrationGUI::on_btFinish_clicked() {

    if(!editCenterPoint && !editRadius) {
        //Saving Homography infos
        this->calibrationCam->writeHomographyInfos();

        //Closing Calibration
        QMessageBox::information(this, tr("Calibracao"), tr("A calibracao foi concluida!"));
        this->~CalibrationGUI();
        this->mw->show();
    }else
        QMessageBox::critical(this, tr("Calibracao"), tr("Porfavor defina o circulo de deteccao antes de prosseguir!"));

}

void CalibrationGUI::errorOcurred(QString error) {
    QMessageBox::critical(this, tr("Calibracao"), tr(error.toAscii().data()));
    this->~CalibrationGUI();
    this->mw->show();
}

void CalibrationGUI::on_btLoadFile_clicked() {

    //Reading video
    fileName = QFileDialog::getOpenFileName(this, tr("Escolha o video de calibracao"), QString(), tr("Video Files (*.avi)"));

    if(!(fileName.isEmpty())) {
        ui->btStart->setEnabled(true);
        QString filename = fileName.section("/", -1, -1);
        ui->lbFile->setText(filename);
    }
}

bool CalibrationGUI::eventFilter(QObject * watched, QEvent * event) {
    if(watched != ui->lbFrame)
        return false;

    if(event->type() == QEvent::MouseButtonPress) {
        if(editCenterPoint) {
            const QMouseEvent* const me = static_cast<const QMouseEvent*>( event );
            const QPointF p = me->posF();
            centerPoint.x = ((double)p.x());
            centerPoint.y = ((double)p.y());
            editCenterPoint = false;
            editRadius = true;
            this->calibrationCam->drawCircle(centerPoint, -1);
            this->setStatus("Agora defina o raio clicando na distancia em que termina o circulo!");
        }else if(editRadius) {
            const QMouseEvent* const mev = static_cast<const QMouseEvent*>(event);
            const QPointF pf = mev->posF();
            double diffX = centerPoint.x - pf.x();
            double diffY = centerPoint.y - pf.y();
            radius = sqrt((diffX*diffX)+(diffY*diffY));
            this->calibrationCam->drawCircle(centerPoint, radius);
            this->setStatus("Podemos finalizar a calibracao!");
            editRadius = false;
        }else
            QMessageBox::critical(this, tr("Calibracao"), tr("Para editar o circulo clique em 'Editar Circulo'"));
    }

    return false;
}

void CalibrationGUI::on_btEditCircle_clicked() {
    editCenterPoint = true;
    editRadius = false;
    this->setStatus("Por favor selecione o centro!");
}

void CalibrationGUI::on_btCancel2_clicked() {
    this->~CalibrationGUI();
    this->mw->show();
}

Point2d CalibrationGUI::getCenter() {
    return this->centerPoint;
}

double CalibrationGUI::getRadius() {
    return this->radius;
}
