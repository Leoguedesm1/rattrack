#ifndef DIALOGFINDCIRCLE_H
#define DIALOGFINDCIRCLE_H

#include <QWidget>

namespace Ui {
class dialogFindCircle;
}

class dialogFindCircle : public QWidget
{
    Q_OBJECT

public slots:
    void encontra_circulo();

public:
    explicit dialogFindCircle(QWidget *parent = 0);
    ~dialogFindCircle();

private slots:
    void on_btCancelar_clicked();

    void on_tbRaioMin_valueChanged(int value);

    void on_tbRaioMax_valueChanged(int value);

    void on_btDefault_clicked();

    void on_btOK_clicked();

private:
    Ui::dialogFindCircle *ui;
};

#endif // DIALOGFINDCIRCLE_H
