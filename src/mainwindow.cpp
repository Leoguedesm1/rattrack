#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    //Full screen and black background
    this->showMaximized();
    ui->lbOriginal->setStyleSheet("background-color: rgb(0, 0, 0);");

    //Disabled buttons
    ui->tbTela->setEnabled(false);
    ui->frameImagens->setEnabled(false);
    ui->frameTools->setEnabled(false);
    ui->btIniciar->setEnabled(false);
    ui->label->setEnabled(false);
    ui->lbAnimal->setEnabled(false);
    ui->label_2->setEnabled(false);
    ui->lbTeste->setEnabled(false);
    ui->label_4->setEnabled(false);
    ui->lbFile->setEnabled(false);
    ui->label_5->hide();
    ui->lbStatus->hide();

    //Creating tracker object
    this->tracker = new Tracker();

    snapshot = 0;
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

void MainWindow::readVideo(Video* v) {

    //Open Video
    v->getCaptureVideo().open(v->getFileName().toAscii().data());

    //Verifying if video has opened - "FALTA FAZER UM TIPO DE RETURN"
    if(!(v->getCaptureVideo().isOpened()))
        throw "Error when reading steam_avi";

    //Saving infos about Video
    v->setFPS(v->getCaptureVideo().get(CAP_PROP_FPS));
    v->setTotalFrames((int) v->getCaptureVideo().get(CAP_PROP_FRAME_COUNT));
    v->setHeight(v->getCaptureVideo().get(CV_CAP_PROP_FRAME_HEIGHT));
    v->setWidth(v->getCaptureVideo().get(CV_CAP_PROP_FRAME_WIDTH));
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

    QObject::connect(this, SIGNAL(triggered(QAction*)), this, SLOT(setDefaultAction(QAction*)));

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

bool MainWindow::getCheckedPathButton() {
    if(ui->cbPath->isChecked())
        return true;
    else
        return false;
}

bool MainWindow::getCheckedIAButton() {
    if(ui->cbIA->isChecked())
        return true;
    else
        return false;
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

    //Adicionando ao layout
    ui->horizontalLayout_3->addWidget(animalInput, 1, 0);
    ui->horizontalLayout_2->addWidget(testeInput, 1, 0);
}

void MainWindow::setFileName(double contFiles) {

    ui->lbFile->setEnabled(true);
    QString filename = fileList.at(contFiles);
    filename = filename.section("/", -1, -1);
    QString nameDir = directory.dirName();
    stringstream namelbFile;
    namelbFile << nameDir.toAscii().data() << "\\" << filename.toAscii().data();
    ui->lbFile->setText(QString::fromStdString(namelbFile.str()));
}

void MainWindow::on_btFile_clicked() {

    reset_interface();

    if(ui->frameTools->isEnabled() == true) ui->frameTools->setEnabled(false);
    ui->btDir->setEnabled(false);
    ui->btCamConfig->setEnabled(false);

    //Lendo arquivo de video
    argv = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Video Files (*.avi)"));
    contFiles = INF;

    //Destravando as configuracoes
    ui->frameImagens->setEnabled(true);
    ui->btIniciar->setEnabled(true);
    ui->label->setEnabled(true);
    ui->label_2->setEnabled(true);
    ui->label_4->setEnabled(true);

    //Indicando nome do arquivo
    ui->lbFile->setEnabled(true);
    QString aux = argv;
    ui->lbFile->setText(aux.section("/", -1, -1));

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

    //Adicionando ao layout
    ui->horizontalLayout_3->addWidget(animalInput, 1, 0);
    ui->horizontalLayout_2->addWidget(testeInput, 1, 0);

}

void MainWindow::on_btDir_clicked() {
    reset_interface();

    if(ui->frameTools->isEnabled() == true) ui->frameTools->setEnabled(false);
    ui->btFile->setEnabled(false);
    ui->btCamConfig->setEnabled(false);

    //Lendo arquivo de video
    le_diretorio();

    //Configurando inicio do teste
    numberFiles = fileList.size();
    contFiles = 0;

    //Indicando nome do arquivo
    setFileName(contFiles);

    //Trocando Label por Caixa de texto para insercao do nome e numero do teste
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

    //Adicionando ao layout
    ui->horizontalLayout_3->addWidget(animalInput, 1, 0);
    ui->horizontalLayout_2->addWidget(testeInput, 1, 0);

    //Destravando as configuracoes
    ui->frameImagens->setEnabled(true);
    ui->btIniciar->setEnabled(true);
    ui->label->setEnabled(true);
    ui->label_2->setEnabled(true);
    ui->label_4->setEnabled(true);

    argv = fileList.at(contFiles);

}

void MainWindow::on_btIniciar_clicked() {
    //Condicoes para poder iniciar o teste...
    if(animalInput->text().isEmpty() || testeInput->text().isEmpty())
        QMessageBox::critical(this, tr("Erro"), tr ("Por favor, preencha os campos 'Animal' e 'Teste'."));
    else {

        //Salvando as variaves nomeAnimal e numeroTeste
        animal = animalInput->text();
        teste = testeInput->text();

        //Excluindo os QLineEdit
        delete animalInput;
        delete testeInput;
        animalInput = NULL;
        testeInput = NULL;

        //Bloqueando botoes
        ui->btDir->setEnabled(true);
        ui->btFile->setEnabled(true);

        //Mostrando os QLabels escondidos com o texto digitado
        ui->lbAnimal->show();
        ui->lbAnimal->setEnabled(true);
        ui->lbAnimal->setText(animal);
        ui->lbTeste->show();
        ui->lbTeste->setEnabled(true);
        ui->lbTeste->setText(teste);
        ui->label_5->show();
        ui->lbStatus->show();
        ui->lbStatus->setText("Iniciando teste...");
        ui->tbTela->setEnabled(true);
        this->createButtonTela();

        //Bloqueando as configuracoes de teste
        ui->frameImagens->setEnabled(false);
        ui->btIniciar->setEnabled(false);

        //Desbloqueando ferramentas
        ui->frameTools->setEnabled(true);

        //Iniciar teste
        try {
            le_video_file(src, argv);
        } catch ( const char * msg) {
            std::cerr << "Got: " << msg << std::endl;
            return;
        }

        rattrack();
    }
}

void MainWindow::on_btReset_clicked() {
    reset_video();
}

void MainWindow::on_btPlay_clicked() {
    playorpause_video();
}

void MainWindow::reset_interface() {
    //Resetar as variaveis do rattrack
    resetar_variaveis();

    //Fundo preto do video
    ui->lbOriginal->setStyleSheet("background-color: rgb(0, 0, 0);"); //Fundo preto do video
    QImage show_image((const uchar *) src_frame.data, src_frame.cols, src_frame.rows, src_frame.step, QImage::Format_RGB888);
    ui->lbOriginal->setPixmap(QPixmap::fromImage(show_image));

    //Bloqueando Ferramentas e resetando opcoes
    ui->frameImagens->setEnabled(false);
    ui->frameTools->setEnabled(false);
    ui->cbVelo->setChecked(false);
    ui->cbAceleracao->setChecked(false);
    ui->cbPath->setChecked(false);
    ui->cbIA->setChecked(false);
    ui->btCamConfig->setEnabled(true);

    ui->lbStatus->setText("Carregando arquivos...");
}

void MainWindow::on_btCamConfig_clicked() {

    //Default Message
    QMessageBox msgBox;
    msgBox.setText("Bem vindo ao assistente de calibracao!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int op = msgBox.exec();

    //Directory
    QDir dir(QString::fromStdString((string) CALIBRATION_DIR_NAME));

    switch(op) {

    case QMessageBox::Ok:

        //Creating calibration directory
        if(!dir.exists())
            QDir().mkdir(dir.absolutePath());

        //Getting image infos for calibration
        int board_w = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de esquinas horizontais: "));
        int board_h = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de esquinas verticais: "));
        int n_boards = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de quadrados: "));
        float measure = QInputDialog::getInt(this, tr("Calibracao"), tr("Medida dos quadrados: "));

        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Video Files (*.avi)"));

        //Giving information to users
        QMessageBox::information(this, tr("Calibracao"), tr("Para cada formato de tabuleiro sera necessario capturar uma "
                                                            "certa quantidade de imagens!\nPara captura pressione a tecla "
                                                            "ENTER.\n""Para sair pressione a tecla ESC."));

        //Creating a new calibration object
        camCalibration = new Calibration(fileName, board_w, board_h, n_boards, measure);

        //Execute Calibration
        camCalibration->executeCalibration();

        //Verifying errors
        if(camCalibration->getError())
            QMessageBox::critical(this, tr("Calibracao"), tr("Houve algum erro inesperado!"));
        else
            QMessageBox::information(this, tr("Calibracao"), tr("Calibracao finalizada com sucesso!"));

        break;

    case QMessageBox::Cancel:
        break;
    }

}

void MainWindow::on_btSair_clicked() {
    exit(1);
}

void MainWindow::on_btSnap_clicked() {
    string save;

    //Criando diretorio para os testes
    QDir dir(QString::fromStdString(TESTES_DIR_NAME));
    if(!dir.exists())
        QDir().mkdir(dir.absolutePath());

    //Criando diretorio para o rato
    QDir dir1(QString::fromStdString(TESTES_DIR_NAME) + "/" + animal);
    if(!dir1.exists())
        QDir().mkdir(dir1.absolutePath());

    //Criando diretorio para o rato
    QDir dir2(QString::fromStdString(TESTES_DIR_NAME) + "/" + animal + "/Snapshots");
    if(!dir2.exists())
        QDir().mkdir(dir2.absolutePath());

    //Gerando nome da imagem
    stringstream filename;
    filename << dir2.absolutePath().toAscii().data() << "/snapshot" << snapshot++ << "_teste_" << teste.toAscii().data() << ".bmp";
    save = filename.str();

    //Salvando imagem
    if(original == 1) imwrite(save, original_frame);
    else imwrite(save, perspective_frame);

}

//RATTRACK FUNCTIONS
void MainWindow::resetar_variaveis() {

    animal = "";
    teste = "";
    argv = "";

    snapshot = 0;
}

void MainWindow::encerra_video() {

    saveImages();

    //Verificando se estamos em um diretorio ou apenas um arquivo
    while(1) {

        //Apenas um arquivo
        if(contFiles == INF) break;

        contFiles++;

        //Ja verificou todos os arquivos do diretorio
        if(contFiles == numberFiles) break;

        //Resetar as variaveis do rattrack
        resetar_variaveis();

        //Resetar tela
        ui->lbOriginal->setStyleSheet("background-color: rgb(0, 0, 0);"); //Fundo preto do video
        QImage show_image((const uchar *) src_frame.data, src_frame.cols, src_frame.rows, src_frame.step, QImage::Format_RGB888);
        ui->lbOriginal->setPixmap(QPixmap::fromImage(show_image));

        //Resetar GUI
        ui->frameTools->setEnabled(false);
        ui->cbVelo->setChecked(false);
        ui->cbAceleracao->setChecked(false);
        ui->cbPath->setChecked(false);
        ui->cbIA->setChecked(false);
        ui->btFile->setEnabled(false);
        ui->btIniciar->setEnabled(true);

        setFileName(contFiles);

        //Trocando Label por Caixa de texto para insercao do nome e numero do teste
        inputNameTeste();

        //Destravando as configuracoes
        ui->frameImagens->setEnabled(true);
        ui->btIniciar->setEnabled(true);
        ui->label->setEnabled(true);
        ui->label_2->setEnabled(true);
        ui->label_4->setEnabled(true);

        argv = fileList.at(contFiles);

        ui->lbStatus->setText("Aguardando usuario...");
        QMessageBox::information(this, tr("Novo video"), tr ("Por favor, preencha os campos 'Animal' e 'Teste' e inicie o proximo video clicando no botao 'Iniciar Teste'"));

    }

    reset_interface();
    QMessageBox::information(this, tr("Fim de teste!"), tr("A analise acabou!"));

}
