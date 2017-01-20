/********************************************************************************
** Form generated from reading UI file 'dialogconfig.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCONFIG_H
#define UI_DIALOGCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dialogConfig
{
public:
    QLabel *label;
    QSlider *tbThresh;
    QLabel *label_2;
    QLabel *label_3;
    QSlider *tbMinArea;
    QLabel *label_4;
    QSlider *tbMaxArea;
    QFrame *line;
    QFrame *line_2;
    QLabel *label_5;
    QSlider *tbRaio;
    QPushButton *btOK;
    QLabel *lbThresh;
    QLabel *lbMin;
    QLabel *lbMax;
    QLabel *lbRaio;

    void setupUi(QWidget *dialogConfig)
    {
        if (dialogConfig->objectName().isEmpty())
            dialogConfig->setObjectName(QString::fromUtf8("dialogConfig"));
        dialogConfig->resize(400, 300);
        label = new QLabel(dialogConfig);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 101, 16));
        tbThresh = new QSlider(dialogConfig);
        tbThresh->setObjectName(QString::fromUtf8("tbThresh"));
        tbThresh->setGeometry(QRect(40, 50, 281, 20));
        tbThresh->setMinimum(1);
        tbThresh->setMaximum(255);
        tbThresh->setValue(100);
        tbThresh->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(dialogConfig);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 90, 59, 14));
        label_3 = new QLabel(dialogConfig);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(31, 121, 61, 16));
        tbMinArea = new QSlider(dialogConfig);
        tbMinArea->setObjectName(QString::fromUtf8("tbMinArea"));
        tbMinArea->setGeometry(QRect(100, 121, 181, 16));
        tbMinArea->setMaximum(500);
        tbMinArea->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(dialogConfig);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(31, 150, 61, 16));
        tbMaxArea = new QSlider(dialogConfig);
        tbMaxArea->setObjectName(QString::fromUtf8("tbMaxArea"));
        tbMaxArea->setGeometry(QRect(100, 150, 181, 16));
        tbMaxArea->setMaximum(500);
        tbMaxArea->setValue(300);
        tbMaxArea->setOrientation(Qt::Horizontal);
        line = new QFrame(dialogConfig);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(20, 70, 361, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(dialogConfig);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(20, 180, 361, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_5 = new QLabel(dialogConfig);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 200, 59, 14));
        tbRaio = new QSlider(dialogConfig);
        tbRaio->setObjectName(QString::fromUtf8("tbRaio"));
        tbRaio->setGeometry(QRect(40, 230, 281, 20));
        tbRaio->setMaximum(5);
        tbRaio->setValue(1);
        tbRaio->setOrientation(Qt::Horizontal);
        btOK = new QPushButton(dialogConfig);
        btOK->setObjectName(QString::fromUtf8("btOK"));
        btOK->setGeometry(QRect(150, 260, 80, 22));
        lbThresh = new QLabel(dialogConfig);
        lbThresh->setObjectName(QString::fromUtf8("lbThresh"));
        lbThresh->setGeometry(QRect(330, 50, 41, 16));
        lbMin = new QLabel(dialogConfig);
        lbMin->setObjectName(QString::fromUtf8("lbMin"));
        lbMin->setGeometry(QRect(310, 120, 59, 14));
        lbMax = new QLabel(dialogConfig);
        lbMax->setObjectName(QString::fromUtf8("lbMax"));
        lbMax->setGeometry(QRect(310, 150, 59, 14));
        lbRaio = new QLabel(dialogConfig);
        lbRaio->setObjectName(QString::fromUtf8("lbRaio"));
        lbRaio->setGeometry(QRect(330, 230, 59, 14));

        retranslateUi(dialogConfig);

        QMetaObject::connectSlotsByName(dialogConfig);
    } // setupUi

    void retranslateUi(QWidget *dialogConfig)
    {
        dialogConfig->setWindowTitle(QApplication::translate("dialogConfig", "Configura\303\247\303\243o de an\303\241lise", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dialogConfig", "<html><head/><body><p><span style=\" font-weight:600;\">Thresholding</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("dialogConfig", "<html><head/><body><p><span style=\" font-weight:600;\">\303\201rea</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("dialogConfig", "<html><head/><body><p><span style=\" font-weight:600;\">M\303\255nimo</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("dialogConfig", "<html><head/><body><p><span style=\" font-weight:600;\">M\303\241ximo</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("dialogConfig", "<html><head/><body><p><span style=\" font-weight:600;\">Raio</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        btOK->setText(QApplication::translate("dialogConfig", "OK", 0, QApplication::UnicodeUTF8));
        lbThresh->setText(QString());
        lbMin->setText(QString());
        lbMax->setText(QString());
        lbRaio->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class dialogConfig: public Ui_dialogConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCONFIG_H
