double **matA,**matB,**matC1,**matC2,**matBd;
double *ptmat;
long variable;
void matrix_mult_ser(void)
{
int i,j,k;
	for(i=0;i<variable;i++)
		for(j=0;j<variable;j++)
			for(k=0;k<variable;k++)
				matC1[i][j]+=matA[i][k]*matB[k][j];
}
void matrix_mult_ser_2(void)
{
int i,j,k;
long temp;
	for(i=0;i<variable;i++) 
	for(k=0;k<variable;k++)
		for(j=0;j<variable;j++)
			matC2[i][j]+=matA[i][k]*matBd[j][k];
}