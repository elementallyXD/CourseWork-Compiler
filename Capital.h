#pragma once
#include <string>

using namespace std;

#pragma warning (disable:4996)
#pragma warning (disable:4018)
#pragma warning (disable:4267)

typedef struct
{
	string lex;
	unsigned int line;
	unsigned int counter = 0;
}Lex;

enum Type_of_lex
{
	REG_8, REG_16, REG_32, DIRECTIVE, SINGLE, UNDEF, TEXT_CONST, HEX_CONST, DEC_CONST, BIN_CONST, COMMAND, REG_SEG, USER_IDENT
};

typedef struct
{
	string lexVal;
	unsigned int line;
	unsigned int length;
	Type_of_lex _type;
	unsigned int count = 0;
}Value;