#include<stdio.h>
#include<math.h>
#include<stdlib.h>
float temp;
FILE *iesire;
char nume[128];
int numar,j;
int main()
{
	printf("\nIntroduceti fisierul de iesire: ");
	gets(nume);
	iesire=(FILE *)fopen(nume,"w");
	printf("Introduceti numarul de variabile: ");
	scanf("%d",&numar);
	for(j=0;j<numar;j++)
	{
		temp=rand()%1000; /* a se vedea compatibilitatea */
		fprintf(iesire,"%.2f ",temp);
	}
	fclose(iesire);
	return 0;
}