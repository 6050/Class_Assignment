/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "globals.h"
/* SIZE is the size of the hash table */
#define SIZE 211

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
   { char * name;
     ExpType type;
     DeclKind dtype;
     LineList lines;
     int memloc ; /* memory location for variable */
     struct BucketListRec * next;
     TreeNode * t;
   } * BucketList;



/* The record for each scope,
 * including name, its bucket,
 * and parent scope.
 */
typedef struct ScopeListRec
   { char * name;
     BucketList bucket[SIZE];
     ScopType stype;
     ExpType type;
     int parent;
     int child[SIZE];
     int level;
     int n_bucket;
     int n_child;
   } * ScopeList;


void scope_init();
int scope_create( char * name, ScopType stype, ExpType type, int parent);
int scope_get_index( char * name );
ScopeList scope_get_scope(int idx);
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * scope, char * name, TreeNode * t, ScopType stype, char * parent );

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
BucketList st_lookup ( char * scope, char * name );

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);

#endif
