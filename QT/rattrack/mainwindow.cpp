#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));

    //Iniciando algumas variaveis
    threshold_value = 130;
    ui->tbThresh->setValue(threshold_value);
    ui->lbThresh->setText(QString::number(threshold_value));
    min_area = 0;
    ui->tbMin->setValue(min_area);
    ui->lbMin->setText(QString::number(min_area));
    max_area = 300;
    ui->tbMax->setValue(max_area);
    ui->lbMax->setText(QString::number(max_area));
    rtrack = 1;
    ui->tbTrack->setValue(rtrack);
    ui->lbTrack->setText(QString::number(rtrack));

    frames = 0;
    tmrTimer = NULL;
    original = 1;
    perspective = 0;
    cont_track = 0;
    numberFiles = 0;
    contFiles = INF;
    snapshot = 0;

    testeInput = NULL;
    animalInput = NULL;

    this->showMaximized(); //Full screen
    ui->lbOriginal->setStyleSheet("background-color: rgb(0, 0, 0);"); //Fundo preto do video

    //Travando os botoes
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

//GUI FUNCTIONS

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
        create_buttonTela();

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

void MainWindow::create_buttonTela() {

    QObject::connect(this, SIGNAL(triggered(QAction*)), this, SLOT(setDefaultAction(QAction*)));

    showPerspective = new QAction("Mostrar Perspectiva", this);
    showOriginal = new QAction("Mostrar Original", this);

    //Adicionando icones
    QPixmap pixmapP(":/perspective.png");
    QIcon PerspectiveIcon(pixmapP);
    showPerspective->setIcon(PerspectiveIcon);

    QPixmap pixmapO(":/original.png");
    QIcon OriginalIcon(pixmapO);
    showOriginal->setIcon(OriginalIcon);

    QObject::connect(showPerspective, SIGNAL(triggered()), this, SLOT(activePerspective()));
    QObject::connect(showOriginal, SIGNAL(triggered()), this, SLOT(activeOriginal()));

    //Criando menu
    tela_menu = new QMenu();
    tela_menu->addAction(showOriginal);
    tela_menu->addAction(showPerspective);
    ui->tbTela->setMenu(tela_menu);
    ui->tbTela->setDefaultAction(showOriginal);
}

void MainWindow::activeOriginal() {

    ui->tbTela->setDefaultAction(showOriginal);
    original = 1;
    perspective = 0;

}

void MainWindow::activePerspective() {

    ui->tbTela->setDefaultAction(showPerspective);
    perspective = 1;
    original = 0;
}

void MainWindow::on_tbThresh_valueChanged(int value) {
    threshold_value = value;
    ui->lbThresh->setText(QString::number(threshold_value));
}

void MainWindow::on_tbMin_valueChanged(int value) {
    min_area = value;
    ui->lbMin->setText(QString::number(min_area));
}

void MainWindow::on_tbMax_valueChanged(int value) {
    max_area = value;
    ui->lbMax->setText(QString::number(max_area));
}

void MainWindow::on_tbTrack_valueChanged(int value) {

    rtrack = value;
    ui->lbTrack->setText(QString::number(rtrack));
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
    assistCalibration();
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
    if(tmrTimer != NULL) {
        tmrTimer->stop();
        delete tmrTimer;
    }

    animal = "";
    teste = "";
    argv = "";

    tmrTimer = NULL;

    total_frames = 0;
    fps = 0;
    FHEIGHT = 0;
    FWIDTH = 0;
    cont_track = 0;
    H2_warp = Scalar(0);
    src_frame = Scalar(0);
    perspective_frame = Scalar(0);
    original_frame = Scalar(0);
    p = Scalar(0);

    snapshot = 0;
}

void MainWindow::le_diretorio() {
    QFileInfo file;

    QString chooseDir = QFileDialog::getExistingDirectory(this, tr("Escolha o diretorio que contenha os videos"), "/home", QFileDialog::DontResolveSymlinks | QFileDialog::ReadOnly);
    QDir workDir(chooseDir);
    workDir.setNameFilters(QStringList()<<"*.avi");
    QFileInfoList files = workDir.entryInfoList();

    foreach (file, files){
        if (file.isFile())
            fileList << file.absoluteFilePath();
    }

    directory = file.dir();
}

void MainWindow::le_video_file(VideoCapture &src, QString argv) {

    src.open(argv.toAscii().data()); //Abrindo video

    //Pegando os frames por segundo e o total de frames do video
    fps = src.get(CAP_PROP_FPS);
    total_frames = (int) src.get(CAP_PROP_FRAME_COUNT);

    //Verificando se o video foi aberto
    if(!src.isOpened() )
        throw "Error when reading steam_avi";
}

void MainWindow::reset_video() {
    cont_track = 0; //Resetando o contador dos pontos
    track_image = Scalar(0); //Resetando a matriz do tracking
    src.set(CAP_PROP_POS_MSEC, 00); //Reiniciando video *alterar*
}

void MainWindow::playorpause_video() {

    if(tmrTimer->isActive() == true) {
        tmrTimer->stop();
        QPixmap pixmap(":/play.png");
        QIcon ButtonIcon(pixmap);
        ui->btPlay->setIcon(ButtonIcon);
        ui->btPlay->setToolTip("Play");
    }else{
        tmrTimer->start(fps);
        QPixmap pixmap(":/pause.png");
        QIcon ButtonIcon(pixmap);
        ui->btPlay->setIcon(ButtonIcon);
        ui->btPlay->setToolTip("Pause");
        ui->lbStatus->setText("Pausado!");
    }

}

void MainWindow::aplica_perspectiva() {
    Mat src_gray;

    if(src_frame.channels() == 1)
        cvtColor(src_frame, src_frame, cv::COLOR_GRAY2RGB);

    /*Passando frame do vídeo para GrayScale*/
    cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY);

    /*Aplicando transformação de perspectiva*/
    warpPerspective(src_gray, transform_image, H2_warp, warpSize);

    /*Copiando matrix de transformação para out*/
    perspective_frame = Mat::zeros( src_frame.rows, src_frame.cols, CV_8U );
    transform_image.copyTo(perspective_frame);
}

void MainWindow::processa_video() {

    src >> src_frame;

    //Mudar para last frame
    if(src_frame.empty()) {
        tmrTimer->stop();
        fileTest.close();
        encerra_video();
    }else{

        aplica_perspectiva();

        Threshold();

        waitKey(20); // waits to display frame
    }
}

void MainWindow::Threshold() {
    Mat aux(perspective_frame.rows, perspective_frame.cols, CV_8U, Scalar(0));

    //Threshold
    for(int i = 0; i < perspective_frame.rows; i++) {
        for(int j = 0; j < perspective_frame.cols; j++) {
            if( ((j-centerX)*(j-centerX)) + ((i-centerY)*(i-centerY)) <= raio*raio) {
                if (perspective_frame.at<uchar>(i, j) < threshold_value)
                    aux.at<uchar>(i, j) = BLACK;
                else
                    aux.at<uchar>(i, j) = WHITE;
            }else
                aux.at<uchar>(i, j) = BLACK;
        }
    }

    medianBlur(aux, aux, 5);
    tracking(aux, track_image);
}

void MainWindow::tracking(Mat aux, Mat track_image) {

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    unsigned int i;
    float area;

    cvtColor(perspective_frame, perspective_frame, CV_GRAY2RGB);
    findContours( aux, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    for(i = 0; i < contours.size(); i++ ){
        area = contourArea(contours[i], false);
        if (area >= min_area && area <= max_area) {
            //cout << "rato de area: " << area << " indice: " << i << "\n";
            break;
        }
    }


    //Caso nao detectarmos o rato
    if (i >= contours.size() || area == 0){

        //Coordinates
        coordAtual = coordBefore;
        coordinates.push_back(coordAtual);
        saveRatInfos(coordBefore, coordAtual, false, fileTest, frames, fps);

        //Setando status
        stringstream status1;
        status1 << "Rato nao encontrado! Ultimas coordenadas: (" << coordAtual.x << ", " << coordAtual.y << ").";
        string status = status1.str();
        ui->lbStatus->setText(QString::fromStdString(status));

        //Configurando a imagem na tela
        Mat p = paint();
        addWeighted(perspective_frame, 1, p, 1, 0.0, perspective_frame); //Adicionando tracking a imagem

        //Aplicando perspectiva
        warpPerspective(perspective_frame, original_frame, H2_warp.inv(), src_frame.size());
        warpPerspective(p, p, H2_warp.inv(), p.size());

        //Mostrando na tela
        if(original == 1) mostrar_imagem(original_frame);
        else mostrar_imagem(perspective_frame);
        return;

        //Caso detectarmos o rato
    }else{

        //Get the moments
        vector<Moments> mu( contours.size() );
        mu[i] = moments( contours[i], false );

        //Get the mass centers:
        vector<Point2f> mc( contours.size() );
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );

        //Coordenadas
        coordBefore = coordAtual;
        coordAtual = mc[i];
        coordinates.push_back(coordAtual);
        saveRatInfos(coordBefore, coordAtual, true, fileTest, frames, fps);

        stringstream status1;
        status1 << "Rato encontrado! Coordenadas: (" << coordAtual.x << ", " << coordAtual.y << ").";
        string status = status1.str();
        ui->lbStatus->setText(QString::fromStdString(status));

        //Desenhando caminho numa matrix
        track_image.at<int>(mc[i].y, mc[i].x) = cont_track;
        cont_track++;
        p = paint();

        //Circulando contornos e destacando centro de massa
        drawContours( perspective_frame, contours, i, Scalar(255,255,0), 1, 8, hierarchy, 0, Point() );
        circle( perspective_frame, mc[i], 2.5, Scalar(0,255,0), -1);

        addWeighted(perspective_frame, 1, p, 1, 0.0, perspective_frame); //Adicionando tracking a imagem
        warpPerspective(perspective_frame, original_frame, H2_warp.inv(), src_frame.size()); //Aplicando perspectiva

        //Criando imagem da tela
        original_frame.copyTo(tela_image(Rect(0, 0, original_frame.cols, original_frame.rows)));
        perspective_frame.copyTo(tela_image(Rect(original_frame.cols, 0, perspective_frame.cols, perspective_frame.rows)));

        //Mostrando na tela
        if(original == 1) mostrar_imagem(original_frame);
        else mostrar_imagem(perspective_frame);

    }

    frames++;
}

Mat MainWindow::paint() {
    Mat painting = Mat::zeros(track_image.rows, track_image.cols, CV_8UC3);
    Vec3b c1 = Vec3b(255,0,0);
    Vec3b c2 = Vec3b(0,255,0);

    for(int i = 0; i < track_image.rows; i++) {
        for(int j = 0; j < track_image.cols; j++) {
            if(track_image.at<int>(i, j) != BLACK) {
                float r = track_image.at<int>(i,j)/( (float) cont_track );
                Vec3b c = (r*c1 + (1-r)*c2);
                if(rtrack == 0)
                    painting.at<Vec3b>(i, j) = c;
                else {
                    Point a;
                    a.x = j;
                    a.y = i;
                    circle( painting, a, rtrack, c, -1);
                }
            }
        }
    }
    return painting;
}

void MainWindow::rattrack() {

    //Coordinates
    coordBefore = coordAtual = Point2d(0,0);

    //Criando diretorio para os testes
    QDir dir(QString::fromStdString(TESTES_DIR_NAME));
    if(!dir.exists())
        QDir().mkdir(dir.absolutePath());

    //Criando diretorio para o rato
    QDir dir1(QString::fromStdString(TESTES_DIR_NAME) + "/" + animal);
    if(!dir1.exists())
        QDir().mkdir(dir1.absolutePath());

    //FileTest
    const string name1= dir1.absolutePath().toAscii().data() + MED_FILE_NAME;
    fileTest.open(name1.c_str());
    fileTest << "frame, tempo, metros, velocidade, aceleracao" << endl;

    //Video informations
    FHEIGHT = src.get(CV_CAP_PROP_FRAME_HEIGHT);
    FWIDTH = src.get(CV_CAP_PROP_FRAME_WIDTH);

    //Mat initialization
    transform_image = Mat::zeros( FHEIGHT, FWIDTH, CV_8U );
    tela_image = Mat(FHEIGHT, FWIDTH*2, CV_8UC3, Scalar(0));
    track_image = Mat(FHEIGHT, FWIDTH, CV_32S);
    warpSize = Size(FWIDTH, FHEIGHT);

    //Resgatando informacoes sobre Homography
    FileStorage fs(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME, FileStorage::READ);
    if( !fs.isOpened()) {
        QMessageBox::critical(this, tr("Erro"), tr("Para realizacao do teste, eh necessario calibrar a camera!\nTeste cancelado!"));
        return;
    }
    fs["homography_matrix"] >> H2_warp;
    if(H2_warp.empty()) {
        QMessageBox::critical(this, tr("Erro"), tr("Impossivel encontrar matriz de calibracao!\nTeste cancelado!"));
        return;
    }
    Point center;
    fs["center_circle"] >> center;
    centerX = center.x;
    centerY = center.y;
    fs["radius"] >> raio;
    fs["pixel_ratio"] >> pixelRatio;
    fs.release();

    //Aplica a matriz H2 em cada frame do video
    tmrTimer = new QTimer();
    QObject::connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processa_video()));
    tmrTimer->start(fps);

    waitKey(0); // key press to close window
}

void MainWindow::saveRatInfos(Point2d before, Point2d atual, bool find, ofstream& fileTest, int frames, double fps) {

    if(frames==0) return;

    //Calc time
    double time = 1.0/fps;

   if(!find) fileTest << frames << ", " << time << ", , , " << endl;
   else {

       //Calc meters
       double xDiff = before.x - atual.x;
       double yDiff = before.y - atual.y;
       double meters = (sqrt((xDiff*xDiff) + (yDiff*yDiff)))*pixelRatio;

       double vel = meters/time;
       double acel = vel/time;

       fileTest << frames << ", " << time << ", " << meters << ", " << vel << ", " << acel << endl;
   }
}

void MainWindow::mostrar_imagem(Mat frame) {

    Mat show_frame;
    cvtColor(frame, show_frame, CV_BGR2RGB);
    cv::resize(show_frame, show_frame, Size(704, 480));
    QImage show_image((const uchar *) show_frame.data, show_frame.cols, show_frame.rows, show_frame.step, QImage::Format_RGB888);
    ui->lbOriginal->setPixmap(QPixmap::fromImage(show_image));
}

void MainWindow::saveImages() {

    string save;

    //Criando diretorio para os testes
    QDir dir(QString::fromStdString(TESTES_DIR_NAME));
    if(!dir.exists())
        QDir().mkdir(dir.absolutePath());

    //Criando diretorio para o rato
    QDir dir1(QString::fromStdString(TESTES_DIR_NAME) + "/" + animal);
    if(!dir1.exists())
        QDir().mkdir(dir1.absolutePath());

    //Caminho
    if(ui->cbPath->isChecked()) {

        ui->lbStatus->setText("Salvando Caminho...");

        //Gerando nome da imagem
        stringstream filename;
        filename << dir1.absolutePath().toAscii().data() << "/caminho_teste_" << teste.toAscii().data() << ".bmp";
        save = filename.str();

        //Salvando imagem
        imwrite(save, track_image);

        ui->lbStatus->setText("Salvo!");
    }

    //Aprendizado
    if(ui->cbIA->isChecked()) {

        ui->lbStatus->setText("Salvando Aprendizado...");
        Mat aprendizagem_image = gera_IA();

        //Gerando nome da imagem
        stringstream filename;
        filename << dir1.absolutePath().toAscii().data() << "/aprendizado_teste_" << teste.toAscii().data() << ".bmp";
        save = filename.str();

        //Salvando imagem
        imwrite(save, aprendizagem_image);

        ui->lbStatus->setText("Aprendizado salvo com sucesso!");
    }
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

Mat MainWindow::gera_IA() {

    Mat IA;
    unsigned int cont = 0;
    Point2f final(coordinates[coordinates.size() - 2].x, coordinates[coordinates.size() - 2].y);

    track_image.copyTo(IA);

    while(cont <= cont_track) {

        line(IA, coordinates[cont], final, Scalar(255,255,255));
        cont+=20;
    }

    return IA;

}

//Calibration functions
void MainWindow::calibrationCamera() {
    int board_w, board_h;
    int n_boards;
    float measure = 25;
    Size imageSize;

    vector< vector< Point2f> > imagePoints;
    vector< vector< Point3f> > objectPoints;

    //NECESSARY INFORMATIONS
    board_w = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de esquinas horizontais: "));
    board_h = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de esquinas verticais: "));
    n_boards = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de quadrados: "));
    measure = QInputDialog::getInt(this, tr("Calibracao"), tr("Medida dos quadrados: "));

    cout << "Bordas horizontais: " << board_w << "\nBordas verticais: " << board_h <<
            "\nNumero de quadrados: " << n_boards << "\nMedida do quadrado: " <<
            measure << "\n";

    //LOAD IMAGES

    //Open video file
    QString argv2 = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Video Files (*.avi)"));
    VideoCapture capture;
    capture.open(argv2.toAscii().data());

    if(!capture.isOpened()){
        QMessageBox::critical(this, tr("Calibracao"), tr("Impossivel ler arquivo de video! Calibração cancelada!"));
        return;
    }

    int successes=0;
    Mat image;
    Mat gray_image;
    vector< Point2f> corners;

    QMessageBox::information(this, tr("Calibracao"), tr("Para cada formato de tabuleiro sera necessario capturar uma certa"
                                                        " quantidade de imagens!\nPara captura pressione a tecla ENTER.\n"
                                                        "Para sair pressione a tecla ESC."));
    while(successes < n_boards) {

        capture >> image;
        cvtColor(image, gray_image, CV_RGB2GRAY);

        //Encontrando ChessBoardPattern
        bool found = findChessboardCorners(gray_image, Size(board_w, board_h), corners);

        //Se foi encontrado preparamos para calibração
        if(found) {
            cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));

            drawChessboardCorners(image, Size(board_w, board_h), corners, found);

            if((int)corners.size() == board_w*board_h) {
                vector< Point2f> v_tImgPT;
                vector< Point3f> v_tObjPT;

                for(int i = 0; i < (int)corners.size(); ++i) {
                    Point2f tImgPT;
                    Point3f tObjPT;

                    tImgPT.x = corners[i].x;
                    tImgPT.y = corners[i].y;

                    tObjPT.x = i%board_w*measure;
                    tObjPT.y = i/board_w*measure;
                    tObjPT.z = 0;

                    v_tImgPT.push_back(tImgPT);
                    v_tObjPT.push_back(tObjPT);
                }

                imagePoints.push_back(v_tImgPT);
                objectPoints.push_back(v_tObjPT);
            }
        }

        //Name of image that going to be save
        stringstream img;
        img << CALIBRATION_DIR_NAME << "/calib" << ++successes << ".bmp";
        string imageName = img.str();

        imageSize = Size(image.cols, image.rows);
        imwrite(imageName, image);
        imshow("calib", image);
        waitKey(10);
    }

    //SAVING INFOS
    saveInfos(imageSize, board_w, board_h, n_boards, measure);

    //CALIBRATION - Don't necessary
    vector< Mat> rvecs, tvecs;
    Mat intrinsic_Matrix(3, 3, CV_64F);
    Mat distortion_coeffs(8, 1, CV_64F);

    calibrateCamera(objectPoints, imagePoints, imageSize, intrinsic_Matrix, distortion_coeffs, rvecs, tvecs);

    //SAVING CALIBRATION
    saveCalibration(intrinsic_Matrix, distortion_coeffs, rvecs, tvecs, objectPoints, imagePoints);

    //HOMOGRAPHY
    vector< Point2f> srcPoints;
    vector< Point2f> dstPoints;
    Point2f point1, point2, point3, point4;

    point1.x = 0;
    point1.y = 0;
    point2.x = 0;
    point2.y = 0;
    point3.x = 0;
    point3.y = 0;
    point4.x = 0;
    point4.y = 0;

    for(int i = 0; i < (int)imagePoints.size(); i++) {

        point1.x = point1.x + (imagePoints[i][board_w-1]).x;
        point1.y = point1.y + (imagePoints[i][board_w-1]).y;
        point2.x = point2.x + (imagePoints[i][0]).x;
        point2.y = point2.y + (imagePoints[i][0]).y;
        point3.x = point3.x + (imagePoints[i][board_w*board_h - (board_w)]).x;
        point3.y = point3.y + (imagePoints[i][board_w*board_h - (board_w)]).y;
        point4.x = point4.x + (imagePoints[i][board_w*board_h - 1]).x;
        point4.y = point4.y + (imagePoints[i][board_w*board_h - 1]).y;
    }

    point1.x = point1.x / (float)n_boards;
    point1.y = point1.y / (float)n_boards;
    point2.x = point2.x / (float)n_boards;
    point2.y = point2.y / (float)n_boards;
    point3.x = point3.x / (float)n_boards;
    point3.y = point3.y / (float)n_boards;
    point4.x = point4.x / (float)n_boards;
    point4.y = point4.y / (float)n_boards;

    srcPoints.push_back(point1);
    srcPoints.push_back(point2);
    srcPoints.push_back(point3);
    srcPoints.push_back(point4);

    int L = 100;
    double pixelRatio = measure / L;

    dstPoints.push_back(Point2f(299, 299));
    dstPoints.push_back(Point2f(299+L, 299));
    dstPoints.push_back(Point2f(299+L, 299-L));
    dstPoints.push_back(Point2f(299, 299-L));

    Mat homography = findHomography(srcPoints, dstPoints, LMEDS);

    vector< Point2f> cornerQuad, finalCornerQuad;

    cornerQuad.push_back(Point2f(0, 0));
    cornerQuad.push_back(Point2f(imageSize.width, 0));
    cornerQuad.push_back(Point2f(imageSize.width, imageSize.height));
    cornerQuad.push_back(Point2f(0, imageSize.height));

    perspectiveTransform(cornerQuad, finalCornerQuad, homography);

    //Achando os pontos mínimos e máximos de cada eixo para encontrar o tamanho da nova imagem
    //* e rotaciona-la com uma nova Homography

    double minx = INF, maxx = -INF, miny = INF, maxy = -INF;
    for (int i=0; i < 4; i++){
        minx = std::min(minx, (double)finalCornerQuad[i].x);
        miny = std::min(miny, (double)finalCornerQuad[i].y);
        maxx = std::max(maxx, (double)finalCornerQuad[i].x);
        maxy = std::max(maxy, (double)finalCornerQuad[i].y);
    }

    int wwidth = maxx - minx;
    int wheight = maxy - miny;

    for (int i=0; i < 4; i++){
        finalCornerQuad[i].x =  (finalCornerQuad[i].x - minx)/( (float) wwidth ) * imageSize.width;
        finalCornerQuad[i].y = (finalCornerQuad[i].y - miny)/( (float) wheight ) * imageSize.height;
    }

    Mat homography2 = findHomography(cornerQuad, finalCornerQuad, LMEDS);

    Mat applyHomography;
    warpPerspective(gray_image, applyHomography, homography2, Size(imageSize.width, imageSize.height));

    vector<Vec3f> circles;
    HoughCircles(applyHomography, circles, HOUGH_GRADIENT, 1, 10, 100, 30, 110, 133);

    Vec3i c = circles[0];

    imshow("calib", applyHomography);
    waitKey();

    imwrite(CALIBRATION_DIR_NAME + "/homographyApply.bmp", applyHomography);

    //SAVING HOMOGRAPHY
    saveHomography(srcPoints, dstPoints, homography2, Point(c[0], c[1]), c[2], pixelRatio);

    destroyWindow("calib");

}

void MainWindow::assistCalibration() {

    QMessageBox msgBox;
    msgBox.setText("Bem vindo ao assistente de calibracao!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int op = msgBox.exec();

    QDir dir(QString::fromStdString((string) CALIBRATION_DIR_NAME));

    switch(op) {

    case QMessageBox::Ok:

        //Criando Pasta para armazenar os arquivos de calibracao
        if(!dir.exists())
            QDir().mkdir(dir.absolutePath());

        //CALIBRATION
        calibrationCamera();

        QMessageBox::information(this, tr("Calibracao"), tr("Calibracao finalizada com sucesso!"));
        break;

    case QMessageBox::Cancel:
        break;
    }
}

void MainWindow::saveInfos(Size imageSize, int board_w, int board_h, int n_boards, float measure) {
    FileStorage fs(CALIBRATION_DIR_NAME + INFO_FILE_NAME, FileStorage::WRITE);
    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;
    fs << "board_width" << board_w;
    fs << "board_height" << board_h;
    fs << "n_boards" << n_boards;
    fs << "square_size" << measure;

    fs.release();
}

void MainWindow::saveCalibration(Mat intrinsic_Matrix, Mat distortion_coeffs, vector< Mat> rvecs, vector< Mat> tvecs,
                                 vector< vector <Point3f> > objectPoints, vector< vector <Point2f> > imagePoints) {

    FileStorage fs(CALIBRATION_DIR_NAME + CALIB_FILE_NAME, FileStorage::WRITE );

    fs << "intrinsic_matrix" << intrinsic_Matrix;
    fs << "distortion_coeffs" << distortion_coeffs;
    fs << "rotation_vector" << rvecs;
    fs << "translation_vector" << tvecs;
    fs << "object_points" << objectPoints;
    fs << "image_points" << imagePoints;

    fs.release();

}

void MainWindow::saveHomography(vector<Point2f> srcPoints, vector<Point2f> dstPoints, Mat homography, Point center, int radius, double pixelRatio) {
    FileStorage fs(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME, FileStorage::WRITE );

    fs << "src_points" << srcPoints;
    fs << "dst_points" << dstPoints;
    fs << "homography_matrix" << homography;
    fs << "center_circle" << center;
    fs << "radius" << radius;
    fs << "pixel_ratio" << pixelRatio;

    fs.release();
}
