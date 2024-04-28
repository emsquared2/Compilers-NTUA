/******************************************************************************
 *  CVS version:
 *     $Id: general.h,v 1.1 2004/05/05 22:00:08 nickie Exp $
 ******************************************************************************
 *
 *  C header file : general.h
 *  Project       : PCL Compiler
 *  Version       : 1.0 alpha
 *  Written by    : Nikolaos S. Papaspyrou (nickie@softlab.ntua.gr)
 *  Date          : May 5, 2004
 *  Description   : Generic symbol table in C, general header file
 *
 *  ---------
 *  National Technical University of Athens.
 *  School of Electrical and Computer Engineering.
 *  Division of Computer Science and Technology.
 *  Software Technology Laboratory
 */


#ifndef __GENERAL_H__
#define __GENERAL_H__


/* ---------------------------------------------------------------------
 * ---------- Prototypes of the memory management functions ------------
 * --------------------------------------------------------------------- */

void * new    (size_t);
void   delete (void *);


/* ---------------------------------------------------------------------
   ----------------- Global variables of the compiler ------------------
   --------------------------------------------------------------------- */

extern int linecount;
extern const char * filename;


#endif
