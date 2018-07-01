all: lex.yy.c sql.tab.c
	gcc -g lex.yy.c sql.tab.c -o lyxsql -std=c99

lex.yy.c: sql.l
	flex sql.l

sql.tab.c: sql.y
	bison -d sql.y

clean:
	rm Lyxsql lex.yy.c sql.tab.c sql.tab.h
