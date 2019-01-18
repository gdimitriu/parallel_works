# Generated automatically from Makefile.in by configure.
###### User configurable options ######
ARCH		=LINUX
COMM		=
INSTALL_DIR	=$(MPI_HOME)
CC		=stgcc
F77		=mpif77
CLINKER		=$(CC)
FLINKER		=$(F77)
OPTFLAGS	=
#
LIB_PATH	=-L$(INSTALL_DIR)/lib/
FLIB_PATH	=$(INSTALL_DIR)/lib/
LIB_LIST	=-lmpich   
#
INCLUDE_DIR	= -I$(INSTALL_DIR)/include -I../include -I/home/gaby/private/parallel/include

### End User configurable options ###

CFLAGS	= -DUSE_SOCKLEN_T -DUSE_U_INT_FOR_XDR -DHAVE_MPICHCONF_H $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH)
FFLAGS	= $(INCLUDE_DIR) $(OPTFLAGS)
LIBS 	=$(LIB_PATH) $(LIB_LIST)
FLIBS 	=$(FLIB_PATH) $(LIB_LIST)
EXECS	=libparallel-mpi-sth.a
OBJ	=mult_sq_mat_check_mpi_sth.o gauss_striped_ciclique_mpi_sth.o \
	diffusion_gauss_striped_ciclique_mpi_sth.o diffusion_sq_striped_ciclique_mpi_sth.o \
	combine_sq_striped_ciclique_mpi_sth.o

default: clean all

all:	$(EXECS)

$(EXECS): $(OBJ) $(INSTALL_DIR)/include/mpi.h 
	ar -vq ../lib/$(EXECS) $(OBJ)
	ranlib ../lib/$(EXECS)

clean:
	/bin/rm -f *.o *~ ../lib/$(EXECS) core

.c.o:
	$(CC) $(CFLAGS) -c $*.c
