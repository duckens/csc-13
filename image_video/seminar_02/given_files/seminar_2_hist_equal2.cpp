#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

int main( int argc, char** argv )
{
  cv::Mat src, src_gray, dst, dst_gray, dst_r, dst_g, dst_b;

  // Load image
  src = cv::imread( argv[1], 1 );

  // Convert to grayscale
  cv::cvtColor( src, src_gray, CV_BGR2GRAY );

  // Apply Histogram Equalization to grayscale
  cv::equalizeHist( src_gray, dst_gray );

  // Split channels
  cv::vector<cv::Mat> src_channels, dst_channels;
  cv::split(src, src_channels);

  // Apply Histogram Equalization to channels
  cv::equalizeHist( src_channels[0], dst_b);
  cv::equalizeHist( src_channels[1], dst_g);
  cv::equalizeHist( src_channels[2], dst_r);

  //Merge the result
  dst_channels.push_back(dst_b);
  dst_channels.push_back(dst_g);
  dst_channels.push_back(dst_r);
  cv::merge(dst_channels, dst);
 
  // Display results
  cv::namedWindow( "Source image", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Source grayscale", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Equalized grayscale", CV_WINDOW_AUTOSIZE ); 
  cv::namedWindow( "Equalized image", CV_WINDOW_AUTOSIZE );

  cv::imshow( "Source image", src );
  cv::imshow( "Source grayscale", src_gray );
  cv::imshow( "Equalized grayscale", dst_gray );
  cv::imshow( "Equalized image", dst );

  // Wait until user exits the program
  cv::waitKey(0);

  return 0;
}