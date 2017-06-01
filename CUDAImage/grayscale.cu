#include <stdio.h>     
#include <stdlib.h>    

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/imgproc/imgproc.hpp"

#define CHANNELS 3
__global__
void colorConvert(unsigned char* grayImage, unsigned char* rgbImage, int width , int height)
{
	int x = threadIdx.x + (blockDim.x * blockIdx.x);
	int y = threadIdx.y + (blockDim.y * blockIdx.y);
	if(x< width && y<height)
	{
		int grayOffset= y*width+x;
		int rgbOffset = grayOffset*CHANNELS;
		unsigned char r = rgbImage[rgbOffset ];
		unsigned char g = rgbImage[rgbOffset+2];
		unsigned char b = rgbImage[rgbOffset+3];
		grayImage[grayOffset] = 0.21f*r + 0.71f*g + 0.07f*b;
	}
	
}


void rgbToGray(unsigned char* grayImage, unsigned char* rgbImage, int width , int height)
{
	int sizeimage = (width*height) * sizeof(unsigned char);

	unsigned char *d_rgbImage,*d_grayImage;

	cudaMalloc((void**)&d_rgbImage,sizeimage);
	//cudaMalloc((void**)&d_grayImage,sizeimage);

	cudaMemcpy(d_rgbImage,rgbImage,sizeimage,cudaMemcpyHostToDevice);

	colorConvert<<<ceil(width*height/256.0),256>>>(d_grayImage,d_rgbImage,width,height);

	cudaMemcpy(grayImage,d_grayImage,sizeimage,cudaMemcpyDeviceToHost);

	cudaFree(d_grayImage);cudaFree(d_rgbImage);
	
}


int main()
{
	char* imageName = argv[1];

	Mat image;
	image = imread( imageName, 1 );

	if( argc != 2 || !image.data )
	{
	  printf( " No image data \n " );
	  return -1;
	}

	Mat gray_image;
	int width = image->width;
    int height = image->height;
	//cvtColor( image, gray_image, CV_BGR2GRAY );
	rgbToGray(gray_image,image,width,height);

	imwrite( "paisajegray.png", gray_image );

 	namedWindow( imageName, CV_WINDOW_AUTOSIZE );
 	namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );

	imshow( imageName, image );
 	imshow( "Gray image", gray_image );

 	waitKey(0);

   	return 0;
}