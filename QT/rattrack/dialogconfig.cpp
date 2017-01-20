#include "dialogconfig.h"
#include "ui_dialogconfig.h"

/*Area Bar*/
int min_area = 0;
int max_area = 300;

/*Track Bar*/
int rtrack = 1;

/*Thresholding Bar*/
int threshold_value = 100;

dialogConfig::dialogConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialogConfig)
{
    ui->setupUi(this);
    ui->tbThresh->setValue(threshold_value);
    ui->tbMinArea->setValue(min_area);
    ui->tbMaxArea->setValue(max_area);
    ui->tbRaio->setValue(rtrack);
    ui->lbThresh->setText(QString::number(threshold_value));
    ui->lbMin->setText(QString::number(min_area));
    ui->lbMax->setText(QString::number(max_area));
    ui->lbRaio->setText(QString::number(rtrack));
}

dialogConfig::~dialogConfig()
{
    delete ui;
}

void dialogConfig::on_btOK_clicked()
{
    this->close();
}

void dialogConfig::on_tbThresh_valueChanged(int value)
{
    threshold_value = value;
    ui->lbThresh->setText(QString::number(threshold_value));
}

void dialogConfig::on_tbMinArea_valueChanged(int value)
{
    min_area = value;
    ui->lbMin->setText(QString::number(min_area));
}

void dialogConfig::on_horizontalSlider_valueChanged(int value)
{
    max_area = value;
    ui->lbMax->setText(QString::number(max_area));
}

void dialogConfig::on_tbRaio_valueChanged(int value)
{
    rtrack = value;
    ui->lbRaio->setText(QString::number(rtrack));
}
