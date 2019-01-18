#include<stdio.h> 
#include<stdlib.h>

int main(int argc,char **argv)
{
	long equ1,equ2,tmpl;
	int flag=0;
	double ser,par,tempd,speedup;
	FILE *fser,*fpar,*fsp;
	
	if(argc!=4)
	{
		printf("syntax: %s serial parallel speedup\n",argv[0]);
		fflush(stdout);
		return -1;
	}
	fser=fopen(argv[1],"r");
	fpar=fopen(argv[2],"r");
	fsp=fopen(argv[3],"w");
	while(!feof(fser) && !feof(fpar) && flag==0)
	{
		fscanf(fser,"%ld ",&equ1);fflush(fser);
		if(feof(fser)) {flag=1; break;}
		fscanf(fpar,"%ld ",&equ2);fflush(fpar);
		if(feof(fpar)) { flag=1; break;}
		if(equ1!=equ2)
		{
			printf("Files not have the same step\n");
			fflush(stdout);
			return -2;
		}
		fprintf(fsp,"%ld ",equ1);fflush(fsp);
		fscanf(fser,"%lf ",&ser);fflush(fser);
		fscanf(fpar,"%lf ",&par);fflush(fpar);
		speedup=ser/par;
		fprintf(fsp,"%lf ",speedup);fflush(fsp);
		fscanf(fser,"%ld ",&equ1);fflush(fser);
		fscanf(fpar,"%ld ",&equ2);fflush(fpar);
		fprintf(fsp,"%ld\n",equ2);fflush(fsp);
		//junk
		fscanf(fser,"%lf ",&ser);fflush(fser);
		fscanf(fpar,"%lf ",&par);fflush(fpar);
		fscanf(fser,"%ld ",&equ1);fflush(fser);
		fscanf(fpar,"%ld ",&equ2);fflush(fpar);
	}
	fclose(fser);
	fclose(fpar);
	fclose(fsp);
	return 0;
}