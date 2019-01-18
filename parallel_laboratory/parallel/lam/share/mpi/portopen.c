/*
 * Copyright 1998-2001, University of Notre Dame.
 * Authors: Jeffrey M. Squyres, Arun Rodrigues, and Brian Barrett with
 *          Kinis L. Meyer, M. D. McNally, and Andrew Lumsdaine
 * 
 * This file is part of the Notre Dame LAM implementation of MPI.
 * 
 * You should have received a copy of the License Agreement for the Notre
 * Dame LAM implementation of MPI along with the software; see the file
 * LICENSE.  If not, contact Office of Research, University of Notre
 * Dame, Notre Dame, IN 46556.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted subject to the conditions specified in the
 * LICENSE file.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * Additional copyrights may follow.
 * 
 *	Ohio Trollius
 *	Copyright 1996 The Ohio State University
 *	NJN
 *
 *	$Id: portopen.c,v 6.7 2001/01/30 18:26:57 jsquyres Exp $
 *
 *	Function:	- open an MPI port
 *	Accepts:	- info
 *			- port name (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <sfh.h>

#include <string.h>

#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>

/*@
   MPI_Open_port - Establish a server port for client MPI connections

Input Parameters:
+ info - extra information handle
- port_name - newly established port

.N fortran

.N IMPI_MPI2

.N Errors
.N MPI_ERR_OTHER
.N MPI_ERR_INTERN

.seealso MPI_Close_port(3), MPI_Comm_accept(3), MPI_Comm_connect(3), MPI_Comm_disconnect(3)

.N WEB
@*/
int
MPI_Open_port(MPI_Info info, char* port_name)
{
	struct _port	port;			/* new port */
	int		port_num;		/* port number */

	lam_initerr();
	lam_setfunc(BLKMPIOPENPORT);
/*
 * Open a port.
 */
	if ((port_num = lam_port_open()) < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIOPENPORT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	LAM_ZERO_ME(port);
	port.prt_num = port_num;
/*
 * Add port to list of open ports.
 */
	sprintf(port.prt_name, "n%d:i%d:%d", lam_myproc->p_gps.gps_node,
		lam_myproc->p_gps.gps_idx, port_num);
	
	if (al_find(lam_ports, &port) != 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIOPENPORT,
					lam_mkerr(MPI_ERR_INTERN, 0)));
	}
	if (al_insert(lam_ports, &port) == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIOPENPORT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Set return value.  The MPI_MAX_PORT_NAME probably isn't necessary, but
 * hey, let's be safe.
 */
	strncpy(port_name, port.prt_name, MPI_MAX_PORT_NAME);
	
        lam_resetfunc(BLKMPIOPENPORT);
	return(MPI_SUCCESS);
}
