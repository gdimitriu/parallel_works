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
 *	Copyright 1997 The Ohio State University
 *	NJN
 *
 *	$Id: lamports.c,v 6.3 1999/05/25 22:08:05 kmeyer1 Exp $
 *
 *	Function:	- port management functions
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <blktype.h>
#include <mpisys.h>
#include <terror.h>
#include <typical.h>

#define LAM_PORTMAX	16
#define LAM_PORTLOW	(BLKHIGH + 1)

/*
 * private functions
 */
static int		port_cmp();

/*
 * private variables
 */
static int		ports[LAM_PORTMAX];	/* port table */

/*
 *	lam_ports_init
 * 
 *	Function:	- initialize port bookkeeping structures
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ports_init()

{
	lam_ports = al_init(sizeof(struct _port), port_cmp);
	return((lam_ports == 0) ? LAMERROR : 0);
}

/*
 *	lam_ports_free
 * 
 *	Function:	- free port bookkeeping structures
 */
void
lam_ports_free()

{
	al_free(lam_ports);
}

/*
 *	lam_port_open
 * 
 *	Function:	- open a LAM MPI port
 *	Returns:	- port number or LAMERROR
 */
int
lam_port_open()

{
	int		i;
/*
 * Search for free port.
 */
	for (i = 0; i < LAM_PORTMAX; i++) {
		if (ports[i] == 0) {
			ports[i] = 1;
			return(LAM_PORTLOW + i);
		}
	}

	return(LAMERROR);
}

/*
 *	lam_port_is_open
 * 
 *	Function:	- checks if port is open
 *	Returns:	- 1 if open, 0 if not open, or LAMERROR if illegal port
 */
int
lam_port_is_open(port)

int			port;

{
	port -= LAM_PORTLOW;
	if (!((0 <= port) && (port < LAM_PORTMAX))) {
		errno = EINVAL;
		return(LAMERROR);
	}

	return(ports[port]);
}

/*
 *	lam_port_close
 * 
 *	Function:	- closes a port
 *	Returns:	- 0 else LAMERROR
 */
int
lam_port_close(port)

int			port;

{
	port -= LAM_PORTLOW;
	if (!((0 <= port) && (port < LAM_PORTMAX))) {
		errno = EINVAL;
		return(LAMERROR);
	}
	
	ports[port] = 0; 
	return(0);
}

/*
 *	port_cmp
 *
 *	Function:	- compare two ports
 *	Accepts:	- ptr to two entries
 *	Returns:	- string comparison of the port names
 */
static int
port_cmp(p1, p2)

struct _port		*p1, *p2;

{
	return(strcmp(p1->prt_name, p2->prt_name));
}
