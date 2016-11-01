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
const double raio = 112;

using namespace cv;
using namespace std;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0 

/*Variáveis Globais*/
// Imagen 704x480
Mat src_frame, out, out_perspective, tela, track, invH, H2;
int threshold_value = 100; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar
double centerX, centerY;
int cont = 1;


VideoCapture src;

/*Area Bar*/
int min_area = 0;
int max_area = 300;
int const max_area_value = 1500;

/*Reset Bar*/
int reset = 0;
int const max_reset = 1;

/*Assinatura das funções*/
void Threshold( int, void* );
void tracking(Mat aux, Mat track);
void Reset( int, void* );
Mat paint();

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
    
    src >> src_frame;
    
    /*Todas as matrizes usadas para transformçãoes*/
    Mat H, src_gray, transform = Mat::zeros( src_frame.rows, src_frame.cols, CV_8U );
    cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY); //Passando frame do vídeo para GrayScale
    const int FHEIGHT = src.get(CV_CAP_PROP_FRAME_HEIGHT);
    const int FWIDTH = src.get(CV_CAP_PROP_FRAME_WIDTH);
 	tela = Mat(FHEIGHT, FWIDTH*2, CV_8UC3, Scalar(0));
  	track = Mat(FHEIGHT, FWIDTH, CV_32S);
  	
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
    Size warpSize(FWIDTH, FHEIGHT);
    	
    for (int i=0; i < 4; i++){
      finalCornerQuad[i].x =  (finalCornerQuad[i].x - minx)/( (float) wwidth ) * FWIDTH;
      finalCornerQuad[i].y = (finalCornerQuad[i].y - miny)/( (float) wheight ) * FHEIGHT;
    }
    	
    H2 = findHomography(cornerQuad, finalCornerQuad, LMEDS);
    	
    warpPerspective(src_gray, transform, H2, warpSize);
    	
    /*Pegando o centro do círculo da imagem transformada*/
    centerTrans.push_back(Point2f((float)240/( (float) wwidth)*FWIDTH, (float)189.5/((float) wheight)*FHEIGHT));
    
    invH = findHomography(finalCornerQuad, cornerQuad, LMEDS);
    	
    perspectiveTransform(centerTrans, center, invH);
    perspectiveTransform(center, centerTrans, H2);
    centerX = centerTrans[0].x;
    centerY = centerTrans[0].y;  
    
   
    /*Loop*/
    for( ; ; )
    {
    	src >> src_frame;
    	
    	if(src_frame.empty())
            break;
        
        /*Passando frame do vídeo para GrayScale*/
        cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY); 
        
        /*Aplicando transformação de perspectiva*/
        warpPerspective(src_gray, transform, H2, warpSize);
        
        /*Copiando matrix de transformação para out*/
   		out_perspective = Mat::zeros( src_frame.rows, src_frame.cols, CV_8U );
		transform.copyTo(out_perspective);
    
  		/*Inicializando a função Thresholding*/
 		Threshold( 0, 0 );
 		
        waitKey(20); // waits to display frame
    }
    waitKey(0); // key press to close window
}

void findPerspective(Mat& H1, Mat& H2, Mat& H3, Mat& H4);

void Reset( int, void* ) {
	if(reset == 0 || reset == 1) 
		src.set(CAP_PROP_POS_MSEC, 380000);
}

void Threshold( int, void* ) {
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

void tracking(Mat aux, Mat track) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int i;
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
  	  imshow("VideoTool", tela);
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
  	
  		//cvtColor(track, track, CV_GRAY2RGB);
		addWeighted(out_perspective1, 1, p, 1, 0.0, out_perspective1); 
  	  	
  	  	warpPerspective(out_perspective1, out, invH, src_frame.size());
  	  	
   		/*Copiando a imagen corrigida por ROI*/ 
  		out.copyTo(tela(Rect(0, 0, out.cols, out.rows)));
  		out_perspective1.copyTo(tela(Rect(out.cols, 0, out_perspective1.cols, out_perspective1.rows)));
 	 	
		imshow( "VideoTool", tela);
	}
}

Mat paint() {
	Mat p = Mat::zeros(track.rows, track.cols, CV_8UC3);
	int color1 = 255;
	int color2 = 0;
	Vec3b c1 = Vec3b(255,0,0);
	Vec3b c2 = Vec3b(0,255,0);
	for(int i = 0; i < track.rows; i++) {
		for(int j = 0; j < track.cols; j++) {
			if(track.at<int>(i, j) != BLACK) {
				/*
				color1 = color1 - 255*track.at<int>(i, j)/ ((float) cont);
				color2 = color2 + (track.at<int>(i, j)/ ((float) cont) )*255;
				p.at<Vec3b>(i, j).val[0] = color1;
				p.at<Vec3b>(i, j).val[1] = color2;
				p.at<Vec3b>(i, j).val[2] = 0;
				*/
				float r = track.at<int>(i,j)/( (float) cont );
				Vec3b c = (r*c1 + (1-r)*c2);
				p.at<Vec3b>(i, j) = c;
			}
		}
	} 
	return p;
}
