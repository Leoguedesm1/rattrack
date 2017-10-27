#include "calibration.h"
#include "calibrationgui.h"
#define PI 3.14159265

Calibration::Calibration(QString fileName, int board_w, int board_h, int n_boards, float measure) {

    this->cg = CalibrationGUI::getInstance();

    this->fileName = fileName;
    this->boardW = board_w;
    this->boardH = board_h;
    this->nBoards = n_boards;
    this->measure = measure;

    //Getting new corners point with a arbitrary value of pixel size (L variable)
    this->pixelRatio = this->measure / L;

    this->writerCalibration = new WriterXML();
    this->writerHomography = new WriterXML();
    this->writerImageInfos = new WriterXML();
    this->writerQuadrant = new WriterXML();
}

void Calibration::executeCalibration() {

    //Setting GUI
    this->cg->showStatus();
    this->cg->setStatus("Iniciando...");

    //Open Video
    this->setCaptureVideo();

    //Verifying if the video has opened
    if(!(this->captureVideo.isOpened()))
        this->cg->errorOcurred("Impossivel abrir arquivo de video!");

    //Analyzing video to get calibration infos (corners, imagepoints and object points)
    this->cg->setStatus("Analisando imagens...");
    Mat imageTest = analyzisVideo();

    //Write a file with calibration images info
    writeImageInfos();

    /*//Getting Calibration - Optional (we do not use this in program)
    getCalibration();*/

    //Getting Homography
    this->cg->setStatus("Calculando homografia...");
    getHomography(imageTest);
}

void Calibration::setCaptureVideo() {
    captureVideo.open(this->fileName.toAscii().data());
}

VideoCapture Calibration::getCaptureVideo() {
    return this->captureVideo;
}

Mat Calibration::analyzisVideo() {

    int successes=0;
    Mat image;
    Mat gray_image;

    while(successes < this->nBoards) {

        this->captureVideo >> image;

        //Verifying if cannot make video calibration with this video
        if(image.empty())
            this->cg->errorOcurred("Nao foi possivel detectar o numero de imagens necessarias!");

        cvtColor(image, gray_image, CV_RGB2GRAY);

        //Finding chessboard pattern in image
        bool found = findChessboardCorners(gray_image, Size(this->boardW, this->boardH), this->corners);

        //if found then we get all calibration infos
        if(found) {
            cornerSubPix(gray_image, this->corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));

            drawChessboardCorners(image, Size(this->boardW, this->boardH), this->corners, found);

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

        //Name of image that going to be save
        stringstream img;
        img << CALIBRATION_DIR_NAME << "/calib" << ++successes << ".bmp";
        string imageName = img.str();

        //Getting size
        this->imageSize = Size(image.cols, image.rows);

        //Saving image
        imwrite(imageName, image);

        //Showing image
        this->cg->showImage(image);
    }

     return gray_image;
}

void Calibration::writeImageInfos() {

    this->writerImageInfos->startFile(CALIBRATION_DIR_NAME + INFO_FILE_NAME);
    this->writerImageInfos->write("image_width"); this->writerImageInfos->write(this->imageSize.width);
    this->writerImageInfos->write("image_height"); this->writerImageInfos->write(this->imageSize.height);
    this->writerImageInfos->write("board_width"); this->writerImageInfos->write(this->boardW);
    this->writerImageInfos->write("board_height"); this->writerImageInfos->write(this->boardH);
    this->writerImageInfos->write("n_boards"); this->writerImageInfos->write(this->nBoards);
    this->writerImageInfos->write("square_size"); this->writerImageInfos->write(this->measure);
    this->writerImageInfos->closeFile();
}

/*void Calibration::getCalibration() {

    vector< Mat> rvecs, tvecs;
    Mat intrinsic_Matrix(3, 3, CV_64F);
    Mat distortion_coeffs(8, 1, CV_64F);

    calibrateCamera(objectPoints, imagePoints, imageSize, intrinsic_Matrix, distortion_coeffs, rvecs, tvecs);

    //SAVING CALIBRATION
    writeCalibrationInfos(intrinsic_Matrix, distortion_coeffs, rvecs, tvecs);
}

void Calibration::writeCalibrationInfos(Mat intrinsic_Matrix, Mat distortion_coeffs, vector<Mat> rvecs, vector<Mat> tvecs) {

    this->writerCalibration->startFile(CALIBRATION_DIR_NAME + CALIB_FILE_NAME);
    this->writerCalibration->write("intrinsic_matrix"); this->writerCalibration->write(intrinsic_Matrix);
    this->writerCalibration->write("distortion_coeffs"); this->writerCalibration->write(distortion_coeffs);
    this->writerCalibration->write("rotation_vector"); this->writerCalibration->write(rvecs);
    this->writerCalibration->write("translation_vector"); this->writerCalibration->write(tvecs);
    this->writerCalibration->write("object_points"); this->writerCalibration->write(this->objectPoints);
    this->writerCalibration->write("image_points"); this->writerCalibration->write(this->imagePoints);
    this->writerCalibration->closeFile();
}*/

void Calibration::getHomography(Mat imageTest) {

    Point2f point1, point2, point3, point4;

    point1.x = 0;
    point1.y = 0;
    point2.x = 0;
    point2.y = 0;
    point3.x = 0;
    point3.y = 0;
    point4.x = 0;
    point4.y = 0;

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

    point1.x = point1.x / (float)this->nBoards;
    point1.y = point1.y / (float)this->nBoards;
    point2.x = point2.x / (float)this->nBoards;
    point2.y = point2.y / (float)this->nBoards;
    point3.x = point3.x / (float)this->nBoards;
    point3.y = point3.y / (float)this->nBoards;
    point4.x = point4.x / (float)this->nBoards;
    point4.y = point4.y / (float)this->nBoards;

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

    //Getting new and correct homography
    this->homography2 = findHomography(cornerQuad, finalCornerQuad, LMEDS);

    warpPerspective(imageTest, this->applyHomography, this->homography2, Size(imageSize.width, imageSize.height));
    cvtColor(this->applyHomography, imageTest, CV_GRAY2RGB);
    this->cg->showImage(imageTest);

    //Save test image
    imwrite(CALIBRATION_DIR_NAME + "/homographyApply.bmp", this->applyHomography);

    //Setting GUI
    this->cg->setTool(true);

    this->cg->setStatus("Para finalizar configure a area de deteccao e os quadrantes com as ferramentas ao lado!");

}

void Calibration::drawCircle(Point2d center, double radius) {
    Mat drawCircle;
    applyHomography.copyTo(drawCircle);
    cvtColor(drawCircle, drawCircle, CV_GRAY2RGB);

    if(center.x == -1 && center.y == -1) {
        this->cg->showImage(drawCircle);
        return;
    }


    //Draw center
    circle(drawCircle, Point(center.x, center.y), 1, Scalar(0,0,255), 5);

    //Draw circle
    if(radius != -1)
        circle(drawCircle, Point(center.x, center.y), radius, Scalar(0,0,255), 3);
    this->cg->showImage(drawCircle);
}

void Calibration::writeHomographyInfos() {
    this->writerHomography->startFile(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME);
    this->writerHomography->write("src_points"); this->writerHomography->write(srcPoints);
    this->writerHomography->write("dst_points"); this->writerHomography->write(dstPoints);
    this->writerHomography->write("homography_matrix"); this->writerHomography->write(homography2);
    this->writerHomography->write("center_circle"); this->writerHomography->write(this->cg->getCenter());
    this->writerHomography->write("radius"); this->writerHomography->write(this->cg->getRadius());
    this->writerHomography->write("pixel_ratio"); this->writerHomography->write(this->pixelRatio);
    this->writerHomography->closeFile();
}

Point2d Calibration::findIntersection(Point2d center, Point2d pLine, double radius) {

    Point2d point1, point2;
    //Equation of line: y-y1 = m(x-x1) -> line is: (center.x, center.y) to (line.x, line.y)
    //Finding m (tangent of line)
    double m = (center.y - pLine.y) / (center.x - pLine.x);

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
    diffX = pLine.x - point1.x;
    diffY = pLine.y - point1.y;
    distance1 = (diffX*diffX) + (diffY*diffY);
    diffX = pLine.x - point2.x;
    diffY = pLine.y - point2.y;
    distance2 = (diffX*diffX) + (diffY*diffY);

    if(distance1 <= distance2) return point1;
    else return point2;
}

void Calibration::drawLine(vector<Point2d> points, Point2d center, double radius) {
    Mat drawCircle;
    applyHomography.copyTo(drawCircle);
    cvtColor(drawCircle, drawCircle, CV_GRAY2RGB);

    circle(drawCircle, Point(center.x, center.y), 1, Scalar(0,0,255), 5);
    circle(drawCircle, Point(center.x, center.y), radius, Scalar(0,0,255), 3);

    for(int i = 0; i < (int) points.size(); i++) {

        if(points.at(i).x != -1 && points.at(i).y != -1)
            line(drawCircle, Point(points.at(i).x, points.at(i).y), Point(center.x, center.y), Scalar(0,0,255), 3);
    }

    this->cg->showImage(drawCircle);
}

void Calibration::calculateQuads() {

    vector<Point2d> pointQuads = this->cg->getPointsQuad();
    Point2d center = this->cg->getCenter();
    double radius = this->cg->getRadius();

    //Calculing the "pattern" quadrants
    vector<Point2d> normalPoints = vector<Point2d>(4);

    //Clockwise
    normalPoints.at(0) = Point2d(center.x+radius, center.y);
    normalPoints.at(1) = Point2d(center.x, center.y-radius);
    normalPoints.at(2) = Point2d(center.x-radius, center.y);
    normalPoints.at(3) = Point2d(center.x, center.y+radius);

    //Calculing tangent of triangle formed with lineQuad (1,2,3,4) and normalQuad (1,2,3,4)
    for(int i = 0; i < (int)normalPoints.size(); i++) {
        Point2d aux;
        aux = Point2d(pointQuads.at(i).x, normalPoints.at(i).y);
        //Distance between pointQuad(i) and aux
        double oppositeLeg = sqrt(((pointQuads.at(i).x - aux.x)*(pointQuads.at(i).x - aux.x)) + ((pointQuads.at(i).y - aux.y)*(pointQuads.at(i).y - aux.y)));
        //Distance between center and aux
        double adjacentLeg = sqrt(((center.x - aux.x)*(center.x - aux.x)) + ((center.y - aux.y)*(center.y - aux.y)));
        double tangent = oppositeLeg / adjacentLeg;
        anglesQuad.push_back(atan(tangent) * 180/PI);
    }

    //Adjusting angles for 360 degrees
    anglesQuad.at(1)+=270;
    anglesQuad.at(2)+=180;
    anglesQuad.at(3)+=90;
}

void Calibration::writeQuadrantInfos() {
    this->writerQuadrant->startFile(CALIBRATION_DIR_NAME + QUADRANT_FILE_NAME);
    this->writerQuadrant->write("quad1_angle"); this->writerQuadrant->write(anglesQuad.at(0));
    this->writerQuadrant->write("quad2_angle"); this->writerQuadrant->write(anglesQuad.at(1));
    this->writerQuadrant->write("quad3_angle"); this->writerQuadrant->write(anglesQuad.at(2));
    this->writerQuadrant->write("quad4_angle"); this->writerQuadrant->write(anglesQuad.at(3));
    this->writerQuadrant->closeFile();
}
