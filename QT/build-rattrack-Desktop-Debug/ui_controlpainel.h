/********************************************************************************
** Form generated from reading UI file 'controlpainel.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPAINEL_H
#define UI_CONTROLPAINEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_controlpainel
{
public:
    QLabel *lbTela;
    QPushButton *btPlay;
    QPushButton *btReset;
    QPushButton *btSair;
    QPushButton *btConfig;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QFrame *line_5;
    QFrame *line_6;
    QLabel *label;
    QLabel *label_2;
    QLabel *lbFile;
    QLabel *lbNome;
    QLabel *label_3;
    QLabel *lbTeste;
    QPushButton *btSave;
    QFrame *line_7;

    void setupUi(QWidget *controlpainel)
    {
        if (controlpainel->objectName().isEmpty())
            controlpainel->setObjectName(QString::fromUtf8("controlpainel"));
        controlpainel->resize(688, 454);
        lbTela = new QLabel(controlpainel);
        lbTela->setObjectName(QString::fromUtf8("lbTela"));
        lbTela->setGeometry(QRect(90, 110, 491, 381));
        btPlay = new QPushButton(controlpainel);
        btPlay->setObjectName(QString::fromUtf8("btPlay"));
        btPlay->setGeometry(QRect(20, 10, 40, 40));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btPlay->sizePolicy().hasHeightForWidth());
        btPlay->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        btPlay->setIcon(icon);
        btReset = new QPushButton(controlpainel);
        btReset->setObjectName(QString::fromUtf8("btReset"));
        btReset->setGeometry(QRect(60, 10, 40, 40));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        btReset->setIcon(icon1);
        btSair = new QPushButton(controlpainel);
        btSair->setObjectName(QString::fromUtf8("btSair"));
        btSair->setGeometry(QRect(180, 10, 40, 40));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/sair.png"), QSize(), QIcon::Normal, QIcon::Off);
        btSair->setIcon(icon2);
        btConfig = new QPushButton(controlpainel);
        btConfig->setObjectName(QString::fromUtf8("btConfig"));
        btConfig->setGeometry(QRect(100, 10, 40, 40));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/config.png"), QSize(), QIcon::Normal, QIcon::Off);
        btConfig->setIcon(icon3);
        line = new QFrame(controlpainel);
        line->setObjectName(QString::fromUtf8("line"));
        line->setWindowModality(Qt::WindowModal);
        line->setGeometry(QRect(20, 50, 1250, 20));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy1);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(controlpainel);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(60, 10, 3, 40));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(controlpainel);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(100, 10, 3, 40));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(controlpainel);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(140, 10, 3, 40));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_5 = new QFrame(controlpainel);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(180, 10, 3, 40));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);
        line_6 = new QFrame(controlpainel);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setGeometry(QRect(20, 10, 3, 40));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);
        label = new QLabel(controlpainel);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(250, 10, 59, 16));
        label_2 = new QLabel(controlpainel);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(365, 30, 52, 16));
        lbFile = new QLabel(controlpainel);
        lbFile->setObjectName(QString::fromUtf8("lbFile"));
        lbFile->setGeometry(QRect(320, 10, 691, 16));
        lbNome = new QLabel(controlpainel);
        lbNome->setObjectName(QString::fromUtf8("lbNome"));
        lbNome->setGeometry(QRect(423, 30, 281, 16));
        label_3 = new QLabel(controlpainel);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(251, 30, 43, 16));
        lbTeste = new QLabel(controlpainel);
        lbTeste->setObjectName(QString::fromUtf8("lbTeste"));
        lbTeste->setGeometry(QRect(300, 30, 59, 16));
        btSave = new QPushButton(controlpainel);
        btSave->setObjectName(QString::fromUtf8("btSave"));
        btSave->setGeometry(QRect(140, 10, 40, 40));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        btSave->setIcon(icon4);
        line_7 = new QFrame(controlpainel);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        line_7->setGeometry(QRect(220, 10, 3, 40));
        line_7->setFrameShape(QFrame::VLine);
        line_7->setFrameShadow(QFrame::Sunken);

        retranslateUi(controlpainel);

        QMetaObject::connectSlotsByName(controlpainel);
    } // setupUi

    void retranslateUi(QWidget *controlpainel)
    {
        controlpainel->setWindowTitle(QApplication::translate("controlpainel", "Rattrack", 0, QApplication::UnicodeUTF8));
        lbTela->setText(QString());
        btPlay->setText(QString());
        btReset->setText(QString());
        btSair->setText(QString());
        btConfig->setText(QString());
        label->setText(QApplication::translate("controlpainel", "<html><head/><body><p><span style=\" font-weight:600;\">Arquivo:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("controlpainel", "<html><head/><body><p><span style=\" font-weight:600;\">Animal:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lbFile->setText(QString());
        lbNome->setText(QString());
        label_3->setText(QApplication::translate("controlpainel", "<html><head/><body><p><span style=\" font-weight:600;\">Teste:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lbTeste->setText(QString());
        btSave->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class controlpainel: public Ui_controlpainel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPAINEL_H
