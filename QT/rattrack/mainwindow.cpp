#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

QString argv;
string animal = " ";
int teste = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->bSelectFile->setEnabled(false);
    ui->lbFIle->setEnabled(false);
    u.src = &src;
    //connect(ui->bOK, SIGNAL(clicked(bool)), this, SLOT(openControlPainel()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openControlPainel() {
    this->close();
    controlPainel = new controlpainel();
    controlPainel->show();
}

void MainWindow::on_rbCam_clicked() {
    ui->bSelectFile->setEnabled(false);
    ui->lbFIle->setText(" ");
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
        //Conectar-se a camera...
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

}

