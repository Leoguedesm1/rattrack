#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
// Imagen 704x480
int left=220, top=120, width=300, height=240;

int main(int argc, char** argv)
{
    //string filename = "teste 1.avi";
    VideoCapture src("teste 1.avi");
    VideoCapture out("teste 1.avi");
    Mat src_frame, out_frame, tela;

    if( !src.isOpened() )
        throw "Error when reading steam_avi";
           
    if( !out.isOpened() )
        throw "Error when reading steam_avi";

    namedWindow( "TestVideo", CV_WINDOW_AUTOSIZE );
    
    for( ; ; )
    {
        src >> src_frame;
        out >> out_frame;
       
        if(src_frame.empty() || out_frame.empty())
            break;
            
        /*Criando imagem da tela*/
  		tela = Mat(src_frame.rows, src_frame.cols+src_frame.cols, src_frame.type());
  		
  		/*Copiando a imagen para a saída da tela*/
  		src_frame.copyTo(tela(Rect(0, 0, src_frame.cols, src_frame.rows)));
  		out_frame.copyTo(tela(Rect(tela.cols/2, 0, out_frame.cols, out_frame.rows)));
  		
        imshow("TestVideo", tela);
        waitKey(20); // waits to display frame
    }
    waitKey(0); // key press to close window
}
