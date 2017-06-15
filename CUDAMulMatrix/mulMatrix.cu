#include <stdio.h>
#include <stdlib.h>

#define BLOCK_WIDTH 16

__global__
void MatrixMultKernel(float *d_M, float *d_N, float *d_P, int Width){
  int Row = blockIdx.y*blockDim.y + threadIdx.y;
  int Col = blockIdx.x*blockDim.x + threadIdx.x;

  if(Row < Width && Col < Width){
      float Pvalue = 0;
      for(int k = 0; k < Width; ++k){
          Pvalue += d_M[Row*Width + k] * d_N[k*Width+Col];
      }
      d_P[Row*Width+Col] = Pvalue;
  }
}

void MatrixMult(float* A, float* B, float* C, int n){
  int size = n*n*sizeof(float);
  float *d_A, *d_B, *d_C;

  cudaMalloc((void **) &d_A, size);
  cudaMemcpy(d_A,A,size,cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_B, size);
  cudaMemcpy(d_B,B,size,cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_C, size);

  int NumBlocks = n/BLOCK_WIDTH;
  if (n % BLOCK_WIDTH) NumBlocks++;
  
  dim3 dimGrid(NumBlocks,NumBlocks);
  dim3 dimBlock(BLOCK_WIDTH,BLOCK_WIDTH);

  MatrixMultKernel<<<dimGrid, dimBlock>>>(d_A,d_B,d_C,n);
  
  cudaMemcpy(C,d_C,size,cudaMemcpyDeviceToHost);

  cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}

void datosRandom(float *array,int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++)
      array[i*n+j] = rand() % 100;
  }
}

void printMatrix(float *array,int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
        printf("%f ", array[i*n+j]);
    }
    printf("\n"); 
  }  
}

int main(){
  int n=10;
  float *h_A,*h_B,*h_C;

  h_A = (float*) malloc(n*n*sizeof(float));
  h_B = (float*) malloc(n*n*sizeof(float));
  h_C = (float*) malloc(n*n*sizeof(float));
  
  datosRandom(h_A,n);
  datosRandom(h_B,n);

  MatrixMult(h_A,h_B,h_C,n);
  printMatrix(h_C,n);
  
  return 0;
}