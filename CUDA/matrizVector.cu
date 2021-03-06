#include <stdio.h>     
#include <stdlib.h>    
#include <time.h>
#include <iostream>

using namespace std;

__global__
void matrixVectorKernel(float* A, float* B, float* C, int n)
{
	int i = threadIdx.x + (blockDim.x * blockIdx.x);
	if(i<n)
	{
		C[i] = 0;
		for(int j=0;j<n;j++)
			 C[i] += A[i*n+j] * B[j];
	}
	
}



void matrixVector(float* A, float* B, float* C, int tam)
{
	int sizeA = (tam*tam) * sizeof(float);
	int size =  tam * sizeof(float);
	float *d_A,*d_B,*d_C;

	cudaMalloc((void**)&d_A,sizeA);
	cudaMalloc((void**)&d_B,size);
	cudaMalloc((void**)&d_C,size);

	cudaMemcpy(d_A,A,sizeA,cudaMemcpyHostToDevice);
	cudaMemcpy(d_B,B,size,cudaMemcpyHostToDevice);

	matrixVectorKernel<<<ceil(tam/256.0),256>>>(d_A,d_B,d_C,tam);
	cudaMemcpy(C,d_C,size,cudaMemcpyDeviceToHost);

	cudaFree(d_A);cudaFree(d_B);cudaFree(d_C);
	
}



void datosRandom(float *array,int n){
	//srand(time(NULL));
	for(int i = 0; i < n; i++){
	    for(int j = 0; j < n; j++)
	    	array[i*n+j] = 1;
	}
}

void printMatrix(float *array,int n){
  for(int i = 0; i < n; i++){
    printf("%f ", array[i]);
  }
  printf("\n");
}
int main()
{
	int n = 10;
	float *h_A,*h_B,*h_C;
	h_A = new float[n*n];
	h_B = new float[n*n];
	h_C = new float[n*n];

	datosRandom(h_A,n);
	datosRandom(h_B,n);
    matrixVector(h_A,h_B,h_C,n);
    printMatrix(h_C,n);

   	return 0;
}