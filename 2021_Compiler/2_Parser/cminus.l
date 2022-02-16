/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for TINY                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  [a-zA-Z][a-zA-Z0-9]*
newline     \n
whitespace  [ \t]+

%%

"int"           {return INT;}
"void"          {return VOID;}
"if"            {return IF;}
"else"          {return ELSE;}
"while"         {return WHILE;}
"return"        {return RETURN;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"/*"            { char c1, c2;
                  c1 = input();

                  while (1){

                    while (c1 != '*')
                    { c2 = c1;
                      c1 = input();
                      if (c1 == EOF) break;
                      if (c1 == '\n') lineno++;
                    }

                    c2 = c1;
                    c1 = input();
                    if (c2 == '*' && c1 == '/') break;
                    if (c1 == EOF) break;
                    if (c1 == '\n') lineno++;

                  }
                }
"<"             {return LT;}
"<="            {return LE;}
">"             {return GT;}
">="            {return GE;}
"=="            {return EQ;}
"!="            {return NE;}
"="             {return ASSIGN;}
";"             {return SEMI;}
","             {return COMMA;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LBRACKET;}
"]"             {return RBRACKET;}
"{"             {return LCURLY;}
"}"             {return RCURLY;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

