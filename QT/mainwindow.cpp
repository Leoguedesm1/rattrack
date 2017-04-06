#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

double raio = 112;
const string XML_FILE_NAME = "/image_list.xml";
const string YML_FILE_NAME = "/calib.yml";
const string CALIBRATION_DIR_NAME = ((string) (QDir::currentPath()).toUtf8().constData()) + "/Calibration";

const double INF = std::numeric_limits<double>::infinity();

//Definindo valores de pixels BRANCO e PRETO
#define WHITE 255
#define BLACK 0

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    coordX = 0;
    coordY = 0;
    tmrTimer = NULL;
    original = 1;
    perspective = 0;
    cont_track = 0;
    numberFiles = 0;
    contFiles = INF;

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

void MainWindow::on_btFile_clicked()
{

    reset_interface();

    if(ui->frameTools->isEnabled() == true) ui->frameTools->setEnabled(false);
    ui->btDir->setEnabled(false);
    ui->btCamConfig->setEnabled(false);

    //Lendo arquivo de video
    argv = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Video Files (*.avi)"));

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

void MainWindow::on_btDir_clicked()
{
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

void MainWindow::on_btIniciar_clicked()
{
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

void MainWindow::on_btReset_clicked()
{
    reset_video();
}

void MainWindow::on_btPlay_clicked()
{
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

void MainWindow::on_tbThresh_valueChanged(int value)
{
    threshold_value = value;
    ui->lbThresh->setText(QString::number(threshold_value));
}

void MainWindow::on_tbMin_valueChanged(int value)
{
    min_area = value;
    ui->lbMin->setText(QString::number(min_area));
}

void MainWindow::on_tbMax_valueChanged(int value)
{
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
    ui->cbOriginal->setChecked(false);
    ui->cbPerspectiva->setChecked(false);
    ui->cbPath->setChecked(false);
    ui->cbIA->setChecked(false);
    ui->cbTela->setChecked(false);
    ui->btCamConfig->setEnabled(true);

    ui->lbStatus->setText("Carregando arquivos...");
}

void MainWindow::on_btCamConfig_clicked()
{
    setCalibrationCamera();
}

void MainWindow::on_btSair_clicked()
{
    exit(1);
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

    coordX = 0;
    coordY = 0;
    total_frames = 0;
    fps = 0;
    FHEIGHT = 0;
    FWIDTH = 0;
    cont_track = 0;
    H2_warp = Scalar(0);
    invH_warp = Scalar(0);
    src_frame = Scalar(0);
    perspective_frame = Scalar(0);
    original_frame = Scalar(0);
    p = Scalar(0);
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

void MainWindow::acha_perspectiva(Mat& transform_image, Mat& H2_warp, Size warpSize, int FHEIGHT, int FWIDTH) {

    src >> src_frame;

    Mat H, src_gray;

    cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY); //Passando frame do vídeo para GrayScale


    /*Aplicando a perspectiva nos pontos brancos = (275, 159) (461, 162) (477, 315) (262, 322)*/
    /*Vetores para encontrar a Homography*/
    vector<Point2f> inputQuad, outputQuad;
    /*Vetores para reaplicar a Homography e ajustar a imagem na tela*/
    vector<Point2f> cornerQuad, finalCornerQuad;
    /*Vetores para encontrar o centro do círculo*/
    vector<Point2f> center, centerTrans;

    /*Encontrando a Homography*/
    /*Pontos para serem modificados*/
    inputQuad.push_back(Point2f(275, 159));
    inputQuad.push_back(Point2f(461, 162));
    inputQuad.push_back(Point2f(477, 315));
    inputQuad.push_back(Point2f(262, 322));

    /*Nova localização dos pontos*/
    outputQuad.push_back(Point2f(199, 199));
    outputQuad.push_back(Point2f(301, 202));
    outputQuad.push_back(Point2f(302, 298));
    outputQuad.push_back(Point2f(199, 302));

    H = findHomography(inputQuad, outputQuad, LMEDS);

    /*Encontrando a nova perspectiva gerada pela Homography*/
    cornerQuad.push_back(Point2f(0, 0));
    cornerQuad.push_back(Point2f(src_gray.cols, 0));
    cornerQuad.push_back(Point2f(src_gray.cols, src_gray.rows));
    cornerQuad.push_back(Point2f(0, src_gray.rows));

    perspectiveTransform(cornerQuad, finalCornerQuad, H);

    /*Achando os pontos mínimos e máximos de cada eixo para encontrar o tamanho da nova imagem
    e rotaciona-la com uma nova Homography*/
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
        finalCornerQuad[i].x =  (finalCornerQuad[i].x - minx)/( (float) wwidth ) * FWIDTH;
        finalCornerQuad[i].y = (finalCornerQuad[i].y - miny)/( (float) wheight ) * FHEIGHT;
    }

    H2_warp = findHomography(cornerQuad, finalCornerQuad, LMEDS);

    warpPerspective(src_gray, transform_image, H2_warp, warpSize);

    /*Pegando o centro do círculo da imagem transformada*/
    centerTrans.push_back(Point2f((float)240/( (float) wwidth)*FWIDTH, (float)189.5/((float) wheight)*FHEIGHT));

    invH_warp = findHomography(finalCornerQuad, cornerQuad, LMEDS);

    perspectiveTransform(centerTrans, center, invH_warp);
    perspectiveTransform(center, centerTrans, H2_warp);
    centerX = centerTrans[0].x;
    centerY = centerTrans[0].y;

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
        encerra_video();
        //QMessageBox::information(this, tr ("Teste"), tr("O video terminou."));
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

        //Setando status
        stringstream status1;
        status1 << "Rato nao encontrado! Ultimas coordenadas: (" << coordX << ", " << coordY << ").";
        string status = status1.str();
        ui->lbStatus->setText(QString::fromStdString(status));

        //Adicionando coordenadas
        coordinates.push_back(Point2f(0, 0));

        //Configurando a imagem na tela
        Mat p = paint();
        addWeighted(perspective_frame, 1, p, 1, 0.0, perspective_frame); //Adicionando tracking a imagem

        //Aplicando perspectiva
        warpPerspective(perspective_frame, original_frame, invH_warp, src_frame.size());
        warpPerspective(p, p, invH_warp, p.size());

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


        //Setando status
        coordX = mc[i].x;
        coordY = mc[i].y;

        //Adicionando coordenadas
        coordinates.push_back(Point2f(coordX, coordY));

        stringstream status1;
        status1 << "Rato encontrado! Coordenadas: (" << coordX << ", " << coordY << ").";
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
        warpPerspective(perspective_frame, original_frame, invH_warp, src_frame.size()); //Aplicando perspectiva

        //Criando imagem da tela
        original_frame.copyTo(tela_image(Rect(0, 0, original_frame.cols, original_frame.rows)));
        perspective_frame.copyTo(tela_image(Rect(original_frame.cols, 0, perspective_frame.cols, perspective_frame.rows)));

        //Mostrando na tela
        if(original == 1) mostrar_imagem(original_frame);
        else mostrar_imagem(perspective_frame);

    }
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

    FHEIGHT = src.get(CV_CAP_PROP_FRAME_HEIGHT);
    FWIDTH = src.get(CV_CAP_PROP_FRAME_WIDTH);

    transform_image = Mat::zeros( FHEIGHT, FWIDTH, CV_8U );
    tela_image = Mat(FHEIGHT, FWIDTH*2, CV_8UC3, Scalar(0));
    track_image = Mat(FHEIGHT, FWIDTH, CV_32S);

    warpSize = Size(FWIDTH, FHEIGHT);

    //Calcula a Matriz H2
    acha_perspectiva(transform_image, H2_warp, warpSize, FHEIGHT, FWIDTH);
    reset_video();

    //Aplica a matriz H2 em cada frame do video
    tmrTimer = new QTimer();
    QObject::connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processa_video()));
    tmrTimer->start(fps);

    waitKey(0); // key press to close window
}

void MainWindow::mostrar_imagem(Mat frame) {

    Mat show_frame;
    cvtColor(frame, show_frame, CV_BGR2RGB);
    cv::resize(show_frame, show_frame, Size(704, 480));
    QImage show_image((const uchar *) show_frame.data, show_frame.cols, show_frame.rows, show_frame.step, QImage::Format_RGB888);
    ui->lbOriginal->setPixmap(QPixmap::fromImage(show_image));
}

void MainWindow::salvar_imagens() {
    QString dirTela = "", dirOriginal = "", dirPerspectiva = "", dirCaminho = "", dirAprendizagem = "";
    string save;

    Mat aprendizagem_image = gera_IA();

    //Original
    if(ui->cbOriginal->isChecked()) {

        ui->lbStatus->setText("Salvando Original...");

        //Gerando nome do arquivo
        stringstream filename;
        filename << animal.toAscii().data() << "_teste_" << teste.toAscii().data() << "_Original.bmp";
        save = filename.str();

        //Salvando
        dirOriginal = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

        //Verificacao do nome modificado
        if(dirOriginal != ".bmp") {
            dirOriginal = dirOriginal.toUtf8().data();
            imwrite(dirOriginal.toAscii().data(), original_frame);
            ui->lbStatus->setText("Original salvo com sucesso!");
        }else cout << "Erro! Por favor insira o nome do arquivo!" << "\n";
    }

    //Perspectiva
    if(ui->cbPerspectiva->isChecked()) {

        ui->lbStatus->setText("Salvando Perspectiva...");

        //Gerando nome do arquivo
        stringstream filename;
        filename << animal.toAscii().data() << "_teste_" << teste.toAscii().data() << "_Perspectiva.bmp";
        save = filename.str();

        //Salvando
        dirPerspectiva = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

        //Verificacao do nome modificado
        if(dirPerspectiva != ".bmp") {
            dirPerspectiva = dirPerspectiva.toUtf8().data();
            imwrite(dirPerspectiva.toAscii().data(), perspective_frame);
            ui->lbStatus->setText("Perspectiva salva com sucesso!");
        }else cout << "Erro! Por favor insira o nome do arquivo!" << "\n";
    }

    //Tela
    if(ui->cbTela->isChecked()) {

        ui->lbStatus->setText("Salvando Tela...");

        //Gerando nome do arquivo
        stringstream filename;
        filename << animal.toAscii().data() << "_teste_" << teste.toAscii().data() << "_Tela.bmp";
        save = filename.str();

        //Salvando
        dirTela = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

        //Verificacao do nome modificado
        if(dirTela != ".bmp") {
            dirTela = dirTela.toUtf8().data();
            imwrite(dirTela.toAscii().data(), tela_image);
            ui->lbStatus->setText("Tela salva com sucesso!");
        }else cout << "Erro! Por favor insira o nome do arquivo!" << "\n";
    }

    //Caminho
    if(ui->cbPath->isChecked()) {

        ui->lbStatus->setText("Salvando Caminho...");

        //Gerando nome do arquivo
        stringstream filename;
        filename << animal.toAscii().data() << "_teste_" << teste.toAscii().data() << "_Caminho.bmp";
        save = filename.str();

        //Salvando
        dirCaminho = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

        //Verificacao do nome modificado
        if(dirCaminho != ".bmp") {
            dirCaminho = dirCaminho.toUtf8().data();
            imwrite(dirCaminho.toAscii().data(), track_image);
            ui->lbStatus->setText("Caminho salvo com sucesso!");
        }else cout << "Erro! Por favor insira o nome do arquivo!" << "\n";
    }

    //Aprendizado
    if(ui->cbIA->isChecked()) {

        ui->lbStatus->setText("Salvando Aprendizado...");

        //Gerando nome do arquivo
        stringstream filename;
        filename << animal.toAscii().data() << "_teste_" << teste.toAscii().data() << "_Aprendizado.bmp";
        save = filename.str();

        //Salvando
        dirAprendizagem = QFileDialog::getSaveFileName(this, tr("Salvar Teste"), QString::fromStdString(save), tr("Image File (*.bmp);;All Files (*)"));

        //Verificacao do nome modificado
        if(dirAprendizagem != ".bmp") {
            dirAprendizagem = dirAprendizagem.toUtf8().data();
            imwrite(dirAprendizagem.toAscii().data(), aprendizagem_image);
            ui->lbStatus->setText("Aprendizado salvo com sucesso!");
        }else cout << "Erro! Por favor insira o nome do arquivo!" << "\n";
    }
}

void MainWindow::encerra_video() {

    salvar_imagens();

    //Verificando se estamos em um diretorio ou apenas um arquivo
    //Arquivo
    if(contFiles == INF) {
        reset_interface();
        QMessageBox::information(this, tr("Fim de teste!"), tr("A analise acabou!"));

    //Diretorio
    }else{

        contFiles++;

        if(contFiles < numberFiles) {

            //Resetar as variaveis do rattrack
            resetar_variaveis();

            //Resetar tela
            ui->lbOriginal->setStyleSheet("background-color: rgb(0, 0, 0);"); //Fundo preto do video
            QImage show_image((const uchar *) src_frame.data, src_frame.cols, src_frame.rows, src_frame.step, QImage::Format_RGB888);
            ui->lbOriginal->setPixmap(QPixmap::fromImage(show_image));

            //Resetar GUI
            ui->frameTools->setEnabled(false);
            ui->cbOriginal->setChecked(false);
            ui->cbPerspectiva->setChecked(false);
            ui->cbPath->setChecked(false);
            ui->cbIA->setChecked(false);
            ui->cbTela->setChecked(false);
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

        }else{
            reset_interface();
            QMessageBox::information(this, tr("Fim de teste!"), tr("A analise acabou!"));

        }
    }
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
void MainWindow::captureImagesCalibration(
        int* numSquares, int* numCornersHor,
        int* numCornersVer, int* numCorners,
        Size* board_sz, vector<Point2f> corners) {

    *numCornersHor = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de esquinas horizontais: "));
    *numCornersVer = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de esquinas verticais: "));
    *numSquares = QInputDialog::getInt(this, tr("Calibracao"), tr("Numero de quadrados: "));

    *numCorners = (*numCornersHor) * (*numCornersVer);
    *board_sz = Size(*numCornersHor, *numCornersVer);

    VideoCapture capture = VideoCapture(0);

    stringstream img;
    img << CALIBRATION_DIR_NAME << "/calib";
    string imageName = img.str();

    int successes=0;

    Mat image;
    Mat gray_image;

    capture >> image;

    //Criando arquivo .xml
    FileStorage fs;
    std::stringstream fileName;
    fileName << CALIBRATION_DIR_NAME << XML_FILE_NAME;
    fs.open(fileName.str(), cv::FileStorage::WRITE);
    int imageSaveIndex = 1;

    fs << "images" << "[";

    QMessageBox::information(this, tr("Calibracao"), tr("Para cada formato de tabuleiro sera necessario capturar uma certa"
                                                        " quantidade de imagens!\nPara captura pressione a tecla ENTER.\n"
                                                        "Para sair pressione a tecla ESC."));

    while(successes < *numSquares) {

        //Encontrando ChessBoardPattern
        cvtColor(image, gray_image, CV_BGR2GRAY);
        bool found = findChessboardCorners(image, *board_sz, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        //Se foi encontrado, desenhamos na imagem
        if(found) {
            cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(image, *board_sz, corners, found);
        }

        string msg = format("Capturando %d/%d", imageSaveIndex, *numSquares/3);

        int baseLine = 0;
        Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
        Point textOrigin(image.cols - 2*textSize.width - 10, image.rows - 2*baseLine - 10);
        putText( image, msg, textOrigin, 1, 1, Scalar(0,255,0));

        imshow("Calib", image);

        capture >> image;

        //Comandos pelo teclado
        int key = waitKey(1);

        //Esc para sair
        if(key==27) return;

        //Enter para capturar
        if(key==13 && found!=0) {

            //Nome da imagem
            std::stringstream filename;
            filename << imageName << "_" << imageSaveIndex << ".bmp";
            std::string dir = filename.str();

            //Salvando imagem em diretorio e no arquivo .xml
            imwrite(dir, gray_image);
            fs << dir;
            imageSaveIndex++;

            successes+=3;

            if(successes>=*numSquares)
                break;
        }
    }

    //Encerrando arquivo .xml
    fs << "]";

    capture.release();
}

double MainWindow::computeReprojectionErrors(
        const vector<vector<Point3f> >& objectPoints,
        const vector<vector<Point2f> >& imagePoints,
        const vector<Mat>& rvecs, const vector<Mat>& tvecs,
        const Mat& cameraMatrix, const Mat& distCoeffs,
        vector<float>& perViewErrors ) {

    vector<Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for( i = 0; i < (int)objectPoints.size(); i++ ) {
        projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i],
                      cameraMatrix, distCoeffs, imagePoints2);
        err = norm(Mat(imagePoints[i]), Mat(imagePoints2), NORM_L2);
        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float)std::sqrt(err*err/n);
        totalErr += err*err;
        totalPoints += n;
    }

    return std::sqrt(totalErr/totalPoints);
}

void MainWindow::calcChessboardCorners(
        Size boardSize,
        float squareSize,
        vector<Point3f>& corners) {
    corners.resize(0);

    for( int i = 0; i < boardSize.height; i++ )
        for( int j = 0; j < boardSize.width; j++ )
            corners.push_back(Point3f(float(j*squareSize),
                                      float(i*squareSize), 0));
}

bool MainWindow::runCalibration(
        vector<vector<Point2f> > imagePoints,
        Size imageSize, Size boardSize,
        float squareSize, float aspectRatio,
        int flags, Mat& cameraMatrix, Mat& distCoeffs,
        vector<Mat>& rvecs, vector<Mat>& tvecs,
        vector<float>& reprojErrs, double& totalAvgErr) {

    cameraMatrix = Mat::eye(3, 3, CV_64F);
    if( flags & CALIB_FIX_ASPECT_RATIO )
        cameraMatrix.at<double>(0,0) = aspectRatio;

    distCoeffs = Mat::zeros(8, 1, CV_64F);

    vector<vector<Point3f> > objectPoints(1);
    calcChessboardCorners(boardSize, squareSize, objectPoints[0]);

    objectPoints.resize(imagePoints.size(),objectPoints[0]);

    double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
                                 distCoeffs, rvecs, tvecs, flags|CALIB_FIX_K4|CALIB_FIX_K5);
    ///*|CALIB_FIX_K3*/|CALIB_FIX_K4|CALIB_FIX_K5);

    cout << "RMS error reported by calibrateCamera: " << rms << "\n";

    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

    totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints,
                                            rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

    return ok;
}

void MainWindow::saveCameraParams(
        const string& filename, Size imageSize, Size boardSize,
        float squareSize, float aspectRatio, int flags,
        const Mat& cameraMatrix, const Mat& distCoeffs,
        const vector<Mat>& rvecs, const vector<Mat>& tvecs,
        const vector<float>& reprojErrs,
        const vector<vector<Point2f> >& imagePoints,
        double totalAvgErr ) {

    std::cout << "Writing to " << filename << std::endl;
    FileStorage fs( filename, FileStorage::WRITE );

    time_t tt;
    time( &tt );
    struct tm *t2 = localtime( &tt );
    char buf[1024];
    strftime( buf, sizeof(buf)-1, "%c", t2 );

    fs << "calibration_time" << buf;

    if( !rvecs.empty() || !reprojErrs.empty() )
        fs << "nframes" << (int)std::max(rvecs.size(), reprojErrs.size());
    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;
    fs << "board_width" << boardSize.width;
    fs << "board_height" << boardSize.height;
    fs << "square_size" << squareSize;

    if( flags & CALIB_FIX_ASPECT_RATIO )
        fs << "aspectRatio" << aspectRatio;

    if( flags != 0 )
    {
        sprintf( buf, "flags: %s%s%s%s",
                 flags & CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
                 flags & CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
                 flags & CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
                 flags & CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "" );
        //cvWriteComment( *fs, buf, 0 );
    }

    fs << "flags" << flags;

    fs << "camera_matrix" << cameraMatrix;
    fs << "distortion_coefficients" << distCoeffs;

    fs << "avg_reprojection_error" << totalAvgErr;
    if( !reprojErrs.empty() )
        fs << "per_view_reprojection_errors" << Mat(reprojErrs);

    if( !rvecs.empty() && !tvecs.empty() )
    {
        CV_Assert(rvecs[0].type() == tvecs[0].type());
        Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
        for( int i = 0; i < (int)rvecs.size(); i++ )
        {
            Mat r = bigmat(Range(i, i+1), Range(0,3));
            Mat t = bigmat(Range(i, i+1), Range(3,6));

            CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
            CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
            //*.t() is MatExpr (not Mat) so we can use assignment operator
            r = rvecs[i].t();
            t = tvecs[i].t();
        }
        //cvWriteComment( *fs, "a set of 6-tuples (rotation vector + translation vector) for each view", 0 );
        fs << "extrinsic_parameters" << bigmat;
    }

    if( !imagePoints.empty() )
    {
        Mat imagePtMat((int)imagePoints.size(), (int)imagePoints[0].size(), CV_32FC2);
        for( int i = 0; i < (int)imagePoints.size(); i++ )
        {
            Mat r = imagePtMat.row(i).reshape(2, imagePtMat.cols);
            Mat imgpti(imagePoints[i]);
            imgpti.copyTo(r);
        }
        fs << "image_points" << imagePtMat;
    }
}

bool MainWindow::readStringList(
        const string& filename, vector<string>& l ) {

    l.resize(0);
    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;
    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
        return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        l.push_back((string)*it);
    return true;
}

bool MainWindow::runAndSave(
        const string& outputFilename,
        const vector<vector<Point2f> >& imagePoints,
        Size imageSize, Size boardSize, float squareSize,
        float aspectRatio, int flags, Mat& cameraMatrix,
        Mat& distCoeffs, bool writeExtrinsics, bool writePoints ) {

    vector<Mat> rvecs, tvecs;
    vector<float> reprojErrs;
    double totalAvgErr = 0;

    bool ok = runCalibration(imagePoints, imageSize, boardSize, squareSize,
                             aspectRatio, flags, cameraMatrix, distCoeffs,
                             rvecs, tvecs, reprojErrs, totalAvgErr);

    printf("%s. avg reprojection error = %.2f\n",
           ok ? "Calibration succeeded" : "Calibration failed",
           totalAvgErr);

    if( ok )
        saveCameraParams( outputFilename, imageSize,
                          boardSize, squareSize, aspectRatio,
                          flags, cameraMatrix, distCoeffs,
                          writeExtrinsics ? rvecs : vector<Mat>(),
                          writeExtrinsics ? tvecs : vector<Mat>(),
                          writeExtrinsics ? reprojErrs : vector<float>(),
                          writePoints ? imagePoints : vector<vector<Point2f> >(),
                          totalAvgErr );
    return ok;
}

bool MainWindow::getCalibrationCamera(Size* boardSize, vector<Point2f> corners, string getErro) {
    Size imageSize;
    float squareSize = 5, aspectRatio = 1;
    Mat cameraMatrix, distCoeffs;
    string outputFilename = CALIBRATION_DIR_NAME + YML_FILE_NAME;
    string inputFilename = CALIBRATION_DIR_NAME + XML_FILE_NAME;

    int i, nframes = 10;
    bool writeExtrinsics = true, writePoints = true;
    int flags = 0;
    int delay = 1000;
    clock_t prevTimestamp = 0;
    int mode = DETECTION;
    vector<vector<Point2f> > imagePoints;
    vector<string> imageList;

    flags |= CALIB_FIX_ASPECT_RATIO;
    flags |= CALIB_ZERO_TANGENT_DIST;
    flags |= CALIB_FIX_PRINCIPAL_POINT;

    if( !inputFilename.empty() ) {
        if( readStringList(inputFilename, imageList) )
            mode = CAPTURING;
    }

    if(imageList.empty()) {
        getErro = "Lista de imagens vazias!";
        return true;
    }

    if( !imageList.empty() )
        nframes = (int)imageList.size();

    for(i = 0;;i++) {
        Mat view, viewGray;

        if( i < (int)imageList.size() )
            view = imread(imageList[i], 1);

        if(view.empty()) {
            if( imagePoints.size() > 0 )
                runAndSave(outputFilename, imagePoints, imageSize,
                           *boardSize, squareSize, aspectRatio,
                           flags, cameraMatrix, distCoeffs,
                           writeExtrinsics, writePoints);
            break;
        }

        imageSize = view.size();

        cvtColor(view, viewGray, COLOR_BGR2GRAY);

        bool found;
        found = findChessboardCorners( view, *boardSize, corners,
                                       CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        // improve the found corners' coordinate accuracy
        if(found) cornerSubPix( viewGray, corners, Size(11,11),
                                Size(-1,-1), TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.1 ));

        if(mode == CAPTURING && found && clock() - prevTimestamp > delay*1e-3*CLOCKS_PER_SEC) {
            imagePoints.push_back(corners);
            prevTimestamp = clock();
        }

        if(found)
            drawChessboardCorners( view, *boardSize, Mat(corners), found );

        imshow("Calib", view);

        if( mode == CAPTURING && imagePoints.size() >= (unsigned)nframes ) {
            if( runAndSave(outputFilename, imagePoints, imageSize,
                           *boardSize, squareSize, aspectRatio,
                           flags, cameraMatrix, distCoeffs,
                           writeExtrinsics, writePoints))
                mode = CALIBRATED;
            else
                mode = DETECTION;
        }

        waitKey(50);
    }

    return false;
}

bool MainWindow::getHomographyMatrix(Size *board_sz, int* numCornersHor, int* numCornersVer, int* numCorners, vector<Point2f> corners, string getErro) {
    string filename = CALIBRATION_DIR_NAME + YML_FILE_NAME;

    FileStorage fs(filename, FileStorage::READ);

    Mat intrinsic, distortion;

    fs["camera_matrix"] >> intrinsic;
    fs["distortion_coefficients"] >> distortion;

    if( !fs.isOpened() || intrinsic.empty() || distortion.empty() ) {
        getErro = "Nao foi possivel carregar os intrinsic parameters do arquivo " + YML_FILE_NAME + "!\n";
        return true;
    }

    fs.release();

    vector<string> imageList;
    unsigned int indexImageList = 0;
    Mat gray_image, image, image0;

    if(readStringList(CALIBRATION_DIR_NAME + XML_FILE_NAME, imageList))
        image0 = imread(imageList[indexImageList]);

    if( image0.empty() ) {
        getErro = "Nao foi possivel carregar a imagem " + imageList[indexImageList] + "!\n";
        return true;
    }

    bool found = false;
    while(!found || indexImageList < imageList.size()) {
        //UNDISTORT OUR IMAGE
        undistort(image0, image, intrinsic, distortion, intrinsic);
        cvtColor(image, gray_image, COLOR_BGR2GRAY);

        //GET THE CHECKERBOARD ON THE PLANE
        bool found = findChessboardCorners(image, *board_sz, corners,
                                           CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);

        if(!found) {
            getErro = "Nao foi possivel encontrar totalmente a chessboard pattern em " + imageList[indexImageList] +
                    ", encontrado apenas " + corners.size() + " de " + *numCorners + " corners!\n";
            return true;
        }else
            break;

        indexImageList++;

        if(indexImageList == imageList.size()) {
            getErro = "Impossivel encontrar a calibracao!";
            return true;
        }
    }

    //Get Subpixel accuracy on those corners
    cornerSubPix(gray_image, corners, Size(11,11),Size(-1,-1), TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.1 ));

    //GET THE IMAGE AND OBJECT POINTS:
    //Object points are at (r,c): (0,0), (*numCornersHor-1,0), (0,*numCornersVer-1), (*numCornersHor-1,*numCornersVer-1)
    //That means corners are at: corners[r**numCornersHor + c]
    Point2f objPts[4], imgPts[4];
    objPts[0].x = 0;         objPts[0].y = 0;
    objPts[1].x = *numCornersHor-1; objPts[1].y = 0;
    objPts[2].x = 0;         objPts[2].y = *numCornersVer-1;
    objPts[3].x = *numCornersHor-1; objPts[3].y = *numCornersVer-1;
    imgPts[0] = corners[0];
    imgPts[1] = corners[*numCornersHor-1];
    imgPts[2] = corners[(*numCornersVer-1)*(*numCornersHor)];
    imgPts[3] = corners[(*numCornersVer-1)*(*numCornersHor) + *numCornersHor-1];

    //DRAW THE POINTS in order: B,G,R,YELLOW
    circle(image,imgPts[0],9,Scalar(255,0,0),3);
    circle(image,imgPts[1],9,Scalar(0,255,0),3);
    circle(image,imgPts[2],9,Scalar(0,0,255),3);
    circle(image,imgPts[3],9,Scalar(0,255,255),3);

    //DRAW THE FOUND CHECKERBOARD
    drawChessboardCorners(image, *board_sz, corners, found);
    //imshow( "Checkers", image );

    //FIND THE HOMOGRAPHY
    Mat H = getPerspectiveTransform(objPts, imgPts);

    //LET THE USER ADJUST THE Z HEIGHT OF THE VIEW
    double Z = 25;
    Mat birds_image;
    for(;;) {//escape key stops
        H.at<double>(2, 2) = Z;
        //USE HOMOGRAPHY TO REMAP THE VIEW
        warpPerspective(image, birds_image, H, image.size(), WARP_INVERSE_MAP + INTER_LINEAR,
                        BORDER_CONSTANT, Scalar::all(0));
        imshow("Calib", birds_image);
        int key = waitKey() & 255;
        if(key == 'u') Z += 0.5;
        if(key == 'd') Z -= 0.5;
        if(key == 27) break;
    }

    //SHOW ROTATION AND TRANSLATION VECTORS
    vector<Point2f> image_points;
    vector<Point3f> object_points;
    for(int i=0;i<4;++i){
        image_points.push_back(imgPts[i]);
        object_points.push_back(Point3f(objPts[i].x, objPts[i].y, 0));
    }

    Mat rvec, tvec, rmat;
    solvePnP(object_points, image_points, intrinsic, Mat(), rvec, tvec);
    Rodrigues(rvec, rmat);

    // PRINT AND EXIT
    cout << "rotation matrix: " << rmat << endl;
    cout << "translation vector: " << tvec << endl;
    cout << "homography matrix: " << H << endl;
    cout << "inverted homography matrix: " << H.inv() << endl;

    return false;
}

void MainWindow::setCalibrationCamera() {

    int numSquares = 0, numCornersHor, numCornersVer, numCorners;
    Size board_sz;
    vector<Point2f> corners;
    QDir dir(QString::fromStdString((string) CALIBRATION_DIR_NAME));
    bool erro = false;
    string getErro;

    QMessageBox msgBox;
    msgBox.setText("Bem vindo ao assistente de calibracao!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int op = msgBox.exec();

    switch(op) {
        case QMessageBox::Ok:

            //Criando Pasta para armazenar os arquivos de calibracao
            if(!dir.exists())
                QDir().mkdir(dir.absolutePath());

            //Detectando e salvando imagens para calibracao
            captureImagesCalibration(&numSquares, &numCornersHor, &numCornersVer, &numCorners, &board_sz, corners);

            //Calibracao
            erro = getCalibrationCamera(&board_sz, corners, getErro);

            if(erro) {
                QMessageBox::critical(this, tr("Calibracao"), tr(getErro.c_str()));
                return;
            }

            //Homography
            erro = getHomographyMatrix(&board_sz, &numCornersHor, &numCornersVer, &numCorners, corners, getErro);

            if(erro) {
                QMessageBox::critical(this, tr("Calibracao"), tr(getErro.c_str()));
                return;
            }

            QMessageBox::information(this, tr("Calibracao"), tr("Calibracao finalizada com sucesso!"));
            break;

        case QMessageBox::Cancel:
            break;
    }
}

