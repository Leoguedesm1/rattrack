#include "tracker.h"
#include "trackergui.h"
#include "filesanddirectoriesconstants.h"
#include "homographyfileconstants.h"
#include "quadrantsfileconstants.h"
#include "fastmodeguiconstants.h"

#define PI 3.14159265

Tracker::Tracker(QObject *parent) : QObject(parent) { }

Tracker::Tracker(QObject *parent, vector<Video *> cap, int mode) : QObject(parent) {

    //Initializing objects
    this->interface = TrackerGUI::getInstance();
    this->videos = cap;
    this->writerLearningIndex = new WriterCSV();

    //Settings
    this->thresholdValue = this->interface->getThreshValue();
    this->thresholdThick = this->interface->getThreshThick();
    this->minArea = this->interface->getMinArea();
    this->maxArea = this->interface->getMaxArea();
    this->screen = this->interface->getScreenMode();

    //Reading configuration files
    this->readHomographyFile();
    this->readQuadrantsFile();

    //Creating main Directory for save results
    this->dirCreator = new DirectoryCreator();
    dirCreator->create(TESTES_DIR_NAME);

    if(mode == FAST)
        this->mode = FAST;
    else
        this->mode = NORMAL;

    this->startAnalysis();
}

void Tracker::startAnalysis() {

    this->cap = this->videos.at(this->interface->getIndexVideo());
    this->cap->getCaptureVideo().set(CAP_PROP_POS_MSEC, 00);

    //Update Interface
    if(this->mode == NORMAL)
        this->interface->setFilename(this->cap->getFileName().section("/", -1, -1));
    else{
        this->interface->setProgressBar();
        this->interface->setStatus(QString::fromStdString(FAST_MODE_GUI_STATUS_EVALUATING));
    }

    //Initializing tracking variables
    this->detector = new Threshold();
    this->countTracking = this->countSnapshot = 0;
    this->warpSize = Size(this->cap->getWidth(), this->cap->getHeight());
    this->imageTracking = Mat::zeros(this->cap->getHeight(), this->cap->getWidth(), CV_32S);
    coordBBfore = coordBefore = coordCurrent = Point2d(0,0);

    //Creating analsis directory for save results
    dirCreator->create(TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data());

    //Start Creation Statistics File
    this->createStatisticsFile();

    //Initializing analysis
    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(proccessVideo()));
    timer->start();
}

void Tracker::readHomographyFile() {
    this->readerHomography = new ReaderXML();
    this->readerHomography->readFile(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME);
    this->readerHomography->getInFile(MATRIX_HOMOGRAPHY_NAME.c_str(), &homographyMatrix);
    this->readerHomography->getInFile(CENTER_CIRCLE_NAME.c_str(), &center);
    this->readerHomography->getInFile(RADIUS_NAME.c_str(), &radius);
    this->readerHomography->getInFile(PIXEL_RATIO_NAME.c_str(), &pixelRatio);
    this->readerHomography->closeFile();
}

void Tracker::readQuadrantsFile() {
    this->readerQuadrants = new ReaderXML();
    if(this->readerQuadrants->exists(CALIBRATION_DIR_NAME + QUADRANT_FILE_NAME)) {
        quadrantAngles = vector<double> (4);
        quadrants = ON;

        this->readerQuadrants->getInFile(QUAD1_NAME.c_str(), &quadrantAngles.at(0));
        this->readerQuadrants->getInFile(QUAD2_NAME.c_str(), &quadrantAngles.at(1));
        this->readerQuadrants->getInFile(QUAD3_NAME.c_str(), &quadrantAngles.at(2));
        this->readerQuadrants->getInFile(QUAD4_NAME.c_str(), &quadrantAngles.at(3));
    }else
        quadrants = OFF;
}

void Tracker::createStatisticsFile() {

    this->writerStatistics = new WriterCSV();

    //Getting path name
    const string name1 = TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data() + STATISTICS_FILE_NAME;

    //Open file to start write
    this->writerStatistics->startFile(name1.c_str());

    //Writing first line
    string data;
    if(quadrants == ON)
        data = "frame, tempo (s), metros (cm), velocidade (cm/s), aceleracao (cm2/s), quadrante\n";
    else
        data = "frame, tempo (s), metros (cm), velocidade (cm/s), aceleracao (cm2/s)\n";
    this->writerStatistics->write(data);
}

void Tracker::proccessVideo() {

    Mat srcFrame;
    this->cap->getCaptureVideo() >> srcFrame;

    if(!(srcFrame.empty())) {

        //Getting the correct image applying homography
        Mat homographyFrame = applyHomography(srcFrame);

        //Transforming image to detect the rat
        Mat imageThreshold = detector->detectRat(homographyFrame, this->center, this->radius, (*thresholdValue));

        //Making imageTrack
        getTrack(imageThreshold, homographyFrame, srcFrame);

        //Releasing space
        srcFrame.release();
        homographyFrame.release();
        imageThreshold.release();

    }else
        this->finishAnalysis();
}

Mat Tracker::applyHomography(Mat srcFrame) {

    //Getting srcFrame in gray scayle
    Mat srcGray;
    if(srcFrame.channels() != 1)
        cvtColor(srcFrame, srcGray, cv::COLOR_RGB2GRAY);
    else
        srcFrame.copyTo(srcGray);

    //Applying Homography matrix to get the perspective image
    Mat homographyFrame;
    warpPerspective(srcGray, homographyFrame, this->homographyMatrix, this->warpSize);

    //Releasing space
    srcGray.release();

    return homographyFrame;
}

void Tracker::getTrack(Mat imageThreshold, Mat homographyFrame, Mat srcFrame) {

    unsigned int i;
    stringstream status1;

    //Getting the countours of image, countours = rat
    cvtColor(homographyFrame, homographyFrame, CV_GRAY2RGB);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(imageThreshold, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    //Getting area of rat
    float area;
    for(i = 0; i < contours.size(); i++ ){
        area = contourArea(contours[i], false);
        if (area >= (*minArea) && area <= (*maxArea)) {
            break;
        }
    }

    //Counting frames
    this->cap->setCountFrames(this->cap->getCountFrames()+1);

    //If do not detected the rat
    if (i >= contours.size() || area == 0) {

        //Updating coordinates
        this->coordCurrent = this->coordBefore;
        this->coordinates.push_back(this->coordCurrent);

        //Calculating the statistics about rat and saving in statisticsFile
        this->calcAndSaveStatistics(false);

        //Setting Status of find rat
        status1 << "Rato nao encontrado! Ultimas coordenadas: (" << this->coordCurrent.x << ", " <<
                   this->coordCurrent.y << ").";

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
        this->coordinates.push_back(this->coordCurrent);

        //Calculating the statistics about rat and saving in statisticsFile
        this->calcAndSaveStatistics(true);

        //Setting Status of find rat
        status1 << "Rato encontrado! Coordenadas: (" << this->coordCurrent.x << ", " << this->coordCurrent.y << ").";

        //Updating path's rat
        this->imageTracking.at<int>(this->coordCurrent.y, this->coordCurrent.x) = this->countTracking;
        this->countTracking++;

        //Drawing countours (=rat) and the center mass in perspectiveFrame
        drawContours(homographyFrame, contours, i, Scalar(255,255,0), 1, 8, hierarchy, 0, Point());
        circle(homographyFrame, this->coordCurrent, 2.5, Scalar(0,255,0), -1);

        //Releasing space
        mu.clear();
        mc.clear();
    }

    //Drawing the path of rat in perspectiveFrame
    Mat path = drawPath();
    addWeighted(homographyFrame, 1, path, 1, 0.0, homographyFrame);

    //Getting the original cam image applying inverse homography matrix
    warpPerspective(homographyFrame, srcFrame, this->homographyMatrix.inv(), srcFrame.size());

    //Updating GUI
    if(this->mode == NORMAL) {
        this->interface->setStatus(QString::fromStdString(status1.str()));
        if((*screen) == CAMERA) {
            cvtColor(srcFrame, srcFrame, CV_BGR2RGB);
            resize(srcFrame, srcFrame, Size(704, 480));
            this->interface->showImage(srcFrame);
        }else{
            cvtColor(homographyFrame, homographyFrame, CV_BGR2RGB);
            resize(homographyFrame, homographyFrame, Size(704, 480));
            this->interface->showImage(homographyFrame);
        }
    }else{
        double value = (100.0 / this->cap->getTotalFrames()) * this->cap->getCountFrames();
        this->interface->updateProgressBar((int)value);
    }

    //Releasing space
    contours.clear();
    hierarchy.clear();
    path.release();
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
                if((*thresholdThick) == 0)
                    path.at<Vec3b>(i, j) = c;
                else {
                    Point a(j, i);
                    circle( path, a, (*thresholdThick), c, -1);
                }
            }
        }
    }

    return path;
}

void Tracker::calcLearningIndex() {

    //Getting path name
    const string name1 = TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data() + LEARNING_FILE_NAME;

    //Open file to start write
    this->writerLearningIndex->startFile(name1.c_str());

    //Writing first line
    string data = "frame, tempo, distancia\n";
    this->writerLearningIndex->write(data);

    double time = 1/this->cap->getFPS();
    Point2d finalPoint = this->coordinates[this->coordinates.size()-1];
    double media = 0;

    for(int i = 0; i < (int) this->coordinates.size()-1; i++) {

        Point2d point = this->coordinates[i];
        double xDiff = finalPoint.x - point.x;
        double yDiff = finalPoint.y - point.y;
        double meters = (sqrt((xDiff*xDiff) + (yDiff*yDiff)))*this->pixelRatio;
        media = media + (meters/30);

        //Writing infos
        stringstream data;
        data << i << ", " << time << ", " << meters << "\n";
        this->writerLearningIndex->write(data.str());

        //Making media every 1sec
        if(i != 0 && (i+1)%30 == 0) {

            //Writing info
            stringstream data1;
            data1 << "media, 1sec, " << media << "\n";
            this->writerLearningIndex->write(data1.str());
            media = 0;
        }
    }

    this->writerLearningIndex->closeFile();
}

void Tracker::calcAndSaveStatistics(bool find) {


    //Calculing time per frame
    double time = 1.0/this->cap->getFPS();
    double vel, meters, acel;

    //Saving initial statistics
    if(this->cap->getCountFrames() == 1 || !find)
        meters = vel = acel = 0;

    //Saving first statistics
    else if(this->cap->getCountFrames() == 2) {

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
    stringstream data;
    if(quadrants == ON)
        data << this->cap->getCountFrames() << ", " << time << ", " << meters << ", " << vel << ", " << acel << ", " << calcQuadrant() << "\n";
    else
        data << this->cap->getCountFrames() << ", " << time << ", " << meters << ", " << vel << ", " << acel << "\n";
    this->writerStatistics->write(data.str());

}

int Tracker::calcQuadrant() {

    double angle = calcAngle();

    //1st Quadrant
    if(isInside(quadrantAngles.at(0), quadrantAngles.at(3), angle))
        return 1;

    //2nd Quadrant
    if(isInside(quadrantAngles.at(1), quadrantAngles.at(0), angle))
        return 2;

    //3th Quadrant
    if(isInside(quadrantAngles.at(2), quadrantAngles.at(1), angle))
        return 3;

    //4th Quadrant
    if(isInside(quadrantAngles.at(3), quadrantAngles.at(2), angle))
        return 4;

    return 0;
}

double Tracker::calcAngle() {

    //Rat position = coordCurrent

    //Legs
    double a = coordCurrent.x - center.x;
    double b = coordCurrent.y - center.y;

    /*Quadrants:
     * 1st - a > 0 && b < 0
     * 2nd - a < 0 && b < 0
     * 3th - a < 0 && b > 0
     * 4th - a > 0 && b > 0*/
    if(a > 0 && b < 0) {
        return (atan(b/a) * 180/PI * (-1));
    }else if(a < 0 && b < 0) {
        return ((atan(a/b) * 180/PI) + 90);
    }else if(a < 0 && b > 0) {
        return ((atan(b/a) * 180/PI * (-1)) + 180);
    }else if(a > 0 && b > 0) {
        return ((atan(a/b) * 180/PI) + 270);
    }

    return 0;
}

bool Tracker::isInside(double superiorAngle, double inferiorAngle, double angle) {
    if(inferiorAngle < superiorAngle) {
        if(angle > inferiorAngle && angle < superiorAngle)
            return true;
    }else{
        if((angle > inferiorAngle && angle < 360) || angle < superiorAngle)
            return true;
    }
    return false;
}

void Tracker::finishAnalysis() {
    //Stopping QTimer
    timer->stop();

    //Finishing statistcs file
    this->writerStatistics->closeFile();

    //Saving path image
    stringstream filename;
    filename << TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data() << "/caminho.bmp";
    imwrite(filename.str(), this->imageTracking);

    //Calculing Learning Index
    this->calcLearningIndex();

    //Saving settings
    this->interface->saveSettings();

    //Releasing space
    this->detector = NULL;
    delete this->timer;
    this->imageTracking.release();
    this->coordinates.clear();

    //Changing status and showing results
    if(this->mode == FAST) {
        this->interface->setStatus(QString::fromStdString(FAST_MODE_GUI_STATUS_FINISH));
        this->interface->setResults(TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data());
        this->interface->blockCancelButton();
    }

    //Verifying analysis queue
    this->interface->setIndexVideo(this->interface->getIndexVideo() + 1);
    if(this->interface->getIndexVideo() < (int) this->videos.size())
        this->startAnalysis();
    else {
        this->interface->restartProgram();
        this->interface = NULL;
    }
}

void Tracker::cancelAnalysis() {
    this->timer->stop();

    this->writerStatistics->closeFile();

    bool removeDir = this->dirCreator->removeDir(TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data());
    if(!removeDir) cout << "erro ao remover diretorio." << endl;

    //Releasing space
    this->detector = NULL;
    delete this->timer;
    this->imageTracking.release();
    this->coordinates.clear();

    //Deleting analysis from queue
    removeFromQueue(this->interface->getIndexVideo());

    //Verifying analysis queue
    if(this->interface->getIndexVideo() < (int) this->videos.size())
        this->startAnalysis();
    else {
        this->interface->restartProgram();
    }
}

void Tracker::restartAnalysis() {
    this->timer->stop();

    this->writerStatistics->closeFile();

    bool removeDir = this->dirCreator->removeDir(TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data());
    if(!removeDir) cout << "erro ao remover diretorio." << endl;

    //Releasing space
    this->detector = NULL;
    delete this->timer;
    this->imageTracking.release();
    this->coordinates.clear();

    this->startAnalysis();
}

void Tracker::setTimer(bool status) {
    if(status)
        this->timer->start();
    else
        this->timer->stop();
}

QTimer* Tracker::getTimer() {
    return this->timer;
}

void Tracker::saveSnapshot() {

    //Creating directory
    this->dirCreator->create(TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data()
                             + SNAPSHOT_DIR_NAME);

    //Saving image
    stringstream filename;
    filename << TESTES_DIR_NAME + "/" + this->cap->getFileName().section("/", -1, -1).toAscii().data()
                + SNAPSHOT_DIR_NAME << "/snapshot" << this->countSnapshot++ << ".bmp";
    imwrite(filename.str(), this->interface->getImage());
}

void Tracker::removeFromQueue(int index) {
    this->videos.erase(this->videos.begin() + index);
}
