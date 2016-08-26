#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image, out;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

  /*Atribuindo os valores de 0 para toda matriz out*/
  for(int i = 0; i < image.rows; i++) {

    for(int j = 0; j < image.cols; j++) {
     
      /*Area of interest*/
      /*Coordenadas: início +- (402, 206) - (966, 604)*/
      if((i >= 206 && i <= 604) && (j >= 402 && j <= 966))
	out.at<int>(i, j) = image.at<int>(i, j);
      else
       out.at<int>(i, j) = 0;
    }
  }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", out );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
