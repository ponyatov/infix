syntax match Comment	"\v#[^\n]*"
syntax match Operator	"\v[\+\-\*\/\^\=\@]"
syntax match Type		"\v[\[\]]"
"syntax match Symbol		"\v\*[^ \n]+"
"syntax match Symbol		"\v[a-zA-Z0-9_\.]+"
syntax match Constant	"\v(file)[^a-z]"
syntax match Number		"\v[\+\-]?[0-9]+(\.[0-9]+)?([eE][\+\-]?[0-9]+)?"

