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

    QString dirOriginal, dir;
    string save;


    if(ui->cbTela->isChecked() != true && ui->cbOriginal->isChecked() != true && ui->cbPerspectiva->isChecked() != true) {
        QMessageBox::critical(this, tr ("Erro"), tr ("Escolha uma das opcoes."));
    }else {
        dirOriginal = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    }

    dir = dirOriginal.toUtf8().data();

    if(ui->cbTela->isChecked() == true) {
        stringstream save1;
        save1 << dir.toAscii().data() << "/Tela_" << animal << "_Teste" << teste << ".bmp";
        save = save1.str();
        imwrite(save, tela);
    }

    if(ui->cbOriginal->isChecked() == true) {
        stringstream save1;
        save1 << dir.toAscii().data() << "/Original_" << animal << "_Teste" << teste << ".bmp";
        save = save1.str();
        imwrite(save, out);
    }

    if(ui->cbPerspectiva->isChecked() == true) {
        stringstream save1;
        save1 << dir.toAscii().data() << "/Perspectiva_" << animal << "_Teste" << teste << ".bmp";
        save = save1.str();
        imwrite(save, out_original);
    }

    QMessageBox::information(this, tr ("Salvar"), tr("Arquivos salvos com sucesso!"));
    this->close();
}

void DialogSave::on_btCancel_clicked()
{
    this->close();
}
