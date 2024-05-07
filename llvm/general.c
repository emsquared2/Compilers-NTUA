/******************************************************************************
 *  CVS version:
 *     $Id: general.c,v 1.1 2004/05/05 22:00:08 nickie Exp $
 ******************************************************************************
 *
 *  C code file : general.c
 *  Project     : PCL Compiler
 *  Version     : 1.0 alpha
 *  Written by  : Nikolaos S. Papaspyrou (nickie@softlab.ntua.gr)
 *  Date        : May 5, 2004
 *  Description : Generic symbol table in C, general variables and functions
 *
 *  ---------
 *  National Technical University of Athens.
 *  School of Electrical and Computer Engineering.
 *  Division of Computer Science and Technology.
 *  Software Technology Laboratory
 */


/* ---------------------------------------------------------------------
   ---------------------------- Header files ---------------------------
   --------------------------------------------------------------------- */

#include <stdlib.h>

#include "general.h"
#include "error.h"


/* ---------------------------------------------------------------------
   ----------- Implementation of memory management functions -----------
   --------------------------------------------------------------------- */

void * new (size_t size)
{
   void * result = malloc(size);
   
   if (result == NULL)
      fatal("\rOut of memory");
   return result;
}

void delete (void * p)
{
   if (p != NULL)
      free(p);
}


/* ---------------------------------------------------------------------
   ------- The input file of the compiler and the line number. ---------
   --------------------------------------------------------------------- */

const char * filename;
int linecount;
