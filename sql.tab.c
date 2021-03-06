/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "sql.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

char database[64]={0};
char rootDir[128]={0};

int yyparse(void);
int yylex(void);
struct Createfieldsdef{//create语句中的字段定义
	char *field; //字段名称
	char *type; //字段类型
	int length; //字段长度
	struct Createfieldsdef *next_fdef; //下一字段
};

struct Createstruct{ //	create语法树根节点
	char *table;  //基本表名称
	struct Createfieldsdef *fdef; //字段定义
};

struct	Conditions{/*条件*/
	struct  Conditions	*left;		//左部条件
	struct  Conditions	*right;		//右部条件
	char comp_op;	/* 'a'是and, 'o'是or, '<' , '>' , '=', ‘!='  */
	int type; /* 0是字段，1是字符串，2是整数 */
	char *value;// 根据type存放字段名、字符串或整数 	
	char *table;	/* NULL或表名 */	
};

struct	Selectedfields{/*select语句中选中的字段*/
	char 	*table;			//字段所属表
	char 	*field;			//字段名称
	struct 	Selectedfields	*next_sf;//下一个字段
};

struct	Selectedtables{	/*select语句中选中的表*/
	char	  *table;		//基本表名称
	struct  Selectedtables  *next_st;	//下一个表
};
struct	Selectstruct{		/*select语法树的根节点*/
	struct Selectedfields 	*sf;	//所选字段
	struct Selectedtables	*st;	//所选基本表
	struct Conditions	*cons;	//条件
};

struct insertValue {//insert语句中的字段定义
	char *value;//字段值
	struct insertValue *nextValue;//下一字段
};

struct Setstruct{//set语句中的字段定义
	char *field; //所选字段
	char *value; //值
	struct Setstruct *next_s; //下一字段
};

void getDB(){
	FILE* fstream;
    char name[40];

    chdir(rootDir);
    fstream = fopen(".dbinfo", "r");
    if(fstream == NULL)
    {
        printf("\nError!\n");
        return;
    }
    while(fscanf(fstream, "%s", name) != EOF)
    {
        printf("%s\n", name);
    }
    fclose(fstream);
    chdir(rootDir);
	//printf("get db\n");
	printf("SQL>");
}
void useDB(){
	char dir[128]={0};
    strcpy(dir, rootDir);
    strcat(dir, "/");
    strcat(dir, database);
    if(chdir(dir) == -1)
        printf("\nError!\n");
    else
    {
        printf("Current Database: \n%s\n", database);
        chdir(rootDir);
    }
	//printf("use db\n");
	printf("SQL>");
}
void createDB(){
	chdir(rootDir);
    if(mkdir(database, S_IRUSR | S_IWUSR | S_IXUSR) == -1)
        printf("\nError!\n");
    else
    {
        FILE* fstream;
        fstream = fopen(".dbinfo", "a+");
        if(fstream == NULL)
        {
            printf("\nError!\n");
            return;
        }
        else
        {
            fprintf(fstream, "%s\n", database);
            printf("\nCreate database %s successfully!\n", database);
            fflush(fstream);
            fclose(fstream);
        }
    }
    strcpy(database, "\0");
    chdir(rootDir);
	//printf("create db\n");
	printf("SQL>");
}
void dropDB(){
	chdir(rootDir);
    if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        char cmd[128]="rm -rf ";
        FILE* filein;
        FILE* fileout;
        char dbname[64] = {0};

        chdir("..");
        strcat(cmd, database);
        system(cmd);

        system("mv .dbinfo .dbinfo.tmp");
        filein = fopen(".dbinfo.tmp", "r");
        fileout = fopen(".dbinfo", "w");

        while(fscanf(filein, "%s", dbname) != EOF)
        {
            if(strcmp(dbname, database) != 0)
            {
                fprintf(fileout, "%s\n", dbname);
            }
        }
        fclose(filein);
        fclose(fileout);
        system("rm .dbinfo.tmp");
    }
    chdir(rootDir);
    printf("Drop database %s successfully.\n", database);
	//printf("drop table\n");
	printf("SQL>");
}

void createTable(struct Createstruct *cs_root){
	int tot = 0, i = 0;
    struct Createfieldsdef *fieldPointer = NULL;
    char rows[64][64]={0};

    chdir(rootDir);
    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        FILE* ftables;
        ftables = fopen(".tables", "a+");
        if(ftables == NULL)
        {
            printf("\nError!\n");
        }
        else
        {
            if(access(cs_root->table, F_OK) != -1)
            {
                printf("Table already exist!\n");
            }
            else {
                fprintf(ftables, "%s\n", cs_root->table);
                fclose(ftables);
                fieldPointer = cs_root->fdef;
                FILE* ftable;
                ftable = fopen(cs_root->table, "a+");
                if(ftable == NULL)
                {
                    printf("\nError!\n");
                }
                else {
                    while(fieldPointer != NULL)
                    {
                        strcpy(rows[tot], fieldPointer->field);
                        tot ++;
                        fieldPointer = fieldPointer->next_fdef;
                    }
                    fprintf(ftable, "%d\n", tot);
                    for(i = tot - 1; i >= 0; i--)
                        fprintf(ftable, "%s\n", rows[i]);
                    printf("\nCreate table %s successfully, %d row(s) created.\n", cs_root->table, tot);
                    fclose(ftable);
                }
            }
            chdir(rootDir);
        }
    }

    fieldPointer = cs_root->fdef;
    while(fieldPointer != NULL)
    {
        struct Createfieldsdef * fieldPointertmp = fieldPointer;
        fieldPointer = fieldPointer->next_fdef;
        free(fieldPointertmp);
    }
    free(cs_root);
    chdir(rootDir);
	//printf("create table\n");
	printf("SQL>");
}
void getTable(){
	chdir(rootDir);
    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        FILE* fstream;
        char name[40];

        fstream = fopen(".tables", "a+");
        if(fstream == NULL)
        {
            printf("\nError!\n");
            return;
        }
        while(fscanf(fstream, "%s", name) != EOF)
        {
            printf("%s\n", name);
        }
        fclose(fstream);
    }
    chdir(rootDir);
	//printf("get table\n");
	printf("SQL>");
}
void dropTable(char * tableName){
	chdir(rootDir);
    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        if(access(tableName, F_OK) != -1)
        {
            char cmd[128]="rm -rf ";
            char tbname[64] = {0};
            FILE* filein;
            FILE* fileout;

            strcat(cmd, tableName);
            system(cmd);

            system("mv .tables .tables.tmp");
            filein = fopen(".tables.tmp", "r");
            fileout = fopen(".tables", "w");

            while(fscanf(filein, "%s", tbname) != EOF)
            {
                if(strcmp(tbname, tableName) != 0)
                {
                    fprintf(fileout, "%s\n", tbname);
                }
            }
            fclose(filein);
            fclose(fileout);
            system("rm .tables.tmp");
        }
        else
            printf("Table %s doesn't exist!\n", tableName);
    }
    chdir(rootDir);
    printf("Drop table %s successfully.\n", tableName);
	//printf("drop table\n");
	printf("SQL>");
}
void insertSingle(char * tableName, struct insertValue* values)
{
	chdir(rootDir);
    char valueChar[64][64] = {0};
    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        if(access(tableName, F_OK) != -1)
        {
            FILE* fileTable;
            int cnt = 0, i = 0;
            fileTable = fopen(tableName, "a+");
            struct insertValue* valuesTmp = values;
            while(valuesTmp != NULL)
            {
                strcpy(valueChar[cnt], valuesTmp->value);
                cnt ++;
                valuesTmp = valuesTmp->nextValue;
            }
            for (i = cnt-1; i >= 0; i--)
            {
                fprintf(fileTable, "%s\n", valueChar[i]);
            }
            fclose(fileTable);
            printf("Insert succeed.\n");
        }
        else
            printf("Table %s doesn't exist!\n", tableName);
    }
    while(values != NULL)
    {
        struct insertValue* valuesTmp = values;
        values = values->nextValue;
        free(valuesTmp);
    }
    chdir(rootDir);
	//printf("insert Single\n");
	printf("SQL>");
}
void insertDouble(char * tableName, struct insertValue* rowNames, struct insertValue* valueNames)
{
	chdir(rootDir);
    char rows[64][64] = {0};
    char insertRow[64][64] = {0}, insertValue[64][64] = {0};
    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        if(access(tableName, F_OK) != -1)
        {
            FILE* fileTable;
            fileTable = fopen(tableName, "at+");
            int cnt = 0, i = 0, j = 0, totRow = 0, totValue = 0, flag = 0;
            struct insertValue* valuesTmp = rowNames;

            fscanf(fileTable, "%d", &cnt);
            for (i = 0; i < cnt; ++i)
            {
                fscanf(fileTable, "%s", rows[i]);
            }
            while(valuesTmp != NULL)
            {
                strcpy(insertRow[totRow], valuesTmp->value);
                totRow++;
                valuesTmp = valuesTmp->nextValue;
            }
            valuesTmp = valueNames;
            while(valuesTmp != NULL)
            {
                strcpy(insertValue[totValue], valuesTmp->value);
                totValue++;
                valuesTmp = valuesTmp->nextValue;
            }
            if (totRow != totValue || totRow != cnt)
            {
                printf("Rows and values don't match!\n");
            }
            else
            {
                for (i = 0; i < cnt; ++i)
                {
                    flag = 0;
                    for (j = 0; j < cnt; ++j)
                    {
                        if (strcmp(rows[i], insertRow[j]) == 0)
                        {
                            fprintf(fileTable, "%s\n", insertValue[j]);
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)
                    {
                        printf("Error, row name doesn't match!\n");
                    }
                }
                printf("Insert succeed.\n");
            }
            fclose(fileTable);
        }
        else
            printf("Table %s doesn't exist!\n", tableName);
    }
    while(rowNames != NULL)
    {
        struct insertValue* valuesTmp = rowNames;
        rowNames = rowNames->nextValue;
        free(valuesTmp);
    }
    while(valueNames != NULL)
    {
        struct insertValue* valuesTmp = valueNames;
        valueNames = valueNames->nextValue;
        free(valuesTmp);
    }
    chdir(rootDir);
	//printf("insert double\n");
	printf("SQL>");
}
void deleteAll(char * tableName){
	chdir(rootDir);
    char rows[64][64] = {0};
    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        if(access(tableName, F_OK) != -1)
        {
            char cmd[128]="rm -rf ";
            FILE* filein;
            FILE* fileout;
            int cnt = 0, i = 0;

            filein = fopen(tableName, "r");
            fscanf(filein, "%d", &cnt);
            for (i = 0; i < cnt; ++i)
            {
                fscanf(filein, "%s", rows[i]);
            }

            fclose(filein);
            strcat(cmd, tableName);
            system(cmd);

            fileout = fopen(tableName, "a+");
            fprintf(fileout, "%d\n", cnt);
            for (i = 0; i < cnt; ++i)
            {
                fprintf(fileout, "%s\n", rows[i]);
            }
            fclose(fileout);
            printf("Delete successfully.\n");
        }
        else
            printf("Table %s doesn't exist!\n", tableName);
    }
	//printf("delete all\n");
	chdir(rootDir);
	printf("SQL>");
}
void selectNoWhere(struct Selectedfields *fieldRoot, struct Selectedtables *tableRoot){
	int totTable = 0, totField = 0, i = 0, j = 0, k = 0;
    char tableName[64][64] = {0}, fieldName[64][64] = {0};
    struct Selectedfields *fieldTmp = fieldRoot;
    struct Selectedtables *tableTmp = tableRoot;

    chdir(rootDir);

    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        while(tableTmp != NULL)
        {
            strcpy(tableName[totTable], tableTmp->table);
            tableTmp = tableTmp->next_st;
            totTable ++;
        }
        if (fieldRoot == NULL)
        {
            int flag = 1;
            for (i = totTable-1; i >= 0; --i)
            {
                if(access(tableName[i], F_OK) == -1)
                {
                    printf("Table %s doesn't exist!\n", tableName[i]);
                    flag = 0;
                    break;
                }
            }
            if (flag && totTable == 1)
            {
                FILE* filein;
                int tot = 0, i = 0;
                char value[64];

                filein = fopen(tableName[0], "r");
                fscanf(filein, "%d", &tot);
                i = tot;
                while(fscanf(filein, "%s", value) != EOF)
                {
                    printf("%*s", 16, value);
                    i--;
                    if (i == 0)
                    {
                        i = tot;
                        printf("\n");
                    }
                }
                fclose(filein);
                printf("Select succeed.\n");
            }
            else if (flag && totTable == 2)
            {
                FILE * fileTable[64];
                FILE * fileTmp;
                int fieldCount[2] = {0}, valueCount[2] = {0};
                char values1[64][64] = {0}, values2[64][64] = {0};
                char tmp[64];
                strcpy(tmp, tableName[0]);
                strcpy(tableName[0], tableName[1]);
                strcpy(tableName[1], tmp);
                fileTmp = fopen(".tmp", "w");
                for (i = 0; i < totTable; ++i)
                {
                    fileTable[i] = fopen(tableName[i], "r");
                }
                totField = 0;
                for (i = 0; i < totTable; ++i)
                {
                    fscanf(fileTable[i], "%d", &fieldCount[i]);
                    totField += fieldCount[i];
                }

                fprintf(fileTmp, "%d\n", totField);
                for (i = 0; i < fieldCount[0]; ++i)
                {
                    fscanf(fileTable[0], "%s", tmp);
                    fprintf(fileTmp, "%s\n", tmp);
                }
                for (i = 0; i < fieldCount[1]; ++i)
                {
                    fscanf(fileTable[1], "%s", tmp);
                    fprintf(fileTmp, "%s\n", tmp);
                }

                while(fscanf(fileTable[0], "%s", tmp) != EOF)
                {
                    strcpy(values1[valueCount[0]], tmp);
                    valueCount[0]++;
                }
                while(fscanf(fileTable[1], "%s", tmp) != EOF)
                {
                    strcpy(values2[valueCount[1]], tmp);
                    valueCount[1]++;
                }

                for (i = 0; i < valueCount[0]/fieldCount[0]; ++i)
                {
                    for (k = 0; k < valueCount[1]/fieldCount[1]; ++k)
                    {
                        for (j = 0; j < fieldCount[0]; ++j)
                        {
                            fprintf(fileTmp, "%s\n", values1[i*fieldCount[0]+j]);
                        }
                        for (j = 0; j < fieldCount[1]; ++j)
                        {
                            fprintf(fileTmp, "%s\n", values2[k*fieldCount[1]+j]);
                        }
                    }
                }
                fclose(fileTmp);
                fclose(fileTable[0]);
                fclose(fileTable[1]);
                fileTmp = fopen(".tmp", "r");
                i = totField;
                fscanf(fileTmp, "%d", &totField);
                while(fscanf(fileTmp, "%s", tmp) != EOF)
                {
                    printf("%*s", 16, tmp);
                    i--;
                    if (i == 0)
                    {
                        i = totField;
                        printf("\n");
                    }
                }
                fclose(fileTmp);
                system("rm -rf .tmp");
                printf("Select succeed.\n");
            }
        }
        else
        {
            int flag = 1;
            char allField[64][64] = {0};
            for (i = totTable-1; i >= 0; --i)
            {
                if(access(tableName[i], F_OK) == -1)
                {
                    printf("Table %s doesn't exist!\n", tableName[i]);
                    flag = 0;
                    break;
                }
            }
            if (flag && totTable == 1)
            {
                FILE* filein;
                int tot = 0, i = 0, j = 0;
                char value[64];
                int isField[64] = {0};

                while(fieldTmp != NULL)
                {
                    strcpy(fieldName[totField], fieldTmp->field);
                    fieldTmp = fieldTmp->next_sf;
                    totField ++;
                }

                filein = fopen(tableName[0], "r");
                fscanf(filein, "%d", &tot);
                for (i = 0; i < tot; ++i)
                {
                    fscanf(filein, "%s", allField[i]);
                    for (j = 0; j < totField; ++j)
                    {
                        if (strcmp(allField[i], fieldName[j]) == 0)
                        {
                            isField[i] = 1;
                            break;
                        }
                    }
                }
                for (i = 0; i < tot; ++i)
                {
                    if (isField[i])
                        printf("%*s", 16, allField[i]);
                }
                printf("\n");

                i = tot;
                while(fscanf(filein, "%s", value) != EOF)
                {
                    if (isField[tot - i])
                    {
                        printf("%*s", 16, value);
                    }
                    i--;
                    if (i == 0)
                    {
                        i = tot;
                        printf("\n");
                    }
                }
                fclose(filein);
                printf("Select succeed.\n");
            }
            else if (flag && totTable == 2)
            {
                FILE * fileTable[64];
                FILE * fileTmp;
                int fieldCount[2] = {0}, valueCount[2] = {0}, isField[64] = {0};
                char values1[64][64] = {0}, values2[64][64] = {0};
                char tmp[64];
                int tot = 0;
                strcpy(tmp, tableName[0]);
                strcpy(tableName[0], tableName[1]);
                strcpy(tableName[1], tmp);
                fileTmp = fopen(".tmp", "w");
                for (i = 0; i < totTable; ++i)
                {
                    fileTable[i] = fopen(tableName[i], "r");
                }
                totField = 0;
                for (i = 0; i < totTable; ++i)
                {
                    fscanf(fileTable[i], "%d", &fieldCount[i]);
                    totField += fieldCount[i];
                }

                fprintf(fileTmp, "%d\n", totField);
                for (i = 0; i < fieldCount[0]; ++i)
                {
                    fscanf(fileTable[0], "%s", tmp);
                    fprintf(fileTmp, "%s\n", tmp);
                }
                for (i = 0; i < fieldCount[1]; ++i)
                {
                    fscanf(fileTable[1], "%s", tmp);
                    fprintf(fileTmp, "%s\n", tmp);
                }

                while(fscanf(fileTable[0], "%s", tmp) != EOF)
                {
                    strcpy(values1[valueCount[0]], tmp);
                    valueCount[0]++;
                }
                while(fscanf(fileTable[1], "%s", tmp) != EOF)
                {
                    strcpy(values2[valueCount[1]], tmp);
                    valueCount[1]++;
                }

                for (i = 0; i < valueCount[0]/fieldCount[0]; ++i)
                {
                    for (k = 0; k < valueCount[1]/fieldCount[1]; ++k)
                    {
                        for (j = 0; j < fieldCount[0]; ++j)
                        {
                            fprintf(fileTmp, "%s\n", values1[i*fieldCount[0]+j]);
                        }
                        for (j = 0; j < fieldCount[1]; ++j)
                        {
                            fprintf(fileTmp, "%s\n", values2[k*fieldCount[1]+j]);
                        }
                    }
                }
                fclose(fileTmp);
                fclose(fileTable[0]);
                fclose(fileTable[1]);
                fileTmp = fopen(".tmp", "r");

                while(fieldTmp != NULL)
                {
                    strcpy(fieldName[totField], fieldTmp->field);
                    fieldTmp = fieldTmp->next_sf;
                    totField ++;
                }

                fscanf(fileTmp, "%d", &tot);
                for (i = 0; i < tot; ++i)
                {
                    fscanf(fileTmp, "%s", allField[i]);
                    for (j = 0; j < totField; ++j)
                    {
                        if (strcmp(allField[i], fieldName[j]) == 0)
                        {
                            isField[i] = 1;
                            break;
                        }
                    }
                }
                for (i = 0; i < tot; ++i)
                {
                    if (isField[i])
                        printf("%*s", 16, allField[i]);
                }
                printf("\n");

                i = tot;
                while(fscanf(fileTmp, "%s", tmp) != EOF)
                {
                    if (isField[tot - i])
                    {
                        printf("%*s", 16, tmp);
                    }
                    i--;
                    if (i == 0)
                    {
                        i = tot;
                        printf("\n");
                    }
                }
                fclose(fileTmp);
                system("rm -rf .tmp");
                printf("Select succeed.\n");
            }
        }
    }

    fieldTmp = fieldRoot;
    tableTmp = tableRoot;
    while(fieldRoot != NULL)
    {
        fieldTmp = fieldRoot;
        fieldRoot = fieldRoot->next_sf;
        free(fieldTmp);
    }
    while(tableRoot != NULL)
    {
        tableTmp = tableRoot;
        tableRoot = tableRoot->next_st;
        free(tableTmp);
    }
    chdir(rootDir);
	//printf("select all\n");
	printf("SQL>");
}
void freeWhere(struct Conditions *conditionRoot){
	if (conditionRoot->left != NULL)
        freeWhere(conditionRoot->left);
    else if (conditionRoot->right != NULL)
        freeWhere(conditionRoot->right);
    else
        free(conditionRoot);
}
int whereSearch(struct Conditions *conditionRoot, int totField, char allField[][64], char value[][64])
{
	char comp_op = conditionRoot->comp_op;

    if (comp_op == 'a')
    {
        return (whereSearch(conditionRoot->left, totField, allField, value) && whereSearch(conditionRoot->right, totField, allField, value) );
    }
    else if (comp_op == 'o')
    {
        return ( whereSearch(conditionRoot->left, totField, allField, value) || whereSearch(conditionRoot->right, totField, allField, value) );
    }
    else
    {
        int field = 0;
        for (int i = 0; i < totField; ++i)
        {
            if (strcmp(allField[i], conditionRoot->left->value) == 0)
            {
                field = i;
                break;
            }
        }
        if (comp_op == '=')
        {
            if (strcmp(value[field], conditionRoot->right->value) == 0)
            {
                return 1;
            }
            else
                return 0;
        }
        else if (comp_op == '>')
        {
            if (conditionRoot->right->type == 0)
            {
                return 0;
            }
            else if (atoi(value[field]) > atoi(conditionRoot->right->value))
            {
                return 1;
            }
            else
                return 0;
        }
        else if (comp_op == '<')
        {
            if (conditionRoot->right->type == 0)
            {
                return 0;
            }
            else if (atoi(value[field]) < atoi(conditionRoot->right->value))
            {
                return 1;
            }
            else
                return 0;
        }
        else if (comp_op == '!')
        {
            if (conditionRoot->right->type == 0)
            {
                return 0;
            }
            else if (atoi(value[field]) != atoi(conditionRoot->right->value))
            {
                return 1;
            }
            else
                return 0;
        }
    }
	
}	
void selectWhere(struct Selectedfields *fieldRoot, struct Selectedtables *tableRoot, struct Conditions *conditionRoot)
{	
	int totTable = 0, totField = 0, i = 0;
    int flag = 1;
    char allField[64][64] = {0};
    char tableName[64][64] = {0}, fieldName[64][64] = {0};
    struct Selectedfields *fieldTmp = fieldRoot;
    struct Selectedtables *tableTmp = tableRoot;
    struct Conditions *conditionTmp = conditionRoot;

    chdir(rootDir);

    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        while(tableTmp != NULL)
        {
            strcpy(tableName[totTable], tableTmp->table);
            tableTmp = tableTmp->next_st;
            totTable ++;
        }

        for (i = totTable-1; i >= 0; --i)
        {
            if(access(tableName[i], F_OK) == -1)
            {
                printf("Table %s doesn't exist!\n", tableName[i]);
                flag = 0;
                break;
            }
        }
        if (flag && (totTable == 1))//single table
        {
            FILE* filein;
            int tot = 0, i = 0, j = 0;
            char values[64][64] = {0};
            int isField[64] = {0};

            while(fieldTmp != NULL)
            {
                strcpy(fieldName[totField], fieldTmp->field);
                fieldTmp = fieldTmp->next_sf;
                totField ++;
            }

            filein = fopen(tableName[0], "r");
            fscanf(filein, "%d", &tot);

            if (fieldRoot != NULL)
            {
                for (i = 0; i < tot; ++i)
                {
                    fscanf(filein, "%s", allField[i]);
                    for (j = 0; j < totField; ++j)
                    {
                        if (strcmp(allField[i], fieldName[j]) == 0)
                        {
                            isField[i] = 1;
                            break;
                        }
                    }
                }
            }
            else  //select *
            {
                for (i = 0; i < tot; ++i)
                {
                    fscanf(filein, "%s", allField[i]);
                    isField[i] = 1;
                }
            }

            for (i = 0; i < tot; ++i)
            {
                if (isField[i])
                    printf("%*s", 16, allField[i]);
            }
            printf("\n");

            int end = 1;
            for (i = 0; ; ++i)
            {
                int conditionFlag = 0;
                end = 1;

                for (j = 0; j < tot; ++j)
                {
                    if(fscanf(filein, "%s", values[j]) == EOF)
                    {
                        end = 0;
                        break;
                    }
                }
                if (end == 0)
                {
                    break;
                }

                conditionFlag = whereSearch(conditionRoot, tot, allField, values);
                if (conditionFlag)
                {
                    for (j = 0; j < tot; ++j)
                    {
                        if (isField[j])
                            printf("%*s", 16, values[j]);
                    }
                    printf("\n");
                }
            }

            fclose(filein);
            printf("Select succeed.\n");
        }
        else if (flag && totTable == 2)
        {
            FILE * fileTable[64];
            FILE * fileTmp;
            int fieldCount[2] = {0}, valueCount[2] = {0};
            char values1[64][64] = {0}, values2[64][64] = {0};
            char tmp[64];
            int tot = 0, i = 0, j = 0, k = 0;
            char values[64][64] = {0};
            int isField[64] = {0};

            strcpy(tmp, tableName[0]);
            strcpy(tableName[0], tableName[1]);
            strcpy(tableName[1], tmp);
            fileTmp = fopen(".tmp", "w");
            for (i = 0; i < totTable; ++i)
                fileTable[i] = fopen(tableName[i], "r");

            totField = 0;
            for (i = 0; i < totTable; ++i)
            {
                fscanf(fileTable[i], "%d", &fieldCount[i]);
                totField += fieldCount[i];
            }

            fprintf(fileTmp, "%d\n", totField);
            for (i = 0; i < fieldCount[0]; ++i)
            {
                fscanf(fileTable[0], "%s", tmp);
                fprintf(fileTmp, "%s\n", tmp);
            }
            for (i = 0; i < fieldCount[1]; ++i)
            {
                fscanf(fileTable[1], "%s", tmp);
                fprintf(fileTmp, "%s\n", tmp);
            }

            while(fscanf(fileTable[0], "%s", tmp) != EOF)
            {
                strcpy(values1[valueCount[0]], tmp);
                valueCount[0]++;
            }
            while(fscanf(fileTable[1], "%s", tmp) != EOF)
            {
                strcpy(values2[valueCount[1]], tmp);
                valueCount[1]++;
            }

            for (i = 0; i < valueCount[0]/fieldCount[0]; ++i)
            {
                for (k = 0; k < valueCount[1]/fieldCount[1]; ++k)
                {
                    for (j = 0; j < fieldCount[0]; ++j)
                    {
                        fprintf(fileTmp, "%s\n", values1[i*fieldCount[0]+j]);
                    }
                    for (j = 0; j < fieldCount[1]; ++j)
                    {
                        fprintf(fileTmp, "%s\n", values2[k*fieldCount[1]+j]);
                    }
                }
            }
            fclose(fileTmp);
            fclose(fileTable[0]);
            fclose(fileTable[1]);
            fileTmp = fopen(".tmp", "r");

            fscanf(fileTmp, "%d", &tot);
            while(fieldTmp != NULL)
            {
                strcpy(fieldName[totField], fieldTmp->field);
                fieldTmp = fieldTmp->next_sf;
                totField ++;
            }

            if (fieldRoot != NULL)
            {
                for (i = 0; i < tot; ++i)
                {
                    fscanf(fileTmp, "%s", allField[i]);
                    for (j = 0; j < totField; ++j)
                    {
                        if (strcmp(allField[i], fieldName[j]) == 0)
                        {
                            isField[i] = 1;
                            break;
                        }
                    }
                }
            }
            else
            {
                for (i = 0; i < tot; ++i)
                {
                    fscanf(fileTmp, "%s", allField[i]);
                    isField[i] = 1;
                }
            }
            for (i = 0; i < tot; ++i)
            {
                if (isField[i])
                    printf("%*s", 16, allField[i]);
            }
            printf("\n");

            int end = 1;
            for (i = 0; ; ++i)
            {
                int conditionFlag = 0;
                end = 1;

                for (j = 0; j < tot; ++j)
                {
                    if(fscanf(fileTmp, "%s", values[j]) == EOF)
                    {
                        end = 0;
                        break;
                    }
                }
                if (end == 0)
                {
                    break;
                }

                conditionFlag = whereSearch(conditionRoot, tot, allField, values);
                if (conditionFlag)
                {
                    for (j = 0; j < tot; ++j)
                    {
                        if (isField[j])
                            printf("%*s", 16, values[j]);
                    }
                    printf("\n");
                }
            }

            fclose(fileTmp);
            system("rm -rf .tmp");
            printf("Select succeed.\n");
        }
    }

    fieldTmp = fieldRoot;
    tableTmp = tableRoot;
    conditionTmp = conditionRoot;
    while(fieldRoot != NULL)
    {
        fieldTmp = fieldRoot;
        fieldRoot = fieldRoot->next_sf;
        free(fieldTmp);
    }
    while(tableRoot != NULL)
    {
        tableTmp = tableRoot;
        tableRoot = tableRoot->next_st;
        free(tableTmp);
    }
    freeWhere(conditionRoot);
    chdir(rootDir);
	//printf("select where\n");
	printf("SQL>");
}

void deleteWhere(char *tableName, struct Conditions *conditionRoot)
{
	int i = 0, j = 0, totField = 0;
    char allField[64][64] = {0};
    char field[64][64] = {0};
    struct Conditions *conditionTmp = conditionRoot;

    chdir(rootDir);

    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        if(access(tableName, F_OK) == -1)
        {
            printf("Table %s doesn't exist!\n", tableName);
        }
        else
        {
            FILE* filein;
            FILE* fileout;
            int end = 1;
            char cp[64] = "cp ";
            char rm[64] = "rm -rf ";
            char tableTmp[64] = {0};
            strcpy(tableTmp, tableName);
            strcat(tableTmp, ".tmp");
            strcat(cp, tableName);
            strcat(cp, " ");
            strcat(cp, tableTmp);
            strcat(rm, tableTmp);

            system(cp);

            filein = fopen(tableTmp, "r");
            fileout = fopen(tableName, "w");

            fscanf(filein, "%d", &totField);
            fprintf(fileout, "%d\n", totField);
            for (int i = 0; i < totField; ++i)
            {
                fscanf(filein, "%s", allField[i]);
                fprintf(fileout, "%s\n", allField[i]);
            }

            for (i = 0; ; ++i)
            {
                int conditionFlag = 0;
                end = 1;

                for (j = 0; j < totField; ++j)
                {
                    if(fscanf(filein, "%s", field[j]) == EOF)
                    {
                        end = 0;
                        break;
                    }
                }
                if (end == 0)
                {
                    break;
                }

                conditionFlag = whereSearch(conditionRoot, totField, allField, field);
                if (!conditionFlag)
                {
                    for (j = 0; j < totField; ++j)
                    {
                        fprintf(fileout, "%s\n", field[j]);
                    }
                }
            }
            fclose(fileout);
            fclose(filein);
            system(rm);
            printf("Delete successfully.\n");
        }
    }
    free(tableName);
    freeWhere(conditionRoot);
	chdir(rootDir);
	//printf("delete where\n");
	printf("SQL>");
}
void updateWhere(char *tableName, struct Setstruct *setRoot, struct Conditions *conditionRoot)
{
	int i = 0, j = 0, totField = 0, totSet = 0, changeFlag = 0;
    char allField[64][64] = {0};
    char field[64][64] = {0};
    char allSet[64][64] = {0};
    char setValue[64][64] = {0};
    struct Setstruct * setTmp = setRoot;
    struct Conditions *conditionTmp = conditionRoot;

    chdir(rootDir);

    if(strlen(database) == 0)
        printf("\nNo database, error!\n");
    else if(chdir(database) == -1)
        printf("\nError!\n");
    else
    {
        if(access(tableName, F_OK) == -1)
        {
            printf("Table %s doesn't exist!\n", tableName);
        }
        else
        {
            FILE* filein;
            FILE* fileout;
            int end = 1;
            char cp[64] = "cp ";
            char rm[64] = "rm -rf ";
            char tableTmp[64] = {0};
            strcpy(tableTmp, tableName);
            strcat(tableTmp, ".tmp");
            strcat(cp, tableName);
            strcat(cp, " ");
            strcat(cp, tableTmp);
            strcat(rm, tableTmp);

            totSet = 0;
            while(setTmp != NULL)
            {
                strcpy(allSet[totSet], setTmp->field);
                strcpy(setValue[totSet], setTmp->value);
                totSet++;
                setTmp = setTmp->next_s;
            }

            system(cp);

            filein = fopen(tableTmp, "r");
            fileout = fopen(tableName, "w");

            fscanf(filein, "%d", &totField);
            fprintf(fileout, "%d\n", totField);
            for (int i = 0; i < totField; ++i)
            {
                fscanf(filein, "%s", allField[i]);
                fprintf(fileout, "%s\n", allField[i]);
            }

            for (i = 0; ; ++i)
            {
                int conditionFlag = 0;
                end = 1;

                for (j = 0; j < totField; ++j)
                {
                    if(fscanf(filein, "%s", field[j]) == EOF)
                    {
                        end = 0;
                        break;
                    }
                }
                if (end == 0)
                {
                    break;
                }

                conditionFlag = whereSearch(conditionRoot, totField, allField, field);
                if (!conditionFlag)
                {
                    for (j = 0; j < totField; ++j)
                    {
                        fprintf(fileout, "%s\n", field[j]);
                    }
                }
                else
                {
                    for (j = 0; j < totField; ++j)
                    {
                        changeFlag = 0;
                        for (int k = 0; k < totSet; ++k)
                        {
                            if (strcmp(allSet[k], allField[j]) == 0)
                            {
                                fprintf(fileout, "%s\n", setValue[k]);
                                changeFlag = 1;
                                break;
                            }
                        }
                        if (!changeFlag)
                        {
                            fprintf(fileout, "%s\n", field[j]);
                        }
                    }
                }
            }
            fclose(fileout);
            fclose(filein);
            system(rm);
            printf("Update successfully.\n");
        }
    }
    free(tableName);
    freeWhere(conditionRoot);
    setTmp = setRoot;
    while(setRoot != NULL)
    {
        setTmp = setRoot;
        setRoot = setRoot->next_s;
        free(setTmp);
    }
	//printf("update where\n");
	chdir(rootDir);
	printf("SQL>");
}

void yyerror(const char *str){
	fprintf(stderr,"error:%s\n",str);
}

int yywrap(){
	return 1;
}

	
int main(){
	printf("***************************\n");
	printf("   Welcome to LYX's SQL! \n");
	printf("***************************\n\n");
	printf("SQL>");
	getcwd(rootDir, sizeof(rootDir) );
	strcat(rootDir,"/DB");
	return yyparse();
}


#line 1448 "sql.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "sql.tab.h".  */
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
#line 1384 "sql.y" /* yacc.c:355  */

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

#line 1532 "sql.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SQL_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1549 "sql.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  41
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   139

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  148

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   282

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,     2,     2,     2,     2,     2,
      28,    29,    32,     2,    31,     2,    33,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
      34,    36,    35,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1418,  1418,  1418,  1419,  1419,  1419,  1419,  1419,  1419,
    1419,  1419,  1419,  1421,  1428,  1434,  1436,  1443,  1450,  1457,
    1459,  1460,  1462,  1467,  1471,  1476,  1479,  1482,  1485,  1487,
    1491,  1498,  1505,  1512,  1518,  1524,  1527,  1529,  1537,  1545,
    1552,  1561,  1570,  1579,  1584,  1589,  1594,  1599,  1604,  1610,
    1614,  1617,  1623,  1629,  1635,  1641,  1648,  1652,  1656,  1660,
    1665,  1667,  1675,  1682,  1690,  1694,  1700
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CREATE", "TABLE", "TABLES", "DATABASE",
  "DATABASES", "INSERT", "SELECT", "UPDATE", "DELETE", "DROP", "EXIT",
  "AND", "OR", "FROM", "WHERE", "VALUES", "INTO", "SET", "SHOW", "QUOTE",
  "USE", "ID", "CHAR", "NUMBER", "INT", "'('", "')'", "';'", "','", "'*'",
  "'.'", "'<'", "'>'", "'='", "'!'", "$accept", "statements", "statement",
  "createsql", "table", "fieldsdefinition", "field_type", "field", "type",
  "usesql", "showsql", "selectsql", "fields_star", "table_fields",
  "table_field", "tables", "conditions", "condition", "comp_left",
  "comp_right", "comp_op", "insertsql", "values", "value", "deletesql",
  "updatesql", "sets", "set", "dropsql", "exitsql", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,    40,    41,
      59,    44,    42,    46,    60,    62,    61,    33
};
# endif

#define YYPACT_NINF -79

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-79)))

#define YYTABLE_NINF -16

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      45,     4,    16,   -20,    -5,    -2,    61,   -14,    44,    17,
      15,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,    -5,    20,    -5,    47,   -79,    58,   -79,    81,    71,
     -79,   -79,    83,    -5,    88,    82,    84,   -79,    75,    77,
      79,   -79,   -79,    85,    80,    19,    87,    -5,    90,    91,
      46,    -5,    86,    89,   -79,   -79,   -79,    87,   -79,    92,
      53,   -79,   -79,   -79,    12,   -79,    76,     0,   -79,    54,
     -79,    93,   -79,   -79,    52,   -79,    68,    53,    94,   -79,
     -79,    65,   -79,    54,   -79,    -5,    64,    54,    91,    54,
     -79,    55,   -79,    25,   -79,    95,    87,    96,   -79,   -79,
      69,    99,   104,    53,    57,   -79,   102,   -79,    59,   -79,
      -8,    54,    54,   -79,   -79,   -79,   -79,   -79,   -79,    97,
      66,   -79,   -79,   101,    98,   -79,   103,   -79,   -79,   107,
     -79,   -79,   -79,   116,   -79,    90,   -79,   -79,   105,   -79,
      53,   -79,   113,   -79,    70,   -79,   106,   -79
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     3,     4,    12,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    19,    28,     0,    31,     0,    27,
      29,    15,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     1,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    23,    22,     0,    14,     0,
       0,    19,    32,    34,     0,    30,     0,     0,    60,     0,
      56,     0,    64,    65,     0,    16,     0,     0,     0,    55,
      54,     0,    51,     0,    25,     0,     0,     0,     0,     0,
      40,     0,    35,     0,    57,     0,     0,     0,    21,    18,
       0,     0,     0,     0,     0,    33,     0,    62,     0,    61,
       0,     0,     0,    58,    47,    48,    43,    44,    45,     0,
       0,    13,    17,     0,     0,    53,     0,    52,    26,     0,
      59,    36,    37,    38,    46,     0,    42,    39,     0,    49,
       0,    63,     0,    20,     0,    41,     0,    50
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -79,   -79,   127,   -79,    -1,   -79,    21,   -44,   -79,   -79,
     -79,   -79,   -79,   -79,    -3,   -79,   -78,   -79,   -79,   -79,
     -79,   -79,   -76,    35,   -79,   -79,   -79,    51,   -79,   -79
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    10,    11,    12,    26,    74,    75,    27,    99,    13,
      14,    15,    28,    29,    90,    64,    91,    92,    93,   137,
     120,    16,    81,    82,    17,    18,    67,    68,    19,    20
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      30,   100,    62,    32,    24,   104,   111,   112,    21,   108,
      22,   110,    25,    76,    33,    41,    37,    87,     1,    31,
      43,   131,    45,     2,     3,     4,     5,     6,     7,    83,
      34,    88,    50,   132,   133,    23,     8,    59,     9,   114,
     115,    40,    84,    85,    44,    65,    63,    60,     1,    38,
      71,    39,    76,     2,     3,     4,     5,     6,     7,   116,
     117,   118,   119,    69,   144,    35,     8,    36,     9,   111,
     112,   111,   112,   111,   112,    78,    70,    79,    24,    80,
     -15,    95,    89,    96,   105,   113,   106,   128,   135,   130,
     107,    46,   136,    97,   102,    98,   103,    47,   124,   146,
     103,   103,    48,    49,    51,    54,    52,    55,    53,    56,
      58,    61,    86,    57,    24,    66,    72,   122,   101,    73,
      77,   125,   126,    94,   123,   121,   129,   138,   139,   141,
     111,   140,   142,   134,   143,   145,   147,    42,   127,   109
};

static const yytype_uint8 yycheck[] =
{
       3,    77,    46,     4,    24,    83,    14,    15,     4,    87,
       6,    89,    32,    57,    16,     0,    30,    17,     3,    24,
      21,    29,    23,     8,     9,    10,    11,    12,    13,    17,
      32,    31,    33,   111,   112,    19,    21,    18,    23,    14,
      15,    24,    30,    31,    24,    48,    47,    28,     3,     5,
      51,     7,    96,     8,     9,    10,    11,    12,    13,    34,
      35,    36,    37,    17,   140,     4,    21,     6,    23,    14,
      15,    14,    15,    14,    15,    22,    30,    24,    24,    26,
      33,    29,    28,    31,    85,    30,    22,    30,    22,    30,
      26,    33,    26,    25,    29,    27,    31,    16,    29,    29,
      31,    31,    31,    20,    16,    30,    24,    30,    24,    30,
      30,    24,    36,    28,    24,    24,    30,    96,    24,    30,
      28,    22,    18,    30,    28,    30,    24,    26,    30,    22,
      14,    28,   135,    36,    29,    22,    30,    10,   103,    88
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,     9,    10,    11,    12,    13,    21,    23,
      39,    40,    41,    47,    48,    49,    59,    62,    63,    66,
      67,     4,     6,    19,    24,    32,    42,    45,    50,    51,
      52,    24,    42,    16,    32,     4,     6,    30,     5,     7,
      24,     0,    40,    42,    24,    42,    33,    16,    31,    20,
      42,    16,    24,    24,    30,    30,    30,    28,    30,    18,
      28,    24,    45,    42,    53,    52,    24,    64,    65,    17,
      30,    42,    30,    30,    43,    44,    45,    28,    22,    24,
      26,    60,    61,    17,    30,    31,    36,    17,    31,    28,
      52,    54,    55,    56,    30,    29,    31,    25,    27,    46,
      60,    24,    29,    31,    54,    42,    22,    26,    54,    65,
      54,    14,    15,    30,    14,    15,    34,    35,    36,    37,
      58,    30,    44,    28,    29,    22,    18,    61,    30,    24,
      30,    29,    54,    54,    36,    22,    26,    57,    26,    30,
      28,    22,    52,    29,    60,    22,    29,    30
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    39,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    41,    41,    42,    43,    43,    44,    45,
      46,    46,    47,    48,    48,    49,    49,    50,    50,    51,
      51,    52,    52,    53,    53,    54,    54,    54,    54,    55,
      56,    57,    57,    58,    58,    58,    58,    58,    58,    59,
      59,    60,    60,    61,    61,    61,    62,    62,    62,    63,
      64,    64,    65,    65,    66,    66,    67
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     7,     4,     1,     1,     3,     2,     1,
       4,     1,     3,     3,     3,     5,     7,     1,     1,     1,
       3,     1,     3,     3,     1,     1,     3,     3,     3,     3,
       1,     3,     1,     1,     1,     1,     2,     1,     1,     8,
      11,     1,     3,     3,     1,     1,     4,     5,     6,     7,
       1,     3,     3,     5,     4,     4,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 13:
#line 1422 "sql.y" /* yacc.c:1646  */
    {
		(yyval.cs_var) = (struct Createstruct *)malloc(sizeof(struct Createstruct));
		(yyval.cs_var)->table=(yyvsp[-4].yych);
		(yyval.cs_var)->fdef=(yyvsp[-2].cfdef_var);
		createTable((yyval.cs_var));
	}
#line 2723 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 1429 "sql.y" /* yacc.c:1646  */
    {
		strcpy(database, (yyvsp[-1].yych));
		createDB();
	}
#line 2732 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 1434 "sql.y" /* yacc.c:1646  */
    {(yyval.yych)=(yyvsp[0].yych);}
#line 2738 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 1437 "sql.y" /* yacc.c:1646  */
    {
			(yyval.cfdef_var) = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
			(yyval.cfdef_var)->next_fdef = NULL;
			(yyval.cfdef_var)->field = (yyvsp[0].cfdef_var)->field;
			(yyval.cfdef_var)->type = (yyvsp[0].cfdef_var)->type;
		}
#line 2749 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 1444 "sql.y" /* yacc.c:1646  */
    {
			(yyval.cfdef_var) = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
			(yyval.cfdef_var)->next_fdef=(yyvsp[-2].cfdef_var);
			(yyval.cfdef_var)->field = (yyvsp[0].cfdef_var)->field;
			(yyval.cfdef_var)->type = (yyvsp[0].cfdef_var)->type;
		}
#line 2760 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 1451 "sql.y" /* yacc.c:1646  */
    {
		(yyval.cfdef_var) = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
		(yyval.cfdef_var)->field = (yyvsp[-1].yych);
		(yyval.cfdef_var)->type = (yyvsp[0].yych);
		(yyval.cfdef_var)->next_fdef = NULL;
	}
#line 2771 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 1457 "sql.y" /* yacc.c:1646  */
    {(yyval.yych)=(yyvsp[0].yych);}
#line 2777 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 1459 "sql.y" /* yacc.c:1646  */
    {(yyval.yych)=(yyvsp[-3].yych);}
#line 2783 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 1460 "sql.y" /* yacc.c:1646  */
    {(yyval.yych) = (yyvsp[0].yych);}
#line 2789 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 1463 "sql.y" /* yacc.c:1646  */
    {
		strcpy(database, (yyvsp[-1].yych));
		useDB();
	}
#line 2798 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 1468 "sql.y" /* yacc.c:1646  */
    {	printf("DATABASES:\n");
		getDB();
	}
#line 2806 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 1472 "sql.y" /* yacc.c:1646  */
    {	printf("TABLES:\n");
		getTable();
	}
#line 2814 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 1477 "sql.y" /* yacc.c:1646  */
    {	selectNoWhere((yyvsp[-3].sf_var),(yyvsp[-1].st_var));
	}
#line 2821 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 1480 "sql.y" /* yacc.c:1646  */
    { selectWhere((yyvsp[-5].sf_var),(yyvsp[-3].st_var),(yyvsp[-1].cons_var));
	}
#line 2828 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 1483 "sql.y" /* yacc.c:1646  */
    {
		(yyval.sf_var)=(yyvsp[0].sf_var);
	}
#line 2836 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 1486 "sql.y" /* yacc.c:1646  */
    { (yyval.sf_var)=NULL;}
#line 2842 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 1488 "sql.y" /* yacc.c:1646  */
    {	
		(yyval.sf_var)=(yyvsp[0].sf_var);
	}
#line 2850 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 1492 "sql.y" /* yacc.c:1646  */
    {
		(yyval.sf_var)=(struct Selectedfields *)malloc(sizeof(struct Selectedfields));
		(yyval.sf_var)->field = (yyvsp[0].sf_var)->field;
		(yyval.sf_var)->table = (yyvsp[0].sf_var)->table;
		(yyval.sf_var)->next_sf = (yyvsp[-2].sf_var);
	}
#line 2861 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 1499 "sql.y" /* yacc.c:1646  */
    {
		(yyval.sf_var) = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
		(yyval.sf_var)->field = (yyvsp[0].yych);
		(yyval.sf_var)->table = NULL;
		(yyval.sf_var)->next_sf = NULL;
	}
#line 2872 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 1506 "sql.y" /* yacc.c:1646  */
    {
		(yyval.sf_var) = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
		(yyval.sf_var)->field = (yyvsp[0].yych);
		(yyval.sf_var)->table = (yyvsp[-2].yych);
		(yyval.sf_var)->next_sf = NULL;
	}
#line 2883 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 1513 "sql.y" /* yacc.c:1646  */
    {
		(yyval.st_var) = (struct Selectedtables *)malloc(sizeof(struct Selectedtables));
		(yyval.st_var)->table = (yyvsp[0].yych);
		(yyval.st_var)->next_st = (yyvsp[-2].st_var);
	}
#line 2893 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 1519 "sql.y" /* yacc.c:1646  */
    { 
		(yyval.st_var) = (struct Selectedtables *)malloc(sizeof(struct Selectedtables));
		(yyval.st_var)->table = (yyvsp[0].yych);
		(yyval.st_var)->next_st = NULL;
	}
#line 2903 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 1525 "sql.y" /* yacc.c:1646  */
    {	(yyval.cons_var)=(yyvsp[0].cons_var);
	}
#line 2910 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 1528 "sql.y" /* yacc.c:1646  */
    {	(yyval.cons_var)=(yyvsp[-1].cons_var); }
#line 2916 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 1530 "sql.y" /* yacc.c:1646  */
    {
		(yyval.cons_var) = (struct Conditions *)malloc(sizeof(struct Conditions));
		(yyval.cons_var)->left = (yyvsp[-2].cons_var);
		(yyval.cons_var)->right = (yyvsp[0].cons_var);
		char c = 'a';
		(yyval.cons_var)->comp_op = c;
	}
#line 2928 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 1538 "sql.y" /* yacc.c:1646  */
    {
		(yyval.cons_var) = (struct Conditions *)malloc(sizeof(struct Conditions));
		(yyval.cons_var)->left = (yyvsp[-2].cons_var);
		(yyval.cons_var)->right = (yyvsp[0].cons_var);
		char c = 'o';
		(yyval.cons_var)->comp_op = c;
	}
#line 2940 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 1546 "sql.y" /* yacc.c:1646  */
    {
		(yyval.cons_var) = (struct Conditions *)malloc(sizeof(struct Conditions));
		(yyval.cons_var)->left = (yyvsp[-2].cons_var);
		(yyval.cons_var)->right = (yyvsp[0].cons_var);
		(yyval.cons_var)->comp_op = (yyvsp[-1].op);
	}
#line 2951 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 1553 "sql.y" /* yacc.c:1646  */
    {
		(yyval.cons_var) = (struct Conditions *)malloc(sizeof(struct Conditions));
		(yyval.cons_var)->type = 0;
		(yyval.cons_var)->value = (yyvsp[0].sf_var)->field;
		(yyval.cons_var)->table = (yyvsp[0].sf_var)->table;
		(yyval.cons_var)->left = NULL;
		(yyval.cons_var)->right = NULL;
	}
#line 2964 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 1562 "sql.y" /* yacc.c:1646  */
    {
		(yyval.cons_var) = (struct Conditions *)malloc(sizeof(struct Conditions));
		(yyval.cons_var)->type = 1;
		(yyval.cons_var)->value = (yyvsp[-1].sf_var)->field;
		(yyval.cons_var)->table = (yyvsp[-1].sf_var)->table;
		(yyval.cons_var)->left = NULL;
		(yyval.cons_var)->right =NULL;
	}
#line 2977 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 1571 "sql.y" /* yacc.c:1646  */
    {	
		(yyval.cons_var) = (struct Conditions *)malloc(sizeof(struct Conditions));
		(yyval.cons_var)->type = 2;
		(yyval.cons_var)->value = (yyvsp[0].yych);
		(yyval.cons_var)->table = NULL;
		(yyval.cons_var)->left = NULL;
		(yyval.cons_var)->right =NULL;
	}
#line 2990 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 1580 "sql.y" /* yacc.c:1646  */
    {
		char c = '<';
		(yyval.op) = c;
	}
#line 2999 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1585 "sql.y" /* yacc.c:1646  */
    {
		char c = '>';
		(yyval.op) = c;
	}
#line 3008 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 1590 "sql.y" /* yacc.c:1646  */
    {
		char c = '=';
		(yyval.op) = c;
	}
#line 3017 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 1595 "sql.y" /* yacc.c:1646  */
    {
		char c = '!';
		(yyval.op) = c;
	}
#line 3026 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1600 "sql.y" /* yacc.c:1646  */
    {
		char c = 'a';
		(yyval.op) = c;
	}
#line 3035 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 1605 "sql.y" /* yacc.c:1646  */
    {
		char c = 'o';
		(yyval.op) = c;
	}
#line 3044 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1611 "sql.y" /* yacc.c:1646  */
    {
		insertSingle((yyvsp[-5].yych),(yyvsp[-2].is_val));
	}
#line 3052 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1615 "sql.y" /* yacc.c:1646  */
    {	insertDouble((yyvsp[-8].yych),(yyvsp[-6].is_val),(yyvsp[-2].is_val));
	}
#line 3059 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1618 "sql.y" /* yacc.c:1646  */
    {
		(yyval.is_val) = (struct insertValue *)malloc(sizeof(struct insertValue));
		(yyval.is_val)->value = (yyvsp[0].is_val)->value;
		(yyval.is_val)->nextValue = NULL;
	}
#line 3069 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1624 "sql.y" /* yacc.c:1646  */
    {
		(yyval.is_val) = (struct insertValue *)malloc(sizeof(struct insertValue));
		(yyval.is_val)->value = (yyvsp[0].is_val)->value;
		(yyval.is_val)->nextValue = (yyvsp[-2].is_val);
	}
#line 3079 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1630 "sql.y" /* yacc.c:1646  */
    {
		(yyval.is_val) = (struct insertValue *)malloc(sizeof(struct insertValue));
		(yyval.is_val)->value=(yyvsp[-1].yych);
		(yyval.is_val)->nextValue = NULL;
	}
#line 3089 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1636 "sql.y" /* yacc.c:1646  */
    {
		(yyval.is_val)=(struct insertValue *)malloc(sizeof(struct insertValue));
		(yyval.is_val)->value = (yyvsp[0].yych);
		(yyval.is_val)->nextValue=NULL;
	}
#line 3099 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1642 "sql.y" /* yacc.c:1646  */
    {
		(yyval.is_val) = (struct insertValue *)malloc(sizeof(struct insertValue));
		(yyval.is_val)->value = (yyvsp[0].yych);
		(yyval.is_val)->nextValue = NULL;
	}
#line 3109 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1649 "sql.y" /* yacc.c:1646  */
    {
		deleteAll((yyvsp[-1].yych));
	}
#line 3117 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1653 "sql.y" /* yacc.c:1646  */
    {
		deleteAll((yyvsp[-1].yych));
	}
#line 3125 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1657 "sql.y" /* yacc.c:1646  */
    {
		deleteWhere((yyvsp[-3].yych),(yyvsp[-1].cons_var));
	}
#line 3133 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1661 "sql.y" /* yacc.c:1646  */
    {
		updateWhere((yyvsp[-5].yych),(yyvsp[-3].s_var),(yyvsp[-1].cons_var));
	}
#line 3141 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1666 "sql.y" /* yacc.c:1646  */
    { (yyval.s_var)=(yyvsp[0].s_var); }
#line 3147 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1668 "sql.y" /* yacc.c:1646  */
    {
 		(yyval.s_var)=(struct Setstruct *)malloc(sizeof(struct Setstruct));
		(yyval.s_var)->next_s = (yyvsp[-2].s_var);
		(yyval.s_var)->field = (yyvsp[0].s_var)->field;
		(yyval.s_var)->value = (yyvsp[0].s_var)->value;
		free((yyvsp[0].s_var));
	}
#line 3159 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1676 "sql.y" /* yacc.c:1646  */
    {
		(yyval.s_var)=(struct Setstruct *)malloc(sizeof(struct Setstruct));
		(yyval.s_var)->field = (yyvsp[-2].yych);
		(yyval.s_var)->value = (yyvsp[0].yych);
		(yyval.s_var)->next_s = NULL;
	}
#line 3170 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1683 "sql.y" /* yacc.c:1646  */
    {
		(yyval.s_var)=(struct Setstruct *)malloc(sizeof(struct Setstruct));
		(yyval.s_var)->field = (yyvsp[-4].yych);
		(yyval.s_var)->value = (yyvsp[-1].yych);
		(yyval.s_var)->next_s=NULL;
	}
#line 3181 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1691 "sql.y" /* yacc.c:1646  */
    {
		dropTable((yyvsp[-1].yych));
	}
#line 3189 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1695 "sql.y" /* yacc.c:1646  */
    {
		strcpy(database,(yyvsp[-1].yych));
		dropDB();
	}
#line 3198 "sql.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1701 "sql.y" /* yacc.c:1646  */
    {
		printf("Bye!\n");
		exit(0);
	}
#line 3207 "sql.tab.c" /* yacc.c:1646  */
    break;


#line 3211 "sql.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
