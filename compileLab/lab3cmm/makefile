parser: lexical.l syntax.y main.c gramtree.c symboltab.c ir.c
	bison -d syntax.y
	flex lexical.l
	gcc -g main.c syntax.tab.c -g gramtree.c -g symboltab.c -g ir.c -lfl -o parser
