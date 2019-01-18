#
# Makefile for workshop examples

# MPIF77 should be the path to your compiler for MPI programs.  Some choices
# are
# SGI: MPIF77 = f77
# IBM: MPIF77 = mpxlf (or mpxlf_r)
# MPICH: MPIF77 = <path for mpich>/mpif77
MPIF77 = /home/MPI/releases/current/build/solaris/ch_p4/bin/mpif77

# MPICC should be the path to your compiler for MPI programs.  Some choices
# are
# SGI: MPICC = cc
# IBM: MPICC = mpcc (or mpcc_r)
# MPICH: MPICC = <path for mpich>/mpicc
MPICC = /home/MPI/releases/current/build/solaris/ch_p4/bin/mpicc

# MPILIBS should be any special libraries needed.  Some choices are
# SGI: MPILIBS = -lmpi
# IBM: MPILIBS = 
# MPICH: MPILIBS =
MPILIBS =
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
MPIRUN = /home/MPI/releases/current/build/solaris/ch_p4/bin/mpirun
MPINP  = -np

EXECS = write1 write2 write3 write4 readtest
#
# Default target builds, runs everything
ALL: $(EXECS)
	make run pgm=write1 np=6
	make run pgm=write2 np=6
	make run pgm=write3 np=6
	make run pgm=write4 np=6

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
	rm -f *.o *~ PI* test1 $(EXECS)
