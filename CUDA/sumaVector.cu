#include <stdio.h>
#include <stdlib.h>    
#include <time.h>
#include <iostream>

using namespace std;

__global__
void vecAddKernel(float *A, float *B, float *C, int n){
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if(i < n) 
    C[i] = A[i] + B[i];
}

void vecAdd(float* A, float* B, float* C, int n){
  int size = n*sizeof(float);
  float *d_A, *d_B, *d_C;

  cudaMalloc((void **) &d_A, size);
  cudaMemcpy(d_A,A,size,cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_B, size);
  cudaMemcpy(d_B,B,size,cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_C, size);

  vecAddKernel<<<ceil(n/256.0), 256>>>(d_A,d_B,d_C,n);
  
  cudaMemcpy(C,d_C,size,cudaMemcpyDeviceToHost);

  cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}

void datosRandom(float *array){
  for(int i = 0; i < n; i++)
  {
     for(int j = 0; j < n; j++)
       array[i*n+j] = rand() % 100;
    }
}

void printMatrix(float *array){
  for(int i = 0; i < n; i++){
      cout<<h_A[i]<<" ";
    }
    cout<<endl;
}

int main(){
  int n,i;
  float *h_A,*h_B,*h_C;
  scanf("%d", &n);
  h_A = (float*) malloc(n*sizeof(float));
  h_B = (float*) malloc(n*sizeof(float));
  h_C = (float*) malloc(n*sizeof(float));
  
  datosRandom(h_A);
  datosRandom(h_B);
  vecAdd(h_A,h_B,h_C,n);
  printMatrix(h_C);

  return 0;
}
