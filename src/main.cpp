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

namespace op
{
	any add(const any& lhs, const any& rhs) {
		return lhs.get<int>() + rhs.get<int>();
	}
	any sub(const any& lhs, const any& rhs) {
		return lhs.get<int>() - rhs.get<int>();
	}
	any mul(const any& lhs, const any& rhs) {
		return lhs.get<int>() * rhs.get<int>();
	}
	any div(const any& lhs, const any& rhs) {
		return lhs.get<int>() / rhs.get<int>();
	}

	any reduceOperatorSequence(const SemanticValues& sv) {
		any result = sv[0];
		for (size_t i = 1; i != sv.size(); i += 2) {
			const auto op = sv[i].get<any(*)(const any&, const any&)>();
			result = op(result, sv[1 + i]);
		}
		return result;
	}
}

int main(int argc, char *argv[])
{

	const auto syntax = R"(
		# Grammar for Calculator...
		Additive <- (Multitive AddOp)* Multitive
		AddOp <- '+' / '-'
		Multitive <- (Primary MulOp)* Primary
		MulOp <- '*' / '/'
		Primary <- '(' Additive ')'
			 / Number
		Number <- < [0-9]+ >

		%whitespace <- [ \t]*
	)";

	parser parser(syntax);

	if (!parser){
		cout << "error creating grammar";
		system("PAUSE");
		return EXIT_FAILURE;
	}

	// (3) Setup actions
	parser["Additive"] = op::reduceOperatorSequence;

	parser["AddOp"] = [](const SemanticValues& sv) {
		switch (sv.choice()) {
		case 0: // "'+'"
			return &op::add;
		default: // "'-'"
			return &op::sub;
		}
	};

	parser["Multitive"] = op::reduceOperatorSequence;

	parser["MulOp"] = [](const SemanticValues& sv) {
		switch (sv.choice()) {
		case 0: // "'*'"
			return &op::mul;
		default: // "'/'"
			return &op::div;
		}
	};

	parser["Number"] = [](const SemanticValues& sv) {
		return stoi(sv.token(), nullptr, 10);
	};

	// (4) Parse
	//parser.enable_packrat_parsing(); // Enable packrat parsing.

	parser.log = [](const size_t ln, const size_t col, const string& msg) {
		cout << "(" << ln << "," << col << ") " << msg;
	};

	const char* const tests[] = {
		" 1 + 2 ",
		" 1 - 2 ",
		" 10 - 5 - 1 ",
		"10 - 5 + 1",
		"10 + 5 - 1",
		"4 / 2",
		"1 + 3 * 4",
		"(1 + 3) * 4",
	};
	for (const auto& test : tests)
	{
		int val;
		parser.parse(test, val);
		cout << test << ": " << val << endl;
	}
	cout << endl;


	/*/


	string grammar = makeGrammar();
	customparser parser(grammar);

	if (!parser) {
		cout << "error parsing grammar";
		string line;
		getline(cin, line);
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

	string line;
	getline(cin, line);
	return 0;
}
