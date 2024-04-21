/******************************************************************************
 *  CVS version:
 *     $Id: error.h,v 1.1 2003/05/13 22:21:01 nickie Exp $
 ******************************************************************************
 *
 *  C header file : error.h
 *  Project       : PCL Compiler
 *  Version       : 1.0 alpha
 *  Written by    : Nikolaos S. Papaspyrou (nickie@softlab.ntua.gr)
 *  Date          : May 14, 2003
 *  Description   : Generic symbol table in C, simple error handler
 *
 *  ---------
 *  National Technical University of Athens.
 *  School of Electrical and Computer Engineering.
 *  Division of Computer Science and Technology.
 *  Software Technology Laboratory
 */


#ifndef __ERROR_H__
#define __ERROR_H__


/* ---------------------------------------------------------------------
   ------------- Prototypes of the error handler functions -------------
   --------------------------------------------------------------------- */

void internal (const char * fmt, ...);
void fatal    (const char * fmt, ...);
void error    (const char * fmt, ...);
void warning  (const char * fmt, ...);


#endif
