expression     → term | not | and | or ;

space          → " " ;

term           → "TERM" space* "(" space* STRING space* ")" ;

not            → "NOT" space* "(" space* expression space* ")" ;

logic_list     → ("AND" | "NOT") space* "(" space* expression+ space* ")" ;

EXAMPLE: OR(AND(TERM( hello ) TERM ( world ) ) AND ( TERM ( alice ) NOT ( TERM ( BOB ) ) )