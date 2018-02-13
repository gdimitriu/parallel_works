/*
	Copyright 2006 Gabriel Dimitriu

	This file is part of scientific_computing.

    scientific_computing is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    scientific_computing is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with scientific_computing; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
*/
/*
	making the speedup for parallel calcul complete
*/
double timeprint(struct timeval t1,struct timeval t2,int T,long dim,FILE *fp,int thread)
{
double operand1,operand2,operand;
operand=0.0;
	operand1=(double)t1.tv_sec+(1e-6)*t1.tv_usec;
	operand2=(double)t2.tv_sec+(1e-6)*t2.tv_usec;
	operand=operand2-operand1;
	fprintf(fp,"%ld %lf %d %lf %d\n",dim,operand,thread,operand/T,T);
	fflush(fp);
	printf("%fs sum of %d executions with %ld equations and %d threads\n",operand,T,dim,thread);fflush(stdout);
	return operand;
}
