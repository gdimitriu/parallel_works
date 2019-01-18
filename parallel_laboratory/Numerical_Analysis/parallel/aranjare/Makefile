# Generated automatically from Makefile.in by configure.
###### User configurable options ######
ARCH		=LINUX
COMM		=
INSTALL_DIR	=/opt/programming/mpi
CC		=mpicc
F77		=mpif77
CCOMP		=gcc
CLINKER		=$(CC)
FLINKER		=$(F77)
OPTFLAGS	=
#
LIB_PATH	=-L$(INSTALL_DIR)/lib/ -L../parallel/lib
FLIB_PATH	=$(INSTALL_DIR)/lib/
LIB_LIST	=-lmpich   
#
INCLUDE_DIR	= -I$(INSTALL_DIR)/include -I./  -I../parallel/include

### End User configurable options ###

CFLAGS	=-fPIC  -DUSE_SOCKLEN_T -DUSE_U_INT_FOR_XDR -DHAVE_MPICHCONF_H $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
FFLAGS	=-fPIC  $(INCLUDE_DIR) $(OPTFLAGS)
LIBS 	=$(LIB_PATH) $(LIB_LIST) 
FLIBS 	=$(FLIB_PATH) $(LIB_LIST)
libs	=-lm -lparallel_mpi -lparallel -lpthread
EXECS	=testing
OBJ	=testing.o


default: all

all:	$(EXECS)

$(EXECS): $(OBJ) $(INSTALL_DIR)/include/mpi.h 
	$(CLINKER) $(OPTFLAGS) -o $(EXECS) $(OBJ) $(LIB_PATH) $(LIB_LIST) $(libs)
	$(CCOMP) generator.c -o generator -lm


clean:
	/bin/rm -f *.o *~ $(EXECS) core generator

.c.o:
	$(CC) $(CFLAGS) -c $*.c
