#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "x86intrin.h"
#include <sys/time.h>
#include <math.h>
#include "util.cpp"

#define ALPHA 0.5

using namespace cv;
using namespace std;

unsigned char* overlay_serial(unsigned char *image1, unsigned char *image2, unsigned int NCOLS1, unsigned int NROWS1, unsigned int NCOLS2, unsigned int NROWS2, unsigned char *out_image){
	for (int row = 0; row < NROWS2; row++)
		for (int col = 0; col < NCOLS2; col++)
			*(out_image + row * NCOLS1 + col) = min(*(image1 + row * NCOLS1 + col) + int(*(image2 + row * NCOLS2 + col) * ALPHA), 255);
	return out_image;
}

__m128i* overlay_parallel(__m128i *pSrc1, __m128i *pSrc2, unsigned int NCOLS1, unsigned int NROWS1, unsigned int NCOLS2, unsigned int NROWS2, __m128i *pRes){
	__m128i a, b, m, amb, bma;
	__m128i shift_mask = _mm_set1_epi8(127); // 01111111
	__m128i black = _mm_set1_epi8(0);
	__m128i white = _mm_set1_epi8(255);
	for (int i = 0; i < NROWS2; i++)
		for (int j = 0; j < NCOLS2 / 16; j++)
		{
			a = _mm_loadu_si128(pSrc1 + i * NCOLS1/16 + j);
			b = _mm_loadu_si128(pSrc2 + i * NCOLS2/16 + j);
			b = b >> 1;  // *0.5
			b = _mm_and_si128(b, shift_mask);
			m = _mm_adds_epu8(a, b);  // Saturation a*0.5+b
			_mm_storeu_si128(pRes + i * NCOLS1/16 + j, m);
		}
	return pRes;
}

int main( )
{
	print_names();

	// LOAD images
	cv::Mat img1 = cv::imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);
	cv::Mat img2 = cv::imread("CA03__Q2__Image__02.png", IMREAD_GRAYSCALE);
	unsigned int NCOLS1 = img1.cols;
	unsigned int NROWS1 = img1.rows;
	unsigned int NCOLS2 = img2.cols;
	unsigned int NROWS2 = img2.rows;

	// Create output images
	cv::Mat out_img1 = cv::imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);
	cv::Mat out_img2 = cv::imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);

	// Pointer to the image data (Matrix of pixels)
	unsigned char *image1;
	unsigned char *image2;
	unsigned char *out_image1;
	unsigned char *out_image2;

	image1  = (unsigned char *) img1.data;
	image2  = (unsigned char *) img2.data;
	out_image1 = (unsigned char *) out_img1.data;
	out_image2 = (unsigned char *) out_img2.data;

	struct timeval start, end;

	// Serial
	gettimeofday(&start, NULL);
	out_image1 = overlay_serial(image1, image2, NCOLS1, NROWS1, NCOLS2, NROWS2, out_image1);
	gettimeofday(&end, NULL);
	long seconds = (end.tv_sec - start.tv_sec);
	long micros1 = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

	// Parallel
	__m128i *pSrc1, *pSrc2;
	__m128i *pRes;

	pSrc1 = (__m128i *) img1.data;
	pSrc2 = (__m128i *) img2.data;
	pRes = (__m128i *) out_img2.data;

	gettimeofday(&start, NULL);
	pRes = overlay_parallel(pSrc1, pSrc2, NCOLS1, NROWS1, NCOLS2, NROWS2, pRes);
	gettimeofday(&end, NULL);
	long seconds2 = (end.tv_sec - start.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end.tv_usec) - (start.tv_usec);

	//DISPLAY images
	cv::namedWindow( "input", cv::WINDOW_AUTOSIZE ); 	// Create a window for display.
	cv::imshow( "input", img1 ); 				// Show our image inside it.
	cv::namedWindow( "Serial Output", cv::WINDOW_AUTOSIZE ); 	// Create a window for display.
	cv::imshow( "Serial Output", out_img1 ); 				// Show our image inside it.
	cv::namedWindow( "Parallel Output", cv::WINDOW_AUTOSIZE ); 	// Create a window for display.
	cv::imshow( "Parallel Output", out_img2 ); 				// Show our image inside it.

	waitKey(0);                       					// Wait for a keystroke in the window

	printf("\nOVERLAY\n");
	printf("Serial Run time = %ld \n", micros1);
	printf("Parallel Run time = %ld \n", micros2);
	printf("Speedup = %f\n\n", (float)(micros1) / (float)micros2);	
    return 0;
}
