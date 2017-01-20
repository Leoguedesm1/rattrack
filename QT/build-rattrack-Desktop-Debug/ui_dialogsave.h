/********************************************************************************
** Form generated from reading UI file 'dialogsave.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSAVE_H
#define UI_DIALOGSAVE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogSave
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QCheckBox *cbTela;
    QCheckBox *cbOriginal;
    QCheckBox *cbPerspectiva;
    QHBoxLayout *horizontalLayout;
    QPushButton *btOK;
    QPushButton *btCancel;

    void setupUi(QWidget *DialogSave)
    {
        if (DialogSave->objectName().isEmpty())
            DialogSave->setObjectName(QString::fromUtf8("DialogSave"));
        DialogSave->resize(283, 194);
        widget = new QWidget(DialogSave);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(50, 40, 170, 106));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        cbTela = new QCheckBox(widget);
        cbTela->setObjectName(QString::fromUtf8("cbTela"));

        verticalLayout->addWidget(cbTela);

        cbOriginal = new QCheckBox(widget);
        cbOriginal->setObjectName(QString::fromUtf8("cbOriginal"));

        verticalLayout->addWidget(cbOriginal);

        cbPerspectiva = new QCheckBox(widget);
        cbPerspectiva->setObjectName(QString::fromUtf8("cbPerspectiva"));

        verticalLayout->addWidget(cbPerspectiva);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btOK = new QPushButton(widget);
        btOK->setObjectName(QString::fromUtf8("btOK"));

        horizontalLayout->addWidget(btOK);

        btCancel = new QPushButton(widget);
        btCancel->setObjectName(QString::fromUtf8("btCancel"));

        horizontalLayout->addWidget(btCancel);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(DialogSave);

        QMetaObject::connectSlotsByName(DialogSave);
    } // setupUi

    void retranslateUi(QWidget *DialogSave)
    {
        DialogSave->setWindowTitle(QApplication::translate("DialogSave", "Salvar", 0, QApplication::UnicodeUTF8));
        cbTela->setText(QApplication::translate("DialogSave", "Tela Inteira", 0, QApplication::UnicodeUTF8));
        cbOriginal->setText(QApplication::translate("DialogSave", "Original", 0, QApplication::UnicodeUTF8));
        cbPerspectiva->setText(QApplication::translate("DialogSave", "Perspectiva", 0, QApplication::UnicodeUTF8));
        btOK->setText(QApplication::translate("DialogSave", "OK", 0, QApplication::UnicodeUTF8));
        btCancel->setText(QApplication::translate("DialogSave", "Cancelar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogSave: public Ui_DialogSave {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSAVE_H
