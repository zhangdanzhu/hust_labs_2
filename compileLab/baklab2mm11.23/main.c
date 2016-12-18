#include "compiler.h"
extern void yyrestart(FILE *);
extern int yyparse();

int main(int argc, char** argv)
{
	SCOPE = 0;
	if (argc <= 1) return 1;
	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		perror(argv[1]);
		return 1;
	}
	CreateSymTab();
	yyrestart(f);
	yyparse();
	return 0;
}


