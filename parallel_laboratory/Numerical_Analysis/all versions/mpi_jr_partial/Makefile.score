include $(BUILD)/config.make
include $(BUILD)/env/mpi
LANGUAGE=gnu-c
PROGRAM=proiect
HEADERS=
SRCS=bench.c
OBJS=bench.o
LDLIBS=-lm -lpthread $(MPI_LIBS) $(MPE_LIBS)
#libs	=-lm -lpthread -lmpich
LDFLAGS=-s
CPPFLAGS=$(MPI_INCLUDE) $(MPE_INCLUDE) -O3
OBJDIR_RULE=prog-wrapper
include $(RULES)/srcdir
