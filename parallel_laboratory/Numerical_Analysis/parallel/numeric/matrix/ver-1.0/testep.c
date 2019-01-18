#include<stdio.h>
#include<sys/time.h>	/* for time functions */
#include<unistd.h>
#include<stdlib.h>
#include"acceleration.h"
#include<math.h>
#include<parallel/parallel-pth.h>

#define N 160 /* Nr of lines/columns of matrix */
#define P 4	/* Nr of threads */
#define POW 4	/* the power of matrix */
double **a,**b,**c; /* matrix */
double *pa,*pb,*pc; /* pointers to matrix */
double **mattemp,**operand,**mtemp,**moutput;
double *x,*y;
double *pmattemp;
double cs[N][N];

void readfile(FILE *fp,double **mat)
{
	int i,j;
	rewind(fp);
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			fscanf(fp,"%lf",&mat[i][j]);
}
int main(int argc,char **argv)
{
	struct timeval t1s,t2s,t1p,t2p,t1,t2;
	int i,j,k,l;
	int ord;
	FILE *in1,*in2,*out1,*out2,*outs,*outs1,*outch,*outpr,*outpc,*outpch,*outpch_mpi,*outps;
	FILE *vects,*vectr;

	if((out1=(FILE *)fopen("out1.dat","w"))==(FILE *)NULL)
	{
		perror("error open out1");
		exit(-1);
	}
	if((out2=(FILE *)fopen("out2.dat","w"))==(FILE *)NULL)
	{
		perror("error open out2");
		exit(-1);
	}
	if((outs=(FILE *)fopen("outs.dat","w"))==(FILE *)NULL)
	{
		perror("error open outs");
		exit(-1);
	}
	if((outs1=(FILE *)fopen("outs1.dat","w"))==(FILE *)NULL)
	{
		perror("error open outs1");
		exit(-1);
	}
	if((outch=(FILE *)fopen("outch.dat","w"))==(FILE *)NULL)
	{
		perror("error open outch");
		exit(-1);
	}
	if((outpr=(FILE *)fopen("outpr.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpr");
		exit(-1);
	}
	if((outpc=(FILE *)fopen("outpc.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpc");
		exit(-1);
	}
	if((outpch=(FILE *)fopen("outpch.dat","w"))==(FILE *)NULL)
	{
		perror("error open outpch");
		exit(-1);
	}
	if((outps=(FILE *)fopen("outps.dat","w"))==(FILE *)NULL)
	{
		perror("error open outps");
		exit(-1);
	}
	if((vects=(FILE *)fopen("vects.dat","w"))==(FILE *)NULL)
	{
		perror("error open vects");
		exit(-1);
	}
	if((vectr=(FILE *)fopen("vectr.dat","w"))==(FILE *)NULL)
	{
		perror("error open vectr");
		exit(-1);
	}

	a=(double **)calloc(N,sizeof(double *));
	pa=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		a[j]=pa;
		pa+=N;
	}
	b=(double **)calloc(N,sizeof(double *));
	pb=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		b[j]=pb;
		pb+=N;
	}
	c=(double **)calloc(N,sizeof(double *));
	pc=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		c[j]=pc;
		pc+=N;
	}	
	mattemp=(double **)calloc(N,sizeof(double *));
	pmattemp=(double *)calloc(N*N,sizeof(double));
	for(j=0;j<N;j++)
	{
		mattemp[j]=pmattemp;
		pmattemp+=N;
	}	
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
	{
		a[i][j]=1.0;
		b[i][j]=1.0;
	}
	x=(double *)calloc(N,sizeof(double));
	y=(double *)calloc(N,sizeof(double));
	for(i=0;i<N;i++)
		x[i]=1.0f;

	/*
		multiplicity secventials for computing acceleration factor
	*/

	gettimeofday(&t1s,NULL);
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			cs[i][j]=0.0;
			for(k=0;k<N;k++)
				cs[i][j]+=a[i][k]*b[k][j];
		}
	gettimeofday(&t2s,NULL);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outs1,"%4.0f ",cs[i][j]);
		fprintf(outs1,"\n");
	}
	/* apelez rutina pe coloane */
	
	gettimeofday(&t1p,NULL);
	mult_sq_mat_striped(N,P,a,b,c,0);
	gettimeofday(&t2p,NULL);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(out2,"%4.0f ",c[i][j]);
		fprintf(out2,"\n");
	}
	printf("Column \n");
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
		c[i][j]=0;

	/* apelez rutina pe rinduri	*/

	gettimeofday(&t1p,NULL);
	mult_sq_mat_striped(N,P,a,b,c,1);
	gettimeofday(&t2p,NULL);
	printf("Row\n");
	acceleration(t1s,t2s,t1p,t2p,P);	
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(out1,"%4.0f ",c[i][j]);
		fprintf(out1,"\n");
	}

	/* apelez rutina pe careuri */

	gettimeofday(&t1p,NULL);
	mult_sq_mat_check(N,P,a,b,c);
	gettimeofday(&t2p,NULL);
	printf("Checkerboard\n");
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outch,"%4.0f ",c[i][j]);
		fprintf(outch,"\n");
	}

	/* apelez rutina de power seriala */

	ord=POW%2;
	if(ord==1)
	{
		operand=mattemp;
		moutput=mattemp;
	}
	else
	{
		operand=c;
		moutput=c;
	}
	gettimeofday(&t1s,NULL);
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			moutput[i][j]=0.0f;
				for(k=0;k<N;k++)
					moutput[i][j]+=a[i][k]*a[k][j];
		}
	if(ord==1)
		moutput=b;
	else
		moutput=mattemp;
	for(l=1;l<(POW-1);l++)
	{
		for(i=0;i<N;i++)
			for(j=0;j<N;j++)
			{
				moutput[i][j]=0.0f;
					for(k=0;k<N;k++)
						moutput[i][j]+=operand[i][k]*a[k][j];
			}
		mtemp=operand;
		operand=moutput;
		moutput=mtemp;
	}
	gettimeofday(&t2s,NULL);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outs,"%4.0f ",c[i][j]);
		fprintf(outs,"\n");
	}
	
	/* apelez rutina de power cu rinduri */

	gettimeofday(&t1p,NULL);
	pow_mat_striped(N,P,a,c,POW,1);
	gettimeofday(&t2p,NULL);
	printf("Pow row\n");
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpr,"%4.0f ",c[i][j]);
		fprintf(outpr,"\n");
	}

	/* apelez rutina de power cu coloane */

	gettimeofday(&t1p,NULL);
	pow_mat_striped(N,P,a,c,POW,0);
	gettimeofday(&t2p,NULL);
	printf("Pow column\n");
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpc,"%4.0f ",c[i][j]);
		fprintf(outpc,"\n");
	}

	/* apelez rutina de power cu carouri */

	gettimeofday(&t1p,NULL);
	pow_mat_check(N,P,a,c,POW);
	gettimeofday(&t2p,NULL);
	printf("Pow check\n");
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			fprintf(outpch,"%4.0f ",c[i][j]);
		fprintf(outpch,"\n");
	}

/* matrix vector multiplication */
	/* serial */
	
	gettimeofday(&t1s,NULL);
	for(i=0;i<N;i++)
	{
		y[i]=0.0;
		for(j=0;j<N;j++)
			y[i]+=a[i][j]*x[j];
	}
	gettimeofday(&t2s,NULL);
	for(i=0;i<N;i++)
		fprintf(vects,"%4.0f ",y[i]);
	fprintf(vects,"\n");
	
	/* row */
	
	gettimeofday(&t1p,NULL);
	mult_sq_mat_vect_striped(N,P,a,x,y);
	gettimeofday(&t2p,NULL);
	printf("Vector mat row\n");
	acceleration(t1s,t2s,t1p,t2p,P);
	for(i=0;i<N;i++)
		fprintf(vectr,"%4.0f ",y[i]);
	fprintf(vectr,"\n");

	fclose(out1);
	fclose(out2);
	fclose(outs);
	fclose(outs1);
	fclose(outch);
	fclose(outpr);
	fclose(outpc);
	fclose(outpch);
	fclose(outps);
	fclose(vects);
	fclose(vectr);

	free(*a);
	free(a);
	free(*b);
	free(b);
	free(*c);
	free(c);
	exit(0);
}
