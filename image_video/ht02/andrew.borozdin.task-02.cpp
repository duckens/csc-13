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

Mat concatImages(std::vector<Mat> images, int limitInRow = 0){

	int size = images.size();
	if(size == 0){
		return Mat();
	}
	if( (limitInRow == 0) || (limitInRow > size) ){
		limitInRow = size;
	}

	int big_cols = std::min(size, limitInRow);
	int big_rows = 1 + ( size + limitInRow - 1 ) % limitInRow;

	int cols_thisRowOfImages = 0;
	int rows_thisRowOfImages = 0;
	int cols = 0;
	int rows = 0;


	for(int i = 0; i < size; i++){

		cols_thisRowOfImages += images[i].cols;
		rows_thisRowOfImages = std::max(rows_thisRowOfImages, images[i].rows);

		if( ( (i + 1) % limitInRow == 0 ) || (i == size - 1) ){
			cols = std::max(cols, cols_thisRowOfImages);
			cols_thisRowOfImages = 0;
			rows += rows_thisRowOfImages;
		}
	}
	

	Mat result_image(rows, cols, images[0].type());

	int currentPosition_cols = 0;
	int currentPosition_rows = 0;
	for(int i = 0; i < big_rows; i++){

		int nextShiftRows = 0;

		for(int j = 0; (j < big_cols) && (j + i * big_cols < size); j++){
			cv::Rect regionOfInterest( cv::Point( currentPosition_cols, currentPosition_rows ), cv::Size( images[i].cols, images[i].rows ));
			images[j + i * big_cols].copyTo( result_image( regionOfInterest ) );
			currentPosition_cols += images[i].cols;
			nextShiftRows = std::max(nextShiftRows, images[i].rows);
		}

		currentPosition_rows += nextShiftRows;
		currentPosition_cols = 0;
	}
	
	return result_image;
}

bool addSaltPepper(Mat input_image, Mat& output_image, double p, double q){
	
	srand(time(0));

	if(input_image.type() != CV_8UC1 || p < 0.0 || q < 0.0 || p + q >= 1) return false;

	int rows = input_image.rows;
	int cols = input_image.cols;
	output_image = Mat(rows, cols, input_image.type());

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



	// task 1.1
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



	// task 1.2
	std::vector<Mat> images_normalizeEqualizeHist(4);
	cvtColor(image, images_normalizeEqualizeHist[0], CV_BGR2GRAY);
	equalizeHist(images_normalizeEqualizeHist[0], images_normalizeEqualizeHist[1]);
	normalize(images_normalizeEqualizeHist[0], images_normalizeEqualizeHist[2]);
	images_normalizeEqualizeHist[3] = images_normalizeEqualizeHist[1] - images_normalizeEqualizeHist[2];
	showImage(concatImages(images_normalizeEqualizeHist), imageFileName + "_normalizeEqualizeHist");


	// task 1.3
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



	// task 1.4
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





	// task 1.5
	std::vector<int> sizes(3);
	sizes[0] = 3;
	sizes[1] = 5;
	sizes[2] = 7;
	for(int i = 1; i <= 3; i++){
		std::vector<Mat> images_blur;
		Mat this_image = images_saltPepper[i];
		for(int j = 0; j < 3; j++){// task 1.5

			Mat blurred_image;
			blurred_image = Mat(this_image.rows, this_image.cols, this_image.type());
			int sz = sizes[j];
			blur(this_image, blurred_image, cv::Size(sz, sz));
			images_blur.push_back(blurred_image);
			
		}
		for(int j = 0; j < 3; j++){

			Mat blurred_image;
			blurred_image = Mat(this_image.rows, this_image.cols, this_image.type());
			int sz = sizes[j];
			GaussianBlur(this_image, blurred_image, cv::Size(sz, sz), 10);
			images_blur.push_back(blurred_image);
			
		}
		for(int j = 0; j < 3; j++){

			Mat blurred_image;
			blurred_image = Mat(this_image.rows, this_image.cols, this_image.type());
			int sz = sizes[j];
			medianBlur(this_image, blurred_image, sz);
			images_blur.push_back(blurred_image);
			
		}
		showImage(concatImages(images_blur, 3), imageFileName + "_" + intToString(i) + "_blur");
	}
	


	// Дальше, честно говоря просто не успел. С ДПФ я знаком по работе, но рзобраться с ним для изображений было бы определенно интересно.
	// Опять же, как я уже говорил - отчетность мне не важна в данном случае.
	// Однако надеюсь что доделаю ДПФ) - это просто интересно)


	
	Mat image_gray;
	cvtColor(image, image_gray, CV_BGR2GRAY);
	Mat padded;
	int m = getOptimalDFTSize( image_gray.rows );
	int n = getOptimalDFTSize( image_gray.cols ); // on the border add zero values
	copyMakeBorder(image_gray, padded, 0, m - image_gray.rows, 0, n - image_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat image_gray_complex;
	Mat image_gray_complex_afterDFT;
	merge(planes, 2, image_gray_complex);         // Add to the expanded another plane with zeros

	dft(image_gray_complex, image_gray_complex_afterDFT);

	Mat image_filter = Mat::zeros(image_gray_complex_afterDFT.size(), CV_32F);
	

	// std::vector<Mat> image_dft(10);
	// dft(image_dft[0], image_dft[1]);
	// showImage(concatImages(image_dft), imageFileName + "_dft");



	waitKey(0);
	return 0;
}