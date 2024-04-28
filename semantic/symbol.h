/******************************************************************************
 *  CVS version:
 *     $Id: symbol.h,v 1.1 2003/05/13 22:21:01 nickie Exp $
 ******************************************************************************
 *
 *  C header file : symbol.h
 *  Project       : PCL Compiler
 *  Version       : 1.0 alpha
 *  Written by    : Nikolaos S. Papaspyrou (nickie@softlab.ntua.gr)
 *  Date          : May 14, 2003
 *  Description   : Generic symbol table in C
 *
 *  Comments: (in Greek iso-8859-7)
 *  ---------
 *  National Technical University of Athens.
 *  School of Electrical and Computer Engineering.
 *  Division of Computer Science and Technology.
 *  Software Technology Laboratory
 */


#ifndef __SYMBOL_H__
#define __SYMBOL_H__


/* ---------------------------------------------------------------------
   --------------------------- Bool type -------------------------------
   --------------------------------------------------------------------- */

#include <stdbool.h>

/*
 * If the above include is not supported by the C implementation
 * you are using, replace it with the following:
*/

#if 0
typedef enum { false=0, true=1 } bool;
#endif


/* ---------------------------------------------------------------------
   --------- Definition of constants in the symbol table ---------------
   --------------------------------------------------------------------- */

#define START_POSITIVE_OFFSET 8     /* Initial positive offset in the A.R. */
#define START_NEGATIVE_OFFSET 0     /* Initial negative offset in the A.R. */


/* ---------------------------------------------------------------------
   ------------ Definition of types in the symbol table ----------------
   --------------------------------------------------------------------- */

/* Data types for the implementation of constants */

typedef int           RepInteger;         /* Integers                  */
typedef unsigned char RepBoolean;         /* Booleans                  */
typedef char          RepChar;            /* Characters                */
typedef long double   RepReal;            /* Reals                     */
typedef const char *  RepString;          /* Strings                   */


/* Data types and function result types */

typedef struct Type_tag * Type;

struct Type_tag {
    enum {                               /****** The kind of type *****/
       TYPE_VOID,                        /* Void result               */
       TYPE_INTEGER,                     /* Integers                  */
       TYPE_BOOLEAN,                     /* Booleans                  */
       TYPE_CHAR,                        /* Characters                */
       TYPE_REAL,                        /* Reals                     */
       TYPE_ARRAY,                       /* Arrays of known size      */
       TYPE_IARRAY,                      /* Arrays of unknown size    */
       TYPE_POINTER                      /* Pointers                  */
    } kind;
    Type           refType;              /* Reference type            */
   //  bool           autocompleteSize;     /* Size is unknown           */
    RepInteger     size;                 /* Size, if an array         */
    unsigned int   refCount;             /* Reference counter         */
};


/* Record types in the symbol table */

typedef enum {            
   ENTRY_VARIABLE,                       /* Variables                  */
   ENTRY_CONSTANT,                       /* Constants                  */
   ENTRY_FUNCTION,                       /* Functions                  */
   ENTRY_PARAMETER,                      /* Function parameters        */
   ENTRY_TEMPORARY                       /* Temporary variables        */
} EntryType;


/* Parameter passing types */

typedef enum {            
   PASS_BY_VALUE,                        /* By value                */
   PASS_BY_REFERENCE                     /* By reference            */
} PassMode;


/* Record type in the symbol table */

typedef struct SymbolEntry_tag SymbolEntry;

struct SymbolEntry_tag {
   const char   * id;                 /* Identifier name               */
   EntryType      entryType;          /* Record type                   */
   unsigned int   nestingLevel;       /* Nesting depth                 */
   unsigned int   hashValue;          /* Hash value                    */
   SymbolEntry  * nextHash;           /* Next entry in H.T             */
   SymbolEntry  * nextInScope;        /* Next entry in scope           */

   union {                            /* Depending on the record type: */

      struct {                                /******* Variable  ******/
         Type          type;                  /* Type                 */
         int           offset;                /* Offset in A.R        */
      } eVariable;

      struct {                                /******** Constant *******/
         Type          type;                  /* Type                  */
         union {                              /* Value                 */
            RepInteger vInteger;              /*    Integer            */
            RepBoolean vBoolean;              /*    Boolean            */
            RepChar    vChar;                 /*    Character          */
            RepReal    vReal;                 /*    Real               */
            RepString  vString;               /*    String             */
         } value;
      } eConstant;

      struct {                                /*********** Function ***********/
         bool          isForward;             /* Forward declaration          */
         SymbolEntry * firstArgument;         /* Parameter list               */
         SymbolEntry * lastArgument;          /* Last parameter               */
         Type          resultType;            /* Result type                  */
         enum {                               /* Parameter State              */
             PARDEF_COMPLETE,                 /* Full definition              */
             PARDEF_DEFINE,                   /* In the midst of definition   */
             PARDEF_CHECK                     /* In the midst of checking     */
         } pardef;
         int           firstQuad;             /* First quadruple              */
      } eFunction;

      struct {                                /******* Parameter *******/
         Type          type;                  /* Type                  */
         int           offset;                /* Offset in A.R         */
         PassMode      mode;                  /* Passing mode          */
         SymbolEntry * next;                  /* Next parameter        */
      } eParameter;

      struct {                                /** Temporary variable  **/
         Type          type;                  /* Type                 */
         int           offset;                /* Offset in A.R.       */
         int           number;
      } eTemporary;

   } u;                               /* End of the union            */
};


/* Type of set of records in the same scope */

typedef struct Scope_tag Scope;

struct Scope_tag {
    unsigned int   nestingLevel;             /* Nesting depth           */
    unsigned int   negOffset;                /* Current negative offset */
    Scope        * parent;                   /* Surrounding scope       */
    SymbolEntry  * entries;                  /* Symbols of the scope    */
};


/* Symbol table lookup type */

typedef enum {
    LOOKUP_CURRENT_SCOPE,
    LOOKUP_ALL_SCOPES
} LookupType;


/* ---------------------------------------------------------------------
   --------------- Global variables of the symbol table ----------------
   --------------------------------------------------------------------- */

extern Scope        * currentScope;       /* Current scope                  */
extern unsigned int   quadNext;           /* Number of the next quadruple   */
extern unsigned int   tempNumber;         /* Enumeration of the temporaries */

extern const Type typeVoid;
extern const Type typeInteger;
extern const Type typeBoolean;
extern const Type typeChar;
extern const Type typeReal;


/* ---------------------------------------------------------------------
   ----- Prototypes of the functions for managing the symbol table -----
   --------------------------------------------------------------------- */

void          initSymbolTable    (unsigned int size);
void          destroySymbolTable (void);

void          openScope          (void);
void          closeScope         (void);

SymbolEntry * newVariable        (const char * name, Type type);
SymbolEntry * newConstant        (const char * name, Type type, ...);
SymbolEntry * newFunction        (const char * name);
SymbolEntry * newParameter       (const char * name, Type type,
                                  PassMode mode, SymbolEntry * f);
SymbolEntry * newTemporary       (Type type);

void          forwardFunction    (SymbolEntry * f);
void          endFunctionHeader  (SymbolEntry * f, Type type);
void          destroyEntry       (SymbolEntry * e);
SymbolEntry * lookupEntry        (const char * name, LookupType type,
                                  bool err);

Type          typeArray          (RepInteger size, Type refType);
Type          typeIArray         (Type refType);
Type          typePointer        (Type refType);
void          destroyType        (Type type);
unsigned int  sizeOfType         (Type type);
bool          equalType          (Type type1, Type type2);
void          printType          (Type type);
void          printMode          (PassMode mode);


#endif
