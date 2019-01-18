#include<stdio.h>
#include<math.h>
#include<stdlib.h>
float temp;
FILE *iesire;
char nume[128];
int numar,i,j;
int main()
{
	printf("\nIntroduceti fisierul de iesire: ");
	gets(nume);
	iesire=(FILE *)fopen(nume,"w");
	printf("Introduceti numarul de ecuatii: ");
	scanf("%d",&numar);
	for(j=0;j<numar;j++)
	{
		for(i=1;i<(numar+1);i++)
		{
			temp=rand()%1000; //a se vedea compatibilitatea
			if((temp>=0)&&(i!=1)) fprintf(iesire,"+%.2fx%u",temp,i);
			else	fprintf(iesire,"%.2fx%u",temp,i);
		}
		temp=rand()%1000; //a se vedea compatibilitatea
		fprintf(iesire,"=%.2f\n",temp);
	}
	fclose(iesire);
	return 0;
}