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

    pointLines = vector<Point2d> (4);
    pointLines.at(0) = Point2d(-1, -1);
    pointLines.at(1) = Point2d(-1, -1);
    pointLines.at(2) = Point2d(-1, -1);
    pointLines.at(3) = Point2d(-1, -1);

    quadrants = false;
    line1 = false;
    line2 = false;
    line3 = false;
    line4 = false;

    this->ui->btAddLine->setEnabled(false);
    this->ui->lbLine1->setEnabled(false);
    this->ui->btDeleteLine1->setEnabled(false);
    this->ui->lbLine2->setEnabled(false);
    this->ui->btDeleteLine2->setEnabled(false);
    this->ui->lbLine3->setEnabled(false);
    this->ui->btDeleteLine3->setEnabled(false);
    this->ui->lbLine4->setEnabled(false);
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

    bool finish = false;
    //Verifying if user adjust detection area
    if(centerPoint.x != -1 && centerPoint.y != -1 && radius != -1) {

        //Verifying if user makes quadrants configuration
        if(pointLines.size() == 0) {

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Calibracao"), tr("A configuracao dos quadrantes nao foi concluida e nao sera salva!\n"
                                                                     "Deseja continuar?"), QMessageBox::Yes|QMessageBox::No);

            if(reply == QMessageBox::Yes)
                finish = true;

        }else{
            this->calibrationCam->calculateQuads();
            this->calibrationCam->writeQuadrantInfos();
            finish = true;
        }
    }else
        QMessageBox::critical(this, tr("Calibracao"), tr("Porfavor defina a area de deteccao antes de finalizar!"));

    if(finish) {
        this->calibrationCam->writeHomographyInfos();
        QMessageBox::information(this, tr("Calibracao"), tr("A calibracao foi finalizada com sucesso! Voce voltara a tela inicial."));
        this->~CalibrationGUI();
        this->mw->unlockStart();
        this->mw->show();
    }
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


    if(quadrants) {
        if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF pos = mouseEvent->posF();

            if(line1) {
                pointLines.at(0) = this->calibrationCam->findIntersection(centerPoint, Point2d(pos.x(), pos.y()), radius);
                ui->btDeleteLine1->setEnabled(true);
            }

            if(line2) {
                pointLines.at(1) = this->calibrationCam->findIntersection(centerPoint, Point2d(pos.x(), pos.y()), radius);
                ui->btDeleteLine2->setEnabled(true);
            }

            if(line3) {
                pointLines.at(2) = this->calibrationCam->findIntersection(centerPoint, Point2d(pos.x(), pos.y()), radius);
                ui->btDeleteLine3->setEnabled(true);
            }

            if(line4) {
                pointLines.at(3) = this->calibrationCam->findIntersection(centerPoint, Point2d(pos.x(), pos.y()), radius);
                ui->btDeleteLine4->setEnabled(true);
            }

            this->calibrationCam->drawLine(pointLines, centerPoint, radius);
        }
    }

    return false;
}

void CalibrationGUI::on_btCircle_clicked() {

    //Verifying if exists any quadrant's configuration
    bool quad = false;
    for(int i = 0; i < (int)pointLines.size(); i++) {
        if(pointLines.at(i).x != -1 && pointLines.at(i).y != -1) {
            quad = true;
            break;
        }
    }

    if(quad) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Calibracao"), tr("Essa acao excluira qualquer configuracao dos quadrantes!\n"
                                                                 "Deseja continuar?"), QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::Yes) {
            circle = true;
            this->ui->btCircle->setEnabled(false);
            this->ui->btEditCircle->setEnabled(true);
            if(centerPoint.x != -1 && centerPoint.y != -1 && radius != -1) {
                this->ui->btDeleteCircle->setEnabled(true);
                this->ui->btMoveCircle->setEnabled(true);
            }
            quadrants = false;
            line1 = false;
            line2 = false;
            line3 = false;
            line4 = false;
            for(int i = 0; i < (int) pointLines.size(); i++)
                pointLines.at(i) = Point2d(-1, -1);
            this->ui->btAddLine->setEnabled(false);
            this->ui->btEditQuad->setEnabled(true);
            this->ui->btDeleteLine1->setEnabled(false);
            this->ui->btDeleteLine2->setEnabled(false);
            this->ui->btDeleteLine3->setEnabled(false);
            this->ui->btDeleteLine4->setEnabled(false);
            this->setStatus("Pronto para editar a area de deteccao!");

        }
    }else{
        circle = true;
        this->ui->btCircle->setEnabled(false);
        this->ui->btEditCircle->setEnabled(true);
        if(centerPoint.x != -1 && centerPoint.y != -1 && radius != -1) {
            this->ui->btDeleteCircle->setEnabled(true);
            this->ui->btMoveCircle->setEnabled(true);
        }
    }
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

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Calibracao"), tr("Essa acao excluira qualquer configuracao dos quadrantes!\n"
                                                             "Deseja continuar?"), QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes) {
        this->ui->btMoveCircle->setEnabled(false);
        editCircle = false;
        moveCircle = false;
        countCircle = 1;
        centerPoint = Point2d(-1, -1);
        current = QPointF(-1, -1);
        before = current;
        radius = -1;
        this->ui->btEditCircle->setEnabled(true);
        this->ui->btDeleteCircle->setEnabled(false);
        this->setStatus("Pronto para editar a area de deteccao!");
        quadrants = false;
        line1 = false;
        line2 = false;
        line3 = false;
        line4 = false;
        for(int i = 0; i < (int) pointLines.size(); i++)
            pointLines.at(i) = Point2d(-1, -1);
        this->ui->btAddLine->setEnabled(false);
        this->ui->btEditQuad->setEnabled(false);
        this->ui->btDeleteLine1->setEnabled(false);
        this->ui->btDeleteLine2->setEnabled(false);
        this->ui->btDeleteLine3->setEnabled(false);
        this->ui->btDeleteLine4->setEnabled(false);
        this->calibrationCam->drawCircle(centerPoint, radius);
    }
}

void CalibrationGUI::on_btEditQuad_clicked() {

    if(centerPoint.x == -1 && centerPoint.y == -1 && radius == -1)
        QMessageBox::critical(this, tr("Erro"), tr("Impossivel ajustar quadrantes antes de configurar a area de deteccao!"));
    else {
        circle = false;
        ui->btAddLine->setEnabled(true);
        ui->btCircle->setEnabled(true);
        ui->btEditCircle->setEnabled(false);
        ui->btMoveCircle->setEnabled(false);
        ui->btDeleteCircle->setEnabled(false);
        quadrants = true;

    }
}

void CalibrationGUI::on_btAddLine_clicked() {

    if(!line1 && pointLines.at(0).x == -1 && pointLines.at(0).y == -1) {
        line1 = true;
        line2 = false;
        line3 = false;
        line4 = false;
        ui->lbLine1->setEnabled(true);
    }else if(!line2 && pointLines.at(0).x != -1 && pointLines.at(0).y != -1 && pointLines.at(1).x == -1 && pointLines.at(1).y == -1) {
        line1 = false;
        line3 = false;
        line4 = false;
        line2 = true;
        ui->lbLine2->setEnabled(true);
    }else if(!line3 && pointLines.at(1).x != -1 && pointLines.at(1).y != -1 && pointLines.at(2).x == -1 && pointLines.at(2).y == -1) {
        line3 = true;
        line1 = false;
        line2 = false;
        line4 = false;
        ui->lbLine3->setEnabled(true);
    }else if(!line4 && pointLines.at(0).x != -1 && pointLines.at(2).y != -1 && pointLines.at(3).x == -1 && pointLines.at(3).y == -1) {
        line4 = true;
        line1 = false;
        line3 = false;
        line2 = false;
        ui->lbLine4->setEnabled(true);
    }else
        QMessageBox::critical(this, tr("Erro"), tr("Primeiro adicione a linha clicando na imagem!"));

    this->setStatus("Clique e arraste o mouse para criar e editar a linha!");
}

void CalibrationGUI::on_btDeleteLine1_clicked() {
    line1 = false;
    pointLines.at(0) = Point2d(-1, -1);
    ui->btDeleteLine1->setEnabled(false);
    ui->lbLine1->setEnabled(false);
    this->calibrationCam->drawLine(pointLines, centerPoint, radius);
}

void CalibrationGUI::on_btDeleteLine2_clicked() {
    line2 = false;
    pointLines.at(1) = Point2d(-1, -1);
    ui->btDeleteLine2->setEnabled(false);
    ui->lbLine2->setEnabled(false);
    this->calibrationCam->drawLine(pointLines, centerPoint, radius);
}

void CalibrationGUI::on_btDeleteLine3_clicked() {
    line3 = false;
    pointLines.at(2) = Point2d(-1, -1);
    ui->btDeleteLine3->setEnabled(false);
    ui->lbLine3->setEnabled(false);
    this->calibrationCam->drawLine(pointLines, centerPoint, radius);
}

void CalibrationGUI::on_btDeleteLine4_clicked() {
    line4 = false;
    pointLines.at(3) = Point2d(-1, -1);
    ui->btDeleteLine4->setEnabled(false);
    ui->lbLine4->setEnabled(false);
    this->calibrationCam->drawLine(pointLines, centerPoint, radius);
}

vector<Point2d> CalibrationGUI::getPointsQuad() {
    return this->pointLines;
}
