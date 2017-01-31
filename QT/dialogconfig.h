#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QWidget>

/*Area Bar*/
extern int min_area;
extern int max_area;

/*Track Bar*/
extern int rtrack;

/*Thresholding Bar*/
extern int threshold_value;

namespace Ui {
class dialogConfig;
}

class dialogConfig : public QWidget
{
    Q_OBJECT

public:
    explicit dialogConfig(QWidget *parent = 0);
    ~dialogConfig();

private slots:
    void on_btOK_clicked();

    void on_tbThresh_valueChanged(int value);

    void on_tbMinArea_valueChanged(int value);

    void on_horizontalSlider_valueChanged(int value);

    void on_tbRaio_valueChanged(int value);

    void on_btDefault_clicked();

private:
    Ui::dialogConfig *ui;
};

#endif // DIALOGCONFIG_H
