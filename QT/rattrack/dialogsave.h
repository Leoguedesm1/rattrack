#ifndef DIALOGSAVE_H
#define DIALOGSAVE_H

#include <QWidget>

namespace Ui {
class DialogSave;
}

class DialogSave : public QWidget
{
    Q_OBJECT

public:
    explicit DialogSave(QWidget *parent = 0);
    ~DialogSave();

private slots:
    void on_btOK_clicked();

    void on_btCancel_clicked();

private:
    Ui::DialogSave *ui;
};

#endif // DIALOGSAVE_H
