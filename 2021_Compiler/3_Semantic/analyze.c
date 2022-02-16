/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

int EXIT = 0;
char * funName;


void error_typeError(TreeNode * t) {
    fprintf(listing, "Error: Type error at %d\n", t->lineno);
    EXIT = 1;
}

void error_symbolError(TreeNode * t) {
    fprintf(listing, "Error: symbol error at %d\n", t->lineno);
    EXIT = 1;
}

void error_undeclError(TreeNode * t) {
    fprintf(listing, "Error: Undeclared Variable \"%s\" at line %d\n", t->attr.name, t->lineno);
    EXIT = 1;
}

void error_redefError(TreeNode * t) {
    fprintf(listing, "Error: Redefinition error at %d (name : %s)\n", t->lineno, t->attr.name);
    EXIT = 1;
}

void error_funcDefError(TreeNode * t) {
    fprintf(listing, "Error: Function definition error at %d\n", t->lineno);
    EXIT = 1;
}

void error_voidVarError(TreeNode * t, char *name) {
    fprintf(listing, "Error: Vairable Type cannot be Void at line %d (name : %s)\n", t->lineno, name);
    EXIT = 1;
}

void error_voidindexing(TreeNode * t) {
    fprintf(listing, "Error: Invalid array indexing at line %d (name: \"%s\"). indices should be integer", t->lineno, t->attr.name);
    EXIT = 1;
}
/* counter for variable memory locations */
static char current_scope[100];
int scope_stack[100];
int stack_size = -1;


int scope_pop() {
    return scope_stack[stack_size--];
}

void scope_push(int num) {
    scope_stack[++stack_size] = num;
}


/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { 
      preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode * t)
{ 
char *child_name;
switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { case CompK:
          child_name = (char*)malloc(100);
          scope_push(scope_get_index(current_scope));
          sprintf(child_name, "%s_%d", current_scope, scope_get_scope(scope_get_index(current_scope))->n_child);
          scope_create(child_name, LocalS, t->type, scope_get_index(current_scope));
          strcpy(current_scope, child_name);
          t->scope = child_name;
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { case IdK:
        case VarEK:
        case CallK:
            if (st_lookup(current_scope, t->attr.name) == NULL)
                error_undeclError(t);
            else
                st_insert(current_scope, t->attr.name, t, 0, current_scope);
        default:
          break;
      }
      break;
    case DeclK:
      switch (t->kind.decl)
      { case VarK:
          if (st_lookup(current_scope, t->attr.name) == NULL) 
              st_insert(current_scope, t->attr.name, t, 0, current_scope);
          else
              error_redefError(t);
          break;
        case FuncK:
          if (scope_get_index(t->attr.name) == -1) {
              st_insert(current_scope, t->attr.name, t, FuncS, 0);
              scope_create(t->attr.name, FuncS, t->type, scope_get_index(current_scope));
              scope_push(scope_get_index(current_scope));
              strcpy(current_scope, t->attr.name);
          }
          else
              error_redefError(t);
          break;
        case ParamK:
          if (st_lookup(current_scope, t->attr.name) == NULL) 
              st_insert(current_scope, t->attr.name, t, 0, current_scope);
          else
              error_redefError(t);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

static void postInsertNode(TreeNode * t) {
    if (t->nodekind == StmtK && t->kind.stmt == CompK) {
        ScopeList s = scope_get_scope(scope_pop());
        strcpy(current_scope, s->name);
    }
    else if (t->nodekind == DeclK && t->kind.decl == FuncK) {
        ScopeList s = scope_get_scope(scope_pop());
        strcpy(current_scope, s->name);
    }

}

        
/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ 
  scope_init();
  strcpy(current_scope, "global");
  scope_push(0);
  traverse(syntaxTree,insertNode,postInsertNode);
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
}

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

static void preCheckNode(TreeNode * t) {
    if (t->nodekind == DeclK) {
        if (t->kind.decl == FuncK)
            funName = t->attr.name;
    }
    else if (t->nodekind == StmtK)
        if (t->kind.stmt == CompK) {
            scope_push(scope_get_index(current_scope));
            strcpy(current_scope, t->scope);
        }
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t)
{ 
int idx;
 switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { case CompK:
          idx = scope_pop();
          strcpy(current_scope, scope_get_scope(idx)->name);
          break;
        case SelectK:
          if (t->child[0] == NULL)
              typeError(t, "expected expression");
          else if (t->child[0]->type == VoidK)
              typeError(t, "invalid if condition type");
          break;
        case IterK:
          if (t->child[0] == NULL)
              typeError(t, "expected expression");
          else if (t->child[0]->type == VoidK)
              typeError(t, "invalid loop condition type");
          break;
        case ReturnK:
          { TreeNode * temp = st_lookup(current_scope, funName)->t;
            int cond1 = t->child[0] != NULL && temp->type == VoidK;
            int cond2 = temp->type == IntK;
            int cond3 = cond2 && (t->child[0] == NULL || t->child[0]->type == VoidK || t->child[0]->type == IntArrK);
                
            if ( cond1 || cond3 )
                typeError(t, "invalid return type");
          break; }
        default:
          break;
      }
    case ExpK:
      switch (t->kind.exp)
      { 
        case AssignK:
          if (t->nodekind != ExpK)
              break;
          if (t->child[0]->type == VoidK || t->child[1]->type == VoidK)
              typeError(t->child[0], "invalid variable type");
          else if (t->child[0]->type == IntArrK && t->child[0]->child[0] == NULL)
              typeError(t->child[0], "invalid variable type");
          else if (t->child[1]->type == IntArrK && t->child[1]->child[0] == NULL)
              typeError(t->child[0], "invalid variable type");
          else
              t->type = t->child[0]->type;
          break;
        case OpK:
          { ExpType l, r;
            TokenType op;

            l = t->child[0]->type;
            r = t->child[1]->type;
            op = t->attr.op;
        
        if (l == VoidK || r == VoidK)
            typeError(t, "void variables can't be operand");
        else if (r != l)
            typeError(t, "invalid expression");
        else
            t->type = IntK;
          break; }
        case ConstK:
          t->type = IntK;
          break;
        case IdK:
        case VarEK:
          { BucketList b = st_lookup(current_scope, t->attr.name);
            if (b == NULL) {
                error_undeclError(t);
                break;
            }

            TreeNode * temp = b->t;

            /* array */
            if (t->child[0] != NULL) {
                if (t->child[0]->type != IntK) {
                    t->type = temp->type;
                    error_voidindexing(t);
                    break;}

                if ((temp->nodekind == DeclK && temp->type != IntArrK) ||
                    (temp->nodekind == DeclK && temp->kind.decl == ParamK && temp->type != IntArrK))
                    typeError(t, "invalid expression");
                else
                    t->type = IntK;
            }
            else
                t->type = temp->type;
          break; }
        case CallK:
          { BucketList b = st_lookup(current_scope, t->attr.name);
              TreeNode * fun, * arg, * param;
              if (b == NULL)
                  break;
              fun = b->t;
              param = fun->child[0];
              arg = t->child[0];

              if (fun->kind.decl != FuncK) {
                  typeError(t, "invalid expression");
                  break;
              }

              while (arg != NULL) {
                  if (param == NULL || arg->type == VoidK) {
                      typeError(arg, "invalid function call arguments");
                      break;
                  }
                  ExpType paramT = param->type;
                  ExpType argT = arg->type;
                  if (paramT != argT) {
                      typeError(arg, "invalid function call(Type mismatch)");
                      break;
                  }
                  else {
                      arg = arg->sibling;
                      param = param->sibling;
                  }
              }
              if (arg == NULL && param != NULL &&
                  param->child[0]->type != VoidK)
                  typeError(t->child[0], "invalid function call parameters");

              t->type = fun->type;
              break;
          }
        default:
          break;
      }
    case DeclK:
      switch (t->kind.decl)
      { case VarK:
          if (t->type == VoidK) 
              error_voidVarError(t, t->attr.name);
          break;
        default:
          break;
      }
    default:
      break;
  }
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree)
{   scope_push(0); /* global scope */
    traverse(syntaxTree,preCheckNode,checkNode);
}
