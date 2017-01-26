#include "dialogfindcircle.h"
#include "ui_dialogfindcircle.h"

dialogFindCircle::dialogFindCircle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialogFindCircle)
{
    ui->setupUi(this);
}

dialogFindCircle::~dialogFindCircle()
{
    delete ui;
}
