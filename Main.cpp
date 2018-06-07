#include "Analyser.h"
#include <conio.h>

int main(void)
{
	printf("\t\tCompiler\n");
	LexAnalysis analyser("Test.asm", "New.lst");
	_getch();
	return 0;
}