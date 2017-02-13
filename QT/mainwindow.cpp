#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controlpainel.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

QString argv;
string animal = " ";
int teste = 0;

controlpainel *controlChild;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->bSelectFile->setEnabled(false);
    ui->lbFIle->setEnabled(false);
    u.src = &src;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openControlPainel() {
    controlChild = NULL;
    controlChild = new controlpainel();
    controlChild->mainParent = this;
    all_reset();
    controlChild->show();
    this->hide();
}

void MainWindow::all_reset() {
    ui->animalName->setText("");
    ui->lbFIle->setText("Sem arquivos...");
    ui->lbFIle->setEnabled(false);
    ui->rbCam->setEnabled(true);
    ui->bSelectFile->setEnabled(false);
    ui->rbCam->setChecked(true);
    ui->spinBox->setValue(0);
}

void MainWindow::on_rbCam_clicked() {
    ui->bSelectFile->setEnabled(false);
    ui->lbFIle->setText("Sem arquivos...");
    ui->lbFIle->setEnabled(false);
}

void MainWindow::on_rbFile_clicked() {
    ui->bSelectFile->setEnabled(true);
    ui->lbFIle->setEnabled(true);
}

void MainWindow::on_bSelectFile_clicked() {
    argv = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Video Files (*.avi)"));
    ui->lbFIle->setText(argv.toAscii().data());
}

void MainWindow::on_bOK_clicked() {


   if(ui->animalName->text().isEmpty() || ui->spinBox->text().toInt() == 0) {
        QMessageBox::critical(this, tr("Erro"), tr ("Por favor, preencha todos os campos."));
        return;
    }else{
       animal = ui->animalName->text().toAscii().constData();
       teste = ui->spinBox->text().toInt();
   }

    if(ui->rbCam->isChecked()) {
        le_video_cam(src, argv.toAscii().data());
        openControlPainel();
    }
    else if(ui->rbFile->isChecked()) {
        if(argv.isEmpty()) {
            QMessageBox::critical(this, tr ("Erro"), tr ("Nao foi possivel abrir o arquivo, tente novamente."));
            return;
        }else {
            le_video_file(src, argv.toAscii().data());
            openControlPainel();
        }
    }
}

void MainWindow::le_video_file(VideoCapture& src, string argv) {
    src.open(argv);
    src.set(CAP_PROP_POS_MSEC, 380000);

    /*Verificando se o video foi aberto*/
    if( !src.isOpened() )
        throw "Error when reading steam_avi";
}

void MainWindow::le_video_cam(VideoCapture &src, string argv){
    src.open(0);
}

