# Generated automatically from Makefile.in by configure.
###### User configurable options ######
ARCH		=LINUX
COMM		=
INSTALL_DIR	=$(MPI_HOME)
CC		=mpicc
F77		=mpif77
CCOMP		=gcc -O
CLINKER		=$(CC)
FLINKER		=$(F77)
OPTFLAGS	=-O 
#
LIB_PATH	=-L$(INSTALL_DIR)/lib/ -L../../parallel/lib -L/opt/lib
FLIB_PATH	=$(INSTALL_DIR)/lib/
LIB_LIST	=-lmpich   
#
INCLUDE_DIR	= -I$(INSTALL_DIR)/include -I./  -I../../parallel/include -I/opt/include

### End User configurable options ###

CFLAGS	= -DUSE_SOCKLEN_T -DUSE_U_INT_FOR_XDR -DHAVE_MPICHCONF_H $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
FFLAGS	= $(INCLUDE_DIR) $(OPTFLAGS)
LIBS 	=$(LIB_PATH) $(LIB_LIST) 
FLIBS 	=$(FLIB_PATH) $(LIB_LIST)
libs	=-lm -lparallel-mpi -lparallel-pthread -lpthread -lz
EXECS	=testing
OBJ	=testing.o


default: all

all:	$(EXECS)

$(EXECS): $(OBJ) $(INSTALL_DIR)/include/mpi.h 
	$(CLINKER) $(OPTFLAGS) -o $(EXECS) $(OBJ) $(LIB_PATH) $(LIB_LIST) $(libs)
	$(CCOMP) generator.c -o generator -lm $(LIB_PATH) -lz


clean:
	/bin/rm -f *.o *~ $(EXECS) core generator

.c.o:
	$(CC) $(CFLAGS) -c $*.c
