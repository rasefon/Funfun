Target = funfun
Objects = main.o lex.yy.o y.tab.o ast.o st.o ff.o
CC = gcc
Includes = \

$(Target): $(Objects) *.c *.h
	$(CC) -g -o $(Target) $(Objects)

main.o: ast.h ast.c ff.h ff.c main.c
	$(CC) -c -g main.c

y.tab.h: Funfun.y ast.h ast.c ff.h ff.c
	bison -y -d -v Funfun.y

y.tab.c: Funfun.y ast.h ast.c ff.h ff.c
	bison -y -d -v Funfun.y

lex.yy.c: Funfun.l Funfun.y y.tab.h ast.c ast.h ff.h ff.c
	flex Funfun.l

yy.tab.o: ast.h st.h ff.h 
	$(CC) -c -g $*.c $(Includes)

lex.y.o: lex.yy.c
	$(CC) -c -g $*.c $(Includes)

st.o: 
	$(CC) -c -g st.c

ast.o: st.h ast.c
	$(CC) -c -g ast.c

ff.o: ast.h ast.c ff.c ff.h
	$(CC) -c -g ff.c

clean:
	rm -rf *.o *.exe *.output lex.yy.c y.tab.*

