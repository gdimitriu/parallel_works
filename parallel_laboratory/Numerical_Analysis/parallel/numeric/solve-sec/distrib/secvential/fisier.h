/*
	header pentru functiile cu fisiere
*/
//functia de salt peste variabila
int fsalt(int contor)
{
	contor++;
	if(contor<10) return 1;
	if((contor<100)&&(contor>9)) return 2;
	if((contor<1000)&&(contor>99)) return 3;
	if((contor<10000)&&(contor>999)) return 4;
	if((contor<100000)&&(contor>9999)) return 5;
	return 6;
}
//functia de citire a sistemului
void read_data()
{
char separatori;
int j,k;
int salt;
char nume[128];
	for(j=0;j<128;j++) nume[j]=0;
	retray:	write(1,"Indroduceti numele fisierului ",30*sizeof(char));
	gets(nume);
	if((fconfig=(FILE *)fopen(nume,"r"))==(FILE *)NULL)
	{
		perror("error open file");
		j++;
		if(j<6) goto retray;
	}
	separatori='0';
	variabila=0;
	rewind(fconfig);
	while(separatori!='=')
	{
		fscanf(fconfig,"%c",&separatori);
		if(separatori=='x') variabila++;
	}
	rewind(fconfig);
	//aloc memorie
	rezultate=(float *)calloc(variabila,sizeof(float *));
	y=(float *)calloc(variabila,sizeof(float *));
	mat=(float **)calloc(variabila,sizeof(float *));
	pmat=(float *)calloc(variabila*(variabila+1),sizeof(float));
	for(j=0;j<variabila;j++)
	{
		mat[j]=pmat;
		pmat+=variabila+1;
	}
	
	//citesc datele si le distribui in structurile masinilor
	for(k=0;k<variabila;k++)
	{
		//citesc matricea A
		for(j=0;j<variabila;j++)
		{
			fscanf(fconfig,"%f",&mat[k][j]);
			salt=fsalt(j);
			fseek(fconfig,salt+1,SEEK_CUR);
		}
		//sar peste egal
		fseek(fconfig,1,SEEK_CUR);
		//citesc b
		fscanf(fconfig,"%f",&mat[k][variabila]);
		fseek(fconfig,1,SEEK_CUR);
	}
	fclose(fconfig);
}