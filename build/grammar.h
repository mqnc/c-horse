
#pragma once
#include "customparser.h"

auto makeGrammar(){return R"(

Start <- _ GlobalThing*
GlobalThing <- VerbatimCpp _

# / GlobalDeclaration _

VerbatimCpp <- '$' (!'$' .) '$'

WS <- <[ \t]+>
_ <- <[ \t]*>
NL <- <[\n]>

)";}

void makeRules(customparser& p){
	
	
	
	
	
	
	
	
	
};


/*


CppCode <- CppComment / CppStringLiteral / .
CppComment <- CppSingleLineComment / CppMultiLineComment
CppSingleLineComment <- <'//' (!NL .)* NL>
CppMultiLineComment <- <'/*' (!'* /' .)* '* /'>
CppStringLiteral <- CppSingleCharacter / CppSimpleString / CppMultilineString
#CppSingleCharacter <- '\'' ('\\' . / (!'\'' .)) '\'
#CppSimpleString <- '"' ('\\' . / (!'"' .)) '"'

Identifier <- <[a-zA-Z_] [a-zA-Z_0-9]>





*/