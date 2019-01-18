void calculez()
{
int i,k,j;
	for(k=0;k<variabila;k++)
	{
		for(j=(k+1);j<variabila;j++)
			mat[k][j]=mat[k][j]/mat[k][k];
		mat[k][variabila]=mat[k][variabila]/mat[k][k];
		mat[k][k]=1;
		for(i=(k+1);i<variabila;i++)
		{
			for(j=(k+1);j<variabila;j++)
				mat[i][j]=mat[i][j]-mat[i][k]*mat[k][j];
			mat[i][variabila]-=mat[i][k]*mat[k][variabila];
			mat[i][k]=0;
		}
	}
}
void back_substitution()
{
int i,k;
	for(k=(variabila-1);k>=0;k--)
	{
		rezultate[k]=mat[k][variabila];
		for(i=(k-1);i>=0;i--)
			mat[i][variabila]-=rezultate[k]*mat[i][k];
	}
}