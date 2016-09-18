#include <stdio.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0 

/*Variáveis Globais*/
// Imagen 704x480
int left1=220, top=120, width=300, height=240;
Mat src_frame, out, tela;
int threshold_value = 110; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar

int min_area = 100;
int max_area = 400;
int const max_area_value = 1000;

/*Assinatura das funções*/
void Threshold( int, void* );
void tracking(Mat aux);

int main(int argc, char** argv) {
	/*Capturando video*/
    VideoCapture src(argv[1]);
	src.set(CAP_PROP_POS_MSEC, 420000);
	
	/*Verificando se o video foi aberto*/
    if( !src.isOpened() )
        throw "Error when reading steam_avi";
                 
	/*Janela*/
    namedWindow( "VideoTool", CV_WINDOW_AUTOSIZE );  	
    	
 	/*Criando a Trackbar para alterar o valor do Thresholding*/
    createTrackbar( "Value", "VideoTool", &threshold_value, max_value, Threshold );
    
    /*Trackbars para a area do rato*/
    createTrackbar( "Min", "VideoTool", &min_area, max_area_value, Threshold );
  	createTrackbar( "Max", "VideoTool", &max_area, max_area_value, Threshold );
    
  	tela = Mat(src.get(CV_CAP_PROP_FRAME_HEIGHT), src.get(CV_CAP_PROP_FRAME_WIDTH)*2, CV_8U, Scalar(0));
    
    /*Loop*/
    for( ; ; )
    {
    	src >> src_frame;
    	
    	if(src_frame.empty())
            break;
        
        Mat src_gray;
        cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY);
    
  		/*Atribuindo os valores de para matriz out usando ROI*/
  		out = src_gray(Rect(left1, top, width, height));
   		
  		/*Copiando a imagen para a saída da tela*/
  		src_gray.copyTo(tela(Rect(0, 0, src_frame.cols, src_frame.rows)));
  		
  		/*Inicializando a função Thresholding*/
 		Threshold( 0, 0 );
 		
        waitKey(20); // waits to display frame
    }
    waitKey(0); // key press to close window
}

void Threshold( int, void* ) {
  Mat aux(out.rows, out.cols, CV_8U, Scalar(0));

  /*Threshold*/
  for(int i = 0; i < out.rows; i++) {
    for(int j = 0; j < out.cols; j++) {
      if (out.at<uchar>(i, j) < threshold_value) 
        aux.at<uchar>(i, j) = BLACK;
      else
        aux.at<uchar>(i, j) = WHITE;
    }
  }
  
  medianBlur(aux, aux, 5);
  
  tracking(aux);

}

void tracking(Mat aux) {
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
  		  //cout << "rato de area: " << area << " indice: " << i << "\n";
  		  break; 
  		}
  	}
  	
  	if (i >= contours.size()){
  	  //cout << "rato não encontrado\n";
  	  imshow("VideoTool", tela);
  	  return;
  	}
  		
  	/// Get the moments
  	vector<Moments> mu( contours.size() );
  	mu[i] = moments( contours[i], false ); 

	///  Get the mass centers:
 	vector<Point2f> mc( contours.size() );
  	mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
  	
  	//Circulando contornos e destacando centro de massa  
  	Scalar color = Scalar(255,0,0);
    drawContours( aux, contours, i, color, 2, 8, hierarchy, 0, Point() );
    circle( aux, mc[i], 4, color, -1, 8, 0 );
  	
	/*Copiando a imagen corrigida por ROI*/
     aux.copyTo(tela(Rect(left1+tela.cols/2, top, aux.cols, aux.rows)));
  
	imshow( "VideoTool", tela);
}
