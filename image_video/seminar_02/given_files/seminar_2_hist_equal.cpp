#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

int main( int argc, char** argv )
{
  cv::Mat src, src_gray, dst_gray;

  // Load image
  src = cv::imread( argv[1], 1 );

  // Convert to grayscale
  cv::cvtColor( src, src_gray, CV_BGR2GRAY );

  // Apply Histogram Equalization to grayscale
  cv::equalizeHist( src_gray, dst_gray );


  // Display results
  cv::namedWindow( "Source image", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Source grayscale", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Equalized grayscale", CV_WINDOW_AUTOSIZE ); 

  cv::imshow( "Source image", src );
  cv::imshow( "Source grayscale", src_gray );
  cv::imshow( "Equalized grayscale", dst_gray );

  // Wait until user exits the program
  cv::waitKey(0);

  return 0;
}