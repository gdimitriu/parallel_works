# Generated automatically from Makefile.in by configure.
###### User configurable options ######
ARCH		=LINUX
COMM		=
INSTALL_DIR	=$(LAM_HOME)
CC		=mpicc
F77		=mpif77
CLINKER		=$(CC)
FLINKER		=$(F77)
OPTFLAGS	=-O
#
LIB_PATH	=-L$(INSTALL_DIR)/lib/ -L../../dstatic/lib
FLIB_PATH	=$(INSTALL_DIR)/lib/
LIB_LIST	=-lmpi -lm -lparallel-mpi
#
INCLUDE_DIR	= -I$(INSTALL_DIR)/include -I../include -I/home/gaby/private/parallel/include -I../../dstatic/include

### End User configurable options ###

CFLAGS	= -DUSE_SOCKLEN_T -DUSE_U_INT_FOR_XDR -DHAVE_MPICHCONF_H $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
FFLAGS	= $(INCLUDE_DIR) $(OPTFLAGS)
LIBS 	=$(LIB_PATH) $(LIB_LIST)
FLIBS 	=$(FLIB_PATH) $(LIB_LIST)
EXECS1	= numeric
OBJ1	=gauss_mpi_server.o
EXECS	=libparallel-mpi-dyn.a
OBJ	=gauss_mpi_client.o


default: clean all

all:	$(EXECS) $(EXECS1)

$(EXECS): $(OBJ) $(INSTALL_DIR)/include/mpi.h 
	ar -vq ../lib/$(EXECS) $(OBJ)
	ranlib ../lib/$(EXECS)

$(EXECS1): $(OBJ1) $(INSTALL_DIR)/include/mpi.h 
	$(CLINKER) $(OPTFLAGS) -o ../bin/$(EXECS1) $(OBJ1) $(LIBS) $(LIB_PATH) $(LIB_LIST)

clean:
	/bin/rm -f *.o *~ ../lib/$(EXECS) core ../bin/$(EXECS1)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
