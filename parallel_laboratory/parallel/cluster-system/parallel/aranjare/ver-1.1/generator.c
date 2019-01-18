#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<zlib.h>
double temp;
gzFile *iesire;
char nume[128];
int numar,j;
int main()
{
	printf("\nIntroduceti fisierul de iesire: ");
	gets(nume);
	iesire=(gzFile *)gzopen(nume,"w");
	printf("Introduceti numarul de variabile: ");
	scanf("%d",&numar);
	for(j=0;j<numar;j++)
	{
		temp=rand()%1000; /* a se vedea compatibilitatea */
		gzwrite(iesire,&temp,sizeof(double));
	}
	gzclose(iesire);
	return 0;
}