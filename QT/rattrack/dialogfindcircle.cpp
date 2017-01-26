#include "dialogfindcircle.h"
#include "ui_dialogfindcircle.h"
#include "controlpainel.h"

int rmin = 110;
int rmax = 120;

dialogFindCircle::dialogFindCircle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialogFindCircle)
{
    ui->setupUi(this);
    ui->tbRaioMax->setValue(rmax);
    ui->tbRaioMin->setValue(rmin);
    ui->lbRaioMin->setText(QString::number(rmin));
    ui->lbRaioMax->setText(QString::number(rmax));
    encontra_circulo();
}

dialogFindCircle::~dialogFindCircle()
{
    delete ui;
}

void dialogFindCircle::on_btCancelar_clicked()
{
    this->close();
}

void dialogFindCircle::encontra_circulo() {

    Mat detect, detect_gray;
    int menor = 0;

    transform_mat.copyTo(detect);
    cvtColor(detect, detect, COLOR_GRAY2BGR);
    medianBlur(detect, detect, 3);

    cvtColor(detect, detect_gray, COLOR_BGR2GRAY);

    vector<Vec3f> circles;
    HoughCircles(detect_gray, circles, HOUGH_GRADIENT, 1, 10, 100, 30, rmin, rmax);

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];

        if(i == 0)
            menor = c[2];
        else {
            if(c[2] < menor)
                menor = c[2];
        }

        circle( detect, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        circle( detect, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
    }

    cout << menor << "\n";
    if(menor != 0)
        raio = menor;

    Mat temp;
    cvtColor(detect, temp, CV_BGR2RGB);
    cv::resize(temp, temp, Size(200, 300));
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    ui->lbTela->setPixmap(QPixmap::fromImage(dest));
    ui->lbTela->resize(ui->lbTela->pixmap()->size());
}

void dialogFindCircle::on_tbRaioMin_valueChanged(int value)
{
    rmin = value;
    ui->lbRaioMin->setText(QString::number(rmin));
    encontra_circulo();
}

void dialogFindCircle::on_tbRaioMax_valueChanged(int value)
{
    rmax = value;
    ui->lbRaioMax->setText(QString::number(rmax));
    encontra_circulo();
}


void dialogFindCircle::on_btDefault_clicked()
{
    rmin = 110;
    rmax = 120;
    ui->lbRaioMin->setText(QString::number(rmin));
    ui->lbRaioMax->setText(QString::number(rmax));
    ui->lbRaioMin->setText(QString::number(rmin));
    ui->lbRaioMax->setText(QString::number(rmax));
    encontra_circulo();
}

void dialogFindCircle::on_btOK_clicked()
{
    cont = 1;
    track = Scalar(0);

    u.src->set(CAP_PROP_POS_MSEC, 380000);

    tmrTimer->start();

    this->close();
}
