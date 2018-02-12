#include "calibrationgui.h"
#include "ui_calibrationgui.h"
#include "mainwindow.h"
#include "calibration.h"

CalibrationGUI* CalibrationGUI::instanceGUI = 0;

CalibrationGUI::CalibrationGUI(QWidget *parent) : QWidget(parent), ui(new Ui::CalibrationGUI) {

    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));

    ui->setupUi(this);
    instanceGUI = this;

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Calibracao rat-track");

    //Remove Window's optionsButton
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->mainLayout = new QVBoxLayout(this);

    this->editCircle = ON;
    this->quadrants = this->moveCircle = OFF;
    this->current = this->before = QPointF(0, 0);
    this->startGUILoadFile();
}

CalibrationGUI::~CalibrationGUI() {
    delete ui;
}

CalibrationGUI* CalibrationGUI::getInstance() {

    if(instanceGUI == 0)
        instanceGUI = new CalibrationGUI;
    return instanceGUI;
}

void CalibrationGUI::centerWindow() {
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);
}

void CalibrationGUI::clearWindow() {
    QList<QWidget *> widgets = findChildren<QWidget *>();
    foreach(QWidget* widget, widgets)
        delete widget;

    this->clearMainLayout();
}

void CalibrationGUI::clearMainLayout() {
    QLayoutItem *item;
    while((item = mainLayout->takeAt(0))) {
        delete item;
        item = NULL;
    }
}

void CalibrationGUI::resetProgram() {
    MainWindow *mw = new MainWindow();
    mw->show();
    close();
}

void CalibrationGUI::errorMessage(QString message) {
    QMessageBox::critical(this, tr("Erro"), tr(message.toAscii()));
}

void CalibrationGUI::startGUILoadFile() {
    QLabel *lbHorCorners, *lbVerCorners, *lbMeasure, *lb1File;
    QIcon icon;
    QFont f;
    f = QFont("Ubuntu", 11, QFont::Bold);

    //Horizontal Corners
    lbHorCorners = new QLabel("Esquinas Horizontais: ");
    lbHorCorners->setFont(f);
    leHorCorners = new QLineEdit();
    QHBoxLayout *layoutHHorCorners = new QHBoxLayout();
    layoutHHorCorners->addWidget(lbHorCorners, 0, Qt::AlignCenter);
    layoutHHorCorners->addStretch(1);
    layoutHHorCorners->addWidget(leHorCorners, 0, Qt::AlignCenter);

    //Vertical Corners
    lbVerCorners = new QLabel("Esquinas Verticais: ");
    lbVerCorners->setFont(f);
    leVerCorners = new QLineEdit();
    QHBoxLayout *layoutHVerCorners = new QHBoxLayout();
    layoutHVerCorners->addWidget(lbVerCorners, 0, Qt::AlignCenter);
    layoutHVerCorners->addStretch(1);
    layoutHVerCorners->addWidget(leVerCorners, 0, Qt::AlignCenter);

    //Measures
    lbMeasure = new QLabel("Medida dos quadrados (cm): ");
    lbMeasure->setFont(f);
    leMeasure = new QLineEdit();
    QHBoxLayout *layoutHMeasure = new QHBoxLayout();
    layoutHMeasure->addWidget(lbMeasure, 0, Qt::AlignCenter);
    layoutHMeasure->addStretch(1);
    layoutHMeasure->addWidget(leMeasure, 0, Qt::AlignCenter);

    //InfoLayout
    QVBoxLayout *layoutInfos = new QVBoxLayout();
    layoutInfos->addLayout(layoutHHorCorners);
    layoutInfos->addLayout(layoutHVerCorners);
    layoutInfos->addLayout(layoutHMeasure);
    layoutInfos->addStretch(1);

    //FileLayout
    lb1File = new QLabel("Arquivo carregado: ");
    lb1File->setFont(f);
    lb2File = new QLabel("Nenhum.");
    lb2File->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QHBoxLayout *layoutHorFile = new QHBoxLayout();
    layoutHorFile->addStretch(1);
    layoutHorFile->addWidget(lb1File);
    layoutHorFile->addWidget(lb2File);
    layoutHorFile->addStretch(1);

    icon = QIcon(":/videofile.png");
    btFile = new QPushButton(icon, "Carregar video de calibracao");
    btFile->setIconSize(QSize(24, 24));
    QObject::connect(btFile, SIGNAL(clicked()), this, SLOT(btFile_clicked()));
    QHBoxLayout *buttonFileLayout = new QHBoxLayout();
    buttonFileLayout->addStretch(1);
    buttonFileLayout->addWidget(btFile);
    buttonFileLayout->addStretch(1);

    QVBoxLayout *layoutVerFile = new QVBoxLayout();
    layoutVerFile->addLayout(buttonFileLayout);
    layoutVerFile->addLayout(layoutHorFile);

    //OptionsLayout
    icon = QIcon(":/start.png");
    btStart = new QPushButton(icon, "Iniciar");
    btStart->setIconSize(QSize(24, 24));
    QObject::connect(btStart, SIGNAL(clicked()), this, SLOT(btStart_clicked()));
    btStart->setEnabled(false);

    icon = QIcon(":/cancel.png");
    btCancel = new QPushButton(icon, "Cancelar");
    btCancel->setIconSize(QSize(24, 24));
    QObject::connect(btCancel, SIGNAL(clicked()), this, SLOT(btCancel_clicked()));

    QHBoxLayout *buttonOptionsLayout = new QHBoxLayout();
    buttonOptionsLayout->addStretch(1);
    buttonOptionsLayout->addWidget(btStart);
    buttonOptionsLayout->addWidget(btCancel);
    buttonOptionsLayout->addStretch(1);

    //MainLayout
    mainLayout->addLayout(layoutInfos);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(layoutVerFile);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonOptionsLayout);
    mainLayout->addStretch(1);

    //Resize Window
    this->adjustSize();
    this->centerWindow();
}

void CalibrationGUI::btFile_clicked() {

    //Getting video
    QString argv = QFileDialog::getOpenFileName(this, tr("Escolha o video de calibracao"), QString(), tr("Video Files (*.avi)"));

    if(argv != "") {

        cap = new Video(argv);

        if(cap->isOpen()) {
            lb2File->setText(cap->getFileName().section("/", -1, -1));
            btStart->setEnabled(true);
        }else{
            QMessageBox::critical(this, tr("Erro"), tr("Nao foi possivel ler o arquivo " + argv.section("/", -1, -1).toAscii()));
            lb2File->setText("Nenhum.");
        }
    }
}

void CalibrationGUI::btStart_clicked() {

    //Getting infos
    QString board_h = leHorCorners->text(), board_v = leVerCorners->text(), measure = leMeasure->text();

    //Verifying if they are valid
    bool valid = true;
    if(board_h.isEmpty() || board_v.isEmpty() || measure.isEmpty())
        valid = false;

    string test = board_h.toUtf8().constData();
    if(!(test.find_first_not_of("0123456789") == string::npos))
        valid = false;

    test = board_v.toUtf8().constData();
    if(!(test.find_first_not_of("0123456789") == string::npos))
        valid = false;

    test = measure.toUtf8().constData();
    if(!(test.find_first_not_of("0123456789") == string::npos))
        valid = false;

    if(valid)
        this->calibrationCam = new Calibration(cap, board_h.toInt()-1, board_v.toInt()-1, board_h.toInt()*board_v.toInt(),
                                               measure.toFloat());
    else
        this->errorMessage("Preencha todos os campos com numeros inteiros!");
}

void CalibrationGUI::btCancel_clicked() {
    this->resetProgram();
}

void CalibrationGUI::startGUICalibration(Mat showFrame) {

    this->clearWindow();

    //Button Mappers
    editLineMapper = new QSignalMapper(this);
    QObject::connect(editLineMapper, SIGNAL(mapped(int)), this, SLOT(btEditLine_clicked(int)));

    deleteLineMapper = new QSignalMapper(this);
    QObject::connect(deleteLineMapper, SIGNAL(mapped(int)), this, SLOT(btDeleteLine_clicked(int)));

    //Instruction Button:
    QIcon icon;
    icon = QIcon(":/instructions.png");
    btInstructions = new QPushButton(icon, "Instrucoes");
    btInstructions->setIconSize(QSize(32, 32));
    QObject::connect(btInstructions, SIGNAL(clicked()), this, SLOT(btInstructions_clicked()));

    //Layout
    QHBoxLayout *layoutInstructions = new QHBoxLayout();
    layoutInstructions->addStretch(1);
    layoutInstructions->addWidget(btInstructions);

    //Detection Area Options:
    //Title
    QFont f("Ubuntu", 12, QFont::Bold);
    QLabel *lbArea = new QLabel("Area de deteccao: ");
    lbArea->setFont(f);
    //Edit Button
    icon = QIcon(":/edit.png");
    btEditCircle = new QPushButton(icon, "Editar");
    btEditCircle->setIconSize(QSize(24, 24));
    QObject::connect(btEditCircle, SIGNAL(clicked()), this, SLOT(btEditCircle_clicked()));
    //Move Button
    icon = QIcon(":/move.png");
    btMoveCircle = new QPushButton(icon, "Mover");
    btMoveCircle->setEnabled(false);
    btMoveCircle->setIconSize(QSize(24, 24));
    QObject::connect(btMoveCircle, SIGNAL(clicked(bool)), this, SLOT(btMoveCircle_clicked()));
    //Delete Button
    icon = QIcon(":/delete.png");
    btDeleteCircle = new QPushButton(icon, "Apagar");
    btDeleteCircle->setIconSize(QSize(24, 24));
    btDeleteCircle->setEnabled(false);
    QObject::connect(btDeleteCircle, SIGNAL(clicked()), this, SLOT(btDeleteCircle_clicked()));
    //Layout
    QHBoxLayout *layoutCircles = new QHBoxLayout();
    layoutCircles->addWidget(lbArea);
    layoutCircles->addStretch(1);
    layoutCircles->addWidget(btEditCircle);
    layoutCircles->addWidget(btMoveCircle);
    layoutCircles->addWidget(btDeleteCircle);
    layoutCircles->addStretch(1);

    //Quadrants CheckBox:
    cbQuadrants = new QCheckBox("Configurar quadrantes?");
    cbQuadrants->setEnabled(false);
    QObject::connect(cbQuadrants, SIGNAL(stateChanged(int)), this, SLOT(cbQuadrants_clicked(int)));
    //Layout
    QHBoxLayout *layoutCheckBox = new QHBoxLayout();
    layoutCheckBox->addStretch(1);
    layoutCheckBox->addWidget(cbQuadrants);
    layoutCheckBox->addStretch(1);

    //Quadrant Options:
    //Title
    lbQuadrants = new QLabel("Quadrantes: ");
    lbQuadrants->setFont(f);
    lbQuadrants->setEnabled(false);
    //Button AddLine
    icon = QIcon(":/addLine.png");
    btAddLine = new QPushButton(icon, "Nova linha");
    btAddLine->setIconSize(QSize(24, 24));
    btAddLine->setEnabled(false);
    QObject::connect(btAddLine, SIGNAL(clicked()), this, SLOT(btAddLine_clicked()));
    //Layout
    QHBoxLayout *layoutHorQuadrant = new QHBoxLayout();
    layoutHorQuadrant->addWidget(lbQuadrants);
    layoutHorQuadrant->addWidget(btAddLine);
    layoutHorQuadrant->addStretch(1);
    //Table for Lines
    tableLines = new QTableWidget(1,1);
    tableLines->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QStringList tableHeader;
    tableHeader << " Opcoes ";
    tableLines->setHorizontalHeaderLabels(tableHeader);
    tableLines->setColumnWidth(0, 200);
    tableLines->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tableLines->setEnabled(false);
    //Table Layout
    QHBoxLayout *layoutTable = new QHBoxLayout();
    layoutTable->addStretch(1);
    layoutTable->addWidget(tableLines);
    layoutTable->addStretch(1);
    //Main Layout
    QVBoxLayout *layoutQuadrant = new QVBoxLayout();
    layoutQuadrant->addLayout(layoutCheckBox);
    layoutQuadrant->addLayout(layoutHorQuadrant);
    layoutQuadrant->addSpacing(15);
    layoutQuadrant->addLayout(layoutTable);

    //Calibration Options:
    //Finish Button
    icon = QIcon(":/finish.png");
    btSave = new QPushButton(icon, "Finalizar");
    btSave->setIconSize(QSize(24, 24));
    btSave->setEnabled(false);
    QObject::connect(btSave, SIGNAL(clicked()), this, SLOT(btSave_clicked()));
    //Cancel Button
    icon = QIcon(":/cancel.png");
    btCancel = new QPushButton(icon, "Cancelar");
    btCancel->setIconSize(QSize(24, 24));
    QObject::connect(btCancel, SIGNAL(clicked()), this, SLOT(btCancelCalibration_clicked()));
    //Layout
    QHBoxLayout *layoutButtons = new QHBoxLayout();
    layoutButtons->addStretch(1);
    layoutButtons->addWidget(btSave);
    layoutButtons->addWidget(btCancel);
    layoutButtons->addStretch(1);

    //Options frame
    QVBoxLayout *optionsLayout = new QVBoxLayout();
    optionsLayout->addStretch(1);
    optionsLayout->addLayout(layoutInstructions);
    optionsLayout->addSpacing(15);
    optionsLayout->addLayout(layoutCircles);
    optionsLayout->addSpacing(15);
    optionsLayout->addLayout(layoutQuadrant);
    optionsLayout->addStretch(1);
    optionsLayout->addLayout(layoutButtons);
    optionsLayout->addStretch(1);

    //Frame label
    lbFrame = new QLabel();
    lbFrame->setFixedSize(QSize(704, 480));
    lbFrame->setStyleSheet(QString::fromStdString("background-color: rgb(0,0,0);"));
    lbFrame->installEventFilter(this);
    QImage imageShow((const uchar *) showFrame.data, showFrame.cols, showFrame.rows, showFrame.step, QImage::Format_RGB888);
    lbFrame->setPixmap(QPixmap::fromImage(imageShow));

    //MainLayout
    QHBoxLayout *mainHorLayout = new QHBoxLayout();
    mainHorLayout->addWidget(lbFrame);
    mainHorLayout->addSpacing(20);
    mainHorLayout->addLayout(optionsLayout);

    mainLayout->addLayout(mainHorLayout);

    this->showMaximized();
}

void CalibrationGUI::btInstructions_clicked() {
    QMessageBox::information(this, tr("Instrucoes"), tr("<b>AREA DE DETECCAO:</b> Para configurar a area de deteccao, clique"
                                                        " no botao 'Editar' e entao selecione a area desejada na imagem a sua"
                                                        "esquerda. Para altera-la o procedimento eh o mesmo. Caso queira"
                                                        " apagar a area e refaze-la clique em 'Apagar'.                      "
                                                        " <b>QUADRANTES:</b> Para configurar os quadrantes, selecione a opcao"
                                                        " 'Configurar Quadrantes?', entao serao liberadas todas as ferramentas. "
                                                        "Clique em 'Nova linha' e clique na imagem para configura-las. "
                                                        "Elas aparecerao na tabela a direita com as opcoes 'Mover' e 'Excluir'."
                                                        " Configure quantos quadrantes quiser!"));
}

void CalibrationGUI::btEditCircle_clicked() {

    int ok = true;
    if(cbQuadrants->isChecked()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Tem certeza disso?");
        msgBox.setText("Sua acao deletera as configuracoes de quadrante feitas. Deseja continuar?");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::No)
            ok = false;
    }

    if(ok) {
        //Circle informations
        this->editCircle = ON;
        this->moveCircle = OFF;
        this->btEditCircle->setEnabled(false);

        //Delete Quadrants Informations
        if(lines.size() > 0) {
            lines.clear();
            this->calibrationCam->clearPoints();
            this->calibrationCam->drawCircle();
            this->tableLines->setRowCount(0);
        }

        this->quadrants = OFF;
        cbQuadrants->setChecked(false);
        this->lbQuadrants->setEnabled(false);
        this->btAddLine->setEnabled(false);
        this->tableLines->setEnabled(false);

    }
}

void CalibrationGUI::btMoveCircle_clicked() {

    int ok = true;
    if(cbQuadrants->isChecked()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Tem certeza disso?");
        msgBox.setText("Sua acao deletera as configuracoes de quadrante feitas. Deseja continuar?");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::No)
            ok = false;
    }

    if(ok) {
        //Circle Informations
        this->moveCircle = ON;
        this->editCircle = OFF;
        this->btEditCircle->setEnabled(true);
        this->btMoveCircle->setEnabled(false);

        //Delete Quadrants Informations
        if(lines.size() > 0) {
            lines.clear();
            this->calibrationCam->clearPoints();
            this->calibrationCam->drawCircle();
            this->tableLines->setRowCount(0);
        }

        this->quadrants = OFF;
        cbQuadrants->setChecked(false);
        this->lbQuadrants->setEnabled(false);
        this->btAddLine->setEnabled(false);
        this->tableLines->setEnabled(false);
    }
}

void CalibrationGUI::btDeleteCircle_clicked() {

    int ok = true;
    if(cbQuadrants->isChecked()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Tem certeza disso?");
        msgBox.setText("Sua acao deletera as configuracoes de quadrante feitas. Deseja continuar?");
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        if(msgBox.exec() == QMessageBox::No)
            ok = false;
    }

    if(ok) {
        //Circle informations
        this->moveCircle = OFF;
        this->editCircle = OFF;

        //Setting default values
        this->calibrationCam->setCenter(Point2f(-1,-1));
        this->calibrationCam->setRadius(1);
        this->before = this->current = QPointF(0,0);

        //Updating GUI
        this->btEditCircle->setEnabled(true);
        this->btSave->setEnabled(false);
        this->btMoveCircle->setEnabled(false);
        this->btDeleteCircle->setEnabled(false);
        this->calibrationCam->drawCircle();

        //Delete Quadrants Informations
        if(lines.size() > 0) {
            lines.clear();
            this->calibrationCam->clearPoints();
            this->calibrationCam->drawCircle();
            this->tableLines->setRowCount(0);
        }

        this->quadrants = OFF;
        cbQuadrants->setChecked(false);
        this->lbQuadrants->setEnabled(false);
        this->btAddLine->setEnabled(false);
        this->tableLines->setEnabled(false);
    }
}

void CalibrationGUI::cbQuadrants_clicked(int value) {
    //2 = checked, 0 = uncheked

    this->editCircle = OFF;
    this->moveCircle = OFF;
    this->btEditCircle->setEnabled(true);
    this->btMoveCircle->setEnabled(true);

    if(value == 2) {
        this->quadrants = ON;
        this->lbQuadrants->setEnabled(true);
        this->btAddLine->setEnabled(true);
        this->tableLines->setEnabled(true);
    }else{
        this->quadrants = OFF;
        this->lbQuadrants->setEnabled(false);
        this->btAddLine->setEnabled(false);
        this->tableLines->setEnabled(false);
    }
}

void CalibrationGUI::btAddLine_clicked() {

    if(lines.size() != 0) {
        Point2d point = this->calibrationCam->getPoint(((int)lines.size())-1);
        if(point.x == -1 && point.y == -1) {
            QMessageBox::critical(this, tr("Erro"), tr("Defina o ponto anterior na imagem!"));
            return;
        }
    }


    for(int i = 0; i < (int) lines.size(); i++)
        lines.at(i) = OFF;
    lines.push_back(ON);
    this->calibrationCam->addPoint();

    QIcon icon;
    QPushButton *bt;
    QVBoxLayout *layoutVerButtons = new QVBoxLayout();

    //Edit Button
    icon = QIcon(":/move.png");
    bt = new QPushButton(icon, "Mover");
    bt->setIconSize(QSize(24,24));
    QObject::connect(bt, SIGNAL(clicked()), editLineMapper, SLOT(map()));
    editLineMapper->setMapping(bt, ((int)lines.size())-1);
    layoutVerButtons->addWidget(bt);
    btEditLine.push_back(bt);

    //Delete Button
    icon = QIcon(":/delete.png");
    bt = new QPushButton(icon, "Apagar");
    bt->setIconSize(QSize(24,24));
    QObject::connect(bt, SIGNAL(clicked()), deleteLineMapper, SLOT(map()));
    deleteLineMapper->setMapping(bt, ((int)lines.size())-1);
    layoutVerButtons->addWidget(bt);
    btDeleteLine.push_back(bt);

    //Widget
    QWidget *btWidget = new QWidget();
    btWidget->setLayout(layoutVerButtons);
    tableLines->setCellWidget(((int)lines.size())-1, 0, btWidget);
    tableLines->insertRow(((int)lines.size()));


}

void CalibrationGUI::btEditLine_clicked(int index) {
    this->editCircle = OFF;
    this->moveCircle = OFF;
    this->btEditCircle->setEnabled(true);
    this->btMoveCircle->setEnabled(true);

    this->quadrants = ON;
    for(int i = 0; i < (int)lines.size(); i++) {
        if(i == index)
            lines.at(i) = ON;
        else
            lines.at(i) = OFF;
    }
}

void CalibrationGUI::btDeleteLine_clicked(int index) {

    //Remove informations
    this->removeLine(index);
    tableLines->removeRow(index);
    this->calibrationCam->removePoint(index);
    this->calibrationCam->drawLines();

    //Updating QSignal Mappers
    for(int i = index; i < (int) lines.size(); i++) {
        QPushButton *bt1 = btEditLine.at(i);
        QPushButton *bt2 = btDeleteLine.at(i);
        editLineMapper->removeMappings(bt1);
        deleteLineMapper->removeMappings(bt2);
        editLineMapper->setMapping(bt1, i);
        deleteLineMapper->setMapping(bt2, i);
    }
}

void CalibrationGUI::removeLine(int index) {

    //EditLine
    vector<QPushButton*>::iterator position = std::find(btEditLine.begin(), btEditLine.end(), btEditLine.at(index));
    if(position != btEditLine.end()) btEditLine.erase(position);

    //DeleteLine
    position = std::find(btDeleteLine.begin(), btDeleteLine.end(), btDeleteLine.at(index));
    if(position != btDeleteLine.end()) btDeleteLine.erase(position);

    //Lines (bool)
    lines.erase(lines.begin() + index);
}

void CalibrationGUI::btSave_clicked() {

    Point2d point = this->calibrationCam->getCenter();

    if(point.x != -1 && point.y != -1) {
        if(lines.size() > 0)
            this->calibrationCam->calcAngles();

        this->calibrationCam->writeCalibrationInfos();

        QMessageBox::information(this, tr("Calibracao"), tr("A calibracao foi finalizada com sucesso! Voce voltara a tela inicial."));

        this->resetProgram();

    }else
        QMessageBox::information(this, tr("Calibracao"), tr("Porfavor defina a area de deteccao antes de finalizar!"));

}

void CalibrationGUI::btCancelCalibration_clicked() {
    this->calibrationCam->cancelCalibration();
}

bool CalibrationGUI::eventFilter(QObject * watched, QEvent * event) {
    if(watched != this->lbFrame)
        return false;

    //Edit Button
    if(editCircle == ON) {
        if(event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            current = mouseEvent->posF();

            //Determining circle
            if(before.x() != 0 && before.y() != 0) {

                //Calculing distance (radius) between points before and current
                double diffX = current.x() - before.x();
                double diffY = current.y() - before.y();
                double distance = sqrt((diffX*diffX) + (diffY*diffY));

                if(diffX+diffY > 0)
                    this->calibrationCam->setRadius(this->calibrationCam->getRadius() + distance);
                else if(diffX+diffY < 0)
                    this->calibrationCam->setRadius(abs(this->calibrationCam->getRadius() - distance));

            }else
                this->calibrationCam->setCenter(Point2d((double)current.x(), (double)current.y()));

            before = current;
            this->btMoveCircle->setEnabled(true);
            this->btDeleteCircle->setEnabled(true);
            this->cbQuadrants->setEnabled(true);
            this->btSave->setEnabled(true);
            this->calibrationCam->drawCircle();
        }
    }

    //MoveButton
    if(moveCircle == ON) {
        if(event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF pos = mouseEvent->posF();
            this->calibrationCam->setCenter(Point2d(((double)pos.x()), ((double)pos.y())));
            this->calibrationCam->drawCircle();
        }
    }

    //Quadrants
    if(quadrants == ON) {
        int index;
        for(index = 0; index < (int) lines.size(); index++) {
            if(lines.at(index) == ON)
                break;
        }

        if(index != (int)lines.size()) {
            if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                QPointF pos = mouseEvent->posF();
                this->calibrationCam->setPoint(Point2d((double)pos.x(), (double)pos.y()), index);
                this->calibrationCam->drawLines();
            }
        }
    }

    return false;
}

void CalibrationGUI::showImage(Mat showFrame) {
    QImage imageShow((const uchar *) showFrame.data, showFrame.cols, showFrame.rows, showFrame.step, QImage::Format_RGB888);
    lbFrame->setPixmap(QPixmap::fromImage(imageShow));
}
