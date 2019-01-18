/*
	header pentru definitii client
*/
#define SIZE 512
#define PORT_circular 55002
#define RETEA 64

FILE *fconfig;				//fisier de configurare
static char buffer[SIZE];
static int porturi[2];	//porturile
char zero[2]="0";
char unu[2]="1";
char doi[2]="2";
//pentru select
int n_ready;
fd_set read_fd,test1_fd;
struct timeval time_2_wait;
//sincronizari
int ready[RETEA]; //sincronizare de ready (a primit configuratia)
int flag_iesire;						//flag de iesire
//componenta retea
int hosts;								//numarul de host-uri
int nr;									//numarul de linii care revin fiecarei masini
int variabila;							//numarul de necunoscute
int client_socket[RETEA];			//socketul client
struct sockaddr_in *server_addr;
int i;		//contoare
typedef struct
{
	char IP[15];
	int port;
} adr;
adr *adresa;

typedef struct
{
	float **mat;
	float *pmat;
	int contor;
	int *vector;
	int ultim;
} sdata;
sdata *data;
float *rezultate;
//mesaje
char mesaj_ack[4]="ACK";
char mesaj_ready[6]="READY";
char mesaj_exit[5]="EXIT";