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

    setMouseTracking(true);
    centerPoint.x = -1;
    centerPoint.y = -1;
    radius = -1;

    circle = false;
    editCircle = false;
    moveCircle = false;
    countCircle = 1;

    this->ui->btEditCircle->setEnabled(false);
    this->ui->btMoveCircle->setEnabled(false);
    this->ui->btDeleteCircle->setEnabled(false);

    this->ui->btAddLine->setEnabled(false);
    this->ui->lbLine1->setEnabled(false);
    this->ui->btEditLine1->setEnabled(false);
    this->ui->btMoveLine1->setEnabled(false);
    this->ui->btDeleteLine1->setEnabled(false);
    this->ui->lbLine2->setEnabled(false);
    this->ui->btEditLine2->setEnabled(false);
    this->ui->btMoveLine2->setEnabled(false);
    this->ui->btDeleteLine2->setEnabled(false);
    this->ui->lbLine3->setEnabled(false);
    this->ui->btEditLine3->setEnabled(false);
    this->ui->btMoveLine3->setEnabled(false);
    this->ui->btDeleteLine3->setEnabled(false);
    this->ui->lbLine4->setEnabled(false);
    this->ui->btEditLine4->setEnabled(false);
    this->ui->btMoveLine4->setEnabled(false);
    this->ui->btDeleteLine4->setEnabled(false);

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

    if(!editCenterPoint) {
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

    if(circle) {
        //Edit Button
        if(editCircle) {
            if(event->type() == QEvent::MouseMove) {

                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                current = mouseEvent->posF();

                //Drawing a point
                if(countCircle == 1) {
                    centerPoint = Point2d(((double)current.x()), ((double)current.y()));
                    countCircle++;

                }else{

                    //Calculing distance (radius) between points before and current
                    double diffX = current.x() - before.x();
                    double diffY = current.y() - before.y();
                    double distance = sqrt((diffX*diffX) + (diffY*diffY));

                    if(diffX+diffY > 0)
                        radius = radius + distance;
                    else if(diffX+diffY < 0)
                        radius = abs(radius - distance);
                }

                this->calibrationCam->drawCircle(centerPoint, radius);
                before = current;
                this->ui->btMoveCircle->setEnabled(true);
                this->ui->btDeleteCircle->setEnabled(true);
            }
        }

        //MoveButton
        if(moveCircle) {
            if(event->type() == QEvent::MouseMove) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                QPointF pos = mouseEvent->posF();
                centerPoint = Point2d(((double)pos.x()), ((double)pos.y()));
                this->calibrationCam->drawCircle(centerPoint, radius);
            }
        }
    }

    return false;
}

void CalibrationGUI::on_btCircle_clicked() {
    circle = true;
    this->ui->btCircle->setEnabled(false);
    this->ui->btEditCircle->setEnabled(true);
    if(centerPoint.x != -1 && centerPoint.y != -1 && radius != -1) {
        this->ui->btDeleteCircle->setEnabled(true);
        this->ui->btMoveCircle->setEnabled(true);
    }
    this->setStatus("Pronto para editar a area de deteccao!");
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

void CalibrationGUI::on_btEditCircle_clicked() {
    editCircle = true;
    moveCircle = false;
    this->ui->btEditCircle->setEnabled(false);
    if(centerPoint.x != -1 && centerPoint.y != -1 && radius != -1) this->ui->btMoveCircle->setEnabled(true);
    this->setStatus("Clique e arraste o mouse sobre a tela para gerar a area de deteccao!");
}

void CalibrationGUI::on_btMoveCircle_clicked() {
    moveCircle = true;
    editCircle = false;
    this->ui->btEditCircle->setEnabled(true);
    this->ui->btMoveCircle->setEnabled(false);
    this->setStatus("Clique e arraste o mouse para mover o centro do circulo!");
}

void CalibrationGUI::on_btDeleteCircle_clicked() {
    this->ui->btMoveCircle->setEnabled(false);
    editCircle = false;
    moveCircle = false;
    countCircle = 1;
    centerPoint = Point2d(-1, -1);
    current = QPointF(-1, -1);
    before = current;
    radius = -1;
    this->calibrationCam->drawCircle(centerPoint, radius);
    this->ui->btEditCircle->setEnabled(true);
    this->ui->btDeleteCircle->setEnabled(false);
    this->setStatus("Pronto para editar a area de deteccao!");
}
