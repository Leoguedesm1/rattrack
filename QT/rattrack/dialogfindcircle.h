#ifndef DIALOGFINDCIRCLE_H
#define DIALOGFINDCIRCLE_H

#include <QWidget>

namespace Ui {
class dialogFindCircle;
}

class dialogFindCircle : public QWidget
{
    Q_OBJECT

public:
    explicit dialogFindCircle(QWidget *parent = 0);
    ~dialogFindCircle();

private:
    Ui::dialogFindCircle *ui;
};

#endif // DIALOGFINDCIRCLE_H
