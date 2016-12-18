parser: lexical.l syntax.y main.c gramtree.c
	bison -d syntax.y
	flex lexical.l
	gcc -g main.c syntax.tab.c -g gramtree.c -lfl -o parser
