#include "tracker.h"
#include "mainwindow.h"

Tracker::Tracker(QObject *parent) : QObject(parent) { }

Tracker::Tracker(QObject *parent, QString animal, QString teste, Video* captureVideo) : QObject(parent){
    this->mw = MainWindow::getInstance();

    this->tmrTimer = new QTimer();

    this->animal = animal;
    this->teste = teste;

    this->captureVideo = captureVideo;

    this->countTracking = 0;
    this->imageTracking = Mat(captureVideo->getHeight(), captureVideo->getWidth(), CV_32S);

    this->imageTransform = Mat::zeros(captureVideo->getHeight(), captureVideo->getWidth(), CV_8U );
    this->warpSize = Size(captureVideo->getWidth(), captureVideo->getHeight());

    coordBBfore = coordBefore = coordCurrent = Point2d(0,0);

    this->detector = new Threshold();

    this->writerCSV = new outCSV();
}

Mat Tracker::getPerspectiveFrame() {
    return this->perspectiveFrame;
}

Point2d Tracker::getCenter() {
    return this->center;
}

double Tracker::getRadius() {
    return this->radius;
}

MainWindow* Tracker::getGUI() {
    return this->mw;
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

void Tracker::saveSnapshot() {

    //Creating directories
    this->createTestDirectory();
    this->createSnapshotDirectory();

    //Creating image name
    stringstream filename;
    filename << TESTES_DIR_NAME + animal.toAscii().data() + "/Snapshots" << "/snapshot" << this->snapshot++ << "_teste_" << this->teste.toAscii().data() << ".bmp";
    string save = filename.str();

    //Saving image
    if(this->mw->getScreen()) imwrite(save, this->srcFrame);
    else imwrite(save, this->perspectiveFrame);
}

Video* Tracker::getCaptureVideo() {
    return this->captureVideo;
}

QTimer* Tracker::getTimer() {
    return this->tmrTimer;
}

void Tracker::executeTracker() {

    //Creating a directory for saving infos test
    createTestDirectory();

    //Starting write the statistics file
    startWriteStatistics();

    //Reading homography configuration
    readHomographyFile();

    //Starting tracking process
    this->tmrTimer->start(this->captureVideo->getFPS());
    
    waitKey(0); // key press to close window
}

void Tracker::readHomographyFile() {

    //Reading Homography File
    this->mw->setReader(new ReaderXml());

    (this->mw->getReader())->readFile(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME);

    string name = "homography_matrix";
    (this->mw->getReader())->getInFile(name.c_str(), &homographyWarp);
    name = "center_circle";
    (this->mw->getReader())->getInFile(name.c_str(), &center);
    name = "radius";
    (this->mw->getReader())->getInFile(name.c_str(), &radius);
    name = "pixel_ratio";
    (this->mw->getReader())->getInFile(name.c_str(), &pixelRatio);

    //Closing file after read
    (this->mw->getReader())->closeFile();
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

void Tracker::createSnapshotDirectory() {
    QDir dir2(QString::fromStdString(TESTES_DIR_NAME) + "/" + this->animal + "/Snapshots");
    if(!dir2.exists())
        QDir().mkdir(dir2.absolutePath());
}

void Tracker::startWriteStatistics() {

    //Getting path name
    const string name1= TESTES_DIR_NAME + "/" + this->animal.toAscii().data() + STATISTICS_FILE_NAME;

    //Open file to start write
    this->writerCSV->startWrite(name1.c_str(), statisticsFile);

    //Writing first line
    this->writerCSV->writeInFile("frame", statisticsFile); this->writerCSV->addColumn(statisticsFile);
    this->writerCSV->writeInFile("tempo", statisticsFile); this->writerCSV->addColumn(statisticsFile);
    this->writerCSV->writeInFile("metros", statisticsFile); this->writerCSV->addColumn(statisticsFile);
    this->writerCSV->writeInFile("velocidade", statisticsFile); this->writerCSV->addColumn(statisticsFile);
    this->writerCSV->writeInFile("aceleracao", statisticsFile); this->writerCSV->endLine(statisticsFile);
}

void Tracker::processVideo() {

    this->captureVideo->getCaptureVideo() >> this->srcFrame;

    //If captureVideo is empty so closed test
    if((this->srcFrame).empty()) {
        this->tmrTimer->stop();
        this->statisticsFile.close();
        this->endVideo();

        //If not so analysis the video
    }else{

        //Getting the correct image applying homography
        applyingHomography();

        //Transforming image to detect the rat
        Mat imageThreshold = detector->detectRat(this->perspectiveFrame, this->center, this->radius, this->mw->getValueThreshold());

        //Making imageTrack
        getTrack(imageThreshold);

        //Waits to display frame
        waitKey(20);
    }
}

void Tracker::applyingHomography() {

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

void Tracker::addCordinates(Point2d coord) {
    this->coordinates.push_back(coord);

}

void Tracker::getTrack(Mat imageThreshold) {

    unsigned int i;

    //Getting the countours of image, countours = rat
    cvtColor(this->perspectiveFrame, this->perspectiveFrame, CV_GRAY2RGB);
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
    this->captureVideo->setCountFrames(this->captureVideo->getCountFrames()+1);

    //If do not detected the rat
    if (i >= contours.size() || area == 0) {

        //Updating coordinates
        this->coordCurrent = this->coordBefore;
        this->addCordinates(this->coordCurrent);

        //Calculating the statistics about rat and saving in statisticsFile
        this->calcAndSaveStatistics(false);

        //Setting Status of find rat
        stringstream status1;
        status1 << "Rato nao encontrado! Ultimas coordenadas: (" << this->coordCurrent.x << ", " <<
                   this->coordCurrent.y << ").";
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
    double vel, meters, acel;

    //If rat do not found then save default statistics
    if(!find) {
        //this->statisticsFile << this->captureVideo->getTotalFrames() << ", " << time << ", , , " << endl;
        this->writerCSV->writeInFile(this->captureVideo->getCountFrames(), statisticsFile);
        this->writerCSV->addColumn(statisticsFile);
        this->writerCSV->writeInFile(time, statisticsFile);
        this->writerCSV->addColumn(statisticsFile);
        this->writerCSV->addColumn(statisticsFile);
        this->writerCSV->addColumn(statisticsFile);
        this->writerCSV->endLine(statisticsFile);

        //If rat has found then calculate meters, speed and aceleration
    }else {

        //Saving initial statistics
        if(this->captureVideo->getCountFrames() == 1)
            meters = vel = acel = 0;

        //Saving first statistics
        else if(this->captureVideo->getCountFrames() == 2) {

            //Metters
            double xDiff = this->coordBefore.x - this->coordCurrent.x;
            double yDiff = this->coordBefore.y - this->coordCurrent.y;
            meters = (sqrt((xDiff*xDiff) + (yDiff*yDiff)))*this->pixelRatio;

            //Speed
            vel = meters/time;

            //Aceleration
            acel = vel / time;

            //Other statistics
        }else{

            //Metters Before
            double xDiffB = this->coordBBfore.x - this->coordBefore.x;
            double yDiffB = this->coordBBfore.y - this->coordBefore.y;
            double metersBefore = (sqrt((xDiffB*xDiffB) + (yDiffB*yDiffB)))*this->pixelRatio;

            //Metters Current
            double xDiffC = this->coordBefore.x - this->coordCurrent.x;
            double yDiffC = this->coordBefore.y - this->coordCurrent.y;
            meters = (sqrt((xDiffC*xDiffC) + (yDiffC*yDiffC)))*this->pixelRatio;

            //Speed
            vel = meters/time;
            double velBefore = metersBefore/time;

            //Aceleration
            acel = (vel-velBefore) / time;
        }

        //Write in statistcs file
        this->writerCSV->writeInFile(this->captureVideo->getCountFrames(), statisticsFile);
        this->writerCSV->addColumn(statisticsFile);
        this->writerCSV->writeInFile(time, statisticsFile);
        this->writerCSV->addColumn(statisticsFile);
        this->writerCSV->writeInFile(meters, statisticsFile);
        this->writerCSV->addColumn(statisticsFile);
        this->writerCSV->writeInFile(vel, statisticsFile);
        this->writerCSV->addColumn(statisticsFile);
        this->writerCSV->writeInFile(acel, statisticsFile);
        this->writerCSV->endLine(statisticsFile);
    }
}

void Tracker::endVideo() {

    //Creating test rat directory
    this->createTestDirectory();

    //Saving Path image
    string save;

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

    //Calculing learning index
    this->calcLearningIndex();

    //Ending GUI
    this->mw->closeTest();
}

void Tracker::calcLearningIndex() {

    //Getting path name
    const string name1 = TESTES_DIR_NAME + "/" + this->animal.toAscii().data() + LEARNING_FILE_NAME;

    //Open file to start write
    ofstream learningFile;
    this->writerCSV->startWrite(name1.c_str(), learningFile);

    //Writing first line
    this->writerCSV->writeInFile("frame", learningFile); this->writerCSV->addColumn(learningFile);
    this->writerCSV->writeInFile("tempo", learningFile); this->writerCSV->addColumn(learningFile);
    this->writerCSV->writeInFile("distancia", learningFile); this->writerCSV->addColumn(learningFile);
    this->writerCSV->endLine(learningFile);

    double time = 1/this->getCaptureVideo()->getFPS();
    Point2d finalPoint = this->coordinates[this->coordinates.size()-1];
    double media = 0;

    for(int i = 0; i < (int) this->coordinates.size()-1; i++) {

        Point2d point = this->coordinates[i];
        double xDiff = finalPoint.x - point.x;
        double yDiff = finalPoint.y - point.y;
        double meters = (sqrt((xDiff*xDiff) + (yDiff*yDiff)))*this->pixelRatio;
        media = media + (meters/30);

        //Writing infos
        this->writerCSV->writeInFile(i, learningFile); this->writerCSV->addColumn(learningFile);
        this->writerCSV->writeInFile(time, learningFile); this->writerCSV->addColumn(learningFile);
        this->writerCSV->writeInFile(meters, learningFile); this->writerCSV->addColumn(learningFile);
        this->writerCSV->endLine(learningFile);

        //Making media every 1sec
        if(i != 0 && (i+1)%30 == 0) {

            //Writing info
            this->writerCSV->writeInFile("media", learningFile); this->writerCSV->addColumn(learningFile);
            this->writerCSV->writeInFile("1sec", learningFile); this->writerCSV->addColumn(learningFile);
            this->writerCSV->writeInFile(media, learningFile); this->writerCSV->addColumn(learningFile);
            this->writerCSV->endLine(learningFile);
            media = 0;
        }
    }
}
