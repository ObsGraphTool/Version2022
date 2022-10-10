/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IN = 258,
     OUT = 259,
     MK = 260,
     ID = 261,
     PLACE = 262,
     ARC = 263,
     FARC = 264,
     TRANS = 265,
     NOEUD = 266,
     FNOEUD = 267,
     NAME = 268,
     RL = 269,
     ATT = 270,
     FATT = 271,
     ARCTYPE = 272,
     FINBALISE = 273,
     MODEL = 274,
     FMODEL = 275,
     VALUATION = 276,
     XML = 277,
     CORD = 278,
     OBS = 279,
     FINALMARKING = 280,
     MULT = 281,
     PLUS = 282,
     ENTIER = 283,
     VARIABLE = 284
   };
#endif
/* Tokens.  */
#define IN 258
#define OUT 259
#define MK 260
#define ID 261
#define PLACE 262
#define ARC 263
#define FARC 264
#define TRANS 265
#define NOEUD 266
#define FNOEUD 267
#define NAME 268
#define RL 269
#define ATT 270
#define FATT 271
#define ARCTYPE 272
#define FINBALISE 273
#define MODEL 274
#define FMODEL 275
#define VALUATION 276
#define XML 277
#define CORD 278
#define OBS 279
#define FINALMARKING 280
#define MULT 281
#define PLUS 282
#define ENTIER 283
#define VARIABLE 284




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 39 "RdPMonteur.y"
{ 	
  int i;
  char *s;
}
/* Line 1529 of yacc.c.  */
#line 112 "RdPMonteur.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

