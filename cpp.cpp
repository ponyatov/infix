#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { return yyparse(); }

Sym::Sym(string T,string V) { tag=T; val=V; line=0; }
Sym::Sym(string V):Sym("sym",V){}
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::head() { ostringstream os; 
	os<<"<"<<tag<<":"<<val<<"> @ "<<this;
	if (line) os<<" line:"<<line;
	return os.str(); }
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

Sym* Sym::pfxadd() { return new Sym("+"+val); }
Sym* Sym::pfxsub() { return new Sym("-"+val); }

Sym* Sym::add(Sym*o) { return new Error(head()+" + "+o->head()); }
Sym* Sym::mul(Sym*o) { return new Error(head()+" * "+o->head()); }

Scalar::Scalar(string T,string V):Sym(T,V){}
Sym* Scalar::eval(Env*e) { return this; }

Error::Error(string V):Scalar("error",V) { yyerror(val); }

Num::Num(string V):Scalar("num",V){ val=atof(V.c_str()); }
Num::Num(float F):Scalar("num","") { val=F; }
string Num::head() { ostringstream os;
	os<<"<"<<tag<<":"<<val<<"> line:"<<line; return os.str(); }
Sym* Num::pfxadd() { val=+val; return this; }
Sym* Num::pfxsub() { val=-val; return this; }
Sym* Num::add(Sym*o) {
	if (o->tag=="int")	return new Num(val + dynamic_cast<Int*>(o)->val);
	if (o->tag=="num")	return new Num(val + dynamic_cast<Num*>(o)->val);
	else				return Sym::mul(o); }
Sym* Num::mul(Sym*o) {
	if (o->tag=="int")	return new Num(val * dynamic_cast<Int*>(o)->val);
	if (o->tag=="num")	return new Num(val * dynamic_cast<Num*>(o)->val);
	else				return Sym::mul(o); }

Int::Int(string V):Scalar("int",V){ val=atoi(V.c_str()); }
Int::Int(int I):Scalar("int","") { val=I; }
string Int::head() { ostringstream os;
	os<<"<"<<tag<<":"<<val<<"> line:"<<line; return os.str(); }
Sym* Int::pfxadd() { val=+val; return this; }
Sym* Int::pfxsub() { val=-val; return this; }
Sym* Int::add(Sym*o) {
	if (o->tag=="int")	return new Int(val + dynamic_cast<Int*>(o)->val);
	if (o->tag=="num")	return new Num(val + dynamic_cast<Num*>(o)->val);
	else				return Sym::mul(o); }
Sym* Int::mul(Sym*o) {
	if (o->tag=="int")	return new Int(val * dynamic_cast<Int*>(o)->val);
	if (o->tag=="num")	return new Num(val * dynamic_cast<Num*>(o)->val);
	else				return Sym::mul(o); }

Hex::Hex(string V):Scalar("hex",V){}
Bin::Bin(string V):Scalar("bin",V){}

Str::Str(string V):Scalar("str",V){}
string Str::head() { ostringstream os;
	os<<"<"<<tag<<":'"<<val<<"'> line:"<<line; return os.str(); }

Op::Op(string V):Sym("op",V){}
Sym* Op::eval(Env*env) { 
	if (val=="~") return nest[0]; else Sym::eval(env);
	if (nest.size()==1) {
		if (val=="+") return nest[0]->pfxadd();
		if (val=="-") return nest[0]->pfxsub();
	}
	if (nest.size()==2) {
		if (val=="+") return nest[0]->add(nest[1]);
		if (val=="*") return nest[0]->mul(nest[1]);
	}
	return this;
}

Vector::Vector():Sym("vector","[]"){}
string Vector::head() { ostringstream os;
	os<<"[] line:"<<line; return os.str(); }

Env::Env(string V):Sym("env",V){}
Env env("global");
