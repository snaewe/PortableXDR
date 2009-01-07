/* rpcgen - Generate XDR bindings automatically.    -*- text -*-
 * Copyright (C) 2008 Red Hat Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

%{
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include "rpcgen_int.h"

extern void yyerror (const char *str);
%}

%union {
  char *str;
}

%type <str> const

%token STRUCT
%token ENUM
%token CONST
%token TYPEDEF
%token UNION
%token SWITCH
%token CASE
%token DEFAULT
%token PROGRAM

%token UNSIGNED
%token SIGNED
%token CHAR
%token SHORT
%token INT
%token HYPER
%token DOUBLE
%token STRING
%token OPAQUE

/* This is sometimes lumped together with the other types, but
 * the special keyword void can only occur after "default:" in
 * union statements.
 */
%token VOID

%token <str> IDENT
%token <str> INTLIT
%token <str> STRLIT

%%

file	: /* empty */
	| stmts
	;

/* Statements. */
stmts	: stmt ';'
	| stmts stmt ';'
	;

stmt	: ENUM IDENT '{' enum_values '}'
	| STRUCT IDENT '{' decls '}'
	| UNION IDENT SWITCH '(' decl ')' '{' union_cases '}'
	| TYPEDEF decl
	| CONST IDENT '=' const
	| PROGRAM
	;

/* Declarations used inside structs and unions.  eg. "int foo;" */
decls	: decl ';'
	| decls decl ';'
	;

decl	: simple_decl
	| fixed_array_decl
	| variable_array_decl
	| pointer_decl
	;

simple_decl
	: type_ident IDENT
	;

fixed_array_decl
	: type_ident IDENT '[' const ']'
	;

variable_array_decl
	: type_ident IDENT '<' const '>'
	| type_ident IDENT '<' '>'
	;

pointer_decl
	: type_ident '*' IDENT
	;

/* Enumerations. */
enum_values
	: enum_value
	| enum_values ',' enum_value
	;

enum_value
	: IDENT
	| IDENT '=' const
	;

/* Case list inside a union. */
union_cases
	: union_case ';'
	| union_cases union_case ';'
	;

union_case
	: CASE const ':' decl
	| DEFAULT ':' VOID
	| DEFAULT ':' decl
	;

/* Constants, which may be integer literals or refer to previously
 * defined constants (using "const" keyword).
 * XXX In future we should probably allow computed constants.
 */
const	: INTLIT
	| IDENT
	;

/* Types. */
type_ident
	: CHAR
	| SIGNED CHAR
	| UNSIGNED CHAR
	| SHORT
	| SIGNED SHORT
	| UNSIGNED SHORT
	| INT
	| SIGNED INT
	| UNSIGNED INT
	| HYPER
	| SIGNED HYPER
	| UNSIGNED HYPER
	| SIGNED
	| UNSIGNED
	| DOUBLE
	| STRING
	| OPAQUE
	| IDENT
	;

%%

void
yyerror (const char *str)
{
  error ("%s", str);
}
