#pragma once
//#include "Analyser.h"
#include "Capital.h"

Value MakeValue(Lex field);
Type_of_lex GetValue(Lex field);

bool reg_8(string field);
bool reg_16(string field);
bool reg_32(string field);
bool directive(string field);
bool hex_const(string field);
bool dec_const(string field);
bool bin_const(string field);
bool text_const(string field);
bool single_symb(string field);
bool command(string field);
bool reg_seg(string field);
bool user_indent(string field);