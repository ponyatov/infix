#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { return yyparse(); }

Sym::Sym(string T,string V) { tag=T; val=V; }
Sym::Sym(string V):Sym("sym",V){}
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::head() { ostringstream os;
	os<<"<"<<tag<<":"<<val<<"> line:"<<line; return os.str(); }
string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+head();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval(Env*env) {
	Sym*E = env->lookup[val]; if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval(env);
	return this; }

Scalar::Scalar(string T,string V):Sym(T,V){}
Sym* Scalar::eval(Env*e) { return this; }

Num::Num(string V):Scalar("num",V){ val=atof(V.c_str()); }
string Num::head() { ostringstream os;
	os<<"<"<<tag<<":"<<val<<"> line:"<<line; return os.str(); }

Int::Int(string V):Scalar("int",V){ val=atoi(V.c_str()); }
string Int::head() { ostringstream os;
	os<<"<"<<tag<<":"<<val<<"> line:"<<line; return os.str(); }

Hex::Hex(string V):Scalar("hex",V){}
Bin::Bin(string V):Scalar("bin",V){}

Str::Str(string V):Scalar("str",V){}
string Str::head() { ostringstream os;
	os<<"<"<<tag<<":'"<<val<<"'> line:"<<line; return os.str(); }

Op::Op(string V):Sym("op",V){}
Sym* Op::eval(Env*) {
	if (val=="~") return nest[0];
	if (nest.size()==1) {
		if (val=="+") return nest[0];
	}
	return this;
}

Vector::Vector():Sym("vector","[]"){}
string Vector::head() { ostringstream os;
	os<<"[] line:"<<line; return os.str(); }

Env::Env(string V):Sym("env",V){}
Env env("global");
