#include "Value.h"

Type_of_lex GetValue(Lex field)
{
	if (reg_seg(field.lex))		return REG_SEG;
	if (directive(field.lex))	return DIRECTIVE;
	if (reg_8(field.lex))		return REG_8;
	if (reg_16(field.lex))		return REG_16;
	if (reg_32(field.lex))		return REG_32;
	if (command(field.lex))		return COMMAND;
	if (single_symb(field.lex)) return SINGLE;
	if (hex_const(field.lex))	return HEX_CONST;
	if (dec_const(field.lex))	return DEC_CONST;
	if (bin_const(field.lex))	return BIN_CONST;
	if (text_const(field.lex))	return TEXT_CONST;
	if (user_indent(field.lex))	return USER_IDENT;
	return UNDEF;
}

Value MakeValue(Lex field)
{
	Value buff;

	buff.lexVal = field.lex;
	buff.line = field.line;
	buff.length = field.lex.length();
	buff._type = GetValue(field);
	buff.count = field.counter;

	return buff;
}

bool reg_8(string field)
{
	string buff = field;
	string templates = "AL AH BL BH CL CH DL DH";

	for (int i = 0; i < buff.length(); i++)
		buff[i] = (char)toupper(buff[i]);

	if (templates.find(buff) != std::string::npos) return true;
	else return false;
}

bool reg_16(string field)
{
	string buff = field;
	string templates = "AX BX CX DX SI DI BP";

	for (int i = 0; i < buff.length(); i++)
		buff[i] = (char)toupper(buff[i]);

	if (templates.find(buff) != std::string::npos) return true;
	else return false;
}

bool reg_32(string field)
{
	string buff = field;
	string templates = "EAX EBX ECX EDX ESI EDI EBP";

	for (int i = 0; i < buff.length(); i++)
		buff[i] = (char)toupper(buff[i]);

	if (templates.find(buff) != std::string::npos) return true;
	else return false;
}

bool command(string field)
{
	string buff = field;
	string templates = "MOV CMP CLI INC ADD DEC XOR OR JB JMP AND";

	for (int i = 0; i < buff.length(); i++)
		buff[i] = (char)toupper(buff[i]);

	if (templates.find(buff) != std::string::npos) return true;
	else return false;
}

bool directive(string field)
{
	string buff = field;
	string templates = "SEGMENT ENDS END = DB DW DD ASSUME";

	for (int i = 0; i < buff.length(); i++)
		buff[i] = (char)toupper(buff[i]);

	if (templates.find(buff) != std::string::npos) return true;
	else return false;
}

bool single_symb(string field)
{
	string buff = field;
	string templates = ", : [ ]";

	if (templates.find(buff) != std::string::npos) return true;
	else return false;
}

bool reg_seg(string field)
{
	string buff = field;
	string templates = "GS DS CS";

	for (int i = 0; i < buff.length(); i++)
		buff[i] = (char)toupper(buff[i]);

	if (templates.find(buff) != std::string::npos) return true;
	else return false;
}

bool hex_const(string field)
{
	string buff = field;
	string hex_symb = "0 1 2 3 4 5 6 7 8 9 A B C D E F";
	string templates = "H h";

	if (!(isdigit(buff[0]))) return false;
	for (int i = 1; i < buff.length() - 1; i++)
	{
		buff[i] = (char)toupper(buff[i]);
		if (hex_symb.find(buff[i]) == std::string::npos) return false;
	}

	if (templates.find(buff[buff.length() - 1]) == std::string::npos) return false;
	return true;
}

bool dec_const(string field)
{
	string buff = field;
	string templates = "D d";

	for (int i = 0; i < buff.length() - 1; i++)
		if (!isdigit(buff[i])) return false;

	if (templates.find(buff[buff.length() - 1]) == std::string::npos && !isdigit(buff[buff.length() - 1])) return false;
	return true;
}

bool bin_const(string field)
{
	string buff = field;
	string bin = "1 0";
	string templates = "B b";

	for (int i = 0; i < buff.length() - 1; i++)
	{
		buff[i] = (char)toupper(buff[i]);
		if (bin.find(buff[i]) == std::string::npos) return false;
	}

	if (templates.find(buff[buff.length() - 1]) == std::string::npos) return false;
	return true;
}

bool text_const(string field)
{
	string buff = field;
	string symb = "\"";

	for (int i = 1; i < buff.length() - 1; i++)
	{
		if (symb.find(buff[i]) != std::string::npos) return false;
	}

	if (symb.find(buff[0]) == std::string::npos || symb.find(buff[buff.length() - 1]) == std::string::npos) return false;
	return true;
}

bool user_indent(string field)
{
	string buff = field;

	for (int i = 0; i < buff.length() - 1; i++)
	{
		if ((!isdigit(buff[i]) && !(isalpha(buff[i])))) return false;
	}
	return true;
}