#include <stdio.h>
#include "lua.hpp"
#include "peglib.h"
#include <string>

using namespace peg;
using namespace std;

int main (int argc, char *argv[])
{ 
/*
   int m = 36, n = 27, result;
   lua_State *L = luaL_newstate();
   luaL_openlibs(L);
   result = luaL_loadfile(L, "script.lua") || lua_pcall(L, 0, 0, 0);
   if (result){
      fprintf(stderr, "Fehler beim Laden des Skripts: %s!\n", lua_tostring(L, -1));
   }

   lua_getglobal(L, "ggT");
   lua_pushnumber(L, m);
   lua_pushnumber(L, n);

   if (lua_pcall(L, 2, 1, 0))
      fprintf(stderr,"Fehler in ggT: %s\n", lua_tostring(L, -1));

   fprintf (stdout, "Der ggT von %d und %d ist %d\n",m ,n , lua_tointeger(L, -1));

   lua_close(L);  
*/   

	/*
    auto syntax = R"(
        LOOP        <- 'for' NUMBER '<' IDENTIFIER '<' NUMBER 'do' BODY 'loop'
		NUMBER      <- [0-9]+
		IDENTIFIER  <- [a-zA-Z_] [a-zA-Z0-9_]*
		BODY        <- IDENTIFIER
    )";*/
	
	auto syntax = R"(
        WORD        <- ABC / ABD
		ABC         <- A B C
		ABD         <- A B D
		A           <- 'a'
		B           <- 'b'
		C           <- 'c'
		D           <- 'd'
    )";

    parser parser(syntax);

	parser["ABC"].enter = [](any& dt) {		std::cout << "entering ABC" << std::endl;	};
	parser["ABD"].enter = [](any& dt) {		std::cout << "entering ABD" << std::endl;	};	
	parser["A"].enter = [](any& dt) {		std::cout << "entering A" << std::endl;	};
	parser["B"].enter = [](any& dt) {		std::cout << "entering B" << std::endl;	};
	parser["C"].enter = [](any& dt) {		std::cout << "entering C" << std::endl;	};	
	parser["D"].enter = [](any& dt) {		std::cout << "entering D" << std::endl;	};
	parser["ABC"].leave = [](any& dt) {		std::cout << "leaving ABC" << std::endl;	};
	parser["ABD"].leave = [](any& dt) {		std::cout << "leaving ABD" << std::endl;	};	
	parser["A"].leave = [](any& dt) {		std::cout << "leaving A" << std::endl;	};
	parser["B"].leave = [](any& dt) {		std::cout << "leaving B" << std::endl;	};
	parser["C"].leave = [](any& dt) {		std::cout << "leaving C" << std::endl;	};	
	parser["D"].leave = [](any& dt) {		std::cout << "leaving D" << std::endl;	};
	
/*	parser["ABC"] = [](const SemanticValues& sv, any& dt) {
		std::cout << "yeah ABC" << std::endl;
		return sv[0].get<string>() + sv[1].get<string>() + sv[2].get<string>();
	};
	parser["ABD"] = [](const SemanticValues& sv, any& dt) {
		std::cout << "yeah ABD" << std::endl;
		return sv[0].get<string>() + sv[1].get<string>() + sv[2].get<string>();
	};	*/
	
	#define S get<string>()
	
	auto concat = [](const SemanticValues& sv) {
		string result;
		for(auto s:sv){result += s.S;}
		return result;
	};
	
	parser["ABC"] = concat;
	parser["ABD"] = concat;
	
	parser["A"] = [](const SemanticValues& sv) {
		std::cout << "yeah A" << std::endl;
		return string("Ar");
	};
	parser["B"] = [](const SemanticValues& sv) {
		std::cout << "yeah B" << std::endl;
		return string("Beh");
	};
	parser["C"] = [](const SemanticValues& sv) {
		std::cout << "yeah C" << std::endl;	
		return string("Ceh");
	};	
	parser["D"] = [](const SemanticValues& sv) {
		std::cout << "yeah D" << std::endl;
		return string("Deh");
	};

	
/*
    // (3) Setup actions
    parser["Additive"] = [](const SemanticValues& sv) {
        switch (sv.choice()) {
        case 0:  // "Multitive '+' Additive"
            return sv[0].get<int>() + sv[1].get<int>();
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
*/
    // (4) Parse
    //parser.enable_packrat_parsing(); // Enable packrat parsing.

	parser.enable_packrat_parsing(); // Enable packrat parsing.
	
	string val;
	
    cout << parser.parse("ab d", val) << endl << endl;
	
	cout << val;
   
   return 0;
}
