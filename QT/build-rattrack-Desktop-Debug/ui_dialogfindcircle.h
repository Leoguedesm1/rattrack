/********************************************************************************
** Form generated from reading UI file 'dialogfindcircle.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGFINDCIRCLE_H
#define UI_DIALOGFINDCIRCLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dialogFindCircle
{
public:
    QLabel *label;
    QSlider *tbRaioMin;
    QLabel *label_2;
    QSlider *tbRaioMax;
    QPushButton *btDefault;
    QLabel *lbTela;
    QLabel *lbRaioMin;
    QLabel *lbRaioMax;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btOK;
    QPushButton *btCancelar;

    void setupUi(QWidget *dialogFindCircle)
    {
        if (dialogFindCircle->objectName().isEmpty())
            dialogFindCircle->setObjectName(QString::fromUtf8("dialogFindCircle"));
        dialogFindCircle->resize(555, 383);
        label = new QLabel(dialogFindCircle);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 91, 16));
        tbRaioMin = new QSlider(dialogFindCircle);
        tbRaioMin->setObjectName(QString::fromUtf8("tbRaioMin"));
        tbRaioMin->setGeometry(QRect(30, 50, 221, 16));
        tbRaioMin->setMinimum(1);
        tbRaioMin->setMaximum(200);
        tbRaioMin->setValue(110);
        tbRaioMin->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(dialogFindCircle);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 90, 91, 16));
        tbRaioMax = new QSlider(dialogFindCircle);
        tbRaioMax->setObjectName(QString::fromUtf8("tbRaioMax"));
        tbRaioMax->setGeometry(QRect(30, 120, 221, 16));
        tbRaioMax->setMinimum(1);
        tbRaioMax->setMaximum(200);
        tbRaioMax->setValue(120);
        tbRaioMax->setOrientation(Qt::Horizontal);
        btDefault = new QPushButton(dialogFindCircle);
        btDefault->setObjectName(QString::fromUtf8("btDefault"));
        btDefault->setGeometry(QRect(130, 170, 80, 22));
        lbTela = new QLabel(dialogFindCircle);
        lbTela->setObjectName(QString::fromUtf8("lbTela"));
        lbTela->setGeometry(QRect(320, 40, 201, 301));
        lbRaioMin = new QLabel(dialogFindCircle);
        lbRaioMin->setObjectName(QString::fromUtf8("lbRaioMin"));
        lbRaioMin->setGeometry(QRect(260, 50, 59, 14));
        lbRaioMax = new QLabel(dialogFindCircle);
        lbRaioMax->setObjectName(QString::fromUtf8("lbRaioMax"));
        lbRaioMax->setGeometry(QRect(260, 120, 59, 14));
        widget = new QWidget(dialogFindCircle);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(90, 250, 168, 24));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btOK = new QPushButton(widget);
        btOK->setObjectName(QString::fromUtf8("btOK"));

        horizontalLayout->addWidget(btOK);

        btCancelar = new QPushButton(widget);
        btCancelar->setObjectName(QString::fromUtf8("btCancelar"));

        horizontalLayout->addWidget(btCancelar);


        retranslateUi(dialogFindCircle);

        QMetaObject::connectSlotsByName(dialogFindCircle);
    } // setupUi

    void retranslateUi(QWidget *dialogFindCircle)
    {
        dialogFindCircle->setWindowTitle(QApplication::translate("dialogFindCircle", "Configura\303\247\303\243o de Calibra\303\247\303\243o", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dialogFindCircle", "<html><head/><body><p><span style=\" font-weight:600;\">Raio m\303\255nimo</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("dialogFindCircle", "<html><head/><body><p><span style=\" font-weight:600;\">Raio m\303\241ximo</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        btDefault->setText(QApplication::translate("dialogFindCircle", "Padr\303\243o", 0, QApplication::UnicodeUTF8));
        lbTela->setText(QString());
        lbRaioMin->setText(QString());
        lbRaioMax->setText(QString());
        btOK->setText(QApplication::translate("dialogFindCircle", "OK", 0, QApplication::UnicodeUTF8));
        btCancelar->setText(QApplication::translate("dialogFindCircle", "Cancelar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dialogFindCircle: public Ui_dialogFindCircle {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGFINDCIRCLE_H
