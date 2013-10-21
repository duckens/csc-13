#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */

int main( int argc, char** argv )
{

  cv::Mat image = cv::imread( argv[1] );
  cv::Mat transformed_image = cv::Mat::zeros( image.size(), image.type() );
  cv::Mat normalized_image = cv::Mat::zeros( image.size(), image.type() );
  // Read image

  /// Initialize values
  std::cout<<" Basic Linear Transforms "<<std::endl;
  std::cout<<"-------------------------"<<std::endl;
  std::cout<<"* Enter the alpha value: ";std::cin>>alpha;
  std::cout<<"* Enter the beta value: "; std::cin>>beta;

  image.convertTo(transformed_image, -1, alpha, beta);

  // int cols = image.cols, rows = image.rows;
  // if(image.isContinuous()){
  //   cols *= rows;
  //   rows = 1;
  // }
  // cols *= 3;
  // for(int i = 0; i < rows; i++){
  //   const uchar* p = image.ptr<uchar>(i);
  //   uchar* new_p = transformed_image.ptr<uchar>(i);
  //   for( int j = 0; j < cols; j++) {
  //     new_p[j] = cv::saturate_cast<uchar>( alpha*( p[j] ) + beta );
  //   }
  // }
 
  // cv::normalize(image, normalized_image, 0, 255, cv::NORM_MINMAX, CV_8UC1);
  cv::normalize(transformed_image, normalized_image, 0, 255, cv::NORM_MINMAX, CV_8UC1);

  /// Create Windows
  cv::namedWindow("Original Image", 1);
  cv::namedWindow("Transformed Image", 1);
  cv::namedWindow("Normalized Image", 1);

  /// Show stuff
  cv::imshow("Original Image", image);
  cv::imshow("Transformed Image", transformed_image);
  cv::imshow("Normalized Image", normalized_image);

  /// Wait until user press some key
  cv::waitKey();
  return 0;
}