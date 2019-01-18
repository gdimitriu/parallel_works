# Generated automatically from Makefile.in by configure.
###### User configurable options ######
ARCH		=LINUX
COMM		=
INSTALL_DIR	=$(MPI_HOME)
CC		=mpicc
F77		=mpif77
CLINKER		=$(CC)
FLINKER		=$(F77)
OPTFLAGS	=
#
LIB_PATH	=-L$(INSTALL_DIR)/lib/ -L../parallel/lib
FLIB_PATH	=$(INSTALL_DIR)/lib/
LIB_LIST	=-lmpich
#
INCLUDE_DIR	= -I$(INSTALL_DIR)/include -I../parallel/include

### End User configurable options ###

CFLAGS	= -DUSE_SOCKLEN_T -DUSE_U_INT_FOR_XDR -DHAVE_MPICHCONF_H $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
FFLAGS	= $(INCLUDE_DIR) $(OPTFLAGS)
LIBS 	=$(LIB_PATH) $(LIB_LIST)
FLIBS 	=$(FLIB_PATH) $(LIB_LIST)
EXECS	=testc
OBJ	=ring-orig.o

default: all

all: $(EXECS)

testc: $(OBJ) $(INSTALL_DIR)/include/mpi.h 
	$(CLINKER) $(OPTFLAGS) -o $(EXECS) $(OBJ) $(LIB_PATH) $(LIB_LIST) -lm

testf: $(OBJ) $(INSTALL_DIR)/include/mpi.h 
	$(FLINKER) $(OPTFLAGS) -o $(EXECS) $(OBJ) $(LIB_PATH) $(LIB_LIST) -lm

clean:
	/bin/rm -f *.o *~ PI* $(EXECS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c
.f.o:
	$(F77) $(FFLAGS) -c $*.f