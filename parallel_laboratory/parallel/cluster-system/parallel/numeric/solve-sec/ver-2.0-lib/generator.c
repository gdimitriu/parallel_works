#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<zlib.h>
double temp;
gzFile *iesire;
char nume[128];
int numar,i,j;
int main()
{
	printf("\nIntroduceti fisierul de iesire: ");
	gets(nume);
	iesire=(gzFile *)gzopen(nume,"w");
	printf("Introduceti numarul de ecuatii: ");
	scanf("%d",&numar);
//	gzwrite(iesire,&numar,sizeof(int));
	for(j=0;j<numar;j++)
	{
		for(i=0;i<numar+1;i++)
		{
			temp=rand()%1000; /* a se vedea compatibilitatea */
			gzwrite(iesire,&temp,sizeof(double));
		}
	}
	gzclose(iesire);
	return 0;
}