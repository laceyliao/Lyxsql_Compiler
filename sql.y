%{
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

            while(fieldTmp != NULL)//field
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
        }//end else-if
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

%}

%union
{
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
}

%token CREATE TABLE TABLES DATABASE DATABASES INSERT SELECT UPDATE DELETE DROP EXIT AND OR FROM WHERE VALUES INTO SET SHOW QUOTE USE ID CHAR NUMBER INT 

%type<yych> table field type NUMBER CHAR INT ID 
%type<op> comp_op
%type<cfdef_var> fieldsdefinition field_type
%type<cs_var> createsql

%type<is_val> values value

%type<sf_var> fields_star table_fields table_field
%type<st_var> tables;
%type<cons_var> condition conditions comp_left comp_right
%type<ss_var> selectsql
%type<s_var> set sets
%left OR
%left AND

%%
statements: statements statement | statement
statement: createsql | showsql | selectsql | insertsql | deletesql | updatesql | dropsql | exitsql | usesql

createsql: CREATE TABLE table '('fieldsdefinition ')'';'
	{
		$$ = (struct Createstruct *)malloc(sizeof(struct Createstruct));
		$$->table=$3;
		$$->fdef=$5;
		createTable($$);
	}
	| CREATE DATABASE ID ';'
	{
		strcpy(database, $3);
		createDB();
	}

table: ID{$$=$1;}

fieldsdefinition: field_type
		{
			$$ = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
			$$->next_fdef = NULL;
			$$->field = $1->field;
			$$->type = $1->type;
		}
		| fieldsdefinition ',' field_type
		{
			$$ = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
			$$->next_fdef=$1;
			$$->field = $3->field;
			$$->type = $3->type;
		}
field_type: field type
	{
		$$ = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
		$$->field = $1;
		$$->type = $2;
		$$->next_fdef = NULL;
	}
field: ID {$$=$1;}

type: CHAR '(' NUMBER ')'  {$$=$1;} 
	| INT {$$ = $1;}

usesql: USE ID ';'
	{
		strcpy(database, $2);
		useDB();
	}
showsql: SHOW DATABASES ';'
	{	printf("DATABASES:\n");
		getDB();
	}
	| SHOW TABLES ';'
	{	printf("TABLES:\n");
		getTable();
	}

selectsql: SELECT fields_star FROM tables ';'
	{	selectNoWhere($2,$4);
	}
	| SELECT fields_star FROM tables WHERE conditions ';'
	{ selectWhere($2,$4,$6);
	}
fields_star: table_fields 
	{
		$$=$1;
	}| '*'
	{ $$=NULL;}
table_fields: table_field
	{	
		$$=$1;
	}
	|table_fields ',' table_field
	{
		$$=(struct Selectedfields *)malloc(sizeof(struct Selectedfields));
		$$->field = $3->field;
		$$->table = $3->table;
		$$->next_sf = $1;
	}
table_field: field
	{
		$$ = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
		$$->field = $1;
		$$->table = NULL;
		$$->next_sf = NULL;
	}
	| table '.' field
	{
		$$ = (struct Selectedfields *)malloc(sizeof(struct Selectedfields));
		$$->field = $3;
		$$->table = $1;
		$$->next_sf = NULL;
	}
tables: tables ',' table
	{
		$$ = (struct Selectedtables *)malloc(sizeof(struct Selectedtables));
		$$->table = $3;
		$$->next_st = $1;
	}
	| table
	{ 
		$$ = (struct Selectedtables *)malloc(sizeof(struct Selectedtables));
		$$->table = $1;
		$$->next_st = NULL;
	}
conditions: condition
	{	$$=$1;
	}
	| '(' conditions ')'
	{	$$=$2; }
	| conditions AND conditions
	{
		$$ = (struct Conditions *)malloc(sizeof(struct Conditions));
		$$->left = $1;
		$$->right = $3;
		char c = 'a';
		$$->comp_op = c;
	}
	| conditions OR conditions
	{
		$$ = (struct Conditions *)malloc(sizeof(struct Conditions));
		$$->left = $1;
		$$->right = $3;
		char c = 'o';
		$$->comp_op = c;
	}
condition: comp_left comp_op comp_right
	{
		$$ = (struct Conditions *)malloc(sizeof(struct Conditions));
		$$->left = $1;
		$$->right = $3;
		$$->comp_op = $2;
	}
comp_left: table_field
	{
		$$ = (struct Conditions *)malloc(sizeof(struct Conditions));
		$$->type = 0;
		$$->value = $1->field;
		$$->table = $1->table;
		$$->left = NULL;
		$$->right = NULL;
	}
comp_right: QUOTE table_field QUOTE
	{
		$$ = (struct Conditions *)malloc(sizeof(struct Conditions));
		$$->type = 1;
		$$->value = $2->field;
		$$->table = $2->table;
		$$->left = NULL;
		$$->right =NULL;
	}
	| NUMBER
	{	
		$$ = (struct Conditions *)malloc(sizeof(struct Conditions));
		$$->type = 2;
		$$->value = $1;
		$$->table = NULL;
		$$->left = NULL;
		$$->right =NULL;
	}
comp_op: '<'
	{
		char c = '<';
		$$ = c;
	}
	| '>'
	{
		char c = '>';
		$$ = c;
	}
	| '='
	{
		char c = '=';
		$$ = c;
	}
	| '!' '='
	{
		char c = '!';
		$$ = c;
	}
	| AND
	{
		char c = 'a';
		$$ = c;
	}
	| OR
	{
		char c = 'o';
		$$ = c;
	}

insertsql: INSERT INTO table VALUES '(' values ')' ';'
	{
		insertSingle($3,$6);
	}
	| INSERT INTO table '(' values ')' VALUES '(' values ')'';'
	{	insertDouble($3,$5,$9);
	}
values: value
	{
		$$ = (struct insertValue *)malloc(sizeof(struct insertValue));
		$$->value = $1->value;
		$$->nextValue = NULL;
	}
	| values ',' value
	{
		$$ = (struct insertValue *)malloc(sizeof(struct insertValue));
		$$->value = $3->value;
		$$->nextValue = $1;
	} 
value: QUOTE ID QUOTE	
	{
		$$ = (struct insertValue *)malloc(sizeof(struct insertValue));
		$$->value=$2;
		$$->nextValue = NULL;
	}
	| NUMBER
	{
		$$=(struct insertValue *)malloc(sizeof(struct insertValue));
		$$->value = $1;
		$$->nextValue=NULL;
	}
	| ID
	{
		$$ = (struct insertValue *)malloc(sizeof(struct insertValue));
		$$->value = $1;
		$$->nextValue = NULL;
	}

deletesql: DELETE FROM table ';'
	{
		deleteAll($3);
	}
	| DELETE '*' FROM table ';'
	{
		deleteAll($4);
	}
	| DELETE FROM table WHERE conditions ';'
	{
		deleteWhere($3,$5);
	}
updatesql: UPDATE table SET sets WHERE conditions ';'
	{
		updateWhere($2,$4,$6);
	}

sets: set
	{ $$=$1; }
	| sets ',' set
	{
 		$$=(struct Setstruct *)malloc(sizeof(struct Setstruct));
		$$->next_s = $1;
		$$->field = $3->field;
		$$->value = $3->value;
		free($3);
	}
set: ID '=' NUMBER
	{
		$$=(struct Setstruct *)malloc(sizeof(struct Setstruct));
		$$->field = $1;
		$$->value = $3;
		$$->next_s = NULL;
	}
	| ID '=' QUOTE ID QUOTE
	{
		$$=(struct Setstruct *)malloc(sizeof(struct Setstruct));
		$$->field = $1;
		$$->value = $4;
		$$->next_s=NULL;
	}

dropsql: DROP TABLE ID ';'
	{
		dropTable($3);
	}
	| DROP DATABASE ID ';'
	{
		strcpy(database,$3);
		dropDB();
	}

exitsql: EXIT ';'
	{
		printf("Bye!\n");
		exit(0);
	}


