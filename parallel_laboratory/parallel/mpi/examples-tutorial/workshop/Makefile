#
# Makefile for workshop examples
SHELL = /bin/sh

# MPIF77 should be the path to your compiler for MPI programs.  Some choices
# are
# SGI: MPIF77 = f77
# IBM: MPIF77 = mpxlf (or mpxlf_r)
# MPICH: MPIF77 = <path for mpich>/mpif77
MPIF77 = mpif77

# MPICC should be the path to your compiler for MPI programs.  Some choices
# are
# SGI: MPICC = cc
# IBM: MPICC = mpcc (or mpcc_r)
# MPICH: MPICC = <path for mpich>/mpicc
MPICC = mpicc
# MPILIBS should be any special libraries needed.  Some choices are
# SGI: MPILIBS = -lmpi
# IBM: MPILIBS = 
# MPICH: MPILIBS =
MPILIBS = -mpich
#
# MPIRUN should be the path to your program to run MPI jobs.
# MPINP is the name of the argument that sets the number of MPI processes to
# use. Some choices are
# SGI: MPIRUN = mpirun
# SGI: MPINP = -np
# IBM: MPIRUN = poe
# IBM: MPINP = -procs
# MPICH: <path for mpich>/mpirun
# MPICH: MPINP = -np
MPIRUN = mpirun
MPINP  = -np

EXECS = buflimit fairness fairwaitsome bcast
#
# Default target builds, runs everything
ALL: $(EXECS)
	make run pgm=buflimit np=2
	make run pgm=fairness np=4
	make run pgm=fairwaitsome np=4
	make run pgm=bcast np=4 

ALLLABS:
	for dir in pt2pt messages collective io ; do \
	    (cd $$dir ; make ) \
	done

OPTFLAGS = -g
#
# Compilation
.f.o:
	$(MPIF77) $(FFLAGS) $(OPTFLAGS) -c $*.f
.f:
	$(MPIF77) $(FFLAGS) $(OPTFLAGS) -o $* $*.f
.c.o:
	$(MPICC)  $(CFLAGS) $(OPTFLAGS) -c $*.c
.c:
	$(MPICC) $(CFLAGS) $(OPTFLAGS) -o $* $*.c
#
# To run a program, use
#    make run pgm=<name> np=<number>
# e.g.,
#    make pgm=buflimit np=2
run: $(pgm)
	$(MPIRUN) $(MPINP) $(np) $(pgm)

clean:
	rm -f *.o *~ PI* $(EXECS)
	for dir in pt2pt messages collective io ; do \
	    (cd $$dir ; make clean ) \
	done
