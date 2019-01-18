# Generated automatically from Makefile.in by configure.
###### User configurable options ######
ARCH		=LINUX
COMM		=
#INSTALL_DIR	=/usr
INSTALL_DIR	=/opt/mpich
CC		=gcc
#CC		=stgcc
F77		=mpif77
CLINKER		=$(CC)
FLINKER		=$(F77)
OPTFLAGS	=-O3
#
LIB_PATH	=-L$(INSTALL_DIR)/lib/ 
FLIB_PATH	=$(INSTALL_DIR)/lib/
LIB_LIST	=-lmpich
#
INCLUDE_DIR	= -I$(INSTALL_DIR)/include -I./

### End User configurable options ###

CFLAGS	= -DUSE_SOCKLEN_T -DUSE_U_INT_FOR_XDR -DHAVE_MPICHCONF_H $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
FFLAGS	= $(INCLUDE_DIR) $(OPTFLAGS)
LIBS 	=$(LIB_PATH) $(LIB_LIST) 
FLIBS 	=$(FLIB_PATH) $(LIB_LIST)
#libs	=-lm -lmpich -lpthread
libs	=-lm -lmpich -lpmpich
EXECS	=gj-mpich
OBJ	=bench.o
#OBJ	=bench-sth.o


default: clean all

all:	$(EXECS)

$(EXECS): $(OBJ) $(INSTALL_DIR)/include/mpi.h 
	$(CLINKER) $(OPTFLAGS) -o $(EXECS) $(OBJ) $(LIB_PATH) $(LIB_LIST) $(libs)


clean:
	/bin/rm -f *.o *~ $(EXECS) core

.c.o:
	$(CC) $(CFLAGS) -c $*.c