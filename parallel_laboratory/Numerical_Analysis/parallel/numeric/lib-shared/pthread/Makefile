# Generated by makegen....

TARGET =libparallel
OBJECTS = matrix_mult.o gauss_stiped.o\
	pthread_barrier_sem.o pthread_barrier.o pthread_semaphores.o

CC = gcc
CFLAGS =-O $(INCDIR) -Wall

LIBS =
LIBDIR =
INCDIR = -I/usr/include -I../include

INSTALLDIR = 

all:	$(OBJECTS)
	ar -vq ../lib/libparallel-pthread.a $(OBJECTS)
	ranlib ../lib/libparallel-pthread.a

clean:
	rm -f a.out core *~ \#* *.bak ../lib/libparallel-pthread.a $(OBJECTS)

