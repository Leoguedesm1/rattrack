#include <stdio.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0 

/*Variáveis Globais*/
// Imagen 704x480
int left=220, top=120, width=300, height=240;
Mat src_frame, out_frame, tela;
int threshold_value = 128; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar

/*Assinatura das funções*/
void Threshold( int, void* );

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
  		
  	/*Criando imagem da tela*/
  	tela = Mat(src_frame.rows, src_frame.cols+src_frame.cols, CV_8U, Scalar(0));
  	
  	//src_frame= Mat(CV_CAP_PROP_FRAME_WIDTH, CV_CAP_PROP_FRAME_HEIGHT, CV_8U, Scalar(0));
    
    
    /*Loop*/
    for( ; ; )
    {
    	src >> src_frame;
    	
    	if(src_frame.empty())
            break;
        
        Mat src_gray;
        cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY);
    
  		/*Atribuindo os valores de para matriz out usando ROI*/
  		out_frame = src_gray(Rect(left, top, width, height));
    
            
   		
  		/*Copiando a imagen para a saída da tela*/
  		//src_frame.copyTo(tela(Rect(0, 0, src_frame.cols, src_frame.rows)));
  		
  		/*Inicializando a função Thresholding*/
 		Threshold( 0, 0 );
 		
        waitKey(20); // waits to display frame
    }
    waitKey(0); // key press to close window
}

std::vector<KeyPoint> Detection(Mat im){
// Set up the detector with default parameters.
//SimpleBlobDetector detector;

// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 100;
	params.maxThreshold = 150;
	params.thresholdStep = 5;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 0;
	params.maxArea = 50;

	// Filter by Circularity
	params.filterByCircularity = false;
	params.minCircularity = 0.1;

	// Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.87;

	// Filter by Inertia
	params.filterByInertia = false;
params.minInertiaRatio = 0.01;

 Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params); 
 
// Detect blobs.
std::vector<KeyPoint> keypoints;
detector->detect(im, keypoints);
 return keypoints;
}

void Threshold( int, void* ) {
  /*Atribuindo os valores de para matriz aux usando ROI*/
  Mat aux;
  out_frame.clone().copyTo(aux);
  
  /*Threshold*/ 
  for(int i = 0; i < aux.rows; i++) {
    for(int j = 0; j < aux.cols; j++) {
      if (aux.at<uchar>(i, j) < threshold_value) 
        aux.at<uchar>(i, j) = BLACK;
      else
        aux.at<uchar>(i, j) = WHITE;
    }
  }
  
  
  medianBlur(aux, aux, 5);
  /*Copiando a imagen corrigida por ROI*/
  //aux.copyTo(tela(Rect(left+tela.cols/2, top, out_frame.cols, out_frame.rows)));

  std::vector<KeyPoint> keypoints = Detection(aux);
  
  // Draw detected blobs as red circles.
  // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
  Mat im_with_keypoints;
  drawKeypoints( out_frame, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

  
  imshow( "VideoTool", aux);

}
