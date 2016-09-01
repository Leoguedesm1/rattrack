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
int threshold_value = 128; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar
int const max_BINARY_value = 255;

/*Coordenadas da imagem*/
/*Coordenadas: início +- (402, 206) - (966, 604)*/
int left=350, top=200, width=390, height=360;

Mat src, out, aux, tela, teste; //Imagens de entrada e saída
int colsTela = 0, rowsTela = 0;
char* window_name = "VideoTool"; 
char* trackbar_value = "Value";

/*Assinatura das funções*/
void Threshold( int, void* );

int main( int argc, char** argv ) {

  /*Carregar uma imagem*/
  src = imread( argv[1], 1 );

  /*Criar uma Janela*/
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /*Criando a imagem de saida*/
  out = Mat(src.rows, src.cols, src.type());

  /*Atribuindo os valores de para matriz out*/
  for(int i = 0; i < src.rows; i++) {
    for(int j = 0; j < src.cols; j++) {
      if((i >= top && i <= top+width-1) && (j >= left && j <= left+height-1))
	out.at<int>(i, j) = src.at<int>(i, j);
      else
        out.at<int>(i, j) = WHITE;
    }
  }

  /*Mudando o tipo da imagem src e out*/
  cvtColor(src, src, CV_RGB2GRAY);
  cvtColor(out, out, CV_RGB2GRAY);

  /*Criando imagem da tela*/
  tela = Mat(src.rows, src.cols+src.cols, src.type());
  colsTela = tela.cols;
  rowsTela = tela.rows;

  /*Copiando imagem out para imagem aux*/
  out.copyTo(aux);
  
  /*Copiando as imagens para a saída da tela*/
  src.copyTo(tela(Rect( 0, 0, src.cols, src.rows)));
  out.copyTo(tela(Rect(src.cols, 0, aux.cols, aux.rows)));
  /*Alterando para o tamanho da tela*/
  Size size(1200, 550);
  resize(tela, tela, size);

  /*Inicializando a função Thresholding*/
  Threshold( 0, 0 );

  /*Criando a Trackbar para alterar o valor do Thresholding*/
  createTrackbar( trackbar_value,
                  window_name, &threshold_value,
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

  /*Threshold*/
  for(int i = top; i < top+width; i++) {
    for(int j = left; j < left+height; j++) {
      if (out.at<int>(i, j) > threshold_value) 
        aux.at<int>(i, j) = BLACK;
      else
      	aux.at<int>(i, j) = WHITE;
    }
  }

  /*Voltando ao tamanho normal para fazer o Thresholding*/
  Size tSize(colsTela, rowsTela);
  resize(tela, tela, tSize);
  /*Copiando as imagens*/
  src.copyTo(tela(Rect( 0, 0, src.cols, src.rows)));
  aux.copyTo(tela(Rect(src.cols, 0, aux.cols, aux.rows)));
  /*Voltando ao tamanho da tela*/
  Size size(1200, 550);
  resize(tela, tela, size);

  imshow( window_name, tela);

}

