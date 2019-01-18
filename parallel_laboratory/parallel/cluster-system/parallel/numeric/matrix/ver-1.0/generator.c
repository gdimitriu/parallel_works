#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<zlib.h>
double temp;
gzFile *iesirea,*iesireb;
char numea[128],numeb[128];
int numar,i,j,flag;
int main()
{
	printf("\nIntroduceti fisierul de iesire a: ");
	gets(numea);
	printf("\nIntroduceti fisierul de iesire b: ");
	gets(numeb);
	iesirea=(gzFile *)gzopen(numea,"w");
	iesireb=(gzFile *)gzopen(numeb,"w");
	printf("Introduceti numarul de variabile: ");
	scanf("%d",&numar);
	printf("\ndoriti generare aleatoare(0) sau incrementala(1): ");
	scanf("%d",&flag); 
	for(i=0;i<numar;i++)
		for(j=0;j<numar;j++)
		{
			if(flag==0)	temp=rand()%1000; /* a se vedea compatibilitatea */
			else temp=j;
			gzwrite(iesirea,&temp,sizeof(double));
		}
	for(i=0;i<numar;i++)
		for(j=0;j<numar;j++)
		{
			if(flag==0)	temp=rand()%1000; /* a se vedea compatibilitatea */
			else temp=j;
			gzwrite(iesireb,&temp,sizeof(double));
		}
	gzclose(iesirea);
	gzclose(iesireb);
	return 0;
}