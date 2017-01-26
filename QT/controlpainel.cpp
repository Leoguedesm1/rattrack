#include "controlpainel.h"
#include "ui_controlpainel.h"

#include <QLabel>
#include <QWidget>
#include <QFileDialog>

const double INF = std::numeric_limits<double>::infinity();
double raio = 112;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0

VideoCapture src;
userdatareset u;

Mat src_frame, out, out_perspective, tela, track, invH, out_original, transform_mat;
double centerX, centerY;
int cont = 1;
QTimer *tmrTimer;

controlpainel::controlpainel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controlpainel)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->btPlay->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->btReset->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->btConfig->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->btSave->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->btSair->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->btCircle->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->lbNome->setText(QString::fromStdString(animal));
    ui->lbTeste->setText(QString::number(teste));
    ui->lbFile->setText(argv);
    u.src = &src;
    rattrack();
}

controlpainel::~controlpainel()
{
    delete ui;
}

void controlpainel::on_btPlay_clicked() {

    if(tmrTimer->isActive() == true) {
        tmrTimer->stop();
        QPixmap pixmap(":/new/prefix1/play.png");
        QIcon ButtonIcon(pixmap);
        ui->btPlay->setIcon(ButtonIcon);
    }else{
        tmrTimer->start();
        QPixmap pixmap(":/new/prefix1/pause.png");
        QIcon ButtonIcon(pixmap);
        ui->btPlay->setIcon(ButtonIcon);
   }
}

void controlpainel::on_btConfig_clicked() {
    dc = new dialogConfig();
    dc->show();
}

void controlpainel::on_btReset_clicked() {
    cont = 1;
    track = Scalar(0);

    u.src->set(CAP_PROP_POS_MSEC, 380000);
}

void controlpainel::on_btSair_clicked(){
    exit(1);
}

/*RATRACK*/
void controlpainel::Threshold( int, void* ) {
    Mat aux(out_perspective.rows, out_perspective.cols, CV_8U, Scalar(0));

     /*Threshold*/
    for(int i = 0; i < out_perspective.rows; i++) {
        for(int j = 0; j < out_perspective.cols; j++) {
            if( ((j-centerX)*(j-centerX)) + ((i-centerY)*(i-centerY)) <= raio*raio) {
                if (out_perspective.at<uchar>(i, j) < threshold_value)
                    aux.at<uchar>(i, j) = BLACK;
                else
                    aux.at<uchar>(i, j) = WHITE;
            }else
                aux.at<uchar>(i, j) = BLACK;
        }
    }

    medianBlur(aux, aux, 5);
    tracking(aux, track);
}

void controlpainel::tracking(Mat aux, Mat track) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    unsigned int i;
    float area;
    Mat out_perspective1;

    cvtColor(out_perspective, out_perspective1, CV_GRAY2RGB);
    findContours( aux, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    for(i = 0; i < contours.size(); i++ ){
        area = contourArea(contours[i], false);
        if (area >= min_area && area <= max_area) {
          cout << "rato de area: " << area << " indice: " << i << "\n";
          break;
        }
    }

    if (i >= contours.size() || area == 0){
        cout << "rato não encontrado\n";
        Mat p = paint();
        addWeighted(out_perspective1, 1, p, 1, 0.0, out_perspective1);
        warpPerspective(out_perspective1, out, invH, src_frame.size());
        out_original = out_perspective1;
        mostra_tela(out_perspective1);

        return;
    }else{

        /// Get the moments
        vector<Moments> mu( contours.size() );
        mu[i] = moments( contours[i], false );

        ///  Get the mass centers:
        vector<Point2f> mc( contours.size() );
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );

        track.at<int>(mc[i].y, mc[i].x) = cont;
        cont++;
        Mat p = paint();

        //Circulando contornos e destacando centro de massa
        drawContours( out_perspective1, contours, i, Scalar(255,255,0), 1, 8, hierarchy, 0, Point() );
        circle( out_perspective1, mc[i], 2.5, Scalar(0,255,0), -1);
        addWeighted(out_perspective1, 1, p, 1, 0.0, out_perspective1);
        warpPerspective(out_perspective1, out, invH, src_frame.size());

        out_original = out_perspective1;
        mostra_tela(out_perspective1);

    }
}

Mat controlpainel::paint() {
    Mat p = Mat::zeros(track.rows, track.cols, CV_8UC3);
    Vec3b c1 = Vec3b(255,0,0);
    Vec3b c2 = Vec3b(0,255,0);
    for(int i = 0; i < track.rows; i++) {
        for(int j = 0; j < track.cols; j++) {
            if(track.at<int>(i, j) != BLACK) {
                float r = track.at<int>(i,j)/( (float) cont );
                Vec3b c = (r*c1 + (1-r)*c2);
                if(rtrack == 0)
                    p.at<Vec3b>(i, j) = c;
                else {
                    Point a;
                    a.x = j;
                    a.y = i;
                    circle( p, a, rtrack, c, -1);
                }
            }
        }
    }
    return p;
}

void controlpainel::acha_perspectiva(Mat& transform_mat, Mat& H2, Size warpSize, int FHEIGHT, int FWIDTH) {

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

    H2 = findHomography(cornerQuad, finalCornerQuad, LMEDS);

    warpPerspective(src_gray, transform_mat, H2, warpSize);

    /*Pegando o centro do círculo da imagem transformada*/
    centerTrans.push_back(Point2f((float)240/( (float) wwidth)*FWIDTH, (float)189.5/((float) wheight)*FHEIGHT));

    invH = findHomography(finalCornerQuad, cornerQuad, LMEDS);

    perspectiveTransform(centerTrans, center, invH);
    perspectiveTransform(center, centerTrans, H2);
    centerX = centerTrans[0].x;
    centerY = centerTrans[0].y;

}

void controlpainel::aplica_perspectiva() {
    Mat src_gray;
    /*Passando frame do vídeo para GrayScale*/
    cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY);

    /*Aplicando transformação de perspectiva*/
    warpPerspective(src_gray, transform_mat, H2, warpSize);

    /*Copiando matrix de transformação para out*/
    out_perspective = Mat::zeros( src_frame.rows, src_frame.cols, CV_8U );
    transform_mat.copyTo(out_perspective);
}

void controlpainel::processa_video() {
    src >> src_frame;

    //if(src_frame.empty())
        // break;

    aplica_perspectiva();

    /*Inicializando a função Thresholding*/
    Threshold( 0, 0 );

    waitKey(20); // waits to display frame
}

void controlpainel::mostra_tela(Mat& out_perspective1) {

    /*Copiando a imagen corrigida por ROI*/
    out.copyTo(tela(Rect(0, 0, out.cols, out.rows)));
    out_perspective1.copyTo(tela(Rect(out.cols, 0, out_perspective1.cols, out_perspective1.rows)));

    Mat temp;
    cvtColor(tela, temp, CV_BGR2RGB);
    cv::resize(temp, temp, Size(1150, 500));
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    ui->lbTela->setPixmap(QPixmap::fromImage(dest));
    ui->lbTela->resize(ui->lbTela->pixmap()->size());

}

void controlpainel::rattrack() {

    transform_mat = Mat::zeros( src_frame.rows, src_frame.cols, CV_8U );
    FHEIGHT = src.get(CV_CAP_PROP_FRAME_HEIGHT);
    FWIDTH = src.get(CV_CAP_PROP_FRAME_WIDTH);

    tela = Mat(FHEIGHT, FWIDTH*2, CV_8UC3, Scalar(0));
    track = Mat(FHEIGHT, FWIDTH, CV_32S);

    warpSize = Size(FWIDTH, FHEIGHT);

    /*Calcula a Matriz H2*/
    acha_perspectiva(transform_mat, H2, warpSize, FHEIGHT, FWIDTH);

    /*Aplica a matriz H2 em cada frame do video*/
    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processa_video()));
    tmrTimer->start(50);

    waitKey(0); // key press to close window
}

void controlpainel::on_btSave_clicked()
{

    ds = new DialogSave();
    ds->show();

}

void controlpainel::on_btCircle_clicked()
{
    tmrTimer->stop();
    dfc = new dialogFindCircle();
    dfc->show();

}
