#include <stdio.h>     
#include <stdlib.h>    

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/imgproc/imgproc.hpp"

#define CHANNELS 3
void blurKernel(unsigned char * in, unsigned char * out, int w, int h) {
      int Col  = blockIdx.x * blockDim.x + threadIdx.x;
      int Row  = blockIdx.y * blockDim.y + threadIdx.y;

      if (Col < w && Row < h) {
          int pixVal = 0;
          int pixels = 0;

          for(int blurRow = -BLUR_SIZE; blurRow < BLUR_SIZE+1; ++blurRow) {
              for(int blurCol = -BLUR_SIZE; blurCol < BLUR_SIZE+1; ++blurCol) {

                  int curRow = Row + blurRow;
                  int curCol = Col + blurCol;

                  if(curRow > -1 && curRow < h && curCol > -1 && curCol < w) {
                      pixVal += in[curRow * w + curCol];
                      pixels++; 
                  }
              }
          }
          out[Row * w + Col] = (unsigned char)(pixVal / pixels);
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