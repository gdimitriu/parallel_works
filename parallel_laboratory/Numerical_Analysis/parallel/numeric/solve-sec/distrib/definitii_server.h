/*
	header pentru  definitii server
*/
#define Port 50000
#define SIZE 512

int host;			//numarul hostului
int host_nr;		//numarul de hosturi
int ec_ultim;		//masina care a primit ultima ecuatie
char primul;		//masina care este prima (0) si ultima (2)
int last;			//ultima linie care s-a trimis

float *temporar;	//linia trimisa
int linie;			//nr liniei care este continuta in temporar
int masina;			//masina care a trimis linia din temporar

//matricea coeficientilor
int nr_linii;		//numarul de linii ce revin masinii
int nr;				//numarul de linii  ce se aloca
int variabila;		//numarul de variabile coloane ale matricii
int *vector;		//vectorul cu numarul de ordine a ecuatiiei
//pointeri pentru matrice
float *pdata;
float **data;
float *rezultate;	//rezultatele calculelor

//date pentru select
fd_set read_fd,test1_fd;
struct timeval time_2_wait;

//mesaje de sincronizare
char mesaj_ack[4]="ACK";
char mesaj_ready[6]="READY";
char mesaj_exit[5]="EXIT";

//structuri de date pentru realizare conexiuni
static char buffer[SIZE];
static char adresa[15];
static int porturi[2];
int PORT;
int listen_socket,		//socket pentru ascultare
	server_socket,		//socket pentru server
	client_len;			//lungimea adresei client
struct sockaddr_in
	server_addr,		//adresa serverului
	client_addr;		//adresa clientului
int listen_socket_dw,		//socket pentru ascultare down
	server_socket_dw,			//socket pentru server down
	client_len_dw;				//lungimea adresei client down
struct sockaddr_in
	server_addr_dw,			//adresa serverului down
	client_addr_dw;			//adresa clientului down
int client_socket_up;		 			//socket client up
struct sockaddr_in server_addr_up;	//adresa serverului up
int flag;
pid_t client1;					//ID pentru client
int status;						//variabila pentru waitpid
char *peroare;
char error;
