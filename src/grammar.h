
#pragma once
#include "customparser.h"
#define RULE(name) p[#name] = [](const SemanticValues& sv)
#define SV const SemanticValues& sv

// $awa /*comment*/ "\"string\"" "easy" 'h' '\\' '\''  R"furz(awa)furz" R"bubi(awa)furz")bubi"$
// unsigned int varname := xxx;
auto test(){return R"(function f[inline] (int a:=xxx) -> (int b:=xxx) := x end;)";}

auto makeGrammar(){return R"(

Start <- nl (GlobalThing)* _

AssignOperator <- ':='

GlobalThing <- VerbatimCpp / GlobalDeclaration

VerbatimCpp <- CppLimiter CppCode* CppLimiter _
CppLimiter <- '$'
CppCode <- CppComment / CppStringLiteral / CppAnything
CppComment <- <CppSingleLineComment / CppMultiLineComment>
CppSingleLineComment <- '//' (!NL .)* NL
CppMultiLineComment <- '/*' (!'*/' .)* '*/'
CppStringLiteral <- <CppCharConstant / CppSimpleString / CppMultiLineString>
CppCharConstant <- '\'' (('\\' .) / .) '\''
CppSimpleString <- '"' (('\\' .) / (!'"' .))* '"'
CppMultiLineString <- 'R"' $delim<[a-zA-Z_0-9]*> '(' (!(')' $delim '"') .)* ')' $delim '"'
CppAnything <- (!CppLimiter .)

GlobalDeclaration <- (SimpleDeclaration / FunctionDeclaration) _

SimpleDeclaration <- Specifier* Declaree (AssignOperator _ Placeholder)? NL
Specifier <- !Declaree Identifier
Declaree <- Identifier (&AssignOperator / &NL)

FunctionDeclaration <- 'function' WS Identifier OptionalSpecifierList OptionalParameters OptionalReturnValues OptionalBody FunctionEnd NL
OptionalSpecifierList <- ('[' _ Identifier* ']' )? _ 
OptionalParameters <- ('(' _ ParameterDeclarationList ')')? _ 
ParameterDeclarationList <- ParameterDeclaration (',' _ ParameterDeclaration)* / _
ParameterDeclaration <- ParameterSpecifier* Parameter (AssignOperator _ Placeholder)?
ParameterSpecifier <- !Parameter Identifier
Parameter <- Identifier (&AssignOperator / &',' / &')')
OptionalReturnValues <- ('->' _ '(' _ ReturnValueList ')')? _
ReturnValueList <- ParameterDeclaration (',' _ ParameterDeclaration)* / _
OptionalBody <- AssignOperator nl (!FunctionEnd .)* 
FunctionEnd <- 'endfunction' / 'end'

Identifier <- <([a-zA-Z_] [a-zA-Z_0-9]* / VerbatimCpp)> _

OpKw <- 'operator'

Prio0 <- Identifier / '(' Expression ')'
Prio1 <- Prio0 '::' Prio1 / Prio0 OpKw '1(' OpName ')' Prio1 / Prio0
Prio2 <- Prio1 '++' / Prio1 '--' / Prio1 '(' Expression ')' / Prio1 '[' Expression ']' / Prio1 '.' Prio2 / Prio1 OpKw '2(' OpName ')' / Prio1
Prio3 <- '++' Prio2 / '--' Prio2 / '+' Prio2 / '-' Prio2 / '!' Prio2 / '~' Prio2 / '(' Expression ')' Prio2 / '*' Prio2 / '&' Prio2 / OpKw '3(' OpName ')' Prio2 / Prio2
Prio4 <- Prio3 '.*' Prio4 / Prio3 '->*' Prio4 / Prio3 OpKw '4(' OpName ')' Prio4 / Prio3
Prio5 <- Prio4 '*' Prio5 / Prio4 '/' Prio5 / Prio4 '%' Prio5 / Prio4 OpKw '5(' OpName ')' Prio5 / Prio4
Prio6 <- Prio5 '+' Prio6 / Prio5 '-' Prio6 / Prio5 OpKw '6(' OpName ')' Prio6 / Prio5
Prio7 <- Prio6 '<<' Prio7 / Prio6 '>>' Prio7 / Prio6 OpKw '7(' OpName ')' Prio7 / Prio6
Prio8 <- Prio7 '<' Prio8 / Prio7 '<=' Prio8 / Prio7 '>' Prio8 / Prio7 '>=' Prio8 / Prio7 OpKw '8(' OpName ')' Prio8 / Prio7
Prio9 <- Prio8 '==' Prio9 / Prio8 '!=' Prio9 / Prio8 OpKw '9(' OpName ')' Prio9 / Prio8
Prio10 <- Prio9 '&' Prio10 / Prio9 OpKw '10(' OpName ')' Prio10 / Prio9
Prio11 <- Prio10 '^' Prio11 / Prio10 OpKw '11(' OpName ')' Prio11 / Prio10
Prio12 <- Prio11 '|' Prio12 / Prio11 OpKw '12(' OpName ')' Prio12 / Prio11
Prio13 <- Prio12 '&&' Prio13 / Prio12 OpKw '13(' OpName ')' Prio13 / Prio12
Prio14 <- Prio13 '||' Prio14 / Prio13 OpKw '14(' OpName ')' Prio14 / Prio13
Prio15 <- Prio14 '?' Prio14 ':' Prio14 / Prio15 '=' Prio14
# / Prio15 '+=' Prio14 / Prio15 '-=' Prio14 / Prio15 '*=' Prio14 / Prio15 '/=' Prio14 / Prio15 '%=' Prio14 / Prio15 '<<=' Prio14 / Prio15 '>>=' Prio14 / Prio15 '&=' Prio14 / Prio15 '^=' Prio14 / Prio15 '|=' Prio14 / Prio15 OpKw '15(' OpName ')' Prio14 / Prio14

Expression <- Placeholder
OpName <- Identifier


Op <- Placeholder
Sym <- '@'

WS <- <([ \t] / ('...' _ NL))> # definite whitespace
_ <- WS? # optional whitespace
NL <- <([;\n] _)+> # definite new line (consuming all new lines)
nl <- NL? _ # optional new line

Placeholder <- <'x'*> _

)";}




void makeRules(customparser& p){
	


	RULE(CppAnything) {
		return sv.token();
	};
	
	p["CppCode"] = concat;
	p["_"] = concat;
	p["CppSimpleStringCharacter"] = concat;
	p["CppStringEscapedCharacter"] = concat;
	
	
	
	
};

