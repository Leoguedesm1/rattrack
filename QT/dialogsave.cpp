#include "dialogsave.h"
#include "ui_dialogsave.h"
#include "controlpainel.h"

#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QLocale>

using namespace std;

DialogSave::DialogSave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogSave)
{
    ui->setupUi(this);
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
}

DialogSave::~DialogSave()
{
    delete ui;
}

void DialogSave::on_btOK_clicked()
{

    QString dirTela = "", dirOriginal = "", dirPerspectiva = "", dirPath = "";
    string save;


    if(ui->cbTela->isChecked() != true && ui->cbOriginal->isChecked() != true && ui->cbPerspectiva->isChecked() != true && ui->cbPath->isChecked() != true) {
        QMessageBox::critical(this, tr ("Erro"), tr ("Escolha uma das opcoes."));
    }else {

        if(ui->cbTela->isChecked() == true) {
            stringstream save1;
            save1 << "Tela_" << animal << "_Teste" << teste << ".bmp";
            save = save1.str();
            dirTela = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

            if(dirTela != "") {
                dirTela = dirTela.toUtf8().data();
                imwrite(dirTela.toAscii().data(), tela);
                QMessageBox::information(this, tr ("Salvar"), tr("Arquivo salvo com sucesso!"));
            }
        }

        if(ui->cbOriginal->isChecked() == true) {
            stringstream save1;
            save1 << "Original_" << animal << "_Teste" << teste << ".bmp";
            save = save1.str();
            dirOriginal = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

            if(dirOriginal != "") {
                dirOriginal = dirOriginal.toUtf8().data();
                imwrite(dirOriginal.toAscii().data(), out);
                QMessageBox::information(this, tr ("Salvar"), tr("Arquivo salvo com sucesso!"));
            }
        }

        if(ui->cbPerspectiva->isChecked() == true) {
            stringstream save1;
            save1 << "Perspectiva_" << animal << "_Teste" << teste << ".bmp";
            save = save1.str();
            dirPerspectiva = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

            if(dirPerspectiva != "") {
                dirPerspectiva = dirPerspectiva.toUtf8().data();
                imwrite(dirPerspectiva.toAscii().data(), out);
                QMessageBox::information(this, tr ("Salvar"), tr("Arquivo salvo com sucesso!"));
            }
        }

        if(ui->cbPath->isChecked() == true) {
            stringstream save1;
            save1 << "Caminho_" << animal << "_Teste" << teste << ".bmp";
            save = save1.str();
            dirPath = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

            if(dirPath != "") {
                dirPath = dirPath.toUtf8().data();
                imwrite(dirPath.toAscii().data(), p);
                QMessageBox::information(this, tr ("Salvar"), tr("Arquivo salvo com sucesso!"));
            }
        }

        this->close();
    }
}

void DialogSave::on_btCancel_clicked()
{
    this->close();
}
