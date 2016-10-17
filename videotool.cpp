#include <stdio.h>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

#include <limits>

const double INF = std::numeric_limits<double>::infinity();

using namespace cv;
using namespace std;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0 

/*Variáveis Globais*/
// Imagen 704x480
Mat src_frame, out, tela, track;
int threshold_value = 130; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar
double centerX, centerY;

VideoCapture src;

/*Area Bar*/
int min_area = 0;
int max_area = 300;
int const max_area_value = 1500;

/*Reset Bar*/
int reset = 0;
int const max_reset = 1;

/*teste*/
//vector<Point2f> centerTrans;

/*Assinatura das funções*/
void Threshold( int, void* );
void tracking(Mat aux, Mat track);
void Reset( int, void* );

int main(int argc, char** argv) {
	/*Capturando video*/
    src.open(argv[1]);
	src.set(CAP_PROP_POS_MSEC, 380000);
	
	/*Verificando se o video foi aberto*/
    if( !src.isOpened() )
        throw "Error when reading steam_avi";
                 
	/*Janela*/
    namedWindow( "VideoTool", WINDOW_NORMAL );  	
    resizeWindow( "VideoTool", 704, 480);
    namedWindow( "Trackbars", WINDOW_NORMAL);
    
    /*Trackbar reseta o video*/
    createTrackbar( "Reset", "Trackbars", &reset, max_reset, Reset );
 	/*Criando a Trackbar para alterar o valor do Thresholding*/
    createTrackbar( "Threshold", "Trackbars", &threshold_value, max_value, Threshold );
    /*Trackbars para a area do rato*/
    createTrackbar( "Area Min", "Trackbars", &min_area, max_area_value, Threshold );
  	createTrackbar( "Area Max", "Trackbars", &max_area, max_area_value, Threshold );
    
 	//tela = Mat(src.get(CV_CAP_PROP_FRAME_HEIGHT), src.get(CV_CAP_PROP_FRAME_WIDTH)*2, CV_8U3C, Scalar(0));
 	//track = Mat(src.get(CV_CAP_PROP_FRAME_HEIGHT), src.get(CV_CAP_PROP_FRAME_WIDTH), CV_8U, Scalar(0));
 	track = Mat(320, 453, CV_8U, Scalar(0));
 	
 	
 	Reset (0, 0);
    
    
    /*Loop*/
    for( ; ; )
    {
    	src >> src_frame;
    	
    	if(src_frame.empty())
            break;
        
        Mat src_gray, transform = Mat::zeros( src_gray.rows, src_gray.cols, CV_8UC3 );
        
        /*Passando frame do vídeo para GrayScale*/
        cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY);
        
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
    	
    	Mat H = findHomography(inputQuad, outputQuad, LMEDS);
    	
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
    	
    	Size warpSize(maxx - minx, maxy - miny);
    	
    	for (int i=0; i < 4; i++){
    	  finalCornerQuad[i].x -= minx;
    	  finalCornerQuad[i].y -= miny;
    	}
    	
    	Mat H2 = findHomography(cornerQuad, finalCornerQuad, LMEDS);
    	
    	warpPerspective(src_gray, transform, H2, warpSize);
    	
    	/*Verificando se é um círculo perfeito*/
    	centerTrans.push_back(Point2f((double)248.5, (double)248.5));
    	
    	Mat invH = findHomography(outputQuad, inputQuad, LMEDS);
    	
    	perspectiveTransform(centerTrans, center, invH);
    	perspectiveTransform(center, centerTrans, H2);
    	centerX = centerTrans[0].x;
    	centerY = centerTrans[0].y;
    	Mat transform2;
    	cvtColor(transform, transform2, CV_GRAY2RGB);	
  		cvtColor(transform, transform, CV_GRAY2RGB);
  		
    	Scalar color = Scalar(0, 255, 0);   	
    	circle( transform2, centerTrans[0], 72, color, CV_FILLED, CV_AA, 0);
    	
    	/*Adicionando transparencia no círculo*/
    	double alpha = 0.5;
    	addWeighted(transform, alpha, transform2, 1.0 - alpha , 0.0, transform2); 
    	
    	//imshow( "VideoTool", transform2);
    	
        /*Copiando matrix de transformação para out*/
   		cvtColor(transform, transform, CV_RGB2GRAY);
   		out = Mat::zeros( src_gray.rows, src_gray.cols, src_gray.type() );
		transform.copyTo(out);
    // reset video
 	//for(;;){
 	//  src >> frame;  	  
  		/*Copiando a imagem para a saída da tela*/
  		//src_gray.copyTo(tela(Rect(0, 0, src_gray.cols, src_gray.rows)));
  		
  		/*Inicializando a função Thresholding*/
 		Threshold( 0, 0 );
 		
        waitKey(20); // waits to display frame
    }
    waitKey(0); // key press to close window
}


void Reset( int, void* ) {
	if(reset == 0 || reset == 1) 
		src.set(CAP_PROP_POS_MSEC, 380000);
}

void Threshold( int, void* ) {
	Mat aux(out.rows, out.cols, CV_8U, Scalar(0));
	
	/*Threshold*/
	//cout << "center: (" << centerX << ", " << centerY << ")" << "\n";
	for(int i = 0; i < out.rows; i++) {
		for(int j = 0; j < out.cols; j++) {
			if( ((j-centerX)*(j-centerX)) + ((i-centerY)*(i-centerY)) <= 72*72) {
				if (out.at<uchar>(i, j) < threshold_value) 
					aux.at<uchar>(i, j) = BLACK;
				else
					aux.at<uchar>(i, j) = WHITE;
			}else
				aux.at<uchar>(i, j) = BLACK;
		}
	}
  
	medianBlur(aux, aux, 5);
	//imshow( "VideoTool", aux);
  
	tracking(aux, track);

}

void tracking(Mat aux, Mat track) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int i;
	
	findContours( aux, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
  	
  	// Get area
  	vector<float> areas(contours.size());
  	//cout << "areas\n";
 	
  	for(i = 0; i < contours.size(); i++ ){
  	    float area = contourArea(contours[i], false);
  		if (area >= min_area && area <= max_area) {
  		  cout << "rato de area: " << area << " indice: " << i << "\n";
  		  cout << "track: " << track.rows << " " << track.cols << "\n";
  		  cout << "src: " << out.rows << " " << out.cols << "\n";
  		  
  		  break; 
  		}
  	}
  	
  	if (i >= contours.size()){
  	  cout << "rato não encontrado\n";
  	  //imshow("VideoTool", aux);
  	  return;
  	}
  		
  	/// Get the moments
  	vector<Moments> mu( contours.size() );
  	mu[i] = moments( contours[i], false ); 

	///  Get the mass centers:
 	vector<Point2f> mc( contours.size() );
  	mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
  	
  	track.at<uchar>(mc[i].y, mc[i].x) = WHITE;
  	
  	cvtColor(out, out, CV_GRAY2RGB);
  	//Circulando contornos e destacando centro de massa  
  	Scalar colordraw = Scalar(255,255,0);
  	Scalar color = Scalar(0,255,0);
    drawContours( out, contours, i, colordraw, 1, 8, hierarchy, 0, Point() );
    circle( out, mc[i], 2.5, color, -1);
  	
  	Mat track2;
  	cvtColor(track, track2, CV_GRAY2RGB);
	addWeighted(out, 1, track2, 1, 0.0, out); 
  	  	
   	/*Copiando a imagen corrigida por ROI*/
    //aux.copyTo(tela(Rect(left1+tela.cols/2, top, aux.cols, aux.rows))); 
  	//out.copyTo(tela(Rect(tela.cols/2, 0, out.cols, out.rows)));
 	 	
	imshow( "VideoTool", out);
}
