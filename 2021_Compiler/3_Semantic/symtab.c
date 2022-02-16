/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "util.h"


/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}


/* the hash table */
static BucketList hashTable[SIZE];
int n_scope;
static ScopeList scopeArr[SIZE];
static int location[SIZE];


/* Create scope */
int scope_create( char * name, ScopType stype, ExpType type, int parent) {
    int i;
    ScopeList s = (ScopeList)malloc(sizeof(struct ScopeListRec));
    s->name = name;
    s->parent = parent;
    s->stype = stype;
    s->type = type;
    if (parent == -1)
        s->level = 0;
    else
        s->level = scopeArr[parent]->level + 1;
    s->n_bucket = 0;
    s->n_child = 0;
    for (i = 0; i < SIZE; ++i) {
        s->bucket[i] = NULL;
        s->child[i] = -1;
    }
    scopeArr[n_scope] = s;
    /* insert to parent */
    if (parent != -1) {
        s = scopeArr[parent];
        s->child[s->n_child++] = n_scope;
    }
    n_scope++;
    return n_scope - 1;
}

void scope_init() {
    char * glob = "global";
    char * out = "output";
    char * in = "input";
    TreeNode * t_out = newDeclNode(FuncK);
    TreeNode * t_in = newDeclNode(FuncK);
    TreeNode * t_out_param = newDeclNode(ParamK);

    t_out_param->type = IntK;
    t_out->child[0] = t_out_param;
    t_out->lineno = 0;

    t_in->type = IntK;
    t_in->lineno = 0;
    scope_create(glob, GlobS, 0, -1);
    st_insert(glob, out, t_out, FuncS, 0);
    st_insert(glob, in, t_in, FuncS, 0);
    scope_create(out, FuncS, IntK, 0);
    scope_create(in, FuncS, VoidK, 0);
    st_insert(out, "value", t_out_param, FuncS, 0);
}

ScopeList scope_get_scope(int idx) {
    return scopeArr[idx];
}

int scope_get_index( char * name ) {
    int i;
    int iter = -1;
    for (i = 0; i < n_scope; ++i) {
        if (!strcmp(scopeArr[i]->name, name)) {
            iter = i;
            break;
        }
    }
    return iter;
}
    

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * scope, char * name, TreeNode * t, ScopType stype, char * parent )
{
  int i;
  BucketList l = NULL;
  ScopeList s;
  int idx = scope_get_index(scope);
  if (idx == -1) 
      idx = scope_create(scope, stype, t->type, scope_get_index(parent));
  s = scopeArr[idx];

  /* find bucket */
  while (1) {
      if (s == 0)
          break;
      for (i = 0; i < s->n_bucket; ++i)
          if (s->bucket[i]->name != NULL &&\
              name != NULL && !strcmp(name, s->bucket[i]->name)) {
              l = s->bucket[i];
              break;
          }
      if (s->parent == -1)
          break;
      else
          s = scopeArr[s->parent];
  }
  if (l == NULL) {
      s = scopeArr[idx];
      l = (BucketList) malloc(sizeof(struct BucketListRec));
      l->name = name;
      l->lines = (LineList) malloc(sizeof(struct LineListRec));
      l->lines->lineno = t->lineno;
      l->memloc = location[idx]++;
      l->lines->next = NULL;
      l->type = t->type;
      l->t = t;
      if (t->nodekind == DeclK)
          l->dtype = t->kind.decl;
      else
          l->dtype = -1;
      s->bucket[s->n_bucket++] = l;
  }
  else /* found in table, so just add line number */
  { LineList iter = l->lines;
    while (iter->next != NULL) iter = iter->next;
    iter->next = (LineList) malloc(sizeof(struct LineListRec));
    iter->next->lineno = t->lineno;
    iter->next->next = NULL;
  }
} /* st_insert */

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
BucketList st_lookup ( char * scope, char * name )
{ 
    int idx = scope_get_index(scope);
    int i;
    ScopeList s;
    if (idx == -1) 
        return NULL;
    else {
        s = scopeArr[idx];
        while (1) {
            if (s == 0)
                break;
            for (i = 0; i < s->n_bucket; ++i)
                if (s->bucket[i]->name != NULL &&\
                    name != NULL && !strcmp(name, s->bucket[i]->name)) {


                    return s->bucket[i];
                }
            if (s->parent == -1)
                break;
            else
                s = scopeArr[s->parent];
        }
        return NULL;
    }
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing)
{
  int i, j, k;
  fprintf(listing,"< Symbol Table >\n");
  fprintf(listing,"Variable Name  Variable Type  Scope Name  Location  Line Numbers\n");
  fprintf(listing,"-------------  -------------  ----------  --------  ------------\n");
  
  for (i = 0; i < n_scope; ++i) {
      ScopeList s = scopeArr[i];
      
      for (j = 0; j < s->n_bucket; ++j) {
          BucketList b = s->bucket[j];
          /* name */
          fprintf(listing, "%-15s", b->name);
          /* type */
          if (b->dtype == VarK || b->dtype == ParamK) {
              if (b->type == IntK)
                  fprintf(listing, "%-15s", "Integer");
              else if (b->type == IntArrK)
                  fprintf(listing, "%-15s", "IntegerArray");
              else if (b->type == VoidK)
                  fprintf(listing, "%-15s", "Void");
          }
          else
              fprintf(listing, "%-15s", "Function");
          char string[100];
          strcpy(string, s->name);
          char *token = strtok(string, "_");
          fprintf(listing, "%-12s", token);
          fprintf(listing, "%-10d", b->memloc);
          LineList l = b->lines;
          while (l != NULL) {
              fprintf(listing, "%4d", l->lineno);
              l = l->next;
          }
          fprintf(listing, "\n");
          
      }
  }

  fprintf(listing,"\n\n< Function Table >\n");
  fprintf(listing,"Function Name  Scope Name  Return Type  Parameter Name  Parameter Type\n");
  fprintf(listing,"-------------  ----------  -----------  --------------  --------------\n");
  for (i = 0; i < 1; ++i) {
      ScopeList s = scopeArr[i];
      
      for (j = 0; j < s->n_bucket; ++j) {
          BucketList b = s->bucket[j];
          /* name */
          /* type */
          if (b->dtype == FuncK) {
              fprintf(listing, "%-15s", b->name);
              fprintf(listing, "%-12s", s->name);
              if (b->type == IntK)
                  fprintf(listing, "%-13s", "Integer");
              else if (b->type == IntArrK)
                  fprintf(listing, "%-13s", "IntegerArray");
              else if (b->type == VoidK)
                  fprintf(listing, "%-13s", "Void");
          }
          else
              continue;
          ScopeList s_child = scopeArr[scope_get_index(b->name)];
          if (s_child->n_bucket == 1) {
              BucketList b_child = s_child->bucket[0];
              if (b_child->dtype == ParamK) {
                  if (b_child->type == VoidK) {
                      fprintf(listing, "%-16s", "");
                      fprintf(listing, "%-14s", "Void");
                  }
                  else if (b_child->type == IntK) {
                      fprintf(listing, "%-16s", b_child->name);
                      fprintf(listing, "%-14s", "Integer");
                  }
                  else if (b_child->type == IntArrK) {
                      fprintf(listing, "%-16s", b_child->name);
                      fprintf(listing, "%-14s", "IntegerArray");
                  }
              }
          }
          else {
              for (k = 0; k < s_child->n_bucket; ++k) {
                  BucketList b_child = s_child->bucket[k];
                  if (b_child->dtype == ParamK) {
                      fprintf(listing, "\n");
                      fprintf(listing, "%-40s", "");
                      if (b_child->type == VoidK) {
                      fprintf(listing, "%-16s", "");
                      fprintf(listing, "%-14s", "Void");
                      }
                      else if (b_child->type == IntK) {
                      fprintf(listing, "%-16s", b_child->name);
                      fprintf(listing, "%-14s", "Integer");
                      }
                      else if (b_child->type == IntArrK) {
                      fprintf(listing, "%-16s", b_child->name);
                      fprintf(listing, "%-14s", "IntegerArray");
                      }
                  }
              }
          }

          fprintf(listing, "\n");
      }
  }
  fprintf(listing,"\n\n< Function and Global Variables >\n");
  fprintf(listing,"   ID Name      ID Type    Data Type \n");
  fprintf(listing,"-------------  ---------  -----------\n");
  for (i = 0; i < 1; ++i) {
      ScopeList s = scopeArr[i];
      
      for (j = 0; j < s->n_bucket; ++j) {
          BucketList b = s->bucket[j];
          fprintf(listing, "%-15s", b->name);
          if (b->dtype == FuncK) 
              fprintf(listing, "%-11s", "Function");
          else if (b->dtype == VarK) 
              fprintf(listing, "%-11s", "Variable");

          if (b->type == IntK)
              fprintf(listing, "%-13s", "Integer");
          else if (b->type == IntArrK)
              fprintf(listing, "%-13s", "IntegerArray");
          else if (b->type == VoidK)
              fprintf(listing, "%-13s", "Void");
        fprintf(listing, "\n");
      }
  }
  
  fprintf(listing,"\n\n< Function Parameter and Local Variables >\n");
  fprintf(listing,"  Scope Name    Nested Level     ID Name      Data Type \n");
  fprintf(listing,"--------------  ------------  -------------  -----------\n");
  for (i = 0; i < n_scope; ++i) {
      ScopeList s = scopeArr[i];
      if (s->stype == GlobS)
          continue;
      
      for (j = 0; j < s->n_bucket; ++j) {
          BucketList b = s->bucket[j];
          if (b->dtype == VarK || b->dtype == ParamK) {
              char string[100];
              strcpy(string, s->name);
              char *token = strtok(string, "_");
              fprintf(listing, "%-16s", token);
              fprintf(listing, "%-14d", s->level);
              fprintf(listing, "%-15s", b->name);
              if (b->type == IntK)
                  fprintf(listing, "%-11s", "Integer");
              else if (b->type == IntArrK)
                  fprintf(listing, "%-11s", "IntegerArray");
              else if (b->type == VoidK)
                  fprintf(listing, "%-11s", "Void");
          }
        fprintf(listing, "\n");
      }
    fprintf(listing, "\n");
  }
} /* printSymTab */
