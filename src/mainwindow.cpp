#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trackergui.h"
#include "filesanddirectoriesconstants.h"
#include "settingsfileconstants.h"
#include "calibrationgui.h"

enum {NORMAL, FAST};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    //Language configuration
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));

    //Centering window
    this->centerWindow();

    //Remove Maximize button window
    this->setFixedSize(this->size());

    //Initializing objects
    this->reader = NULL;

    //Verifying Settings file
    if(!existsSettingsFile())
        this->createDefaultSettings();

    this->startInterface();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::centerWindow() {
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);
}

void MainWindow::setStatus(string status, int time) {
    string message = "Status: " + status;
    this->statusBar()->showMessage(tr(message.c_str()), time);
}

bool MainWindow::existsCalibrationFile() {
    if(this->reader != NULL) this->reader = NULL;

    this->reader = new ReaderXML();
    return this->reader->exists(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME);
}

bool MainWindow::existsSettingsFile() {
    if(this->reader != NULL) this->reader = NULL;

    this->reader = new ReaderXML();
    return this->reader->exists(SETTINGS_DIR_NAME + ADVANCED_SETTINGS_FILE_NAME);
}

void MainWindow::startInterface() {

    ui->btCamConfig->setEnabled(true);
    ui->lb1File->setEnabled(false);
    ui->lb2File->setEnabled(false);
    ui->lb2File->setText("");
    ui->cbFastMode->setEnabled(false);
    ui->btStart->setEnabled(false);
    ui->btCancel->setEnabled(false);
    ui->cbFastMode->setChecked(false);
    ui->cbFastMode->setEnabled(false);

    if(!existsCalibrationFile()) {
        ui->btFile->setEnabled(false);
        ui->btDir->setEnabled(false);
        ui->lbTest->setEnabled(false);
        ui->lbOr->setEnabled(false);
        this->setStatus("Aguardando configuracao inicial...", 0);
    }else{
        ui->btFile->setEnabled(true);
        ui->btDir->setEnabled(true);
        ui->lbTest->setEnabled(true);
        ui->lbOr->setEnabled(true);
        this->setStatus("Aguardando carregamento de arquivo/pasta...", 0);
    }
}

void MainWindow::on_btFile_clicked() {

    //Getting video
    QString argv = QFileDialog::getOpenFileName(this, tr("Escolha o video para ser analisado"), QString(), tr("Video Files (*.avi)"));

    if(argv != "") {

        //Clearing vector
        this->captureVideos.clear();

        //Reading video
        Video* cap = new Video(argv);

        if(cap->isOpen()) {

            //Adding video in the queue
            this->captureVideos.push_back(cap);

            //Updating GUI
            ui->btDir->setEnabled(false);
            ui->lb1File->setEnabled(true);
            ui->lb2File->setEnabled(true);
            ui->btStart->setEnabled(true);
            ui->btCancel->setEnabled(true);
            ui->cbFastMode->setEnabled(true);
            this->setStatus("Video carregado! Aguardando inicio de teste...", 0);

            //Setting filename
            ui->lb2File->setText(cap->getFileName().section("/", -1, -1));

        }else{
            QMessageBox::critical(this, tr("Erro"), tr("Nao foi possivel ler o arquivo " + argv.section("/", -1, -1).toAscii()));
            this->startInterface();
        }
    }
}

void MainWindow::on_btDir_clicked() {

    //Getting directory
    QString chooseDir = QFileDialog::getExistingDirectory(this, tr("Escolha o diretorio que contenha os videos"), QString(), QFileDialog::DontResolveSymlinks | QFileDialog::ReadOnly);

    if(chooseDir != "") {

        //Clearing vector
        this->captureVideos.clear();

        //Setting directory config (.avi files)
        QDir workDir(chooseDir);
        workDir.setNameFilters(QStringList()<<"*.avi");

        //Reading files
        stringstream list;
        int i = 1;
        foreach (QFileInfo file, workDir.entryInfoList()){
            if (file.isFile()) {

                //Reading Video
                Video* cap = new Video(file.absoluteFilePath());

                if(cap->isOpen()) {

                    //Adding video in queue
                    this->captureVideos.push_back(cap);

                    //Add filename in list
                    list << i++ << " - " << cap->getFileName().section("/", -1, -1).toAscii().data() << endl;

                }else
                    QMessageBox::critical(this, tr("Erro"), tr("Nao foi possivel ler o arquivo " + cap->getFileName().toAscii()));
            }
        }

        if(this->captureVideos.size() > 0) {

            //Updating GUI
            ui->btFile->setEnabled(false);
            ui->lb1File->setEnabled(true);
            ui->lb2File->setEnabled(true);
            ui->lb2File->setText(QString::fromStdString(list.str()));
            ui->btStart->setEnabled(true);
            ui->btCancel->setEnabled(true);
            ui->cbFastMode->setEnabled(true);
            this->setStatus("Videos carregados! Aguardando inicio de teste...", 0);
        }
    }
}

void MainWindow::on_btCancel_clicked() {
    this->resetInterface();
}

void MainWindow::resetInterface() {

    //Clearing vector
    this->captureVideos.clear();

    //Updating GUI
    this->startInterface();
}

void MainWindow::on_btStart_clicked() {

    //Define mode
    int mode = NORMAL;
    if(ui->cbFastMode->isChecked())
        mode = FAST;

    //Changing interface
    TrackerGUI *tg = new TrackerGUI(0, mode, this->captureVideos);
    tg->show();
    close();
}

void MainWindow::on_btCamConfig_clicked() {
    CalibrationGUI *cg = new CalibrationGUI();
    cg->show();
    close();
}

void MainWindow::createDefaultSettings() {

    //Creating directory
    this->dirCreator = new DirectoryCreator();
    this->dirCreator->create(SETTINGS_DIR_NAME);

    //Creating default settings file
    this->writer = new WriterXML();
    this->writer->startFile(SETTINGS_DIR_NAME + ADVANCED_SETTINGS_FILE_NAME);
    this->writer->write(THRESHOLD_VALUE_NAME); this->writer->write(THRESHOLD_VALUE);
    this->writer->write(MINIMUM_AREA_NAME); this->writer->write(MINIMUM_AREA);
    this->writer->write(MAXIMUM_AREA_NAME); this->writer->write(MAXIMUM_AREA);
    this->writer->write(THRESHOLD_THICKNESS_NAME); this->writer->write(THRESHOLD_THICKNESS);
    this->writer->write(SCREEN_MODE_NAME); this->writer->write(SCREEN_MODE);
    this->writer->closeFile();
}
