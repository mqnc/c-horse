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

int main (int argc, char *argv[])
{ 
	
	/*

	customparser testparser(R"(
		prio4 <- prio3 '+' prio4 / prio3 '-' prio4 / prio3
		prio3 <- prio2 '*' prio3 / prio2 '/' prio3 / prio2
		prio2 <- prio15 '^' prio2 / prio15
		prio15 <- '+' prio1 / '-' prio1 / prio1
		prio1 <- '(' prio4 ')' / prio0
		prio0 <- [a-z]
	)");
	
	testparser.enable_packrat_parsing();

	RULE(prio1) {OPTIONS{
		case 0:	return SV(0); // "(" + SV(0) + ")";
		case 1:	return SV(0);
	}};

	RULE(prio15) {OPTIONS{
		case 0:	return "pos(" + SV(0) + ")";
		case 1:	return "neg(" + SV(0) + ")";
		case 2: return SV(0);
	}};

	RULE(prio2) {OPTIONS{
		case 0:	return "pow(" + SV(0) + "," + SV(1) + ")";
		case 1:	return SV(0);
	}};

	RULE(prio3) {OPTIONS{
		case 0:	return "mul(" + SV(0) + "," + SV(1) + ")";
		case 1:	return "div(" + SV(0) + "," + SV(1) + ")";
		case 2:	return SV(0);
	}};

	RULE(prio4) {OPTIONS{
		case 0:	return "add(" + SV(0) + "," + SV(1) + ")";
		case 1:	return "sub(" + SV(0) + "," + SV(1) + ")";
		case 2:	return SV(0);
	}};


	string ret;
	bool ok = testparser.parse(R"(+a*-(b+-c)^+d)", ret);
	cout << "testresult: " << ok << endl << ret << endl << endl;


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
