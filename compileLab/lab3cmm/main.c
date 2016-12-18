#include "compiler.h"
#include "ir.h"
extern void yyrestart(FILE *);
extern int yyparse();

char *outfile = "ir.out";

int main(int argc, char** argv)
{
	fp = fopen("symbol_table.txt", "w+");
	NOERROR = 1;
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
	outputFile(outfile);
	return 0;
}


