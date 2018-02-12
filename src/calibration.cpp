#include "calibration.h"
#include "calibrationgui.h"
#include "filesanddirectoriesconstants.h"
#include "homographyfileconstants.h"
#include "quadrantsfileconstants.h"

const double INF = std::numeric_limits<double>::infinity();
const int L = 100;

#define PI 3.14159265

Calibration::Calibration(Video *cap, int board_w, int board_h, int n_boards, float measure) {

    this->interface = CalibrationGUI::getInstance();

    //Creating directory to save calibration infos
    this->dirCreator = new DirectoryCreator();
    this->dirCreator->create(CALIBRATION_DIR_NAME);

    this->video = cap;
    this->boardW = board_w;
    this->boardH = board_h;
    this->nBoards = n_boards;
    this->measure = measure;
    this->imageSize = Size(this->video->getWidth(), this->video->getHeight());

    //Getting new corners point with a arbitrary value of pixel size (L variable)
    this->pixelRatio = this->measure / L;

    this->center = Point2f(-1,-1);
    this->radius = 1;

    //Start Calibration
    if(!findChessboardCalibration()) {
        this->interface->errorMessage("Nao foram encontrados frames suficientes para calibracao!");
        this->dirCreator->removeDir(CALIBRATION_DIR_NAME);
        this->interface->startGUILoadFile();
    }else{
        this->calcHomography();
    }
}

void Calibration::cancelCalibration() {
    this->dirCreator->removeDir(CALIBRATION_DIR_NAME);
    this->interface->resetProgram();
}

bool Calibration::findChessboardCalibration() {

    int foundFrames = 0;
    Mat src, srcGray;

    while(foundFrames < this->nBoards) {

        this->video->getCaptureVideo() >> src;

        //Verifying if cannot make video calibration with this video
        if(src.empty())
            return false;

        cvtColor(src, srcGray, CV_RGB2GRAY);

        //Finding chessboard pattern in image
        bool found = findChessboardCorners(srcGray, Size(this->boardW, this->boardH), this->corners);

        //if found then we get all calibration infos
        if(found) {
            cornerSubPix(srcGray, this->corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));

            drawChessboardCorners(src, Size(this->boardW, this->boardH), this->corners, found);

            foundFrames++;

            if((int)this->corners.size() == this->boardW*this->boardH) {
                vector< Point2f> v_tImgPT;
                vector< Point3f> v_tObjPT;

                for(int i = 0; i < (int)this->corners.size(); ++i) {
                    Point2f tImgPT;
                    Point3f tObjPT;

                    tImgPT.x = this->corners[i].x;
                    tImgPT.y = this->corners[i].y;

                    tObjPT.x = i%this->boardW*this->measure;
                    tObjPT.y = i/this->boardW*this->measure;
                    tObjPT.z = 0;

                    v_tImgPT.push_back(tImgPT);
                    v_tObjPT.push_back(tObjPT);
                }

                this->imagePoints.push_back(v_tImgPT);
                this->objectPoints.push_back(v_tObjPT);
            }
        }
    }

    srcGray.copyTo(this->homographyImage);
    return true;
}

void Calibration::calcHomography() {

    Point2f point1(0,0), point2(0,0), point3(0,0), point4(0,0);

    //getting approximately the corners points
    for(size_t i = 0; i < this->imagePoints.size(); i++) {
        point1.x = point1.x + (this->imagePoints[i][this->boardW-1]).x;
        point1.y = point1.y + (this->imagePoints[i][this->boardW-1]).y;
        point2.x = point2.x + (this->imagePoints[i][0]).x;
        point2.y = point2.y + (this->imagePoints[i][0]).y;
        point3.x = point3.x + (this->imagePoints[i][this->boardW*this->boardH - (this->boardW)]).x;
        point3.y = point3.y + (this->imagePoints[i][this->boardW*this->boardH - (this->boardW)]).y;
        point4.x = point4.x + (this->imagePoints[i][this->boardW*this->boardH - 1]).x;
        point4.y = point4.y + (this->imagePoints[i][this->boardW*this->boardH - 1]).y;
    }

    point1 = Point2f(point1.x / (float) this->nBoards, point1.y / (float)this->nBoards);
    point2 = Point2f(point2.x / (float) this->nBoards, point2.y / (float)this->nBoards);
    point3 = Point2f(point3.x / (float) this->nBoards, point3.y / (float)this->nBoards);
    point4 = Point2f(point4.x / (float) this->nBoards, point4.y / (float)this->nBoards);

    srcPoints.push_back(point1);
    srcPoints.push_back(point2);
    srcPoints.push_back(point3);
    srcPoints.push_back(point4);

    dstPoints.push_back(Point2f(299, 299));
    dstPoints.push_back(Point2f(299+L, 299));
    dstPoints.push_back(Point2f(299+L, 299-L));
    dstPoints.push_back(Point2f(299, 299-L));

    //Getting homography
    Mat homography = findHomography(srcPoints, dstPoints, LMEDS);

    //Getting normal size image and the convex hull
    vector< Point2f> cornerQuad, finalCornerQuad;

    cornerQuad.push_back(Point2f(0, 0));
    cornerQuad.push_back(Point2f(imageSize.width, 0));
    cornerQuad.push_back(Point2f(imageSize.width, imageSize.height));
    cornerQuad.push_back(Point2f(0, imageSize.height));

    perspectiveTransform(cornerQuad, finalCornerQuad, homography);

    double minx = INF, maxx = -INF, miny = INF, maxy = -INF;
    for (int i=0; i < 4; i++){
        minx = min(minx, (double)finalCornerQuad[i].x);
        miny = min(miny, (double)finalCornerQuad[i].y);
        maxx = max(maxx, (double)finalCornerQuad[i].x);
        maxy = max(maxy, (double)finalCornerQuad[i].y);
    }

    int wwidth = maxx - minx;
    int wheight = maxy - miny;

    for (int i=0; i < 4; i++){
        finalCornerQuad[i].x =  (finalCornerQuad[i].x - minx)/( (float) wwidth ) * imageSize.width;
        finalCornerQuad[i].y = (finalCornerQuad[i].y - miny)/( (float) wheight ) * imageSize.height;
    }

    //Getting new and correct homography
    this->homographyMatrix = findHomography(cornerQuad, finalCornerQuad, LMEDS);

    warpPerspective(this->homographyImage, this->homographyImage, this->homographyMatrix, Size(imageSize.width, imageSize.height));
    cvtColor(this->homographyImage, this->homographyImage, CV_GRAY2RGB);

    //Updating GUI
    this->interface->startGUICalibration(this->homographyImage);
}

Point2d Calibration::getCenter() {
    return this->center;
}

void Calibration::setCenter(Point2d point) {
    center = point;
}

void Calibration::setRadius(double radius) {
    this->radius = radius;
}

double Calibration::getRadius() {
    return this->radius;
}

void Calibration::drawCircle() {

    this->homographyImage.copyTo(this->drawCircleMatrix);

    if(center.x != -1 && center.y != -1) {
        //Draw center
        circle(this->drawCircleMatrix, Point(center.x, center.y), 1, Scalar(0,0,255), 3);
        //Draw circle
        circle(this->drawCircleMatrix, Point(center.x, center.y), radius, Scalar(0,0,255), 3);
    }

    this->interface->showImage(this->drawCircleMatrix);
}

Point2d Calibration::findIntersection(Point2d point) {

    Point2d point1, point2;
    //Equation of line: y-y1 = m(x-x1) -> line is: (center.x, center.y) to (line.x, line.y)
    //Finding m (tangent of line)
    double m = (center.y - point.y) / (center.x - point.x);

    //Taking (x1,y1) = (line.x, line.y) we have the equation like this: y = m*x - m*line.x + line.y
    //Take b = line.y - m*line.x
    double b = center.y - (m*center.x);

    //Now we have y = m*x + b
    //Equation of circle: (x - center.x)^2 + (y - center.y)^2 = radius^2
    //Replacing y for m*x + b we have: (x - center.x)^2 + (m*x + b - center.y)^2 = radius^2
    //Take b1 = b - center.y
    double b1 = b - center.y;

    //So we have (x - center.x)^2 + (m*x + b1)^2 = radius^2
    //Resolving this we found: (1+m^2)x^2 + (2*m*b1 - 2*center.x)x + (b1^2 - radius^2) = 0
    //Consider a1 = 1+m^2, b2 = 2(m*b1-centerx), c1 = centerx^2 + b1^2 - radius^2
    double a1 = 1 + (m*m);
    double b2 = 2*((m*b1)-center.x);
    double c1 = (center.x*center.x) + (b1*b1) - (radius*radius);

    //Finally we have: a1*x^2 + b2*x + c1 = 0
    //Finding the Delta: delta = b2^2 - 4*a1*c1
    double delta = ((b2*b2) - (4*a1*c1));
    double raiz = sqrt(delta);

    //Resolving x
    point1.x = (raiz - b2) / (2*a1);
    point2.x = -(raiz + b2) / (2*a1);

    //Resolving y with the equation of line y = m*x + b
    point1.y = (m*point1.x) + b;
    point2.y = (m*point2.x) + b;

    //To finish we have to choose the point closest to the clicked, so we calcule the distance and compare
    double diffX, diffY, distance1, distance2;
    diffX = point.x - point1.x;
    diffY = point.y - point1.y;
    distance1 = (diffX*diffX) + (diffY*diffY);
    diffX = point.x - point2.x;
    diffY = point.y - point2.y;
    distance2 = (diffX*diffX) + (diffY*diffY);

    if(distance1 <= distance2) return point1;
    else return point2;
}

void Calibration::addPoint() {
    lines.push_back(Point2d(-1,-1));
}

void Calibration::setPoint(Point2d point, int index) {
    lines.at(index) = this->findIntersection(point);
}

Point2d Calibration::getPoint(int index) {
    return lines.at(index);
}

void Calibration::clearPoints() {
    lines.clear();
}

void Calibration::removePoint(int index) {
    lines.erase(lines.begin() + index);
}

void Calibration::drawLines() {
    Mat drawLines;
    this->drawCircleMatrix.copyTo(drawLines);

    for(int i = 0; i < (int) lines.size(); i++) {

        if(lines.at(i).x != -1 && lines.at(i).y != -1)
            line(drawLines, Point(lines.at(i).x, lines.at(i).y), Point(center.x, center.y), Scalar(0,0,255), 3);
    }

    this->interface->showImage(drawLines);
}

void Calibration::calcAngles() {

    for(int i = 0; i < (int)lines.size(); i++) {

        Point2d point = lines.at(i);

        //Legs
        double angle;
        double a = point.x - center.x;
        double b = point.y - center.y;

        /*Quadrants:
         * 1st - a > 0 && b < 0
         * 2nd - a < 0 && b < 0
         * 3th - a < 0 && b > 0
         * 4th - a > 0 && b > 0*/
        if(a > 0 && b < 0) {
            angle = atan(b/a) * 180/PI * (-1);
        }else if(a < 0 && b < 0) {
            angle = (atan(a/b) * 180/PI) + 90;
        }else if(a < 0 && b > 0) {
            angle = (atan(b/a) * 180/PI * (-1)) + 180;
        }else if(a > 0 && b > 0) {
            angle = (atan(a/b) * 180/PI) + 270;
        }

        anglesQuad.push_back(angle);
    }

    //Insertion sort
    double key;
    for(int i = 0; i < (int) anglesQuad.size(); i++) {
        key = anglesQuad.at(i);
        int j = i-1;

        while(j >= 0 && anglesQuad.at(j) > key) {
            anglesQuad.at(j+1) = anglesQuad.at(j);
            j--;
        }

        anglesQuad.at(j+1) = key;
    }
}

void Calibration::writeCalibrationInfos() {

    writer = new WriterXML();

    if(anglesQuad.size() > 0) {
        writer->startFile(CALIBRATION_DIR_NAME + QUADRANT_FILE_NAME);
        writer->write(QUAD1_NAME); writer->write(anglesQuad.at(0));
        writer->write(QUAD2_NAME); writer->write(anglesQuad.at(1));
        writer->write(QUAD3_NAME); writer->write(anglesQuad.at(2));
        writer->write(QUAD4_NAME); writer->write(anglesQuad.at(3));
        writer->closeFile();
    }

    writer->startFile(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME);
    writer->write(IN_POINTS_NAME); writer->write(srcPoints);
    writer->write(OUT_POINTS_NAME); writer->write(dstPoints);
    writer->write(MATRIX_HOMOGRAPHY_NAME); writer->write(homographyMatrix);
    writer->write(CENTER_CIRCLE_NAME); writer->write(center);
    writer->write(RADIUS_NAME); writer->write(radius);
    writer->write(PIXEL_RATIO_NAME); writer->write(pixelRatio);
    writer->closeFile();
}
