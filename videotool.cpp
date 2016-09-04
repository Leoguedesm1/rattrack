#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0 

/*Variáveis Globais*/
int threshold_value = 128; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar

/*Coordenadas da imagem*/
/*Coordenadas: início +- (402, 206) - (966, 604)*/
// Imagen 704x480
int left=220, top=120, width=300, height=240;

Mat out, tela; //Imagens de entrada e saída

char* window_name = "VideoTool"; 
char* trackbar_value = "Value";


/*Assinatura das funções*/
void Threshold( int, void* );

int main( int argc, char** argv ) {

  /*Carregar uma imagem*/
  Mat src = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  /*Criar uma Janela*/
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /*Atribuindo os valores de para matriz out usando ROI*/
  out = src(Rect(left, top, width, height));
  
  /*Criando imagem da tela*/
  tela = Mat(src.rows, src.cols+src.cols, CV_8U, Scalar(0));
 
  /*Copiando a imagen para a saída da tela*/
  src.copyTo(tela(Rect(0, 0, src.cols, src.rows)));

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
  /*Copiando a imagen corrigida por ROI*/
  aux.copyTo(tela(Rect(left+tela.cols/2, top, aux.cols, aux.rows)));
  
  imshow( window_name, tela);

}
