#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0 

/*Variáveis Globais*/
int threshold_value = 137; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar
RNG rng(12345);

/*Coordenadas da imagem*/
/*Coordenadas: início +- (402, 206) - (966, 604)*/
// Imagen 704x480
int left1=220, top=120, width=300, height=240;

Mat out, tela; //Imagens de entrada e saída

/*Assinatura das funções*/
void Threshold( int, void* );
void tracking(Mat aux);

int main( int argc, char** argv ) {

  /*Carregar uma imagem*/
  Mat src = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  /*Criar uma Janela*/
  namedWindow( "VideoTool" , CV_WINDOW_AUTOSIZE );

  /*Atribuindo os valores de para matriz out usando ROI*/
  out = src(Rect(left1, top, width, height));
  
  /*Criando imagem da tela*/
  tela = Mat(src.rows, src.cols+src.cols, CV_8U, Scalar(0));
 
  /*Copiando a imagen para a saída da tela*/
  src.copyTo(tela(Rect(0, 0, src.cols, src.rows)));

  /*Inicializando a função Thresholding*/
  Threshold( 0, 0 );

  /*Criando a Trackbar para alterar o valor do Thresholding*/
  createTrackbar( "Value",
                  "VideoTool", &threshold_value,
                  max_value, Threshold );


  /*Loop para encerrar o programa*/
  while(true) {
    int c;
    c = waitKey( 20 );
    if( (char)c == 27 )
      { break; }
   }

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
	
	findContours( aux, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
  	
  	/// Get the moments
  	vector<Moments> mu(contours.size() );
  	//for( int i = 0; i < contours.size(); i++ )
  		mu[0] = moments( contours[0], false ); 

	///  Get the mass centers:
 	vector<Point2f> mc( contours.size() );
  	//for( int i = 0; i < contours.size(); i++ ) {
  		mc[0] = Point2f( mu[0].m10/mu[0].m00 , mu[0].m01/mu[0].m00 ); 
  	//}
  
 	//for( int i = 0; i< contours.size(); i++ ) {
  		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    	//drawContours( aux, contours, i, color, 2, 8, hierarchy, 0, Point() );
    	circle( aux, mc[0], 4, color, -1, 8, 0 );
  	//}
  	
	/*Copiando a imagen corrigida por ROI*/
    aux.copyTo(tela(Rect(left1+tela.cols/2, top, aux.cols, aux.rows)));
  
	imshow( "VideoTool", tela);
}
