	for(i=0;i<N;i++) qi[i]=0.0;
	for(i=0;i<N;i++)
	{
		sum1=0.0;
		for(j=i+1;j<N;j++)
		sum1+=fabs(mat[i][j]/mat[i][i]);
		for(j=0;j<i;j++)
			sum1+=fabs(mat[i][j]/mat[i][i])*qi[j];
		qi[i]=sum1;
	}
	q=qi[0];
	for(i=1;i<N;i++) if(q<qi[i]) q=qi[i];
