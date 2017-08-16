#include "mainwindow.h"
#include "tracker.h"
#include "ui_mainwindow.h"
#include "calibrationgui.h"

MainWindow* MainWindow::instance = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));

    ui->setupUi(this);

    //Default settings
    this->setValueThreshold(130);
    this->setMinValueArea(0);
    this->setMaxValueArea(300);
    this->setThicknessThreshold(1);
    this->setScreen(true);

    testeInput = NULL;
    animalInput = NULL;
    advancedSettings = true;
    setAdvancedSettings();

    //Full screen and black background
    this->showMaximized();
    ui->lbOriginal->setStyleSheet("background-color: rgb(0, 0, 0);");

    //Disabled buttons
    ui->frameTools->setEnabled(false);
    ui->btIniciar->setEnabled(false);
    ui->label->setEnabled(false);
    ui->lbAnimal->setEnabled(false);
    ui->label_2->setEnabled(false);
    ui->lbTeste->setEnabled(false);
    ui->label_4->setEnabled(false);
    ui->lbFile->setEnabled(false);

    if(!existsCalibration()) {
        ui->btFile->setEnabled(false);
        ui->btDir->setEnabled(false);
    }

    instance = this;

    this->tracker = NULL;
}

MainWindow* MainWindow::getInstance() {

    if(instance == 0)
        instance = new MainWindow;
    return instance;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addVideo(Video* v) {
    this->captureVideos.push_back(v);
}

void MainWindow::removeVideo() {
    vector<Video*>::iterator position = std::find(this->captureVideos.begin(), this->captureVideos.end(),
                                                  this->tracker->getCaptureVideo());
    if(position != this->captureVideos.end())
        this->captureVideos.erase(position);
}

void MainWindow::readDirectoryVideos() {

    //User chooses the directory with all videos
    QString chooseDir = QFileDialog::getExistingDirectory(this, tr("Escolha o diretorio que contenha os videos"), "/home", QFileDialog::DontResolveSymlinks | QFileDialog::ReadOnly);

    //Setting directory config (.avi files)
    QDir workDir(chooseDir);
    workDir.setNameFilters(QStringList()<<"*.avi");

    //Creating file list
    foreach (QFileInfo file, workDir.entryInfoList()){
        if (file.isFile()) {
            Video* v = new Video(file.absoluteFilePath());
            this->addVideo(v);
        }
    }
}

void MainWindow::readVideo(Video *v) {

    //Open Video
    VideoCapture cap;
    cap.open(v->getFileName().toAscii().data());
    v->setCaptureVideo(cap);

    //Verifying if video has opened - "FALTA FAZER UM TIPO DE RETURN"
    if(!(v->getCaptureVideo()).isOpened())
        throw "Error when reading steam_avi";

    //Saving infos about Video
    v->setFPS((v->getCaptureVideo()).get(CAP_PROP_FPS));
    v->setTotalFrames((int) (v->getCaptureVideo()).get(CAP_PROP_FRAME_COUNT));
    v->setHeight((v->getCaptureVideo()).get(CV_CAP_PROP_FRAME_HEIGHT));
    v->setWidth((v->getCaptureVideo()).get(CV_CAP_PROP_FRAME_WIDTH));
}

void MainWindow::setAdvancedSettings() {
    if(!advancedSettings) {
        advancedSettings = true;
        ui->frameAdvancedTools->show();
   } else {
        advancedSettings = false;
        ui->frameAdvancedTools->hide();
    }
}

void MainWindow::setButtonPlay(bool status) {

    //status = true then the video is playing so u have to pause
    if(status) {
        QPixmap pixmap(":/play.png");
        QIcon ButtonIcon(pixmap);
        ui->btPlay->setIcon(ButtonIcon);
        ui->btPlay->setToolTip("Pause");
        ui->lbStatus->setText("Pausado!");

    //status = false then the video is not playing so u have to play
    }else{
        QPixmap pixmap(":/pause.png");
        QIcon ButtonIcon(pixmap);
        ui->btPlay->setIcon(ButtonIcon);
        ui->btPlay->setToolTip("Play");
    }

}

void MainWindow::showImage(Mat image) {

    Mat showFrame;
    cvtColor(image, showFrame, CV_BGR2RGB);
    cv::resize(showFrame, showFrame, Size(704, 480));
    QImage imageShow((const uchar *) showFrame.data, showFrame.cols, showFrame.rows, showFrame.step, QImage::Format_RGB888);
    ui->lbOriginal->setPixmap(QPixmap::fromImage(imageShow));
}

void MainWindow::setStatus(string status) {
    ui->lbStatus->setText(QString::fromStdString(status));
}

void MainWindow::setReader(ReaderInterface *reader) {
    this->reader = reader;
}

ReaderInterface* MainWindow::getReader() {
    return this->reader;
}

int MainWindow::getValueThreshold() {
    return this->valueThreshold;
}

void MainWindow::setValueThreshold(int value) {
    this->valueThreshold = value;
    ui->tbThresh->setValue(value);
    ui->lbThresh->setText(QString::number(value));
}

int MainWindow::getMinValueArea() {
    return this->minValueArea;
}

void MainWindow::setMinValueArea(int value) {
    this->minValueArea = value;
    ui->tbMin->setValue(value);
    ui->lbMin->setText(QString::number(value));
}

int MainWindow::getMaxValueArea() {
    return this->maxValueArea;
}

void MainWindow::setMaxValueArea(int value) {
    this->maxValueArea = value;
    ui->tbMax->setValue(value);
    ui->lbMax->setText(QString::number(value));
}

int MainWindow::getThicknessThreshold() {
    return this->thicknessTrack;
}

void MainWindow::setThicknessThreshold(int value) {
    this->thicknessTrack = value;
    ui->tbTrack->setValue(value);
    ui->lbTrack->setText(QString::number(value));
}

bool MainWindow::getScreen() {
    return this->screen;
}

void MainWindow::setScreen(bool value) {
    this->screen = value;
}

void MainWindow::createButtonTela() {

    QObject::connect(ui->tbTela, SIGNAL(triggered(QAction*)), ui->tbTela, SLOT(setDefaultAction(QAction*)));

    showPerspective = new QAction("Mostrar Perspectiva", this);
    showOriginal = new QAction("Mostrar Original", this);

    //Add icons
    QPixmap pixmapP(":/perspective.png");
    QIcon PerspectiveIcon(pixmapP);
    showPerspective->setIcon(PerspectiveIcon);

    QPixmap pixmapO(":/original.png");
    QIcon OriginalIcon(pixmapO);
    showOriginal->setIcon(OriginalIcon);

    QObject::connect(showPerspective, SIGNAL(triggered()), this, SLOT(activePerspective()));
    QObject::connect(showOriginal, SIGNAL(triggered()), this, SLOT(activeOriginal()));

    //Creating menu
    screenMenu = new QMenu();
    screenMenu->addAction(showOriginal);
    screenMenu->addAction(showPerspective);
    ui->tbTela->setMenu(screenMenu);
    ui->tbTela->setDefaultAction(showOriginal);
}

void MainWindow::activeOriginal() {

    ui->tbTela->setDefaultAction(showOriginal);
    this->setScreen(true);

}

void MainWindow::activePerspective() {

    ui->tbTela->setDefaultAction(showPerspective);
    this->setScreen(false);
}

void MainWindow::on_tbThresh_valueChanged(int value) {
    this->setValueThreshold(value);
}

void MainWindow::on_tbMin_valueChanged(int value) {
    this->setMinValueArea(value);
}

void MainWindow::on_tbMax_valueChanged(int value) {
    this->setMaxValueArea(value);
}

void MainWindow::on_tbTrack_valueChanged(int value) {
    this->setThicknessThreshold(value);
}

void MainWindow::inputNameTeste() {

    //Escondendo labels
    ui->lbAnimal->hide();
    ui->lbTeste->hide();

    //Criando caixas de texto
    if (!animalInput) animalInput = new QLineEdit;
    animalInput->setPlaceholderText("Digite o nome do animal...");
    animalInput->setFocus();

    if (!testeInput) testeInput = new QLineEdit;
    testeInput->setPlaceholderText("Digite o numero do teste...");
    testeInput->setFocus();

    QFont font("", 0);
    QFontMetrics fm(font);
    animalInput->setFixedSize(fm.width("Digite o nome do animal..."), fm.height());
    testeInput->setFixedSize(fm.width("Digite o numero do teste..."), fm.height());

    //Adicionando ao layout
    ui->horizontalLayout_2->addWidget(animalInput, 0, 0);
    ui->horizontalLayout_3->addWidget(testeInput);
}

void MainWindow::setFileName() {

    ui->lbFile->setEnabled(true);
    Video* cap = this->captureVideos[0];
    QString filename = cap->getFileName();
    filename = filename.section("/", -1, -1);
    ui->lbFile->setText(filename);
}

void MainWindow::on_btFile_clicked() {

    //Blockiing buttons gui
    if(ui->frameTools->isEnabled() == true) ui->frameTools->setEnabled(false);
    ui->btDir->setEnabled(false);
    ui->btCamConfig->setEnabled(false);

    //Getting video
    QString argv = QFileDialog::getOpenFileName(this, tr("Escolha o video para ser analisado"), QString(), tr("Video Files (*.avi)"));
    Video* cap = new Video(argv);
    this->addVideo(cap);

    //Setting file name
    this->setFileName();

    //Realising tools
    ui->btIniciar->setEnabled(true);
    ui->label->setEnabled(true);
    ui->label_2->setEnabled(true);
    ui->label_4->setEnabled(true);

    //Changing labels to input text box
    this->inputNameTeste();

}

void MainWindow::on_btDir_clicked() {

    //Blocking buttons gui
    if(ui->frameTools->isEnabled() == true) ui->frameTools->setEnabled(false);
    ui->btFile->setEnabled(false);
    ui->btCamConfig->setEnabled(false);

    //Reading directory
    this->readDirectoryVideos();

    //Setting filename on gui
    this->setFileName();

    //Changing label to input text box
    this->inputNameTeste();

    //Releasing tools
    ui->btIniciar->setEnabled(true);
    ui->label->setEnabled(true);
    ui->label_2->setEnabled(true);
    ui->label_4->setEnabled(true);

}

void MainWindow::on_btIniciar_clicked() {

    //Conditions to initiate test
    Video* cap = this->captureVideos[0];
    if(cap->getFileName() == NULL)
        QMessageBox::critical(this, tr("Erro"), tr("Por favor, selecione o arquivo de video."));
    else if(animalInput->text().isEmpty() || testeInput->text().isEmpty())
        QMessageBox::critical(this, tr("Erro"), tr ("Por favor, preencha os campos 'Animal' e 'Teste'."));
    else {

        //Getting test infos
        animal = animalInput->text();
        teste = testeInput->text();

        //Modifying GUI
        delete animalInput;
        delete testeInput;
        animalInput = NULL;
        testeInput = NULL;

        //Blocking buttons
        ui->btDir->setEnabled(true);
        ui->btFile->setEnabled(true);

        //Changing GUI
        ui->lbAnimal->show();
        ui->lbAnimal->setEnabled(true);
        ui->lbAnimal->setText(animal);
        ui->lbTeste->show();
        ui->lbTeste->setEnabled(true);
        ui->lbTeste->setText(teste);
        ui->label_5->show();
        ui->lbStatus->show();
        ui->lbStatus->setText("Iniciando teste...");
        this->createButtonTela();

        //Blocking gui configurations
        ui->btIniciar->setEnabled(false);
        ui->btCamConfig->setEnabled(false);

        //Releasing video tools
        ui->frameTools->setEnabled(true);
        ui->tbTela->setEnabled(true);
        ui->btPlay->setEnabled(true);
        ui->btReset->setEnabled(true);
        ui->btSnap->setEnabled(true);

        //Reading video
        this->readVideo(this->captureVideos[0]);

        //Creating tracker
        if(this->tracker == NULL)
            this->tracker = new Tracker(0, this->animal, this->teste, this->captureVideos[0]);

        connect(this->tracker->getTimer(), SIGNAL(timeout()), this->tracker, SLOT(processVideo()));

        //Initiate tracker
        this->tracker->executeTracker();
    }
}

void MainWindow::on_btReset_clicked() {
    this->tracker->resetCaptureVideo();
}

void MainWindow::on_btPlay_clicked() {
    this->tracker->pauseVideo();
}

void MainWindow::on_btCamConfig_clicked() {
    this->hide();
    camCalibration = new CalibrationGUI();
    camCalibration->show();
}

void MainWindow::on_btSnap_clicked() {
    this->tracker->saveSnapshot();
}

void MainWindow::resetInterface() {

    //Full screen and black background
    this->showMaximized();
    ui->lbOriginal->setStyleSheet("background-color: rgb(0, 0, 0);");

    //Disabled buttons
    ui->tbTela->setEnabled(false);
    ui->btPlay->setEnabled(false);
    ui->btReset->setEnabled(false);
    ui->btSnap->setEnabled(false);
    ui->frameTools->setEnabled(false);

    ui->btIniciar->setEnabled(false);

    ui->label->setEnabled(false);
    ui->lbAnimal->setEnabled(false);

    ui->label_2->setEnabled(false);
    ui->lbTeste->setEnabled(false);

    ui->label_4->setEnabled(false);
    ui->lbFile->setEnabled(false);

    ui->lbAnimal->setText("");
    ui->lbFile->setText("");
    ui->lbTeste->setText("");

    ui->btCamConfig->setEnabled(true);
}

void MainWindow::closeTest() {

    //Removing analyzed video
    this->removeVideo();

    //Black background
    Mat show = Mat::zeros(704, 480, CV_8UC3);
    this->showImage(show);

    //Delete tracker
    this->tracker = NULL;

    //Checking for more tests
    if(this->captureVideos.size() > 0) {

        //Preparing GUI for next test
        ui->frameTools->setEnabled(false);
        ui->btFile->setEnabled(false);
        ui->btIniciar->setEnabled(true);
        ui->label->setEnabled(true);
        ui->label_2->setEnabled(true);
        ui->label_4->setEnabled(true);

        //Changing label for input text box
        this->inputNameTeste();

        //Setting new file name on gui
        this->setFileName();

        //Setting status
        this->setStatus("Aguardando usuario...");

        //User info
        QMessageBox::information(this, tr("Novo video"), tr ("Por favor, preencha os campos 'Animal' e 'Teste' e inicie o"
                                                             " proximo video clicando no botao 'Iniciar Teste'"));
    }else{

        //Reset interface
        this->resetInterface();

        //User info
        QMessageBox::information(this, tr("Fim de teste!"), tr("A analise acabou!"));
    }
}

void MainWindow::on_btAdvancedSettings_clicked() {
    setAdvancedSettings();
}

bool MainWindow::existsCalibration() {
    this->reader = new ReaderXml();
    return this->reader->exists(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME);
}
