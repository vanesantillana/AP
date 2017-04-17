#include <stdio.h>
#include  <time.h>
#include<string.h>
#include <stdlib.h>

/* int addInt(int n, int m) { */
/*   return n+m; */
/* } */

/* int add2to3(int (*functionPtr)(int, int)) { */
/*   printf("%d\n", (*functionPtr)(2, 3)); */
/* } */


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

void m_llenar_ceros(int n,int m,int mat[n][m]){
  int i,j;
  for (i=0 ; i<n; i++)
    for(j=0; j<m; j++){
      mat[i][j]=0;
    }
}



void create_marks_csv(char *filename,double a[][3],int n,int m){
  printf("\n Creating %s.csv file",filename);
  FILE *fp;
  int i,j;
  filename=strcat(filename,".csv");
  fp=fopen(filename,"w+");
  fprintf(fp,"Id, Algoritmo, Datos, Tiempo");
  for(i=0;i<m;i++){
    fprintf(fp,"\n%d,",i+1);
    for(j=0;j<n;j++)
      fprintf(fp,"%f, ",a[i][j]);
  }
  fclose(fp);
  printf("\n %sfile created",filename);
}


void timeCount(int tam,double lista[tam][3],char *nombre){
  double tiempos[tam];    
  double total[tam][3];

  for(int x=0;x<tam;x++){
    int n=lista[x][0];
    int m=lista[x][1];
    int o=lista[x][2];
    int a[n][m],b[m][o],c[n][o];
    m_llenar(n,m,a);
    m_llenar(m,o,b);
    m_llenar_ceros(n,o,c);
    clock_t begin = clock();
    //ALGORITMo
    m_filas(n,m,o,c,a,b);
    
    clock_t end = clock(); 
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    total[x][0]=1;
    total[x][1]=987;
    total[x][2]=time_spent;
  }
  create_marks_csv(nombre,total,tam,3);
 
  //return time_spent; 
}


int main(){  

  
  double a[3][3]={{1000,200,1000},{100,200,100},{100,170,200}};
  char nombre[20]="archivo1";
  timeCount(3,a,nombre);
//   //printf("%f\n",timeCount(functionPtr));
  
//     // clock_t begin = clock();  

//     //  for(int a=0;a<100000000;a++){  
//     //    //printf("%d\n",a);  
//     // }  
//     // clock_t end = clock();  
//     // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;  
//     // printf("%f\n",time_spent);  


//    //   add2to3( functionPtr);
//   //printf("%d\n",sum);
//   //printf("Hello, World!\n");
  return 0;
 }

