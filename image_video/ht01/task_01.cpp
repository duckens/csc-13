#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include <cv.h>
#include <highgui.h>

using namespace cv;

int main(int argc, char* argv[]){

	if( argc != 2){
		std::cout << "Usage: " << argv[0] << " image_file_name" << std::endl;
		return -1;
	}

	std::string imageFileFullName = std::string(argv[1]);
	std::string imageFileName = std::string("");
	std::string imageFileExtension = std::string("");

	unsigned int lastDotFound = imageFileFullName.rfind(".");
	if( lastDotFound!=std::string::npos ){
		imageFileExtension = imageFileFullName.substr(lastDotFound);
		imageFileName = imageFileFullName.substr(0, lastDotFound);
	} else {
		imageFileName = imageFileFullName;
	}
	if(imageFileExtension == std::string("") || imageFileExtension == std::string(".")){
		imageFileExtension = ".jpg";
	}


	Mat image = imread(imageFileFullName.c_str(), CV_LOAD_IMAGE_COLOR);
	imwrite( (imageFileName + "_copy" + imageFileExtension).c_str(), image );

	namedWindow( imageFileFullName.c_str(), CV_WINDOW_AUTOSIZE );	// Create a window for display.
	imshow( imageFileFullName.c_str(), image );						// Show our image inside it.

	int rows = image.rows;
	int cols = image.cols;

	Mat image_leftHalf = image(cv::Rect(0,0,cols/2,rows));
	imwrite( (imageFileName + "_leftHalf" + imageFileExtension).c_str(), image_leftHalf );



	Mat image_swapped = Mat(rows, cols, image.type());
	std::string imageSwappedName = imageFileName + "_swapped" + imageFileExtension;

	int halfSize_top = rows / 2;
	int halfSize_bottom = rows - halfSize_top;

	Mat image_source_bottom = image( cv::Rect( 0, 0, cols, halfSize_top ) );
	Mat image_source_top = image( cv::Rect(0, halfSize_top, cols, halfSize_bottom) );

	Mat image_target_bottom = image_swapped( cv::Rect( 0, 0, cols, halfSize_bottom ) );
	Mat image_target_top = image_swapped( cv::Rect(0, halfSize_bottom, cols, halfSize_top) );

	image_source_bottom.copyTo(image_target_top);
	image_source_top.copyTo(image_target_bottom);
	imwrite( imageSwappedName.c_str(), image_swapped );

	namedWindow( imageSwappedName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageSwappedName.c_str(), image_swapped );
	
	std::string imageRName = imageFileName + "_R" + imageFileExtension;
	std::string imageGName = imageFileName + "_G" + imageFileExtension;
	std::string imageBName = imageFileName + "_B" + imageFileExtension;
	std::vector<Mat> imageChannels(3);
	split(image, imageChannels);
	Mat image_R = imageChannels[2];
	Mat image_G = imageChannels[1];
	Mat image_B = imageChannels[0];
	namedWindow( imageRName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageRName.c_str(), image_R );
	namedWindow( imageGName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageGName.c_str(), image_G );
	namedWindow( imageBName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageBName.c_str(), image_B );
	imwrite( imageRName.c_str(), image_R );
	imwrite( imageGName.c_str(), image_G );
	imwrite( imageBName.c_str(), image_B );




	// _mean inmage is done in a dumb way, but the only one I could write without for() loops
	Mat image_mean = Mat(image_R);
	image_mean = image_mean / 3;
	image_mean += image_G / 3;
	image_mean += image_B / 3;
	std::string imageMeanName = imageFileName + "_mean" + imageFileExtension;
	namedWindow( imageMeanName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageMeanName.c_str(), image_mean );
	imwrite( imageMeanName.c_str(), image_mean );



	// mean using cvtColor()
	Mat image_afterCvtColor;
	cvtColor(image, image_afterCvtColor, CV_BGR2GRAY);
	Mat image_difference = (image_mean - image_afterCvtColor) * 100;
	std::string imageDifferenceName = imageFileName + "_difference" + imageFileExtension;
	namedWindow( imageDifferenceName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageDifferenceName.c_str(), image_difference );
	imwrite( imageDifferenceName.c_str(), image_difference );

	Mat image_HSV;
	cvtColor(image, image_HSV, CV_BGR2HSV);
	Mat image_maximums = Mat::ones(image_HSV.rows, image_HSV.cols, image_HSV.type()) * 255;

	Mat images_from[] = {image_HSV, image_maximums};
	int from_to[] = { 0,0, 3,1, 3,2 };
	mixChannels( images_from, 2, &image_HSV, 1, from_to, 3 );
	std::string imageHSVName = imageFileName + "_HSV" + imageFileExtension;
	namedWindow( imageHSVName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageHSVName.c_str(), image_HSV );
	imwrite( imageHSVName.c_str(), image_HSV );
	

	Mat image_backFromHSV;
	cvtColor(image_HSV, image_backFromHSV, CV_HSV2BGR);
	std::string imageBackFromHSVName = imageFileName + "_backFromHSV" + imageFileExtension;
	namedWindow( imageBackFromHSVName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageBackFromHSVName.c_str(), image_backFromHSV );
	imwrite( imageBackFromHSVName.c_str(), image_backFromHSV );


	waitKey(0);				// Wait for a keystroke in the window
    return 0;
}