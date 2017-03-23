#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print(int n,int m, int a[n][m])
{
	int i,j;
	for (i=0 ; i<n; i++){
		for(j=0; j<m; j++){
			printf("%d\t",a[i][j]);
		}
		printf("\n");
	}
}

void m_filas(int n, int m, int o,  int a[n][o], int b[n][m], int c[m][o])
{
   	int i, j, k ;

   	for (i=0;i<n; i++)
      	for (j = 0; j<o; j++)
        	for (k=0;k<m; k++)
            	a[i][j] += b[i][k]* c[k][j] ;
    //print(n,o,a);
}

void m_columnas(int n, int m, int o, int a[n][o], int b[n][m], int c[m][o])
{
   	int i, j, k ;

   	for (i=0;i<n; i++)
      	for (k=0;k<m; k++)
         	for (j = 0; j<o; j++)
            	a[i][j] += b[i][k]* c[k][j];
    //print(n,o,a);
}

void m_llenar(int n,int m,int mat[n][m])
{
	int i,j;
	srand (time(NULL));
	for (i=0 ; i<n; i++)
		for(j=0; j<m; j++){
			int var=rand()%100;
			mat[i][j]=var;
		}
}

void m_llenar_ceros(int n,int m,int mat[n][m])
{
	int i,j;
	for (i=0 ; i<n; i++)
		for(j=0; j<m; j++){
			mat[i][j]=0;
		}
}

int main() {
//EJEMPLO INICIAL
	/*
	int n=5, m=2, o=4;
	int x[5][2]={0,1,2,3,4,5,6,7,8,9};
	int y[2][4]={2,2,2,2,2,2,2,2};
	int v[5][4]={0};
	m_columnas(n,m,o,v,x,y);
	//m_filas(n,m,o,v,x,y);
	*/
//TAMAÑO DE LAS MATRICES
	int n=1000,
		m=500,
		o=1000;
	int a[n][m], 
		b[m][o],
		c[n][o];
	
//LLENO LAS MATRICES
	m_llenar(n,m,a);
	m_llenar(m,o,b);
	m_llenar_ceros(n,o,c);
	
//IMPRIMO MATRICES
	/*print(n,m,a);
	
	printf("\n");

	print(m,o,b);

	printf("\n");*/

//MULTIṔLICACION DE MATRICES
	m_filas(n,m,o,c,a,b);
	//m_columnas(n,m,o,c,a,b);

    return 0;
}