#include "tracker.h"

Tracker::Tracker(QString animal, QString teste, Video* captureVideo) {
    this->mw = MainWindow::getInstance();
    this->tmrTimer = new QTimer();

    this->animal = animal;
    this->teste = teste;

    this->captureVideo = captureVideo;

    this->countTracking = 0;
    this->imageTracking = Mat(this->captureVideo->getHeight(), this->captureVideo->getWidth(), CV_32S, Scalar(0));

    imageTransform = Mat::zeros(this->captureVideo->getHeight(), this->captureVideo->getWidth(), CV_8U );
    this->warpSize = Size(this->captureVideo->getHeight(), this->captureVideo->getWidth());

    imageScreen = Mat(this->captureVideo->getHeight(), (this->captureVideo->getWidth())*2, CV_8UC3, Scalar(0));

    coordBBfore = coordBefore = coordCurrent = Point2d(0,0);
}

Tracker::Tracker() {
    this->mw = MainWindow::getInstance();
    this->tmrTimer = new QTimer();

    this->countTracking = 0;
    this->imageTracking = Mat(this->captureVideo->getHeight(), this->captureVideo->getWidth(), CV_32S, Scalar(0));

    imageTransform = Mat::zeros(this->captureVideo->getHeight(), this->captureVideo->getWidth(), CV_8U );
    this->warpSize = Size(this->captureVideo->getHeight(), this->captureVideo->getWidth());

    imageScreen = Mat(this->captureVideo->getHeight(), (this->captureVideo->getWidth())*2, CV_8UC3, Scalar(0));

    coordBBfore = coordBefore = coordCurrent = Point2d(0,0);
}

void Tracker::resetCaptureVideo() {
    this->countTracking = 0;
    this->imageTracking = Scalar(0);
    this->captureVideo->getCaptureVideo().set(CAP_PROP_POS_MSEC, 00);
}

void Tracker::pauseVideo() {

    //If video is playing then stop
    if(this->tmrTimer->isActive()) {
        this->tmrTimer->stop();

        //Setting GUI
        this->mw->setButtonPlay(true);

    //If video has stop then play
    }else{
        this->tmrTimer->start(captureVideo->getFPS());

        //Setting GUI
        this->mw->setButtonPlay(false);
    }
}

void Tracker::executeTracker() {

    //Creating a directory for saving infos test
    createTestDirectory();

    //Starting write the statistics file
    startWriteStatistics();

    //Reading homography configuration
    readHomographyFile();

    //Starting tracking process
    QObject::connect(this->tmrTimer, SIGNAL(timeout()), this->mw, SLOT(processVideo()));
    this->tmrTimer->start(this->captureVideo->getFPS());

    waitKey(0); // key press to close window
}

void Tracker::readHomographyFile() {

    //Reading Homography File
    FileStorage fs(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME, FileStorage::READ);

    //Verifying if file is open
    if(!(fs.isOpened())) {
        QMessageBox::critical(this->mw, QObject::tr("Erro"), QObject::tr("Para realizacao do teste, eh necessario calibrar a camera!\nTeste cancelado!"));
        return;
    }

    //Saving homography infos in Tracker object
    fs["homography_matrix"] >> this->homographyWarp;
    fs["center_circle"] >> this->center;
    fs["radius"] >> this->radius;
    fs["pixel_ratio"] >> this->pixelRatio;

    //Verifying if homography matrix is empty
    if(this->homographyWarp.empty()) {
        QMessageBox::critical(this->mw, QObject::tr("Erro"), QObject::tr("Impossivel encontrar matriz de calibracao!\nTeste cancelado!"));
        return;
    }

    //Closing file after read
    fs.release();
}

void Tracker::createTestDirectory() {

    //Creating main directory for tests
    QDir dir(QString::fromStdString(TESTES_DIR_NAME));
    if(!dir.exists())
        QDir().mkdir(dir.absolutePath());

    //Creating animal directory
    QDir dir1(QString::fromStdString(TESTES_DIR_NAME) + "/" + this->animal);
    if(!dir1.exists())
        QDir().mkdir(dir1.absolutePath());
}

void Tracker::startWriteStatistics() {

    //Getting path name
    const string name1= TESTES_DIR_NAME + "/" + this->animal.toAscii().data() + STATISTICS_FILE_NAME;

    //Open file to start write
    this->statisticsFile.open(name1.c_str());

    //Writing first line
    this->statisticsFile << "frame, tempo, metros, velocidade, aceleracao" << endl;
}

void Tracker::processVideo() {

    this->captureVideo->getCaptureVideo() >> this->srcFrame;

    //If captureVideo is empty so closed test
    if((this->srcFrame).empty()) {
        this->tmrTimer->stop();
        this->statisticsFile.close();

        //"MAKE A FUNCTION"
        this->endVideo();

    //If not so analysis the video
    }else{

        //Getting the correct image applying homography
        getPerspectiveFrame();

        //Transforming image to detect the rat
        Mat imageThreshold = getThreshold();

        //Making imageTrack
        getTrack(imageThreshold);

        //Waits to display frame
        waitKey(20);
    }
}

void Tracker::getPerspectiveFrame() {

    //Getting srcFrame in gray scayle
    Mat srcGray;
    if(this->srcFrame.channels() != 1)
        cvtColor(srcFrame, srcGray, cv::COLOR_RGB2GRAY);
    else
        this->srcFrame.copyTo(srcGray);

    //Applying Homography matrix to get the perspective image
    warpPerspective(srcGray, this->imageTransform, this->homographyWarp, this->warpSize);

    //Copying transformImage
    this->perspectiveFrame = Mat::zeros(this->srcFrame.rows, this->srcFrame.cols, CV_8U);
    this->imageTransform.copyTo(this->perspectiveFrame);
}

Mat Tracker::getThreshold() {
    Mat imageThreshold(this->perspectiveFrame.rows, this->perspectiveFrame.cols, CV_8U, Scalar(0));

    //Threshold
    for(int i = 0; i < this->perspectiveFrame.rows; i++) {
        for(int j = 0; j < this->perspectiveFrame.cols; j++) {
            if(((j-this->center.x)*(j-this->center.x)) + ((i-this->center.y)*(i-this->center.y)) <= this->radius*this->radius) {
                if (this->perspectiveFrame.at<uchar>(i, j) < this->mw->getValueThreshold())
                    imageThreshold.at<uchar>(i, j) = BLACK;
                else
                    imageThreshold.at<uchar>(i, j) = WHITE;
            }else
                imageThreshold.at<uchar>(i, j) = BLACK;
        }
    }

    medianBlur(imageThreshold, imageThreshold, 5);
    return imageThreshold;
}

void Tracker::addCordinates(Point2d coord) {
    this->coordinates.push_back(coord);
}

void Tracker::getTrack(Mat imageThreshold) {

    unsigned int i;
    //Getting the countours of image, countours = rat
    cvtColor(perspectiveFrame, perspectiveFrame, CV_GRAY2RGB);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(imageThreshold, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    //Getting area of rat
    float area;
    for(i = 0; i < contours.size(); i++ ){
        area = contourArea(contours[i], false);
        if (area >= this->mw->getMinValueArea() && area <= this->mw->getMaxValueArea()) {
            break;
        }
    }

    //Counting frames
    this->captureVideo->setTotalFrames(this->captureVideo->getTotalFrames()+1);

    //If do not detected the rat
    if (i >= contours.size() || area == 0) {

        //Updating coordinates
        this->coordCurrent = this->coordBefore;
        this->addCordinates(this->coordCurrent);

        //Calculating the statistics about rat and saving in statisticsFile
        this->calcAndSaveStatistics(false);

        //Setting Status of find rat
        stringstream status1;
        status1 << "Rato nao encontrado! Ultimas coordenadas: (" << this->coordCurrent.x << ", " << this->coordCurrent.y << ").";
        string status = status1.str();
        this->mw->setStatus(status);

        //Drawing the path of rat in perspectiveFrame
        Mat path = drawPath();
        addWeighted(this->perspectiveFrame, 1, path, 1, 0.0, this->perspectiveFrame);

        //Getting the original cam image applying inverse homography matrix
        warpPerspective(this->perspectiveFrame, this->srcFrame, this->homographyWarp.inv(), this->srcFrame.size());
        //warpPerspective(p, p, H2_warp.inv(), p.size());

        //Showing the image
        if(this->mw->getScreen()) this->mw->showImage(this->srcFrame);
        else this->mw->showImage(this->perspectiveFrame);
        return;

    //If detected rat
    }else{

        //Get the moments
        vector<Moments> mu(contours.size());
        mu[i] = moments(contours[i], false);

        //Get the mass centers:
        vector<Point2f> mc(contours.size());
        mc[i] = Point2f(mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00);

        //Updating coordinates
        this->coordBBfore = this->coordBefore;
        this->coordBefore = this->coordCurrent;
        this->coordCurrent = mc[i];
        this->addCordinates(this->coordCurrent);

        //Calculating the statistics about rat and saving in statisticsFile - "MAKE A FUNCTION"
        this->calcAndSaveStatistics(true);

        //Setting Status of find rat
        stringstream status1;
        status1 << "Rato encontrado! Coordenadas: (" << this->coordCurrent.x << ", " << this->coordCurrent.y << ").";
        string status = status1.str();
        this->mw->setStatus(status);

        //Drawing path of rat
        this->imageTracking.at<int>(this->coordCurrent.y, this->coordCurrent.x) = this->countTracking;
        this->countTracking++;
        Mat pathRat = drawPath();

        //Drawing countours (=rat) and the center mass in perspectiveFrame
        drawContours(this->perspectiveFrame, contours, i, Scalar(255,255,0), 1, 8, hierarchy, 0, Point());
        circle(this->perspectiveFrame, this->coordCurrent, 2.5, Scalar(0,255,0), -1);

        //Add path in perpectiveFrame
        addWeighted(this->perspectiveFrame, 1, pathRat, 1, 0.0, this->perspectiveFrame);

        //Getting original cam image applying inverse homography matrix
        warpPerspective(this->perspectiveFrame, this->srcFrame, this->homographyWarp.inv(), this->srcFrame.size());

        //Criando imagem da tela
        //original_frame.copyTo(tela_image(Rect(0, 0, original_frame.cols, original_frame.rows)));
        //perspective_frame.copyTo(tela_image(Rect(original_frame.cols, 0, perspective_frame.cols, perspective_frame.rows)));

        //Showing image in screen
        if(this->mw->getScreen()) this->mw->showImage(this->srcFrame);
        else this->mw->showImage(this->perspectiveFrame);

    }
}

Mat Tracker::drawPath() {
    Mat path = Mat::zeros(this->imageTracking.rows, this->imageTracking.cols, CV_8UC3);
    Vec3b c1 = Vec3b(255,0,0);
    Vec3b c2 = Vec3b(0,255,0);

    for(int i = 0; i < this->imageTracking.rows; i++) {
        for(int j = 0; j < this->imageTracking.cols; j++) {
            if(this->imageTracking.at<int>(i, j) != BLACK) {
                float r = this->imageTracking.at<int>(i,j)/( (float) this->countTracking);
                Vec3b c = (r*c1 + (1-r)*c2);
                if(this->mw->getThicknessThreshold() == 0)
                    path.at<Vec3b>(i, j) = c;
                else {
                    Point a;
                    a.x = j;
                    a.y = i;
                    circle( path, a, this->mw->getThicknessThreshold(), c, -1);
                }
            }
        }
    }
    return path;
}

void Tracker::calcAndSaveStatistics(bool find) {

    //Calculing time per frame
    double time = 1.0/this->captureVideo->getFPS();

    //If rat do not found then save default statistics
    if(!find)
        this->statisticsFile << this->captureVideo->getTotalFrames() << ", " << time << ", , , " << endl;

    //If rat has found then calculate meters, speed and aceleration
    else {

        //Saving initial statistics
        if(this->captureVideo->getTotalFrames() == 1)
            this->statisticsFile << this->captureVideo->getTotalFrames() << ", " << time << ", 0, 0, 0" << endl;

        //Saving first statistics
        else if(this->captureVideo->getTotalFrames() == 2) {

            //Metters
            double xDiff = this->coordBefore.x - this->coordCurrent.x;
            double yDiff = this->coordBefore.y - this->coordCurrent.y;
            double meters = (sqrt((xDiff*xDiff) + (yDiff*yDiff)))*this->pixelRatio;

            //Speed
            double vel = meters/time;

            //Aceleration
            double acel = (vel-0) / time;

            //Write in statistcs file
            this->statisticsFile << this->captureVideo->getTotalFrames() << ", " << time << ", " << meters << ", " << vel << ", " << acel << endl;

        //Other statistics
        }else{

            //Metters Before
            double xDiffB = this->coordBBfore.x - this->coordBefore.x;
            double yDiffB = this->coordBBfore.y - this->coordBefore.y;
            double metersBefore = (sqrt((xDiffB*xDiffB) + (yDiffB*yDiffB)))*this->pixelRatio;

            //Metters Current
            double xDiffC = this->coordBefore.x - this->coordCurrent.x;
            double yDiffC = this->coordBefore.y - this->coordCurrent.y;
            double metersCurrent = (sqrt((xDiffC*xDiffC) + (yDiffC*yDiffC)))*this->pixelRatio;

            //Speed
            double velCurrent = metersCurrent/time;
            double velBefore = metersBefore/time;

            //Aceleration
            double acel = (velCurrent-velBefore) / time;

            //Write in statistcs file
            this->statisticsFile << this->captureVideo->getTotalFrames() << ", " << time << ", " << metersCurrent << ", " << velCurrent << ", " << acel << endl;
        }
    }
}

void Tracker::endVideo() {

    //Creating test rat directory
    this->createTestDirectory();

    //Saving Path image
    string save;
    if(this->mw->getCheckedPathButton()) {

        //Setting status
        this->mw->setStatus("Salvando Caminho...");

        //Setting image name
        stringstream filename;
        filename << TESTES_DIR_NAME + "/" + this->animal.toAscii().data() << "/caminho_teste_" << teste.toAscii().data() << ".bmp";
        save = filename.str();

        //Saving image
        imwrite(save, imageTracking);

        //Setting status
        this->mw->setStatus("Salvo!");
    }

    //Saving IA
    if(this->mw->getCheckedIAButton()) {

        //Setting status
        this->mw->setStatus("Salvando Aprendizado...");

        //Getting IA image
        Mat imageIA = getImageIA();

        //Setting image name
        stringstream filename;
        filename << TESTES_DIR_NAME + "/" + this->animal.toAscii().data() << "/aprendizado_teste_" << teste.toAscii().data() << ".bmp";
        save = filename.str();

        //Saving image
        imwrite(save, imageIA);

        //Setting status
        this->mw->setStatus("Aprendizado salvo com sucesso!");
    }
}

Mat Tracker::getImageIA() {
    Mat IA;
    unsigned int cont = 0;
    Point2f final(this->coordinates[this->coordinates.size() - 2].x, this->coordinates[this->coordinates.size() - 2].y);

    this->imageTracking.copyTo(IA);

    while(cont <= (unsigned int)this->countTracking) {

        line(IA, this->coordinates[cont], final, Scalar(255,255,255));
        cont+=20;
    }

    return IA;
}
