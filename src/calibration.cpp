#include "calibration.h"
#include "calibrationgui.h"

Calibration::Calibration(QString fileName, int board_w, int board_h, int n_boards, float measure) {

    this->cg = CalibrationGUI::getInstance();
    this->cg->setTotalValueLoadBar(n_boards);
    this->cg->setProgressBar(true);

    this->fileName = fileName;
    this->boardW = board_w;
    this->boardH = board_h;
    this->nBoards = n_boards;
    this->measure = measure;

    //Getting new corners point with a arbitrary value of pixel size (L variable)
    this->pixelRatio = this->measure / L;
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

        //Modifying ProgressBar
        this->cg->setValueLoadBar(successes);

        //Showing image
        this->cg->showImage(image);
    }

     return gray_image;
}

void Calibration::writeImageInfos() {
    outXML* writer = new outXML();
    FileStorage fs = writer->startFile(CALIBRATION_DIR_NAME + INFO_FILE_NAME);
    writer->writeInFile("image_width", this->imageSize.width, &fs);
    writer->writeInFile("image_height", this->imageSize.height, &fs);
    writer->writeInFile("board_width", this->boardW, &fs);
    writer->writeInFile("board_height", this->boardH, &fs);
    writer->writeInFile("n_boards", this->nBoards, &fs);
    writer->writeInFile("square_size", this->measure, &fs);
    writer->closeFile(&fs);
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
    outXML* writer = new outXML();
    FileStorage fs = writer->startFile(CALIBRATION_DIR_NAME + CALIB_FILE_NAME);
    writer->writeInFile("intrinsic_matrix", intrinsic_Matrix, &fs);
    writer->writeInFile("distortion_coeffs", distortion_coeffs, &fs);
    writer->writeInFile("rotation_vector", rvecs, &fs);
    writer->writeInFile("translation_vector", tvecs, &fs);
    writer->writeInFile("object_points", this->objectPoints, &fs);
    writer->writeInFile("image_points", this->imagePoints, &fs);
    writer->closeFile(&fs);
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

    //Save test image
    imwrite(CALIBRATION_DIR_NAME + "/homographyApply.bmp", this->applyHomography);

    //Setting GUI
    this->cg->setTool(true);

    //Default Settings
    this->cg->setRadiusMaxValue(133);
    this->cg->setRadiusMinValue(110);

}

void Calibration::calcRadius() {

    //Finding center and radius
    Mat drawCircle;
    applyHomography.copyTo(drawCircle);
    vector<Vec3f> circles;
    HoughCircles(drawCircle, circles, HOUGH_GRADIENT, 1, 10, 100, 30, this->cg->getRadiusMinValue(),
                 this->cg->getRadiusMaxValue());

    //Draw circle
    if(circles.size() > 0) {

        //Setting Status
        this->cg->setStatus("Calculando circulo...");
        this->cg->setBtOKRadius(true);

        cvtColor(drawCircle, drawCircle, CV_GRAY2RGB);
        this->c = circles[0];
        circle(drawCircle, Point(c[0], c[1]), c[2], Scalar(255, 0, 0), 1, 8, 0);
        circle(drawCircle, Point(c[0], c[1]), 1, Scalar(255, 0, 0), 1, 8, 0);

        //Showing circle found
        this->cg->showImage(drawCircle);
    }else{
        this->cg->setStatus("Circulo nao encontrado!");
        drawCircle = Mat::zeros(704, 480, CV_8UC3);
        this->cg->showImage(drawCircle);
        this->cg->setBtOKRadius(false);
    }
}

void Calibration::writeHomographyInfos() {
    outXML* writer = new outXML();
    FileStorage fs = writer->startFile(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME);
    writer->writeInFile("src_points", srcPoints, &fs);
    writer->writeInFile("dst_points", dstPoints, &fs);
    writer->writeInFile("homography_matrix", homography2, &fs);
    writer->writeInFile("center_circle", Point(c[0], c[1]), &fs);
    writer->writeInFile("radius", c[2], &fs);
    writer->writeInFile("pixel_ratio", this->pixelRatio, &fs);
    writer->closeFile(&fs);
}
