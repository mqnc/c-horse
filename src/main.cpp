#include <stdio.h>
//#include "lua.hpp"
#include "peglib.h"
#include "customparser.h"
#include "grammar.h"
#include <fstream>
#include <string>

using namespace peg;
using namespace std;

#define READFILE(STR, FILE) {ifstream ifs(FILE); STR.assign( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));}

#define RULE(name) testparser[#name] = [](const SemanticValues& sv)
#define SV(I) sv[I].get<string>()
#define OPTIONS switch (sv.choice())

int main(int argc, char *argv[])
{

	auto syntax = R"(
        # Grammar for Calculator...
		Sub <- Subtitive
        Subtitive    <- Sub '-' Multitive / Multitive
        Multitive   <- Primary '*' Multitive / Primary
        Primary     <- '(' Subtitive ')' / Number
        Number      <- < [0-9]+ >
        %whitespace <- [ \t]*
    )";

	parser parser(syntax);

	if (!parser){
		cout << "error creating grammar";
		system("PAUSE");
		return EXIT_FAILURE;
	}

	// (3) Setup actions
	parser["Subtitive"] = [](const SemanticValues& sv) {
		switch (sv.choice()) {
		case 0:  // "Multitive '-' Subtitive"
			return sv[0].get<int>() - sv[1].get<int>();
		default: // "Multitive"
			return sv[0].get<int>();
		}
	};

	parser["Multitive"] = [](const SemanticValues& sv) {
		switch (sv.choice()) {
		case 0:  // "Primary '*' Multitive"
			return sv[0].get<int>() * sv[1].get<int>();
		default: // "Primary"
			return sv[0].get<int>();
		}
	};

	parser["Number"] = [](const SemanticValues& sv) {
		return stoi(sv.token(), nullptr, 10);
	};

	// (4) Parse
	//parser.enable_packrat_parsing(); // Enable packrat parsing.

	int val;
	parser.log = [&](size_t ln, size_t col, const string& msg) {
		cout << "(" << ln << "," << col << ") " << msg;
	};
	parser.parse(" 10 - 5 - 1 ", val);

	cout << "testresult: " << val << endl << endl;


	/*/


	string grammar = makeGrammar();
	customparser parser(grammar);

	if (!parser) {
		cout << "error parsing grammar";
		system("PAUSE");
		return EXIT_FAILURE;
	}

	parser.enable_packrat_parsing();

	// create custom actions
	makeRules(parser);
		
	string val;
	//cout << parser.parse(R"($$)", val) << endl << endl;
	string src = test();
	cout << src << endl << endl;
	auto result = parser.parse(src, val);
	if (result) { cout << "################### SUCCESS!!! ################" << endl; }
	else        { cout << "################### FAILURE!!! ################" << endl; }
	cout << val << endl << endl << endl;
	
	/**/

	system("PAUSE");
	return 0;
}
