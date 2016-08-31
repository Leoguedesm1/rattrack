#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

using namespace cv;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0 

/*Variáveis Globais*/
int threshold_value = 0; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar
int const max_BINARY_value = 255;

Mat src, out, tela; //Imagens de entrada e saída
char* window_name = "VideoTool"; 
char* trackbar_value = "Value";

/*Assinatura das funções*/
void Threshold( int, void* );

int main( int argc, char** argv ) {

  /*Carregar uma imagem*/
  src = imread( argv[1], 1 );

  /*Criar uma Janela*/
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /*Criando a Trackbar para alterar o valor do Thresholding*/
  createTrackbar( trackbar_value,
                  window_name, &threshold_value,
                  max_value, Threshold );

  /*Inicializando a função Thresholding*/
  Threshold( 0, 0 );

  /*Loop para encerrar o programa*/
  while(true) {
    int c;
    c = waitKey( 20 );
    if( (char)c == 27 )
      { break; }
   }

}

void Threshold( int, void* ) {
  /*Coordenadas da imagem*/
  int left=350, top=200, width=390, height=360;

  /*Criando a imagem de saida*/
  out = Mat(src.rows, src.cols, src.type());
	
  /*Criando imagem da tela*/
  tela = Mat(src.rows, src.cols+out.cols, src.type());

  /*Atribuindo os valores de para matriz out*/
  for(int i = 0; i < src.rows; i++) {

    for(int j = 0; j < src.cols; j++) {
     
      /*Area of interest*/
      /*Coordenadas: início +- (402, 206) - (966, 604)*/
      if((i >= top && i <= top+width-1) && (j >= left && j <= left+height-1))
	out.at<int>(i, j) = src.at<int>(i, j);
      else
       out.at<int>(i, j) = 0;
    }
  }

  /*Copiando as imagens para a saída da tela*/
  src.copyTo(tela(Rect( 0, 0, src.cols, src.rows)));
  out.copyTo(tela(Rect(src.cols, 0, out.cols, out.rows)));
  Size size(1200, 550);
  resize(tela, tela, size);

  /*Threshold
  for(i = Xi; i <= X; i++) {
    for(j = Yi; j <= Y; j++) {
      if (src.at<int>(i, j) > threshold_value) {
        out.at<int>(i, j) = WHITE;
      }else{
      	out.at<int>(i, j) = BLACK;
      }
    }
  }*/
 
  imshow( window_name, tela);
 }

