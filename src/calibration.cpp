#include "calibration.h"

Calibration::Calibration(QString fileName, int board_w, int board_h, int n_boards, float measure) {
    this->fileName = fileName;
    this->boardW = board_w;
    this->boardH = board_h;
    this->nBoards = n_boards;
    this->measure = measure;

    this->error = false;
}

bool Calibration::getError() {
    return this->error;
}

void Calibration::executeCalibration() {

    //Open Video
    this->setCaptureVideo();

    //Verifying if the video has opened
    if(!(this->captureVideo.isOpened())){
        this->error = true;
        return;
    }

    //Analyzing video to get calibration infos (corners, imagepoints and object points)
    Mat imageTest = analyzisVideo();

    //Verifying analyzis video errors
    if(this->error == true)
        return;

    //Write a file with calibration images info
    writeImageInfos();

    //Getting Calibration - Optional (we do not use this in program)
    getCalibration();

    //Getting Homography
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
        if(image.empty()) {
            this->error = true;
            return gray_image;
        }

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

        /*Showing image - optional
         * imshow("calib", image);
         * waitKey(10); */
    }

     return gray_image;
}

void Calibration::writeImageInfos() {

    FileStorage fs(CALIBRATION_DIR_NAME + INFO_FILE_NAME, FileStorage::WRITE);
    fs << "image_width" << this->imageSize.width;
    fs << "image_height" << this->imageSize.height;
    fs << "board_width" << this->boardW;
    fs << "board_height" << this->boardH;
    fs << "n_boards" << this->nBoards;
    fs << "square_size" << this->measure;

    fs.release();

}

void Calibration::getCalibration() {

    vector< Mat> rvecs, tvecs;
    Mat intrinsic_Matrix(3, 3, CV_64F);
    Mat distortion_coeffs(8, 1, CV_64F);

    calibrateCamera(objectPoints, imagePoints, imageSize, intrinsic_Matrix, distortion_coeffs, rvecs, tvecs);

    //SAVING CALIBRATION
    writeCalibrationInfos(intrinsic_Matrix, distortion_coeffs, rvecs, tvecs);
}

void Calibration::writeCalibrationInfos(Mat intrinsic_Matrix, Mat distortion_coeffs, vector<Mat> rvecs, vector<Mat> tvecs) {
    FileStorage fs(CALIBRATION_DIR_NAME + CALIB_FILE_NAME, FileStorage::WRITE );

    fs << "intrinsic_matrix" << intrinsic_Matrix;
    fs << "distortion_coeffs" << distortion_coeffs;
    fs << "rotation_vector" << rvecs;
    fs << "translation_vector" << tvecs;
    fs << "object_points" << this->objectPoints;
    fs << "image_points" << this->imagePoints;

    fs.release();
}

void Calibration::getHomography(Mat imageTest) {

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

    //Getting new corners point with a arbitrary value of pixel size (L variable)
    int L = 100;
    double pixelRatio = this->measure / L;

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
    Mat homography2 = findHomography(cornerQuad, finalCornerQuad, LMEDS);

    Mat applyHomography;
    warpPerspective(imageTest, applyHomography, homography2, Size(imageSize.width, imageSize.height));

    vector<Vec3f> circles;
    HoughCircles(applyHomography, circles, HOUGH_GRADIENT, 1, 10, 100, 30, 110, 133);

    Vec3i c = circles[0];

    /*Showing circle found
    imshow("calib", applyHomography);
    waitKey();*/

    //Save test image
    imwrite(CALIBRATION_DIR_NAME + "/homographyApply.bmp", applyHomography);

    //SAVING HOMOGRAPHY
    writeHomographyInfos(srcPoints, dstPoints, homography2, Point(c[0], c[1]), c[2], pixelRatio);

}

void Calibration::writeHomographyInfos(vector<Point2f> srcPoints, vector<Point2f> dstPoints, Mat homography2, Point center, int radius, double pixelRatio) {

    FileStorage fs(CALIBRATION_DIR_NAME + HOMOGRAPHY_FILE_NAME, FileStorage::WRITE );

    fs << "src_points" << srcPoints;
    fs << "dst_points" << dstPoints;
    fs << "homography_matrix" << homography2;
    fs << "center_circle" << center;
    fs << "radius" << radius;
    fs << "pixel_ratio" << pixelRatio;

    fs.release();
}
