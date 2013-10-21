#include <cstdlib>
#include <iostream>
	
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cv.h>
#include <highgui.h>

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */

using namespace cv;

bool saltPepper(Mat image, double probability_low, double probability_high){
	if( (probability_low + probability_high >= 1.0) || (probability_low < 0.0) || (probability_high < 0.0) ){
		return false;
	}
	int largeEnoughNumber = 1000000000;
	int probability_low_int = probability_low * largeEnoughNumber;
	int probability_high_int = probability_high * largeEnoughNumber;
	int maxNumber = 255;

	int cols = image.cols;
	int rows = image.rows;
	for(int i = 0; i < rows; i++){
		uchar* p = image.ptr<uchar>(i);
		for( int j = 0; j < cols; j++) {
			int randomNumber = rand();
			if(randomNumber < probability_low_int){
				p[j] = 0;
			} else if(randomNumber < probability_low_int + probability_high_int){
				p[j] = maxNumber;
			}
		}
	}
	return true;
}

int main( int argc, char** argv )
{

	if( argc != 2){
		std::cout << "Usage: " << argv[0] << " image_file_name" << std::endl;
		return -1;
	}

	srand(time(NULL));

	// Read image
	cv::Mat image = cv::imread( argv[1] );
	// cv::Mat image_transformed_convertTo = cv::Mat::zeros( image.size(), image.type() );
	// cv::Mat image_transformed_loop = cv::Mat::zeros( image.size(), image.type() );
	Mat image_gray;
	cvtColor(image, image_gray, CV_BGR2GRAY);
	saltPepper(image_gray, 0.1, 0.1);

	cv::namedWindow("Original Image", 1);
	cv::namedWindow("Salt pepper image", 1);
	cv::imshow("Original Image", image);
	cv::imshow("Salt pepper image", image_gray);

	cv::waitKey();
	return 0;
}