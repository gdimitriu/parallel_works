/*
	header pentru functiile cu fisiere
*/
//functia de citire a adreselor si porturilor masinilor implicate
int readconfig()
{
int nr,i;
char test1;
	adresa=(adr *)calloc(RETEA,sizeof(adr));
	if((fconfig=(FILE *)fopen("gauss.conf","r"))<0)
	{
		perror("FILE error");
		exit(1);
	}
	nr=1;
	for(i=0;i<15;i++)
	while(!feof(fconfig))
	{
		test1=0;
		i=0;
		while(test1==0)
		{
			fscanf(fconfig,"%c",&adresa[nr-1].IP[i]);
			if(adresa[nr-1].IP[i]==':') 
			{
				adresa[nr-1].IP[i]=0;
				fscanf(fconfig,"%d",&adresa[nr-1].port);
				test1=1;
			}
			i++;
		}
		if(feof(fconfig)) 
		{	
			fclose(fconfig);
			return nr;
		}
		fseek(fconfig,1,SEEK_CUR);
		nr++;
	}
	for(i=0;i<nr;i++)
	printf("%d %s",i,adresa[i].IP);
	nr--;
	fclose(fconfig);
	return nr;
}
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
int j,k,temp;
int salt;
float temp1;
char nume[128];
	for(i=0;i<128;i++) nume[i]=0;
	i=0;
	retray:	write(1,"Indroduceti numele fisierului ",30*sizeof(char));
	gets(nume);
	if((fconfig=(FILE *)fopen(nume,"r"))==(FILE *)NULL)
	{
		perror("error open file");
		i++;
		if(i<6) goto retray;
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
	temp1=(float)variabila/(float)hosts;
	nr=(int)ceil((double)temp1);
	//aloc memorie
	data=(sdata *)calloc(hosts,sizeof(sdata));
	for(i=0;i<hosts;i++)
	{
		data[i].mat=(float **)calloc(nr,sizeof(float *));
		data[i].pmat=(float *)calloc(nr*(variabila+1),sizeof(float));
		for(j=0;j<nr;j++)
		{
			data[i].mat[j]=data[i].pmat;
			data[i].pmat+=variabila+1;
		}
		data[i].contor=0;
		data[i].vector=(int *)calloc(nr,sizeof(int));
	}
	
	//citesc datele si le distribui in structurile masinilor
	flag_iesire=0;
	i=0;
	k=0;
	while(flag_iesire==0)
	{
		//citesc matricea A
		for(j=0;j<variabila;j++)
		{
			fscanf(fconfig,"%f",&data[i].mat[data[i].contor][j]);
			salt=fsalt(j);
			fseek(fconfig,salt+1,SEEK_CUR);
		}
		//sar peste egal
		fseek(fconfig,1,SEEK_CUR);
		data[i].mat[data[i].contor][variabila]=0;
		//citesc b
		fscanf(fconfig,"%f",&data[i].mat[data[i].contor][variabila]);
		fseek(fconfig,1,SEEK_CUR);
		data[i].vector[data[i].contor]=k;
		k++;
		data[i].contor++;
		temp=0;
		for(j=0;j<hosts;j++) temp+=data[j].contor;
		if(temp==variabila)		flag_iesire=1;
		i++;
		if(i==hosts) i=0;
	}
	if(i==0) i=(hosts-1);
	else i--;
	for(j=0;j<hosts;j++)
	{
		if(j==i)	data[j].ultim=1;
		else data[j].ultim=0;
	}
	fclose(fconfig);
}