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
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
int gauss_fast_omp(long dim,int thread,double **mat,double *x,double *libre);
int gaussJ_fast_omp(long dim,int thread,double **mat,double *x,double *libre);
/*
    dim c'est la dimension de matrice
    thread c'est le numere de threads
    mat c'est la matrice de system
    libre c'est le termen libre
    x c'est le resoudre
*/