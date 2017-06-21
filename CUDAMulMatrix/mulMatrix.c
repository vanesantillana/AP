#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#define BLOCK_WIDTH 4
#define TILE_WIDTH 4


__global__ 
void MatrixMulKernel(float* M, float* N, float* P, int Width) {

  int Row = blockIdx.y*blockDim.y+threadIdx.y;
  int Col = blockIdx.x*blockDim.x+threadIdx.x;

  if ((Row < Width) && (Col < Width)) {
    float Pvalue = 0;

    for (int k = 0; k < Width; ++k) {
      Pvalue += M[Row*Width+k]*N[k*Width+Col];
    }
    P[Row*Width+Col] = Pvalue;
  }
}

__global__ 
void MatrixMulNewKernel(float* d_M, float* d_N, float* d_P,int Width) {
  __shared__ float Mds[TILE_WIDTH][TILE_WIDTH];
  __shared__ float Nds[TILE_WIDTH][TILE_WIDTH];
  int bx = blockIdx.x; int by = blockIdx.y;
  int tx = threadIdx.x; int ty = threadIdx.y;
  
  int Row = by * TILE_WIDTH + ty;
  int Col = bx * TILE_WIDTH + tx;
  float Pvalue = 0;

  for (int ph = 0; ph < Width/TILE_WIDTH; ++ph) {
    //carga d_M y d_N en la memoria compartida
    Mds[ty][tx] = d_M[Row*Width + ph*TILE_WIDTH + tx];
    Nds[ty][tx] = d_N[(ph*TILE_WIDTH + ty)*Width + Col];
    __syncthreads();
    for (int k = 0; k < TILE_WIDTH; ++k) {
      Pvalue += Mds[ty][k] * Nds[k][tx];
    }
    __syncthreads();
  }

  d_P[Row*Width + Col] = Pvalue;
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

  MatrixMulNewKernel<<<dimGrid, dimBlock>>>(d_A,d_B,d_C,n);

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
  int n=4;
  float *h_A,*h_B,*h_C;

  h_A = (float*) malloc(n*n*sizeof(float));
  h_B = (float*) malloc(n*n*sizeof(float));
  h_C = (float*) malloc(n*n*sizeof(float));
  
  datosRandom(h_A,n);
  datosRandom(h_B,n);

  float time;
  //tiempos
  time =clock();
  MatrixMult(h_A,h_B,h_C,n);
  time = clock() - time;
  printf("Time:  %f \n", time/CLOCKS_PER_SEC);

  printMatrix(h_C,n);
  
  return 0;
}

