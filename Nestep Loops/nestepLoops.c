#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
clock_t inicio1, inicio2, inicio, fin1, fin2, fin;
double tiempoTotal1, tiempoTotal2,tiempoTotal;

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
   	inicio1 = clock();

   	int i, j, k ;
   	for (i=0;i<n; i++)
      	for (j = 0; j<o; j++)
        	for (k=0;k<m; k++)
            	a[i][j] += b[i][k]* c[k][j] ;
    
    fin1 = clock();
    tiempoTotal1 = ((double) (fin1 - inicio1)) * 1000 / CLOCKS_PER_SEC;
    printf("%f\n", tiempoTotal1);
    //print(n,o,a);
}

void m_columnas(int n, int m, int o, int a[n][o], int b[n][m], int c[m][o])
{
   	inicio2 = clock();

   	int i, j, k ;
   	for (i=0;i<n; i++)
      	for (k=0;k<m; k++)
         	for (j = 0; j<o; j++)
            	a[i][j] += b[i][k]* c[k][j];

    fin2 = clock();
    tiempoTotal2 = ((double) (fin2 - inicio2)) * 1000 / CLOCKS_PER_SEC;
    printf("%f\n", tiempoTotal2);
    //print(n,o,a);
}

void m_MatrixBlocked(int n, int m, int o, int BlockSize,int a[n][o], int b[n][m],int c[m][o])
{
	inicio = clock();

	int iIter ,jIter ,kIter ,i ,j ,k;
	for(iIter = 0; iIter < n; iIter+=BlockSize)
        for(jIter = 0; jIter < m; jIter += BlockSize)
            for(kIter = 0; kIter < o; kIter += BlockSize)
                for(i = iIter; i < MIN(iIter+BlockSize,n); i++)
                    for(j = jIter; j < MIN(jIter+BlockSize,m); j++)
                        for(k = kIter; k < MIN(kIter+BlockSize,o); k++)
                            a[i][j] += b[i][k] * c[k][j];

    fin = clock();
    tiempoTotal = ((double) (fin - inicio)) * 1000 / CLOCKS_PER_SEC;
    printf("%f\n", tiempoTotal);
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
/*
//EJEMPLO INICIAL
	int x[2][3]={0,1,2,3,4,5};
	int y[3][3]={2,2,2,2,2,2,2,2,2};
	int v[2][3]={0};
	//m_columnas(n,m,o,v,x,y);
	//m_filas(n,m,o,v,x,y);
	m_MatrixBlocked(2,3,3,3,v,x,y);

*/
//TAMAÑO DE LAS MATRICES
	int n=200,
		m=1000,
		o=200;
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
	//m_filas(n,m,o,c,a,b);
	//m_columnas(n,m,o,c,a,b);
	int BlockSize=50;
	m_MatrixBlocked(n,m,o,BlockSize,c,a,b);

    return 0;
}