#include <stdio.h>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <limits>
#include <QFileDialog>

const double INF = std::numeric_limits<double>::infinity();
const double raio = 112;

using namespace cv;
using namespace std;

/*Definindo valores de pixels BRANCO e PRETO*/
#define WHITE 255
#define BLACK 0 

/*Estruturas Trackbar*/
/*Reset*/
typedef struct {
	VideoCapture* src;
}userdatareset;

/*Variáveis Globais*/
// Imagen 704x480
Mat src_frame, out, out_perspective, tela, track, invH;
int threshold_value = 100; //Valor do Threshold
int const max_value = 255; //Valor máximo da Trackbar
double centerX, centerY;
int cont = 1;
int pause = 0;

/*Area Bar*/
int min_area = 0;
int max_area = 300;
int const max_area_value = 500;

/*Track Bar*/
int rtrack = 1;
int const max_rtrack_value = 5;

/*Assinatura das funções*/
void acha_perspectiva(VideoCapture src, Mat& transform, Mat& H2, Size warpSize, int FHEIGHT, int FWIDTH);
void aplica_perspectiva(Mat& transform, Mat& H2, Size warpSize);
void processa_video(VideoCapture src, Mat& transform, Mat& H2, Size warpSize);
void Threshold( int, void* );
void tracking(Mat aux, Mat track);
void Reset( int, void* udr );
Mat paint();
void le_video(VideoCapture& src, char** argv);
void cria_janela(VideoCapture src, userdatareset& u);
void Pause(int, void*);
void Sair(int, void*);
void Save(int, void*);

int main(int argc, char** argv) {
    VideoCapture src; //Video de entrada
 	
    /*Le o video de entrada*/
    le_video(src, argv);
                     
    Mat H2, transform = Mat::zeros( src_frame.rows, src_frame.cols, CV_8U ); //H2 = Homography, transform = perspectiva
    const int FHEIGHT = src.get(CV_CAP_PROP_FRAME_HEIGHT); //FHEIGHT = Altura do video
    const int FWIDTH = src.get(CV_CAP_PROP_FRAME_WIDTH); //FWIDTH = Largura do video
 	Size warpSize(FWIDTH, FHEIGHT); //warpSize = tamanho do warp 
    
    /* Parametro Void* da função Reset*/
    userdatareset u; 	
    u.src = &src;
    
 	tela = Mat(FHEIGHT, FWIDTH*2, CV_8UC3, Scalar(0));
  	track = Mat(FHEIGHT, FWIDTH, CV_32S);
   
    /*Cria as janelas e os trackbars*/                 
	cria_janela(src, u);  
	
    /*Calcula a Matriz H2*/
    acha_perspectiva(src, transform, H2, warpSize, FHEIGHT, FWIDTH);
    
    /*Aplica a matriz H2 em cada frame do video*/
    processa_video(src, transform, H2, warpSize);
   	
    waitKey(0); // key press to close window
}

void Reset( int , void* udr) {
	userdatareset u = *((userdatareset*)udr); 
		
	cont = 1;
	track = Scalar(0);
		
	u.src->set(CAP_PROP_POS_MSEC, 380000);
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
  		Mat p = paint();
  		addWeighted(out_perspective1, 1, p, 1, 0.0, out_perspective1); 
  	  	warpPerspective(out_perspective1, out, invH, src_frame.size());
  		
  		/*Copiando a imagen corrigida por ROI*/ 
  		out.copyTo(tela(Rect(0, 0, out.cols, out.rows)));
  		out_perspective1.copyTo(tela(Rect(out.cols, 0, out_perspective1.cols, out_perspective1.rows)));
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
	Vec3b c1 = Vec3b(255,0,0);
	Vec3b c2 = Vec3b(0,255,0);
	for(int i = 0; i < track.rows; i++) {
		for(int j = 0; j < track.cols; j++) {
			if(track.at<int>(i, j) != BLACK) {
				float r = track.at<int>(i,j)/( (float) cont );
				Vec3b c = (r*c1 + (1-r)*c2);
				if(rtrack == 0)
					p.at<Vec3b>(i, j) = c;
				else {
					Point a;
					a.x = j;
					a.y = i;
					circle( p, a, rtrack, c, -1);
				}
			}
		}
	} 
	return p;
}

void acha_perspectiva(VideoCapture src, Mat& transform, Mat& H2, Size warpSize, int FHEIGHT, int FWIDTH) {
	
	src >> src_frame;
	
	Mat H, src_gray;
	cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY); //Passando frame do vídeo para GrayScale
	 
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
   
}

void aplica_perspectiva(Mat& transform, Mat& H2, Size warpSize) {
	Mat src_gray;
	/*Passando frame do vídeo para GrayScale*/
    cvtColor(src_frame, src_gray, cv::COLOR_RGB2GRAY); 
        
	/*Aplicando transformação de perspectiva*/
	warpPerspective(src_gray, transform, H2, warpSize);
        
	/*Copiando matrix de transformação para out*/
	out_perspective = Mat::zeros( src_frame.rows, src_frame.cols, CV_8U );
	transform.copyTo(out_perspective);
}

void processa_video(VideoCapture src, Mat& transform, Mat& H2, Size warpSize) {
	/*Loop*/
    for( ; ; ) {
    	 
        if(pause == 1) {
        	waitKey(5);
        	continue;
        } else	
    		src >> src_frame;
    	
    	if(src_frame.empty())
            break;
           
        aplica_perspectiva(transform, H2, warpSize);
            
 		/*Inicializando a função Thresholding*/
 		Threshold( 0, 0 );
 			
        waitKey(20); // waits to display frame
    }
}

void le_video(VideoCapture& src, char** argv) {
	src.open(argv[1]);
	src.set(CAP_PROP_POS_MSEC, 380000);
	
	/*Verificando se o video foi aberto*/
    if( !src.isOpened() )
        throw "Error when reading steam_avi";
}

void cria_janela(VideoCapture src, userdatareset& u) {
	/*Janela*/
    namedWindow( "VideoTool", WINDOW_NORMAL ); 
    resizeWindow( "VideoTool", 1000, 600); 	
	  
    /*Button reseta o video*/
    createButton("Reset", Reset, (void*) (&u), CV_PUSH_BUTTON);
    /*Button pausa o video*/
    createButton("Pause", Pause, NULL, CV_PUSH_BUTTON);
    
    createButton("Save", Save, NULL, CV_PUSH_BUTTON);
  	createButton("Sair", Sair, NULL, CV_PUSH_BUTTON);
  	
 	/*Criando a Trackbar para alterar o valor do Thresholding*/
    createTrackbar( "Threshold", (char*) NULL, &threshold_value, max_value, Threshold );
    /*Trackbars para a area do rato*/
    createTrackbar( "Area Min", (char*) NULL, &min_area, max_area_value, Threshold );    
  	createTrackbar( "Area Max", (char*) NULL, &max_area, max_area_value, Threshold );
  	/*Tracbars raio tracking*/
  	createTrackbar( "Raio Track", (char*) NULL, &rtrack, max_rtrack_value, Threshold );
  	
  	
}

void Pause(int, void*) {
	if(pause == 0)
		pause = 1;
	else
		pause = 0;
}

void Sair(int, void*) {
	exit(1);
}

void Save(int, void*) {
	
}
