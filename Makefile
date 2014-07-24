Target = funfun
Objects = main.o lex.yy.o y.tab.o ast.o st.o ff.o
CC = gcc
Flags = -g -Wall -c
Includes = \

$(Target): $(Objects) *.c *.h
	$(CC) -g -Wall -o $(Target) $(Objects)

main.o: ast.h ast.c ff.h ff.c main.c
	$(CC) $(Flags) main.c

y.tab.h: Funfun.y ast.h ast.c ff.h ff.c
	bison -y -d -v Funfun.y

y.tab.c: Funfun.y ast.h ast.c ff.h ff.c
	bison -y -d -v Funfun.y

lex.yy.c: Funfun.l Funfun.y y.tab.h ast.c ast.h ff.h ff.c
	flex Funfun.l

yy.tab.o: ast.h st.h ff.h 
	$(CC)  $(Flags) yy.tab.c 

lex.y.o: lex.yy.c
	$(CC) $(Flags) lex.yy.c 

st.o: 
	$(CC) $(Flags) st.c

ast.o: st.h ast.c
	$(CC) $(Flags) ast.c

ff.o: ast.h ast.c ff.c ff.h
	$(CC) $(Flags) ff.c

clean:
	rm -rf *.o *.exe *.output lex.yy.c y.tab.*

