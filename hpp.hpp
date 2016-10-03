#ifndef _H_HPP
#define _H_HPP

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Env;
struct Sym {
	string tag,val; Sym(string,string); Sym(string); int line;
	vector<Sym*> nest; void push(Sym*);
	virtual string head(); string pad(int); virtual string dump(int=0);
	virtual Sym* eval(Env*);
};
struct Env:Sym { Env(string); map<string,Sym*> lookup; };
extern Env env;

struct Scalar:Sym { Scalar(string,string); Sym*eval(Env*); };

struct Num:Scalar { Num(string); float val; string head(); };
struct Int:Scalar { Int(string); int val; string head(); };
struct Hex:Scalar { Hex(string); };
struct Bin:Scalar { Bin(string); };

struct Str:Scalar { Str(string); string head(); };

struct Op:Sym { Op(string); Sym*eval(Env*); };

struct Vector:Sym { Vector(); string head(); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); \
	yylval.o->line=yylineno; \
	return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_HPP
