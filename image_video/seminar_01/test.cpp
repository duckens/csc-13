//
// for testing
//
// robocraft.ru
//

#include <cv.h>
#include <highgui.h>

#include <cstdlib>
#include <cstdio>

int main(int argc, char* argv[]){

	IplImage* image = NULL;
	IplImage* dst = NULL;

	// имя картинки задаётся первым параметром
	char filename[] = "board.jpg";

	// получаем картинку
	image = cvLoadImage(filename, 1);

	printf("[i] image: %s\n", filename);
	
	assert( image != 0 );

	// покажем изображение
	cvNamedWindow( "image" );
	cvShowImage( "image", image );

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage( &image );
	cvReleaseImage( &dst );

	// удаляем окна
	cvDestroyAllWindows();

    return 0;
}