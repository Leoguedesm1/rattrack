/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QRadioButton *rbCam;
    QRadioButton *rbFile;
    QPushButton *bSelectFile;
    QPushButton *bOK;
    QLabel *label;
    QFrame *line;
    QLabel *label_4;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpinBox *spinBox;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *animalName;
    QLabel *lbFIle;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        rbCam = new QRadioButton(centralWidget);
        rbCam->setObjectName(QString::fromUtf8("rbCam"));
        rbCam->setGeometry(QRect(89, 152, 81, 20));
        rbCam->setChecked(true);
        rbFile = new QRadioButton(centralWidget);
        rbFile->setObjectName(QString::fromUtf8("rbFile"));
        rbFile->setGeometry(QRect(90, 180, 81, 20));
        bSelectFile = new QPushButton(centralWidget);
        bSelectFile->setObjectName(QString::fromUtf8("bSelectFile"));
        bSelectFile->setGeometry(QRect(182, 181, 80, 22));
        bOK = new QPushButton(centralWidget);
        bOK->setObjectName(QString::fromUtf8("bOK"));
        bOK->setGeometry(QRect(150, 260, 80, 22));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 351, 16));
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(20, 20, 371, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(31, 151, 51, 16));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 2, 2));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(30, 100, 93, 25));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        spinBox = new QSpinBox(layoutWidget1);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        horizontalLayout_3->addWidget(spinBox);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(30, 50, 179, 24));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        animalName = new QLineEdit(layoutWidget2);
        animalName->setObjectName(QString::fromUtf8("animalName"));

        horizontalLayout->addWidget(animalName);

        lbFIle = new QLabel(centralWidget);
        lbFIle->setObjectName(QString::fromUtf8("lbFIle"));
        lbFIle->setGeometry(QRect(30, 210, 331, 41));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbFIle->sizePolicy().hasHeightForWidth());
        lbFIle->setSizePolicy(sizePolicy);
        lbFIle->setTextFormat(Qt::AutoText);
        lbFIle->setWordWrap(true);
        MainWindow->setCentralWidget(centralWidget);
        rbCam->raise();
        rbFile->raise();
        bSelectFile->raise();
        bOK->raise();
        label->raise();
        line->raise();
        label_4->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        lbFIle->raise();
        lbFIle->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        rbCam->setText(QApplication::translate("MainWindow", "C\303\242mera", 0, QApplication::UnicodeUTF8));
        rbFile->setText(QApplication::translate("MainWindow", "Arquivo", 0, QApplication::UnicodeUTF8));
        bSelectFile->setText(QApplication::translate("MainWindow", "Selecione", 0, QApplication::UnicodeUTF8));
        bOK->setText(QApplication::translate("MainWindow", "Iniciar", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Configura\303\247\303\265es</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Escolha:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Teste:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Animal:", 0, QApplication::UnicodeUTF8));
        lbFIle->setText(QApplication::translate("MainWindow", "Sem arquivos...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
