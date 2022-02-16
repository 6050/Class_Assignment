/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NOTELSE = 258,
    ELSE = 259,
    IF = 260,
    INT = 261,
    RETURN = 262,
    VOID = 263,
    WHILE = 264,
    ID = 265,
    NUM = 266,
    EQ = 267,
    NE = 268,
    LT = 269,
    LE = 270,
    GT = 271,
    GE = 272,
    SEMI = 273,
    COMMA = 274,
    LPAREN = 275,
    RPAREN = 276,
    LBRACKET = 277,
    RBRACKET = 278,
    LCURLY = 279,
    RCURLY = 280,
    ASSIGN = 281,
    PLUS = 282,
    MINUS = 283,
    TIMES = 284,
    OVER = 285,
    ERROR = 286
  };
#endif
/* Tokens.  */
#define NOTELSE 258
#define ELSE 259
#define IF 260
#define INT 261
#define RETURN 262
#define VOID 263
#define WHILE 264
#define ID 265
#define NUM 266
#define EQ 267
#define NE 268
#define LT 269
#define LE 270
#define GT 271
#define GE 272
#define SEMI 273
#define COMMA 274
#define LPAREN 275
#define RPAREN 276
#define LBRACKET 277
#define RBRACKET 278
#define LCURLY 279
#define RCURLY 280
#define ASSIGN 281
#define PLUS 282
#define MINUS 283
#define TIMES 284
#define OVER 285
#define ERROR 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
