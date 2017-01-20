#include "dialogsave.h"
#include "ui_dialogsave.h"
#include "controlpainel.h"

#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

using namespace std;

DialogSave::DialogSave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogSave)
{
    ui->setupUi(this);
}

DialogSave::~DialogSave()
{
    delete ui;
}

void DialogSave::on_btOK_clicked()
{
    //UTF-8 ASCII Table
    const char tA[] = {192, 193, 194, 195}; //A
    const char ta[] = {225,226,227,228}; //á,â,ã,à
    const char tE[] = {201, 202}; //E
    const char te[] = {233,234}; //é,ê
    const char tI[] = {205};
    const char ti[] = {237}; //í
    const char tO[] = {211, 212, 213};
    const char to[] = {243,244,245}; //ó,ô,õ
    const char tU[] = {218, 220};
    const char tu[] = {250,252}; //ú,ü
    const char tC[] = {199};
    const char tc[] = {231}; //ç

    QString dirOriginal;
    string save;

    if(ui->cbTela->isChecked() != true && ui->cbOriginal->isChecked() != true && ui->cbPerspectiva->isChecked() != true) {
        QMessageBox::critical(this, tr ("Erro"), tr ("Escolha uma das opcoes."));
    }else {
        dirOriginal = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    }

    QString dir = dirOriginal.replace(" ", "\\ ");

    for (unsigned int j = 0; j < sizeof(tA); j++) dir.replace(tA[j],"A");
    for (unsigned int j = 0; j < sizeof(tE); j++) dir.replace(tE[j],"E");
    for (unsigned int j = 0; j < sizeof(tI); j++) dir.replace(tI[j],"I");
    for (unsigned int j = 0; j < sizeof(tO); j++) dir.replace(tO[j],"O");
    for (unsigned int j = 0; j < sizeof(tU); j++) dir.replace(tU[j],"U");
    for (unsigned int j = 0; j < sizeof(tC); j++) dir.replace(tC[j],"C");
    for (unsigned int j = 0; j < sizeof(ta); j++) dir.replace(tA[j],"a");
    for (unsigned int j = 0; j < sizeof(te); j++) dir.replace(tE[j],"e");
    for (unsigned int j = 0; j < sizeof(ti); j++) dir.replace(tI[j],"i");
    for (unsigned int j = 0; j < sizeof(to); j++) dir.replace(tO[j],"o");
    for (unsigned int j = 0; j < sizeof(tu); j++) dir.replace(tU[j],"u");
    for (unsigned int j = 0; j < sizeof(tc); j++) dir.replace(tC[j],"c");


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

    cout << save << "\n";
    exit(1);
    this->close();
}

void DialogSave::on_btCancel_clicked()
{
    this->close();
}
