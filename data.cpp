#include "stdafx.h"
#include "defs.h"

int endian = 1;

U *p1;
U *p2;
U *p3;
U *p4;
U *p5;
U *p6;
U *p7;
U *p8;

U *zero, *one, *imaginaryunit;

U symtab[NSYM], *binding[NSYM], *arglist[NSYM];

int expanding;
int verbosing;
int esc_flag;
int draw_flag;
int test_flag;
char logbuf[1000];
