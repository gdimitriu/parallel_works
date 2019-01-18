/* Programul CLIENT pentru gauss*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include"definitii.h"
#include"fisier.h"
#include"calcul.h"

int main()
{
int j,i;
	read_data();
	calculez();
	back_substitution();
	for(j=0;j<variabila;j++) printf("X%d=%f\n",j,rezultate[j]);
	free(*mat);
	free(mat);
	free(rezultate);
	free(y);
	return 0;
}
