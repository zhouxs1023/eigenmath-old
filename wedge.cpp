#include "stdafx.h"

#include "defs.h"

#define U p1
#define V p2
#define W p3

void
wedge2(void)
{
	save();

	V = pop();
	U = pop();

	push(U);
	push(V);
	outer();

	push(V);
	push(U);
	outer();

	subtract();

	restore();
}

void
wedge3(void)
{
	save();

	W = pop();
	V = pop();
	U = pop();

	push(U);	// u v w
	push(V);
	outer();
	push(W);
	outer();

	push(V);	// v w u
	push(W);
	outer();
	push(U);
	outer();

	add();

	push(W);	// w u v
	push(U);
	outer();
	push(V);
	outer();

	add();

	push(V);	// v u w
	push(U);
	outer();
	push(W);
	outer();

	subtract();

	push(U);	// u w v
	push(W);
	outer();
	push(V);
	outer();

	subtract();

	push(W);	// w v u
	push(V);
	outer();
	push(U);
	outer();

	subtract();

	restore();
}

static char *s[] = {

	"u=(u1,u2,u3,u4)",
	"u",

	"v=(v1,v2,v3,v4)",
	"v",

	"w=(w1,w2,w3,w4)",
	"w",

	"wedge(u,v)-outer(u,v)+outer(v,u)",
	"0",

	"wedge(u,v,w)-outer(u,v,w)-outer(v,w,u)-outer(w,u,v)+outer(v,u,w)+outer(u,w,v)+outer(w,v,u)",
	"0",
};

void
test_wedge(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
