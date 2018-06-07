#include "Analyser.h"
#include <fstream>

//the first function
LexAnalysis::LexAnalysis(const char* testfile, const char* lst)
{
	//Open testfile and read
	FILE *Test;
	//create file for my new listing
	ofstream fout(lst);
	// TEST file read in this buff
	char buff[128];

	Test = fopen(testfile, "r");
	if (Test == NULL) return;

	//reading test
	while (fgets(buff, 127, Test))
		TestFile.push_back(buff);

	Splite();
	PrintVectSenteces(fout);
	fclose(Test);
	fout.close();
}

//Splite Test file
void LexAnalysis::Splite()
{
	int position_1, position_2;
	Lex buff;

	for (int i = 0; i < TestFile.size(); i++)
	{
		buff.line = i + 1;
		buff.counter = -1;
		for (int j = 0; j < TestFile[i].length(); j++)
		{
			if (TestFile[i][j] == ';') break;
			position_1 = j;
			if (isalpha(TestFile[i][j]) || isdigit(TestFile[i][j]) || TestFile[i][j] == '_' || TestFile[i][j] == '@')
			{
				while (isalpha(TestFile[i][j]) || isdigit(TestFile[i][j]) || TestFile[i][j] == '_' || TestFile[i][j] == '@')
				{
					position_2 = j;
					j++;
				}
				buff.lex = TestFile[i].substr(position_1, position_2 - position_1 + 1);
				buff.counter = buff.counter + 1;
				lexs.push_back(GetToUpper(buff));
				if (!isspace(TestFile[i][j]))
				{
					buff.lex = TestFile[i].substr(j, 1);
					buff.counter = buff.counter + 1;
					lexs.push_back(GetToUpper(buff));
				}
			}
			else if (TestFile[i][j] == '"') {
				j++;
				while (TestFile[i][j] != '"') {
					j++;
					position_2 = j;
				}
				buff.lex = TestFile[i].substr(position_1, position_2 - position_1 + 1);
				buff.counter = buff.counter + 1;
				lexs.push_back(buff);
			}
			else if (!isspace(TestFile[i][j]))
			{
				buff.lex = TestFile[i].substr(position_1, 1);
				buff.counter = buff.counter + 1;
				lexs.push_back(GetToUpper(buff));
			}
		}
		CountsVal.push_back(buff.counter);
	}
	Make_Value_Vector();
	MakeVectSentences(values);
}

//Making Value
void LexAnalysis::Make_Value_Vector()
{
	for (int i = 0; i < lexs.size(); i++)
		values.push_back(MakeValue(lexs[i]));
}

//Making Vect Sentences
void LexAnalysis::MakeVectSentences(vector<Value> field)
{
	int j, delta = 0;
	LexsLines buff;

	for (int k = 0; k < CountsVal.size(); k++)
	{
		j = 0;
		if (CountsVal[k] >= 0) {
			while (j <= CountsVal[k])
			{
				buff.Lexinline.push_back(field[j + delta]);
				j++;
			}
			delta += j;
			VectSentences.push_back(buff);
			buff.Lexinline.clear();
		}
	}
}

//resets all flags and sets the start states
void LexAnalysis::AllFlagsInNull(int iterator) {
	VectSentences[iterator].label_pos = -1;
	VectSentences[iterator].dir_pos = -1;
	VectSentences[iterator].dir_count = 0;
	VectSentences[iterator].constanta_pos = -1;
	VectSentences[iterator].mn_pos = -1;
	VectSentences[iterator].mn_count = 0;
	VectSentences[iterator].op1_pos = -1;
	VectSentences[iterator].op1_count = -1;
	VectSentences[iterator].op2_count = -1;
	VectSentences[iterator].op2_pos = -1;

	VectSentences[iterator].op1_count = 0;
	VectSentences[iterator].op2_count = 0;
	VectSentences[iterator].op2 = false;
	VectSentences[iterator].mn = false;
	VectSentences[iterator].op1 = false;
	VectSentences[iterator].coma = false;
	VectSentences[iterator].doubleDot = false;
	VectSentences[iterator].dir = false;
	VectSentences[iterator].label = false;
}

//Fuction all print
void LexAnalysis::PrintVectSenteces(ofstream &f)
{
	bool NewLine = true, Op1 = true, Op1_first = true, Op2_first = true;
	bool NewOperand;
	int CurrentLine = 0;
	__offset = 0, code__offset = 0, data__offset = 0;

	for (int i = 0; i < VectSentences.size(); i++)
	{
		AllFlagsInNull(i);

		NewOperand = false;

		for (int k = 0; k < VectSentences[i].Lexinline.size(); k++)
		{
			//printing file	
			if (TestFile[CurrentLine] == "\n" || TestFile[CurrentLine][0] == ';') CurrentLine++;
			if ((CurrentLine + 1) == VectSentences[i].Lexinline[k].line && NewLine == false) NewLine = true;
			if (NewLine == true)
			{
				if (VectSentences[i].Lexinline[k]._type != DIRECTIVE)
				{
					cout << "\n" << setiosflags(ios::right) << setfill('0') << setw(4) << hex << uppercase << __offset << "   " << TestFile[CurrentLine];
					f << setiosflags(ios::right) << setfill('0') << setw(4) << hex << uppercase << __offset << "   " << TestFile[CurrentLine];
				}
				else
				{
					cout << "\n" << setiosflags(ios::right) << "   " << TestFile[CurrentLine];
					f << setiosflags(ios::right) << "   " << TestFile[CurrentLine];
				}
				NewLine = false;
				CurrentLine++;
			}
			//print line properties
			cout << resetiosflags(ios::right) << setiosflags(ios::left) << dec << setfill(' ') << setw(4) << VectSentences[i].Lexinline[k].line << setw(4) << VectSentences[i].Lexinline[k].length << setw(8) << VectSentences[i].Lexinline[k].count << setw(13) << VectSentences[i].Lexinline[k].lexVal;
			switch (VectSentences[i].Lexinline[k]._type)
			{
			case REG_8:
				cout << "8-bit REG" << endl;
				break;
			case REG_16:
				cout << "16-bit REG" << endl;
				break;
			case REG_32:
				cout << "32-bit REG" << endl;
				break;
			case REG_SEG:
				cout << "SEG-REG" << endl;
				break;
			case COMMAND:
				cout << "COMMAND" << endl;
				break;
			case HEX_CONST:
				cout << "HEX CONST" << endl;
				break;
			case DEC_CONST:
				cout << "DEC CONST" << endl;
				break;
			case BIN_CONST:
				cout << "BIN CONST" << endl;
				break;
			case TEXT_CONST:
				cout << "TEXT CONST" << endl;
				break;
			case DIRECTIVE:
				cout << "DIRECTIVE" << endl;
				break;
			case SINGLE:
				cout << "SINGLE SYMBOL" << endl;
				break;
			case USER_IDENT:
				cout << "USER IDENTIFIER" << endl;
				break;
			case UNDEF:
				cout << "UNDEF" << endl;
				break;
			}
			//make vect of flags
			if (VectSentences[i].Lexinline[k]._type == COMMAND)
			{
				VectSentences[i].mn_pos = k;
				VectSentences[i].mn = true;
				VectSentences[i].mn_count++;
				VectSentences[i].mn_name = VectSentences[i].Lexinline[k].lexVal;
			}

			if ((ifoperand(i, k) || (ifconstanta(i, k))) && (VectSentences[i].Lexinline[0]._type != USER_IDENT))
			{
				if ((VectSentences[i].Lexinline[k].lexVal != ",") && (VectSentences[i].op2 == false)) {
					if (Op1_first) {
						VectSentences[i].op1 = true;
						VectSentences[i].op1_name = VectSentences[i].Lexinline[k].lexVal;
						VectSentences[i].op1_pos = k;
						Op1_first = false;
					}

					Operand1.push_back(VectSentences[i].Lexinline[k]);
				}
				else {
					VectSentences[i].coma = true;
					VectSentences[i].op2 = true;

					if (VectSentences[i].Lexinline[k].lexVal != ",") {
						if (k > VectSentences[i].op1_count)
							if (Op2_first) {
								VectSentences[i].op2_pos = k;
								Op2_first = false;
							}

						if (VectSentences[i].Lexinline[k].lexVal != ",")
							Operand2.push_back(VectSentences[i].Lexinline[k]);

						NewOperand = true;
					}
				}
				if (VectSentences[i].Lexinline[k].lexVal != ",") {
					if (NewOperand == false)	VectSentences[i].op1_count++;
					else VectSentences[i].op2_count++;
				}
			}

			if (VectSentences[i].Lexinline[0]._type == USER_IDENT)
			{
				VectSentences[i].label = true;
				VectSentences[i].label_name = VectSentences[i].Lexinline[0].lexVal;
				VectSentences[i].label_pos = 0;
			}
			if (VectSentences[i].Lexinline[k].lexVal == ":")
			{
				VectSentences[i].doubleDot = true;
			}
			if (VectSentences[i].Lexinline[k]._type == DIRECTIVE)
			{
				VectSentences[i].dir = true;
				VectSentences[i].dir_count++;
				VectSentences[i].dir_pos = k;
				VectSentences[i].dir_name = VectSentences[i].Lexinline[k].lexVal;
			}
			if (ifconstanta(i, k) || (VectSentences[i].Lexinline[k]._type == USER_IDENT))
			{
				VectSentences[i].constanta = true;
				VectSentences[i].constanta_pos = k;
				VectSentences[i].constanta_type = VectSentences[i].Lexinline[k]._type;
				VectSentences[i].constanta_name = VectSentences[i].Lexinline[k].lexVal;
			}
		}
		/*cout << "1 -> ";
		for (int j = 0; j < Operand1.size(); j++)
		cout  << Operand1[j].lexVal << " ";
		cout << endl;

		cout << "2-> ";
		for (int j = 0; j < Operand2.size(); j++) {
		cout << Operand2[j].lexVal << " ";
		}
		cout << endl;*/

		Op1_first = true;
		Op2_first = true;
		BuffMake(i);
		cout << endl;
		Operand1.clear();
		Operand2.clear();
	}
	Out_Tables(f);
}

//bool function for compare 
bool LexAnalysis::ifconstanta(int iterator1, int interator2)
{
	return ((VectSentences[iterator1].Lexinline[interator2]._type == HEX_CONST) || (VectSentences[iterator1].Lexinline[interator2]._type == DEC_CONST) || (VectSentences[iterator1].Lexinline[interator2]._type == TEXT_CONST) || (VectSentences[iterator1].Lexinline[interator2]._type == BIN_CONST));
}

//bool function for compare 
bool LexAnalysis::ifoperand(int iterator1, int interator2)
{
	return ((VectSentences[iterator1].Lexinline[interator2]._type == REG_SEG) || (VectSentences[iterator1].Lexinline[interator2]._type == REG_16) || (VectSentences[iterator1].Lexinline[interator2]._type == REG_8) || (VectSentences[iterator1].Lexinline[interator2]._type == REG_32) || (VectSentences[iterator1].Lexinline[interator2]._type == USER_IDENT) || ((VectSentences[iterator1].Lexinline[interator2]._type == SINGLE)));
}

//Function print lex
void LexAnalysis::BuffMake(int iterator)
{
	Counters buff;
	Label buff1;
	if (VectSentences[iterator].mn == true)
	{
		cout << "MNEMOCODE (" << VectSentences[iterator].mn_count << " -> " << VectSentences[iterator].mn_pos << ")";
		if (VectSentences[iterator].op1)
			cout << " OPERAND1 (" << VectSentences[iterator].op1_count << " -> " << VectSentences[iterator].op1_pos << ")";
		if (VectSentences[iterator].op2)
			cout << ", OPERAND2 (" << VectSentences[iterator].op2_count << " -> " << VectSentences[iterator].op2_pos << ")";
	}
	else if (VectSentences[iterator].label == true)
	{
		cout << "LABEL (" << 1 << " -> " << VectSentences[iterator].label_pos << ")";

		if (VectSentences[iterator].doubleDot == true) {
			cout << ":";
			buff1.Name = VectSentences[iterator].label_name;
			buff1.Attr = "CODE";
			buff1.Type = "NEAR";
			buff1.offsets = __offset;
			Labels.push_back(buff1);
		}
		if (VectSentences[iterator].dir == true)
		{
			if ((VectSentences[iterator].dir_name != "SEGMENT") && (VectSentences[iterator].dir_name != "ENDS"))
			{
				buff.Attr = "DATA";
				buff.Type = TypeofCounter(iterator, buff);
				buff.Users = VectSentences[iterator].label_name;
				buff.offsets = __offset;
				buff.Direct = VectSentences[iterator].dir_name;
				cout << "\n" << buff.Direct << endl;
				UserIndef.push_back(buff);
			}
			cout << " DIRECTIVE (" << VectSentences[iterator].dir_count << "->" << VectSentences[iterator].dir_pos << ")";

			FindDirOffset(iterator);
			if (VectSentences[iterator].dir_name == "SEGMENT")	Segments.push_back(VectSentences[iterator].label_name);

			if ((VectSentences[iterator].constanta == true) && (VectSentences[iterator].constanta_pos != 0))
				cout << " OPERAND (" << 1 << "->" << VectSentences[iterator].constanta_pos << ")";
		}
	}
	else if (VectSentences[iterator].dir == true) {
		cout << "DIRECTIVE (" << VectSentences[iterator].dir_count << "->" << VectSentences[iterator].dir_pos << ")";

		if (VectSentences[iterator].op1 == true)
			cout << " OPERAND1 (" << VectSentences[iterator].op1_count << " -> " << VectSentences[iterator].op1_pos << ")";

		if (VectSentences[iterator].op2 == true)
			cout << ", OPERAND2 (" << VectSentences[iterator].op2_count << " -> " << VectSentences[iterator].op2_pos << ")";
	}

	FindCommandOffset(iterator);
}

//Making Offsets for sentences
void LexAnalysis::FindCommandOffset(int iterator)
{
	bool finded1 = false;

	if (VectSentences[iterator].mn)
	{
		__offset++;
		if (VectSentences[iterator].mn_name == "INC")
		{
			for (int j = 0; j < Operand1.size(); j++)
			if (Operand1[j]._type != REG_32) __offset++;
		}
		
		/*Making JMPs offsets*/
		else if (VectSentences[iterator].mn_name == "JMP")
		{
			finded1 = false;
			for (int j = 0; j < Labels.size(); j++)
				if (Labels[j].Name == VectSentences[iterator].op1_name)
				{
					__offset++;
					finded1 = true;
				}
				else {
					for (int j = 0; j < UserIndef.size(); j++)
						if (UserIndef[j].Users == VectSentences[iterator].op1_name)
						{
							__offset += 5;
							finded1 = true;
						}
				}
				if (!finded1) __offset += 4;
		}

		else if (VectSentences[iterator].mn_name == "JB")
		{
			finded1 = false;
			for (int j = 0; j < Labels.size(); j++)
				if (Labels[j].Name == VectSentences[iterator].op1_name)
				{
					__offset++;
					finded1 = true;
				}
			if (!finded1) __offset += 5;
		}

		else if (VectSentences[iterator].mn_name == "XOR") //!!!!!!!!!!!!!!!!!!!!!!!!dodelatb
		{
			__offset += 2; // comand + sib
			for (int j = 0; j < Operand1.size(); j++)
			{
				if (Operand1[j].lexVal == "[") __offset++;
				if ((Operand1[j].lexVal == "BP") || (Operand1[j].lexVal == "EDI") || (Operand1[j].lexVal == "ESI")) __offset++;
				if ((Operand1[j].lexVal == "EBP")) __offset += 2;
				for (int j1 = 0; j1 < UserIndef.size(); j1++)
					if (Operand1[j].lexVal == UserIndef[j1].Users)
					{
						if (UserIndef[j1].Direct == "DW") __offset += 2;
						else __offset++;
					}
			}

			/*for (int j = 0; j < Operand2.size(); j++)
			{
				if (Operand2[j].lexVal == "[") __offset++;
				if ((Operand2[j].lexVal == "BP") || (Operand2[j].lexVal == "EDI") || (Operand2[j].lexVal == "ESI")) __offset++;
				if ((Operand2[j].lexVal == "EBP")) __offset += 2;
				for (int j1 = 0; j1 < UserIndef.size(); j1++)
					if (Operand2[j].lexVal == UserIndef[j1].Users)
					{
						if (UserIndef[j1].Direct == "DW") __offset += 2;
						else __offset++;
					}
			}*/
		}

		else if (VectSentences[iterator].mn_name == "CMP")
		{
			__offset += 2; // comand + sib
			for (int j = 0; j < Operand1.size(); j++)
			{
				/*	if (Operand1[j].lexVal == "[") __offset++;
					if ((Operand1[j].lexVal == "BP") || (Operand1[j].lexVal == "EDI") || (Operand1[j].lexVal == "ESI")) __offset++;
					if ((Operand1[j].lexVal == "EBP")) __offset += 2;
					for (int j1 = 0; j1 < UserIndef.size(); j1++)
					if (Operand1[j].lexVal == UserIndef[j1].Users)
					{
						if (UserIndef[j1].Direct == "DW") __offset += 2;
						else __offset++;
					}*/
			}

			for (int j = 0; j < Operand2.size(); j++)
			{
				if (Operand2[j].lexVal == "[") __offset++;
				if ((Operand2[j].lexVal == "BP") || (Operand2[j].lexVal == "EDI") || (Operand2[j].lexVal == "ESI")) __offset++;
				if ((Operand2[j].lexVal == "EBP")) __offset += 2;
				for (int j1 = 0; j1 < UserIndef.size(); j1++)
					if (Operand2[j].lexVal == UserIndef[j1].Users)
					{
						if (UserIndef[j1].Direct == "DW") __offset += 2;
						else __offset++;
					}
			}
		}
		
		else if (VectSentences[iterator].mn_name == "OR") //!!!!!!!!!!!!!!!!!!!!!!!!dodelatb
		{
			__offset += 2; // comand + sib
			for (int j = 0; j < Operand1.size(); j++)
			{
				if (Operand1[j].lexVal == "[") __offset++;
				if ((Operand1[j].lexVal == "BP") || (Operand1[j].lexVal == "EDI") || (Operand1[j].lexVal == "ESI")) __offset++;
				if ((Operand1[j].lexVal == "EBP")) __offset += 2;
				for (int j1 = 0; j1 < UserIndef.size(); j1++)
					if (Operand1[j].lexVal == UserIndef[j1].Users)
					{
						if (UserIndef[j1].Direct == "DW") __offset += 2;
						else __offset++;
					}
			}

			for (int j = 0; j < Operand2.size(); j++)
			{
				if (Operand2[j]._type == HEX_CONST) __offset++;
				else if (Operand2[j]._type == DEC_CONST) __offset++;
				else if (Operand2[j]._type == BIN_CONST) __offset++;
			}
		}

		else if (VectSentences[iterator].mn_name == "DEC")
		{
			__offset += 2; // comand + sib
			for (int j = 0; j < Operand1.size(); j++)
			{
				if (Operand1[j].lexVal == "[") __offset++;
				if ((Operand1[j].lexVal == "BP") || (Operand1[j].lexVal == "EDI") || (Operand1[j].lexVal == "ESI") || (Operand1[j].lexVal == "EBX") || (Operand1[j].lexVal == "EAX") || (Operand1[j].lexVal == "ECX") || (Operand1[j].lexVal == "EDX")) __offset++;
				if ((Operand1[j].lexVal == "EBP")) __offset += 2;
				for (int j1 = 0; j1 < UserIndef.size(); j1++)
					if (Operand1[j].lexVal == UserIndef[j1].Users)
					{
						if (UserIndef[j1].Direct == "DW") __offset += 2;
						else __offset++;
					}
			}
		}
		
		else if (VectSentences[iterator].mn_name == "ADD")
		{
			__offset++; // command
			bool reg16_1 = false, reg16_2 = false;

			for (int j = 0; j < Operand1.size(); j++)
			{
				if (Operand1[j]._type == REG_16) reg16_1 = true;
			}

			for (int j = 0; j < Operand2.size(); j++)
			{
				if (Operand2[j]._type == REG_16) reg16_2 = true;
			}
			
			if (reg16_1 && reg16_2) __offset++;
		}

		else if (VectSentences[iterator].mn_name == "MOV")
		{
			//__offset++;
			for (int j = 0; j < Operand1.size(); j++)
			{
				if (Operand1[j]._type == REG_16) __offset++;
				if (Operand1[j]._type == REG_32) __offset += 3;
				//if bl,hex then Eror
			}
			for (int j = 0; j < Operand2.size(); j++)
			{
				if (Operand2[j]._type == HEX_CONST) __offset += 2;
				else if (Operand2[j]._type == DEC_CONST) __offset++;
				else if (Operand2[j]._type == BIN_CONST) __offset++;
			}
		}
		
		else if (VectSentences[iterator].mn_name == "AND") {
			__offset += 2; // comand + sib
			for (int j = 0; j < Operand1.size(); j++)
			{
				if (Operand1[j].lexVal == "[") __offset++;
				if ((Operand1[j].lexVal == "GS") ||(Operand1[j].lexVal == "CS") ||(Operand1[j].lexVal == "DS")) __offset++;
				if ((Operand1[j].lexVal == "BP") || (Operand1[j].lexVal == "EDI") || (Operand1[j].lexVal == "ESI")) __offset++;
				if ((Operand1[j].lexVal == "EBP")) __offset += 2;
				for (int j1 = 0; j1 < UserIndef.size(); j1++)
					if (Operand1[j].lexVal == UserIndef[j1].Users)
					{
						if (UserIndef[j1].Direct == "DW") __offset += 2;
						else __offset++;
					}
			}
		}
	}
}

//Making DATA offsets
void LexAnalysis::FindDirOffset(int iterator)
{
	if ((VectSentences[iterator].label) && (VectSentences[iterator].dir_name == "DB") && (VectSentences[iterator].constanta_type == BIN_CONST))
	{
		__offset++;
	}
	else if (VectSentences[iterator].dir_name == "DW")
		__offset += 2;
	else if ((VectSentences[iterator].dir_name == "DB") && (VectSentences[iterator].constanta_type == TEXT_CONST))
		__offset += VectSentences[iterator].constanta_name.length() - 2;
	else if (VectSentences[iterator].dir_name == "DD")
		__offset += 4;

	else if (VectSentences[iterator].dir_name == "ENDS")
	{
		if (VectSentences[iterator].label_name == "CODE")
			code__offset = __offset;
		else if (VectSentences[iterator].label_name == "DATA")
			data__offset = __offset;

		__offset = 0;
	}
}

//for table masm
string LexAnalysis::TypeofCounter(int iterator, Counters buff)
{
	string field = "\0";

	if (VectSentences[iterator].dir_name == "DB") field = "L BYTE";
	if (VectSentences[iterator].dir_name == "DW") field = "L WORD";
	if (VectSentences[iterator].dir_name == "DD") field = "L DWORD";

	return field;
}

//Out Table masm
void LexAnalysis::Out_Tables(ofstream &f)
{
	cout << "\n\n" << "\t Name" << "\t\t Size" << "\tLength" << "\tAlign" << "\tCombine Class" << endl;
	f << "\n\n" << "\t Name" << "\t\t\tSize" << "\tLength" << "\tAlign" << "\tCombine Class" << endl;

	f << resetiosflags(ios::right);
	cout << setiosflags(ios::left) << setfill('.') << setw(17) << Segments[0] << "\t 32 bit   " << setiosflags(ios::right) << setfill('0') << setw(4) << hex << data__offset << "\tPARA" << "\tNONE" << endl;
	f << setiosflags(ios::left) << setfill('.') << setw(17) << Segments[0] << "\t 32 bit   " << setiosflags(ios::right) << setfill('0') << setw(4) << hex << data__offset << "\tPARA" << "\tNONE" << endl;

	cout << resetiosflags(ios::right);
	f << resetiosflags(ios::right);

	cout << setiosflags(ios::left) << setfill('.') << setw(17) << Segments[1] << "\t 32 bit   " << setiosflags(ios::right) << setfill('0') << setw(4) << hex << code__offset << "\tPARA" << "\tNONE" << endl;
	f << setiosflags(ios::left) << setfill('.') << setw(17) << Segments[1] << "\t 32 bit   " << setiosflags(ios::right) << setfill('0') << setw(4) << hex << code__offset << "\tPARA" << "\tNONE" << endl;

	cout << resetiosflags(ios::right);
	f << resetiosflags(ios::right);

	cout << "\n\nSymbols:";
	f << "\n\nSymbols:";

	cout << "\n\n" << "\t Name" << "\t\t Type" << "\tValue" << "\tAttr" << endl;
	f << "\n\n" << "\t Name" << "\t\t\tType" << "\tValue" << "\tAttr" << endl;

	for (int i = 0; i < UserIndef.size(); i++)
	{
		cout << setiosflags(ios::left) << setfill('.') << setw(17) << UserIndef[i].Users << "\t" << UserIndef[i].Type << "\t" << setiosflags(ios::right) << setfill('0') << setw(4) << hex << UserIndef[i].offsets << "\t" << UserIndef[i].Attr << endl;
		f << setiosflags(ios::left) << setfill('.') << setw(17) << UserIndef[i].Users << "\t" << UserIndef[i].Type << "\t" << setiosflags(ios::right) << setfill('0') << setw(4) << hex << UserIndef[i].offsets << "\t" << UserIndef[i].Attr << endl;

		cout << resetiosflags(ios::right);
		f << resetiosflags(ios::right);
	}

	for (int i = 0; i < Labels.size(); i++) {
		cout << setiosflags(ios::left) << setfill('.') << setw(17) << Labels[i].Name << "\t" << UserIndef[i].Type << "\t" << setiosflags(ios::right) << setfill('0') << setw(4) << hex << UserIndef[i].offsets << "\t" << UserIndef[i].Attr << endl;
		f << setiosflags(ios::left) << setfill('.') << setw(17) << Labels[i].Name << "\t" << UserIndef[i].Type << "\t" << setiosflags(ios::right) << setfill('0') << setw(4) << hex << UserIndef[i].offsets << "\t" << UserIndef[i].Attr << endl;

		cout << resetiosflags(ios::right);
		f << resetiosflags(ios::right);
	}
}

//Function make all words ot upperkeys
Lex GetToUpper(Lex field)
{
	for (int k = 0; k <= field.lex.length() - 1; k++)
		field.lex[k] = (char)toupper(field.lex[k]);
	return field;
}

//destroyer
LexAnalysis::~LexAnalysis()
{
	TestFile.clear();
	lexs.clear();
	values.clear();
	CountsVal.clear();
	VectSentences.clear();
	Labels.clear();
	UserIndef.clear();
	Segments.clear();
}