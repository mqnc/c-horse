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
#define STR get<string>()

int main (int argc, char *argv[])
{ 

	string grammar = makeGrammar();
	customparser parser(grammar);

	if (!parser) {
		cout << "error parsing grammar";
		system("PAUSE");
		return EXIT_FAILURE;
	}

	parser.enable_packrat_parsing();

	// default action is to concat all semantic values or if those are empty all tokens
	auto concat = [](const SemanticValues& sv) {
		string result;
		if (sv.size() == 0) {
			for (auto t : sv.tokens) {
				result += string(t.first, t.second);
			}
		}
		for(auto s:sv){
			result += s.STR;
		}
		return result;
	};

	for(auto rule=parser.m_grammar->begin(); rule!=parser.m_grammar->end(); rule++){
		const auto rulename = rule->first.c_str();
		cout << rulename << enld;
		
		// noisy concat:
		parser[rulename] = [rulename, concat](const SemanticValues& sv){
			auto result = concat(sv);
			cout << rulename << ":"	<< result << endl;
			return result;
		};

		// silent concat:
		//parser[rule->first.c_str()] = concat;
	}

	// create custom actions
	makeRules(parser);
		
	string val;
	cout << parser.parse(R"($awa /*comment*/ "\"string\""$)", val) << endl << endl;
	cout << val << endl;
	
	system("PAUSE");
   return 0;
}
