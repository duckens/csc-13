#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include <cv.h>
#include <highgui.h>

using namespace cv;

std::string intToString(int x){

	char str[20];
	sprintf(str, "%d", x);

	return std::string(str);

}

Mat linearGradTransform(Mat image, double a, double b){


	int cols = image.cols;
	int rows = image.rows;

	Mat transformed_image(rows, cols, image.type());

	cols *= 3;

	for(int i = 0; i < rows; i++){
		const uchar* p = image.ptr<uchar>(i);
		uchar* new_p = transformed_image.ptr<uchar>(i);
		for( int j = 0; j < cols; j++) {
			new_p[j] = cv::saturate_cast<uchar>( a * ( p[j] ) + b );
		}
	}
	return transformed_image;
}

void showImage(Mat image, std::string name){
	namedWindow( name.c_str(), CV_WINDOW_AUTOSIZE );
	imshow( name.c_str(), image );
}

void saveImage(Mat image, std::string path){
	imwrite( path.c_str(), image );
}

std::pair< std::string, std::string > parseFilename(const char* arg){
	std::string imageFileFullName = std::string(arg);
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
	return std::pair< std::string, std::string >(imageFileName, imageFileExtension);
}

Mat readImage(std::string path){
	return imread(path.c_str(), CV_LOAD_IMAGE_COLOR);
}

Mat concatImages(std::vector<Mat> images){

	int size = images.size();
	if(size == 0){
		return Mat();
	}

	int cols = 0;
	int rows = 0;
	for(int i = 0; i < size; i++){
		cols += images[i].cols;
		rows = std::max(rows, images[i].rows);
	}

	Mat result_image(rows, cols, images[0].type());

	int currentPosition_cols = 0;
	for(int i = 0; i < size; i++){
		cv::Rect regionOfInterest( cv::Point( currentPosition_cols, 0 ), cv::Size( images[i].cols, images[i].rows ));
		images[i].copyTo( result_image( regionOfInterest ) );
		currentPosition_cols += images[i].cols;
	}
	
	return result_image;
}

bool addSaltPepper(Mat input_image, Mat& output_image, double p, double q){
	
	srand(time(0));

	if(input_image.type() != CV_8UC1 || p < 0.0 || q < 0.0 || p + q >= 1) return false;

	int rows = input_image.rows;
	int cols = input_image.cols;
	output_image = Mat(rows, cols, input_image.type());
	// input_image.copyTo(output_image);

	const int largeNumber = 1000000;
	const int borderSalt = largeNumber * p;
	const int borderPepper = borderSalt + largeNumber * q;

	for(int i = 0; i < rows; i++){
		const uchar* p_in = input_image.ptr<uchar>(i);
		uchar* p_out = output_image.ptr<uchar>(i);
		for( int j = 0; j < cols; j++) {
			int randomNumber = rand() % largeNumber;
			if(randomNumber < borderSalt){
				p_out[j] = 0;
			} else if(randomNumber < borderPepper){
				p_out[j] = 255;
			} else {
				p_out[j] = cv::saturate_cast<uchar>( p_in[j] );
			}
		}
	}

	return true;
}

bool addGaussianNoise(Mat input_image, Mat& output_image, double mean, double stddev){
	
	srand(time(0));

	if(input_image.type() != CV_8UC1) return false;

	int rows = input_image.rows;
	int cols = input_image.cols;
	output_image = Mat(rows, cols, input_image.type());
	Mat noise_image = Mat(rows, cols, input_image.type());
	input_image.copyTo(output_image);
	randn(noise_image, mean, stddev);

	output_image += noise_image;
	
	return true;
}

int main(int argc, char* argv[]){

	if( argc != 2){
		std::cout << "Usage: " << argv[0] << " image_file_name" << std::endl;
		return -1;
	}
	
	std::pair< std::string, std::string > namesPair = parseFilename(argv[1]);
	std::string imageFileName = namesPair.first;
	std::string imageFileExtension = namesPair.second;
	std::string imageFileFullName = imageFileName + imageFileExtension;


	Mat image = readImage(imageFileFullName);
	showImage(image, imageFileFullName);



	
	std::vector< std::pair< double, double > > gradations;
	gradations.push_back(std::pair< double, double >( 0.5, 0.0 ));
	gradations.push_back(std::pair< double, double >( 1.0, -125.0 ));
	gradations.push_back(std::pair< double, double >( 2.0, 0.0 ));
	gradations.push_back(std::pair< double, double >( 2.0, -255.0 ));
	gradations.push_back(std::pair< double, double >( 1.0, 125.0 ));

	std::vector<Mat> images_linearGradTransform(gradations.size());

	for(int i = 0; i < gradations.size(); i++){
		images_linearGradTransform[i] = linearGradTransform(image, gradations[i].first, gradations[i].second);
	}
	showImage(concatImages(images_linearGradTransform), imageFileName + "_linearGradTransform");




	std::vector<Mat> images_normalizeEqualizeHist(4);
	cvtColor(image, images_normalizeEqualizeHist[0], CV_BGR2GRAY);
	// showImage(images_normalizeEqualizeHist[2], "norm");
	equalizeHist(images_normalizeEqualizeHist[0], images_normalizeEqualizeHist[1]);
	normalize(images_normalizeEqualizeHist[0], images_normalizeEqualizeHist[2]);
	images_normalizeEqualizeHist[3] = images_normalizeEqualizeHist[1] - images_normalizeEqualizeHist[2];
	showImage(concatImages(images_normalizeEqualizeHist), imageFileName + "_normalizeEqualizeHist");



	std::vector< std::pair< double, double > > saltPepperProbabilities;
	saltPepperProbabilities.push_back(std::pair< double, double >( 0.05, 0.05 ));
	saltPepperProbabilities.push_back(std::pair< double, double >( 0.1, 0.1 ));
	saltPepperProbabilities.push_back(std::pair< double, double >( 0.15, 0.15 ));
	std::vector<Mat> images_saltPepper(saltPepperProbabilities.size() + 1);
	bool allOk = true;
	cvtColor(image, images_saltPepper[0], CV_BGR2GRAY);
	for(int i = 0; i < saltPepperProbabilities.size(); i++){
		allOk &= addSaltPepper(images_saltPepper[0], images_saltPepper[i + 1], saltPepperProbabilities[i].first, saltPepperProbabilities[i].second);
	}
	std::cout << "salt Pepper Ok = " << allOk << std::endl;
	showImage(concatImages(images_saltPepper), imageFileName + "_saltPepper");



	std::vector< std::pair< double, double > > gaussianNoiseParams;
	gaussianNoiseParams.push_back(std::pair< double, double >( 0.0, 30.0 ));
	gaussianNoiseParams.push_back(std::pair< double, double >( 0.0, 60.0 ));
	gaussianNoiseParams.push_back(std::pair< double, double >( 50.0, 30.0 ));
	std::vector<Mat> images_gaussianNoise(gaussianNoiseParams.size() + 1);
	allOk = true;
	cvtColor(image, images_gaussianNoise[0], CV_BGR2GRAY);
	for(int i = 0; i < gaussianNoiseParams.size(); i++){
		allOk &= addGaussianNoise(images_gaussianNoise[0], images_gaussianNoise[i + 1], gaussianNoiseParams[i].first, gaussianNoiseParams[i].second);
	}
	std::cout << "Gaussian Noise Ok=" << allOk << std::endl;
	showImage(concatImages(images_gaussianNoise), imageFileName + "_gaussianNoise");

	


	waitKey(0);
	return 0;
}