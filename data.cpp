#include "stdafx.h"

#include "defs.h"

U *p1;
U *p2;
U *p3;
U *p4;
U *p5;
U *p6;
U *p7;
U *p8;
U *last;
U *tmp;
U *nil;
U *formal_arg[6];

struct symbol *symtbl[64];

U *table;
U *a;
U *b;
U *x;
U *yya;
U *yyb;
U *yyx;

U *_zero;
U *_one;
U *_minus_one;
U *unit_imaginary;
U *plus_one_half;

int expanding;
int conjugating;
int verbosing;
int floating;
