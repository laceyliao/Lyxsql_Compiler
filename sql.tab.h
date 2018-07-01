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

#ifndef YY_YY_SQL_TAB_H_INCLUDED
# define YY_YY_SQL_TAB_H_INCLUDED
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
    CREATE = 258,
    TABLE = 259,
    TABLES = 260,
    DATABASE = 261,
    DATABASES = 262,
    INSERT = 263,
    SELECT = 264,
    UPDATE = 265,
    DELETE = 266,
    DROP = 267,
    EXIT = 268,
    AND = 269,
    OR = 270,
    FROM = 271,
    WHERE = 272,
    VALUES = 273,
    INTO = 274,
    SET = 275,
    SHOW = 276,
    QUOTE = 277,
    USE = 278,
    ID = 279,
    CHAR = 280,
    NUMBER = 281,
    INT = 282
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 1384 "sql.y" /* yacc.c:1909  */

	char *yych; //字面量
	char op;
	struct Createfieldsdef *cfdef_var; //字段定义
	struct Createstruct *cs_var; //整个create语句

	struct Selectedfields *sf_var; //所选字段
	struct Selectedtables *st_var; //所选表格
	struct Conditions *cons_var; //条件语句
	struct Selectstruct *ss_var; //整个select语句

	struct Setstruct *s_var; //Update set
	
	struct insertValue *is_val; //Insert Value

#line 98 "sql.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SQL_TAB_H_INCLUDED  */
