#pragma once
#include <iomanip> 
#include <vector>
#include <iostream>
#include "Value.h"
#include "Capital.h"

class LexAnalysis
{
	unsigned short __offset, code__offset, data__offset;

	typedef struct {
		bool label;
		bool dir;
		bool mn;
		bool constanta;
		bool op1;
		bool op2;
		bool coma;
		bool doubleDot;

		unsigned short label_pos;
		unsigned short dir_pos;
		unsigned short dir_count;
		unsigned short constanta_pos;
		unsigned short mn_pos;
		unsigned short mn_count;
		unsigned short op1_pos;
		unsigned short op1_count;
		unsigned short op2_count;
		unsigned short op2_pos;

		Type_of_lex	op1_type;
		Type_of_lex	op2_type;
		Type_of_lex constanta_type;

		string op1_name;
		string op2_name;
		string dir_name;
		string label_name;
		string constanta_name;
		string mn_name;

		vector<Value> Lexinline;
	}LexsLines;

	typedef struct {
		string Users;
		unsigned short offsets;
		string Attr;
		string Type;
		string Direct;
	}Counters;

	typedef struct {
		string Name;
		unsigned short offsets;
		string Attr;
		string Type;
	}Label;

	vector<Lex> lexs;
	vector<Value> values;
	vector<string> TestFile;
	vector<int> CountsVal;
	vector<LexsLines> VectSentences;
	vector<Label> Labels;
	vector<string> Segments;
	vector<Counters> UserIndef;
	vector<Value>Operand1;
	vector<Value>Operand2;

public:
	LexAnalysis(const char *testfile, const char* lst);
	~LexAnalysis();
	void Splite();
	void Make_Value_Vector();
	void MakeVectSentences(vector<Value> field);
	void PrintVectSenteces(ofstream &f);
	void Out_Tables(ofstream &f);
	void BuffMake(int iterator);
	void FindCommandOffset(int);
	void FindDirOffset(int);
	string TypeofCounter(int, Counters);
	bool ifconstanta(int iterator1, int interator2);
	bool ifoperand(int iterator1, int interator2);
	void AllFlagsInNull(int iterator);
};

Lex GetToUpper(Lex field);