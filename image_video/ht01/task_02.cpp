#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include <cv.h>
#include <highgui.h>

#define IMAGE_MAXINUM_VALUE 255

using namespace cv;

// http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html?highlight=cvtcolor#cvtcolor
// http://www.easyrgb.com/index.php?X=MATH


std::string intToStr(int x){
	char str[20];
	sprintf(str, "%d", x);
	return std::string(str);
}

void evaluateCIELabManually(uchar X, uchar Y, uchar Z, uchar& CIE_L, uchar& CIE_a, uchar& CIE_b){
	double ref_X =  95.047;
	double ref_Y = 100.000;
	double ref_Z = 108.883;

	double var_X = X / ref_X;
	double var_Y = Y / ref_Y;
	double var_Z = Z / ref_Z;

	if( var_X > 0.008856 ){
		var_X = pow( var_X, (1.0/3.0) );
	} else {
		var_X = ( 7.787 * var_X ) + ( 16.0 / 116.0 );
	}

	if( var_Y > 0.008856 ){
		var_Y = pow( var_Y, (1.0/3.0) );
	} else {
		var_Y = ( 7.787 * var_Y ) + ( 16.0 / 116.0 );
	}

	if( var_Z > 0.008856 ){
		var_Z = pow( var_Z, (1.0/3.0) );
	} else {
		var_Z = ( 7.787 * var_Z ) + ( 16.0 / 116.0 );
	}


	CIE_L = ( 116 * var_Y ) - 16;
	CIE_a = 500 * ( var_X - var_Y );
	CIE_b = 200 * ( var_Y - var_Z );
}

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
	int rows = image.rows;
	int cols = image.cols;





	std::vector<Mat> images_swapped;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(i == j) continue;
			int from_to[6] = { 0,0, 1,1, 2,2 };
			int restDigit = 3 - i - j;
			from_to[1] = i;
			from_to[3] = j;
			from_to[5] = restDigit;
			images_swapped.push_back( Mat( rows, cols, image.type() ) );
			mixChannels(&image, 1, &(images_swapped[images_swapped.size() - 1]), 1, from_to, 3);

			std::string thisImageFileName = imageFileName + "_" + intToStr(i) + intToStr(j) + intToStr(restDigit) + imageFileExtension;

			namedWindow( thisImageFileName.c_str(), CV_WINDOW_AUTOSIZE );
			imshow( thisImageFileName.c_str(), images_swapped[images_swapped.size() - 1] );
			imwrite( thisImageFileName.c_str(), images_swapped[images_swapped.size() - 1] );
		}
	}
	



	Mat image_HSV;
	cvtColor(image, image_HSV, CV_BGR2HSV);
	std::vector<Mat> image_HSV_channels(3);
	split(image_HSV, image_HSV_channels);
	image_HSV_channels[2] = IMAGE_MAXINUM_VALUE - image_HSV_channels[2];
	Mat* images_from = &(image_HSV_channels[0]);
	int from_to[] = { 0,0, 1,1, 2,2 };
	mixChannels( images_from, 3, &image_HSV, 1, from_to, 3 );
	cvtColor(image_HSV, image_HSV, CV_HSV2BGR);
	std::string imageHSVInvertedFileName = imageFileName + "_HSV_inverted" + imageFileExtension;
	namedWindow( imageHSVInvertedFileName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageHSVInvertedFileName.c_str(), image_HSV );
	imwrite( imageHSVInvertedFileName.c_str(), image_HSV );





	Mat image_CIELab = Mat(rows, cols, image.type());
	cvtColor(image, image_CIELab, CV_BGR2Lab);
	std::string imageCIELabCvtColorFileName = imageFileName + "_CIE_Lab" + imageFileExtension;
	namedWindow( imageCIELabCvtColorFileName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageCIELabCvtColorFileName.c_str(), image_CIELab );
	imwrite( imageCIELabCvtColorFileName.c_str(), image_CIELab );






	Mat image_CIELab_manually = Mat(rows, cols, image.type());
	int cols_3 = cols * 3;
	for(int i = 0; i < rows; i++){
		const uchar* p = image.ptr<uchar>(i);
		uchar* new_p = image_CIELab_manually.ptr<uchar>(i);

		for(int j = 0; j < cols_3; j += 3){
			evaluateCIELabManually(p[j], p[j + 1], p[j + 2], new_p[j], new_p[j + 1], new_p[j + 2]);
		}
	}
	std::string imageCIELabManuallyFileName = imageFileName + "_CIE_Lab_manually" + imageFileExtension;
	namedWindow( imageCIELabManuallyFileName.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( imageCIELabManuallyFileName.c_str(), image_CIELab_manually );
	imwrite( imageCIELabManuallyFileName.c_str(), image_CIELab_manually );







	waitKey(0);
    return 0;
}