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
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action1;
    QWidget *centralWidget;
    QLabel *lbOriginal;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLabel *lbStatus;
    QToolButton *tbTela;
    QFrame *frameTools;
    QLabel *label_6;
    QFrame *line_2;
    QLabel *label_7;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QSlider *tbMin;
    QLabel *lbMin;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QSlider *tbMax;
    QLabel *lbMax;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_10;
    QSlider *tbThresh;
    QLabel *lbThresh;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_11;
    QSlider *tbTrack;
    QLabel *lbTrack;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btPlay;
    QPushButton *btReset;
    QPushButton *btSnap;
    QWidget *layoutWidget4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btFile;
    QPushButton *btDir;
    QPushButton *btIniciar;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLabel *lbFile;
    QFrame *frameImagens;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QCheckBox *cbPath;
    QCheckBox *cbIA;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *cbVelo;
    QCheckBox *cbAceleracao;
    QFrame *line;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLabel *lbAnimal;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *lbTeste;
    QPushButton *btSair;
    QPushButton *btCamConfig;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1312, 707);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        action1 = new QAction(MainWindow);
        action1->setObjectName(QString::fromUtf8("action1"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lbOriginal = new QLabel(centralWidget);
        lbOriginal->setObjectName(QString::fromUtf8("lbOriginal"));
        lbOriginal->setGeometry(QRect(510, 80, 704, 480));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbOriginal->sizePolicy().hasHeightForWidth());
        lbOriginal->setSizePolicy(sizePolicy1);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(510, 570, 132, 21));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(label_5);

        lbStatus = new QLabel(layoutWidget);
        lbStatus->setObjectName(QString::fromUtf8("lbStatus"));
        sizePolicy1.setHeightForWidth(lbStatus->sizePolicy().hasHeightForWidth());
        lbStatus->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(lbStatus);

        tbTela = new QToolButton(centralWidget);
        tbTela->setObjectName(QString::fromUtf8("tbTela"));
        tbTela->setGeometry(QRect(510, 30, 51, 31));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tbTela->sizePolicy().hasHeightForWidth());
        tbTela->setSizePolicy(sizePolicy2);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/original.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbTela->setIcon(icon);
        tbTela->setIconSize(QSize(24, 24));
        tbTela->setPopupMode(QToolButton::MenuButtonPopup);
        tbTela->setToolButtonStyle(Qt::ToolButtonIconOnly);
        frameTools = new QFrame(centralWidget);
        frameTools->setObjectName(QString::fromUtf8("frameTools"));
        frameTools->setGeometry(QRect(50, 400, 381, 271));
        frameTools->setFrameShape(QFrame::StyledPanel);
        frameTools->setFrameShadow(QFrame::Raised);
        label_6 = new QLabel(frameTools);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 0, 211, 31));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        line_2 = new QFrame(frameTools);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(10, 20, 321, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_7 = new QLabel(frameTools);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(11, 111, 187, 16));
        layoutWidget1 = new QWidget(frameTools);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(13, 173, 321, 46));
        verticalLayout_5 = new QVBoxLayout(layoutWidget1);
        verticalLayout_5->setSpacing(5);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(label_8);

        tbMin = new QSlider(layoutWidget1);
        tbMin->setObjectName(QString::fromUtf8("tbMin"));
        tbMin->setMaximum(500);
        tbMin->setValue(0);
        tbMin->setOrientation(Qt::Horizontal);

        horizontalLayout_9->addWidget(tbMin);

        lbMin = new QLabel(layoutWidget1);
        lbMin->setObjectName(QString::fromUtf8("lbMin"));
        sizePolicy1.setHeightForWidth(lbMin->sizePolicy().hasHeightForWidth());
        lbMin->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(lbMin);


        verticalLayout_5->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy1.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(label_9);

        tbMax = new QSlider(layoutWidget1);
        tbMax->setObjectName(QString::fromUtf8("tbMax"));
        tbMax->setMaximum(500);
        tbMax->setValue(300);
        tbMax->setOrientation(Qt::Horizontal);

        horizontalLayout_10->addWidget(tbMax);

        lbMax = new QLabel(layoutWidget1);
        lbMax->setObjectName(QString::fromUtf8("lbMax"));
        sizePolicy1.setHeightForWidth(lbMax->sizePolicy().hasHeightForWidth());
        lbMax->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(lbMax);


        verticalLayout_5->addLayout(horizontalLayout_10);

        layoutWidget2 = new QWidget(frameTools);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(13, 141, 321, 17));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setSizeConstraint(QLayout::SetNoConstraint);
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(layoutWidget2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy1.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(label_10);

        tbThresh = new QSlider(layoutWidget2);
        tbThresh->setObjectName(QString::fromUtf8("tbThresh"));
        tbThresh->setMaximum(255);
        tbThresh->setValue(110);
        tbThresh->setOrientation(Qt::Horizontal);

        horizontalLayout_8->addWidget(tbThresh);

        lbThresh = new QLabel(layoutWidget2);
        lbThresh->setObjectName(QString::fromUtf8("lbThresh"));
        sizePolicy1.setHeightForWidth(lbThresh->sizePolicy().hasHeightForWidth());
        lbThresh->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(lbThresh);

        layoutWidget3 = new QWidget(frameTools);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(13, 234, 321, 17));
        horizontalLayout_11 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy1.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy1);

        horizontalLayout_11->addWidget(label_11);

        tbTrack = new QSlider(layoutWidget3);
        tbTrack->setObjectName(QString::fromUtf8("tbTrack"));
        tbTrack->setMinimum(1);
        tbTrack->setMaximum(5);
        tbTrack->setOrientation(Qt::Horizontal);

        horizontalLayout_11->addWidget(tbTrack);

        lbTrack = new QLabel(layoutWidget3);
        lbTrack->setObjectName(QString::fromUtf8("lbTrack"));
        sizePolicy1.setHeightForWidth(lbTrack->sizePolicy().hasHeightForWidth());
        lbTrack->setSizePolicy(sizePolicy1);

        horizontalLayout_11->addWidget(lbTrack);

        widget = new QWidget(frameTools);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(21, 50, 122, 32));
        horizontalLayout_7 = new QHBoxLayout(widget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        btPlay = new QPushButton(widget);
        btPlay->setObjectName(QString::fromUtf8("btPlay"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        btPlay->setIcon(icon1);
        btPlay->setIconSize(QSize(24, 24));

        horizontalLayout_7->addWidget(btPlay);

        btReset = new QPushButton(widget);
        btReset->setObjectName(QString::fromUtf8("btReset"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        btReset->setIcon(icon2);
        btReset->setIconSize(QSize(24, 24));

        horizontalLayout_7->addWidget(btReset);

        btSnap = new QPushButton(widget);
        btSnap->setObjectName(QString::fromUtf8("btSnap"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/snapshot.png"), QSize(), QIcon::Normal, QIcon::Off);
        btSnap->setIcon(icon3);
        btSnap->setIconSize(QSize(24, 24));

        horizontalLayout_7->addWidget(btSnap);

        layoutWidget4 = new QWidget(centralWidget);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(51, 40, 383, 302));
        verticalLayout_4 = new QVBoxLayout(layoutWidget4);
        verticalLayout_4->setSpacing(15);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btFile = new QPushButton(layoutWidget4);
        btFile->setObjectName(QString::fromUtf8("btFile"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btFile->sizePolicy().hasHeightForWidth());
        btFile->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(btFile);

        btDir = new QPushButton(layoutWidget4);
        btDir->setObjectName(QString::fromUtf8("btDir"));
        sizePolicy3.setHeightForWidth(btDir->sizePolicy().hasHeightForWidth());
        btDir->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(btDir);

        btIniciar = new QPushButton(layoutWidget4);
        btIniciar->setObjectName(QString::fromUtf8("btIniciar"));
        sizePolicy3.setHeightForWidth(btIniciar->sizePolicy().hasHeightForWidth());
        btIniciar->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(btIniciar);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setSizeConstraint(QLayout::SetFixedSize);
        label_4 = new QLabel(layoutWidget4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(label_4);

        lbFile = new QLabel(layoutWidget4);
        lbFile->setObjectName(QString::fromUtf8("lbFile"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lbFile->sizePolicy().hasHeightForWidth());
        lbFile->setSizePolicy(sizePolicy4);
        lbFile->setTextFormat(Qt::AutoText);
        lbFile->setWordWrap(true);

        horizontalLayout_5->addWidget(lbFile);


        verticalLayout_4->addLayout(horizontalLayout_5);

        frameImagens = new QFrame(layoutWidget4);
        frameImagens->setObjectName(QString::fromUtf8("frameImagens"));
        frameImagens->setFrameShape(QFrame::StyledPanel);
        frameImagens->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frameImagens);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        cbPath = new QCheckBox(frameImagens);
        cbPath->setObjectName(QString::fromUtf8("cbPath"));

        verticalLayout->addWidget(cbPath);

        cbIA = new QCheckBox(frameImagens);
        cbIA->setObjectName(QString::fromUtf8("cbIA"));
        sizePolicy1.setHeightForWidth(cbIA->sizePolicy().hasHeightForWidth());
        cbIA->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(cbIA);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
        cbVelo = new QCheckBox(frameImagens);
        cbVelo->setObjectName(QString::fromUtf8("cbVelo"));

        verticalLayout_2->addWidget(cbVelo);

        cbAceleracao = new QCheckBox(frameImagens);
        cbAceleracao->setObjectName(QString::fromUtf8("cbAceleracao"));

        verticalLayout_2->addWidget(cbAceleracao);


        horizontalLayout->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        line = new QFrame(frameImagens);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 0, 1, 1);

        label_3 = new QLabel(frameImagens);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        line->raise();
        label_3->raise();

        verticalLayout_4->addWidget(frameImagens);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(10);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        label = new QLabel(layoutWidget4);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(label);

        lbAnimal = new QLabel(layoutWidget4);
        lbAnimal->setObjectName(QString::fromUtf8("lbAnimal"));
        sizePolicy1.setHeightForWidth(lbAnimal->sizePolicy().hasHeightForWidth());
        lbAnimal->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(lbAnimal);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
        label_2 = new QLabel(layoutWidget4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_2);

        lbTeste = new QLabel(layoutWidget4);
        lbTeste->setObjectName(QString::fromUtf8("lbTeste"));
        lbTeste->setEnabled(true);
        sizePolicy1.setHeightForWidth(lbTeste->sizePolicy().hasHeightForWidth());
        lbTeste->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(lbTeste);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_4->addLayout(verticalLayout_3);

        btSair = new QPushButton(centralWidget);
        btSair->setObjectName(QString::fromUtf8("btSair"));
        btSair->setGeometry(QRect(1240, 10, 51, 41));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/sair.png"), QSize(), QIcon::Normal, QIcon::Off);
        btSair->setIcon(icon4);
        btSair->setIconSize(QSize(24, 24));
        btCamConfig = new QPushButton(centralWidget);
        btCamConfig->setObjectName(QString::fromUtf8("btCamConfig"));
        btCamConfig->setGeometry(QRect(1170, 10, 51, 41));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/cam_config.png"), QSize(), QIcon::Normal, QIcon::Off);
        btCamConfig->setIcon(icon5);
        btCamConfig->setIconSize(QSize(24, 24));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Rattrack v2", 0, QApplication::UnicodeUTF8));
        action1->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        lbOriginal->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Status:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lbStatus->setText(QString());
#ifndef QT_NO_TOOLTIP
        tbTela->setToolTip(QApplication::translate("MainWindow", "Opcoes de Tela", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        tbTela->setText(QString());
        label_6->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Ferramentas do video</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt; font-weight:600;\">Configuracoes avancadas</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Area min</p></body></html>", 0, QApplication::UnicodeUTF8));
        lbMin->setText(QString());
        label_9->setText(QApplication::translate("MainWindow", "Area max", 0, QApplication::UnicodeUTF8));
        lbMax->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Thresh", 0, QApplication::UnicodeUTF8));
        lbThresh->setText(QString());
        label_11->setText(QApplication::translate("MainWindow", "Track", 0, QApplication::UnicodeUTF8));
        lbTrack->setText(QString());
#ifndef QT_NO_TOOLTIP
        btPlay->setToolTip(QApplication::translate("MainWindow", "Pause", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btPlay->setText(QString());
#ifndef QT_NO_TOOLTIP
        btReset->setToolTip(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btReset->setText(QString());
#ifndef QT_NO_TOOLTIP
        btSnap->setToolTip(QApplication::translate("MainWindow", "Snapshot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btSnap->setText(QString());
        btFile->setText(QApplication::translate("MainWindow", "Carregar Arquivo", 0, QApplication::UnicodeUTF8));
        btDir->setText(QApplication::translate("MainWindow", "Selecionar Pasta", 0, QApplication::UnicodeUTF8));
        btIniciar->setText(QApplication::translate("MainWindow", "Iniciar Teste", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Arquivo:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lbFile->setText(QString());
        cbPath->setText(QApplication::translate("MainWindow", "Caminho/Metragem (cm)", 0, QApplication::UnicodeUTF8));
        cbIA->setText(QApplication::translate("MainWindow", "Indice de Aprendizado", 0, QApplication::UnicodeUTF8));
        cbVelo->setText(QApplication::translate("MainWindow", "Velocidade M\303\251dia (cm/s)", 0, QApplication::UnicodeUTF8));
        cbAceleracao->setText(QApplication::translate("MainWindow", "Acelera\303\247\303\243o (cm\302\262/s)", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Dados analisados:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Animal:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lbAnimal->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Teste:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lbTeste->setText(QString());
#ifndef QT_NO_TOOLTIP
        btSair->setToolTip(QApplication::translate("MainWindow", "Sair", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btSair->setText(QString());
#ifndef QT_NO_TOOLTIP
        btCamConfig->setToolTip(QApplication::translate("MainWindow", "Calibracao da Camera", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btCamConfig->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
