#include "stdafx.h"
#include "defs.h"

int test_flag;
extern int out_count;
extern void run(char *);
extern FILE *logfile;

#define DEBUG 0

char *script[] = {

#if GMP

// gmp

	"2/3",
	"2/3",

	"2.0/3",
	"0.666666666666666666667e0",

	"2/3.0",
	"0.666666666666666666667e0",

	"2.0/3.0",
	"0.666666666666666666667e0",

	"-2.0/3.0",
	"-0.666666666666666666667e0",

#else

	"2/3",
	"2/3",

	"2.0/3",
	"0.666667",

	"2/3.0",
	"0.666667",

	"2.0/3.0",
	"0.666667",

	"-2.0/3.0",
	"-0.666667",

#endif

// symbols

	"a=quote(a)",
	"a",

	"b=quote(b)",
	"b",

	"c=quote(c)",
	"c",

	"d=quote(d)",
	"d",

// scanner

	"a 5",			// 2nd factor is T_INTEGER
	"5*a",

	"a 5.0",		// 2nd factor is T_DOUBLE
	"5*a",

//	"a \"hello\"",		// 2nd factor is T_STRING
//	"\"hello\"*a",

// print format

	"2*1/a",
	"2/a",

	"-1/a",
	"-1/a",

	"a/b",
	"a/b",

//

	"2^(1/2)",
	"2^(1/2)",

	"r*exp(i*phi)",		// sort order (see ~e in misc.c)
	"r*exp(i*phi)",

	// string arguments are printed with quotes

//	"quote(print(a,\" \",b))",
//	"print(a,\" \",b)",

//	"float(2/3)",
//	"0.666667",

/* from yacas to do list */

	"2380105039001857 * 1260448573",
	"3000000000000000000000061",

// floating point

	"A=float(1000!)",
	"",

#if 0
#ifdef MAC

	"A",
	"Inf",

	"A-A",
	"NaN",

	"A/A",
	"NaN",
#else
	"A",
	"inf",

	"A-A",
	"nan",

	"A/A",
	"nan",
#endif
#endif

	"A=quote(A)",
	"",

// divide by zero

	"1/0",
	"divide by zero",

	"1.0/0.0",
	"divide by zero",

/* sum */

	"0+0",
	"0",

	"0+a",
	"a",

	"a+0",
	"a",

	"0+(a+b)",
	"a+b",

	"(a+b)+0",
	"a+b",

	"1+2",
	"3",

	"a+0",
	"a",

	"0+a",
	"a",

	"a+a",
	"2*a",

	"a+2*a",
	"3*a",

	"2*a+a",
	"3*a",

	"2*a+3*a",
	"5*a",

	"a*b+a*b",
	"2*a*b",

	"a*b+2*a*b",
	"3*a*b",

	"2*a*b+a*b",
	"3*a*b",

	"2*a*b+3*a*b",
	"5*a*b",

	"1/2*a+1/2*a",
	"a",

	"1/2*a*b+1/2*a*b",
	"a*b",

	"a+a+a",
	"3*a",

	"a+b+b",
	"a+2*b",

	"2*a+2*a",
	"4*a",

	"1/4*a-1/4*a",
	"0",

	"(a+b+c+d)-(a+b+c+d)",
	"0",

/* power */

	"0^0",	// see ross' book, p. 129
	"1",

	"a^0",
	"1",

	"a^1",
	"a",

	"1^a",
	"1",

	"a^a",
	"a^a",

	"2^(1/2)",
	"2^(1/2)",

//	"a^(b+c)",
//	"a^b*a^c",

//	"a^(b+c+d)",
//	"a^b*a^c*a^d",

	"(a*b)^c",
	"a^c*b^c",

	"(a*b*c)^d",
	"a^d*b^d*c^d",

	"2^3",
	"8",

	"a^(2*a)",
	"a^(2*a)",

	"a^(2*a*b)",
	"a^(2*a*b)",

	"(a^2)^3",
	"a^6",

	"a^2^3",
	"a^8",
/*
	"12^(1/2)",
	"2*3^(1/2)",

	"12^(-1/2)",
	"1/2*(1/3)^(1/2)",

	"8^(2/3)",
	"4",

	"8^(-2/3)",
	"1/4",

	"2^(3/2)",
	"2*2^(1/2)",

	"(3/2)^(-1/2)",
	"(2/3)^(1/2)",

	"(3/4)^(1/2)",
	"1/2*3^(1/2)",

	"(9/4)^(1/2)",
	"3/2",

	"(9/4)^(3/2)",
	"27/8",

	"3*3^(-1/2)",
	"3^(1/2)",
*/
	"(a^b)^c",
	"a^(b*c)",

	"((a+b)^(-2))^(-1)",
//	"a^2+b^2+2*a*b",
	"2*a*b+a^2+b^2",

#if 0
	"(-27)^(1/3)",
	"-3",
#endif

	// make sure scanner doesn't produce imaginary

	"quote((-a/b)^(1/2))",
	"(-a/b)^(1/2)",

#if 0

/* roots of large numbers */

	"x=10^20",
	"x",

	"x^(1/2)",
	"10000000000",

	"x^(-1/2)",
	"1/10000000000",

#endif

//-----------------------------------------------------------------------------
//
//	log
//
//-----------------------------------------------------------------------------

	"log(1)",
	"0",

	"log(exp(1))",
	"1",

	"log(exp(x))",
	"x",

	"exp(log(x))",
	"x",

	"log(x^2)",
	"2*log(x)",

	"log(1/x)",
	"-log(x)",

	"log(a^b)",
	"b*log(a)",

	"log(2)",
	"log(2)",

	"log(2.0)",
	"0.693147",

	"float(log(2))",
	"0.693147",

//-----------------------------------------------------------------------------
//
//	arctan
//
//-----------------------------------------------------------------------------

#if 0

/* arctan2 */

	"arctan2(0,0)",
	"0",

	"arctan2(0,1)",
	"0",

	"arctan2(0,-1)",
	"pi",

	"arctan2(1,0)",
	"1/2*pi",

	"arctan2(-1,0)",
	"-1/2*pi",

/* sin */

	"sin(-pi)",		// -180 degrees
	"0",

	"sin(-pi*5/6)",		// -150 degrees
	"-1/2",

	"sin(-3*pi/4)",		// -135 degrees
	"-1/2*2^(1/2)",

	"sin(-2*pi/3)",		// -120 degrees
	"-1/2*3^(1/2)",

	"sin(-pi/2)",		// -90 degrees
	"-1",

	"sin(-pi/3)",		// -60 degrees
	"-1/2*3^(1/2)",

	"sin(-pi/4)",		// -45 degrees
	"-1/2*2^(1/2)",

	"sin(-pi/6)",		// -30 degrees
	"-1/2",

	"sin(0)",		// 0 degrees
	"0",

	"sin(pi/6)",		// 30 degrees
	"1/2",

	"sin(pi/4)",		// 45 degrees
	"1/2*2^(1/2)",

	"sin(pi/3)",		// 60 degrees
	"1/2*3^(1/2)",

	"sin(pi/2)",		// 90 degrees
	"1",

	"sin(2*pi/3)",		// 120 degrees
	"1/2*3^(1/2)",

	"sin(3*pi/4)",		// 135 degrees
	"1/2*2^(1/2)",

	"sin(pi*5/6)",		// 150 degrees
	"1/2",

	"sin(pi)",		// 180 degrees
	"0",

	"sin(pi*7/6)",		// 210 degrees
	"-1/2",

/* cos */

	"cos(-pi)",		// -180 degrees
	"-1",

	"cos(-pi*5/6)",		// -150 degrees
	"-1/2*3^(1/2)",

	"cos(-pi*3/4)",		// -135 degrees
	"-1/2*2^(1/2)",

	"cos(-pi*2/3)",		// -120 degrees
	"-1/2",

	"cos(-pi/2)",		// -90 degrees
	"0",

	"cos(-pi/3)",		// -60 degrees
	"1/2",

	"cos(-pi/4)",		// -45 degrees
	"1/2*2^(1/2)",

	"cos(-pi/6)",		// -30 degrees
	"1/2*3^(1/2)",

	"cos(0)",		// 0 degrees
	"1",

	"cos(pi/6)",		// 30 degrees
	"1/2*3^(1/2)",

	"cos(pi/4)",		// 45 degrees
	"1/2*2^(1/2)",

	"cos(pi/3)",		// 60 degrees
	"1/2",

	"cos(pi/2)",		// 90 degrees
	"0",

	"cos(pi*2/3)",		// 120 degrees
	"-1/2",

	"cos(pi*3/4)",		// 135 degrees
	"-1/2*2^(1/2)",

	"cos(pi*5/6)",		// 150 degrees
	"-1/2*3^(1/2)",

	"cos(pi)",		// 180 degrees
	"-1",

#endif

//-----------------------------------------------------------------------------
//
//	functions of symbols
//
//-----------------------------------------------------------------------------

	"log(a)",
	"log(a)",

	"exp(a)",
	"exp(a)",

	"cos(a)",
	"cos(a)",

	"sin(a)",
	"sin(a)",

	"tan(a)",
	"tan(a)",

	"arccos(a)",
	"arccos(a)",

	"arcsin(a)",
	"arcsin(a)",

	"arctan(a)",
	"arctan(a)",

//-----------------------------------------------------------------------------
//
//	functions of floating point
//
//-----------------------------------------------------------------------------

#if GMP == 0

	"log(2.0)",
	"0.693147",

	"exp(2.0)",
	"7.38906",

	"cos(1.2)",
	"0.362358",

	"sin(1.2)",
	"0.932039",

	"tan(1.2)",
	"2.57215",

	"arccos(.12)",
	"1.45051",

	"arcsin(.12)",
	"0.12029",

	"arctan(.12)",
	"0.119429",

	"sqrt(-2.0)",
	"1.41421*i",

#endif

/* powers of negative numbers */

#if 0
	"(-1)^(2/3)",
	"1",

	"(-1)^(-2/3)",
	"1",

	"(-2)^(1/2)",
	"i*2^(1/2)",
#endif

// complex numbers

	"i",
	"i",

	"i^2",
	"-1",

	"1/i",
	"-i",

	"(1/i)^2",
	"-1",

	"(-1)^(1/2)",
	"i",

	"conj(x+i*y)",
	"x-i*y",

	"conj((-1)^(1/3))",
	"-(-1)^(2/3)",

	"conj((-1)^(2/3))",
	"-(-1)^(1/3)",

	"conj((-1)^(1/10))",
	"-(-1)^(9/10)",

	"conj((-1)^(4/3))",
	"(-1)^(2/3)",

	"conj((-1)^(7/3))",
	"-(-1)^(2/3)",

	"(3+2*i)*(1+4*i)",
	"-5+14*i",
#if 0
	"(-1)^(1/3)",
	"1/2+1/2*i*3^(1/2)",

	"a=i^(1/3)",
	"a",

	"a*a*a",
	"i",

	"a=(1/3*i)^(1/3)",
	"a",

	"a*a*a",
	"1/3*i",

	"a=(-1/3*i)^(1/3)",
	"a",

	"a*a*a",
	"-1/3*i",

#endif

#if 0

	"A=4*(a+b)",
	"A",

	"B=-3*(a+b)",
	"B",

	"C=2*(a+b)",
	"C",

#endif

#if 0

	"A+B+C",
	"3*(a+b)",

	"A+C+B",
	"3*(a+b)",

	"B+A+C",
	"3*(a+b)",

	"B+C+A",
	"3*(a+b)",

	"C+A+B",
	"3*(a+b)",

	"C+B+A",
	"3*(a+b)",
#endif
	"A=quote(A)",
	"A",

	"B=quote(B)",
	"B",

//-----------------------------------------------------------------------------
//
//	derivative
//
//-----------------------------------------------------------------------------

#if 0	// now in derivative.c

	"x=quote(x)",
	"",

	"f=quote(f)",
	"",

	"g=quote(g)",
	"",

	"d(a,x)",
	"0",

	"d(x,x)",
	"1",

	"d(x^2,x)",
	"2*x",

	"d(log(x),x)",
	"1/x",

	"d(exp(x),x)",
	"exp(x)",

	"d(a^x,x)",
	"a^x*log(a)",

	"d(x^x,x)-(x^x+x^x*log(x))",
	"0",

	"d(log(x^2+5),x)",
	"2*x/(5+x^2)",

	"d(d(f(x),x),y)",
	"0",

	"d(d(f(x),y),x)",
	"0",

	"d(d(f(y),x),y)",
	"0",

	"d(d(f(y),y),x)",
	"0",

	"d((x*y*z,y,x+z),(x,y,z))",
	"((y*z,x*z,x*y),(0,1,0),(1,0,1))",

	"d(x+z,(x,y,z))",
	"(1,0,1)",

	"d(cos(theta)^2,cos(theta))",
	"2*cos(theta)",

	"d(f())",
	"d(f(),x)",

	"d(x^2)",
	"2*x",

	"d(t^2)",
	"2*t",

	"d(t^2 x^2)",
	"2*t^2*x",

#endif

//-----------------------------------------------------------------------------
//
//	integral
//
//-----------------------------------------------------------------------------

	"integral(a,x)",
	"a*x",

	"integral(a*b,x)",
	"a*b*x",

	"integral(x,x)",
	"1/2*x^2",

	"integral(a*x,x)",
	"1/2*a*x^2",

	"integral(a*b*x,x)",
	"1/2*a*b*x^2",

	"integral(a+b,x)",
	"a*x+b*x",

	"integral(1/x,x)",
	"log(x)",

	"integral(x^a,x)",
	"x^(1+a)/(1+a)",

	"integral(exp(x),x)",
	"exp(x)",

	"integral(exp(a*x),x)",
	"exp(a*x)/a",

	"integral(exp(a*x+b),x)",
	"exp(b+a*x)/a",

	"integral(x*exp(A*x^2+B),x)",
	"exp(B+A*x^2)/(2*A)",

	"integral(log(x),x)",
	"-x+x*log(x)",

	"integral(log(a*x+b),x)",
	"-x+x*log(b+a*x)+b*log(b+a*x)/a",

	"integral(sin(x),x)",
	"-cos(x)",

	"integral(cos(x),x)",
	"sin(x)",

	"integral(sin(x)*cos(x),x)",		// 318
	"1/2*sin(x)^2",

	"integral(sin(a*x)*cos(a*x),x)",	// 318
	"sin(a*x)^2/(2*a)",

	// integral w/o 2nd arg

	"integral(1+x+x^2+x^3)",
	"x+1/2*x^2+1/3*x^3+1/4*x^4",

//-----------------------------------------------------------------------------
//
//	dsolve
//
//-----------------------------------------------------------------------------

//	"dsolve(d(y(x),x)-2*x*y(x)-x,y(x),x)",
//	"-1/2+C*exp(x^2)",

//-----------------------------------------------------------------------------
//
//	sum of tensors
//
//-----------------------------------------------------------------------------

	"((a,b),(c,d))+((1,2),(3,4))",
	"((1+a,2+b),(3+c,4+d))",

	// mixed rank

	"(b1,b2,b3)+((a11,a12,a13),(a21,a22,a23),(a31,a32,a33))",
	"(b1,b2,b3)+((a11,a12,a13),(a21,a22,a23),(a31,a32,a33))",

//----------------------------------------------------------------------------
//
//	scalar times tensor
//
//-----------------------------------------------------------------------------

	"c=((1,1),(1,1))",
	"((1,1),(1,1))",

	"a*b*c",
	"((a*b,a*b),(a*b,a*b))",

	"c*d*e",
	"((d*e,d*e),(d*e,d*e))",

	"a*b*c*d*e",
	"((a*b*d*e,a*b*d*e),(a*b*d*e,a*b*d*e))",

	"c=quote(c)",
	"c",

// det

	"det(a)",
	"det(a)",

	"det(((1,2),(3,4)))",
	"-2",

	"det(((2,3,-2,5),(6,-2,1,4),(5,10,3,-2),(-1,2,2,3)))",
	"-1629",

	"det(A)",
	"det(A)",

	"det(((A/(A-B),1),(B/(A-B),1)))",
	"A/(A-B)-B/(A-B)",			// add "simplify" to get 1

	// make sure the sign of det is handled for row interchange

	"det(((1,0,0),(0,0,1),(0,1,0)))",
	"-1",

	"det(((a11-x,a12),(a21,a22-x)))-(a11*a22-a11*x-a12*a21-a22*x+x^2)",
//	"a11*a22-a11*x-a12*a21-a22*x+x^2",
	"0",

// from ginac time_lw_O.cpp
//
//	"det(("
//	"(a6, a5, a4, a3, a2, a1, 0,  0,  0,  0,  0,  0,  0,  0,  0 ),"
//	"(0,  0,  a6, 0,  a5, a4, 0,  a3, a2, a1, 0,  0,  0,  0,  0 ),"
//	"(0,  a6, 0,  a5, a4, 0,  a3, a2, a1, 0,  0,  0,  0,  0,  0 ),"
//	"(0,  0,  0,  a6, 0,  0,  a5, a4, 0,  0,  a3, a2, a1, 0,  0 ),"
//	"(0,  0,  0,  0,  a6, 0,  0,  a5, a4, 0,  0,  a3, a2, a1, 0 ),"
//	"(0,  0,  0,  0,  0,  a6, 0,  0,  a5, a4, 0,  0,  a3, a2, a1),"
//	"(0,  0,  0,  b6, 0,  0,  b5, b4, 0,  0,  b3, b2, b1, 0,  0 ),"
//	"(0,  0,  0,  0,  b6, 0,  0,  b5, b4, 0,  0,  b3, b2, b1, 0 ),"
//	"(0,  b6, 0,  b5, b4, 0,  b3, b2, b1, 0,  0,  0,  0,  0,  0 ),"
//	"(0,  0,  b6, 0,  b5, b4, 0,  b3, b2, b1, 0,  0,  0,  0,  0 ),"
//	"(0,  0,  0,  0,  0,  b6, 0,  0,  b5, b4, 0,  0,  b3, b2, b1),"
//	"(0,  0,  0,  0,  0,  c6, 0,  0,  c5, c4, 0,  0,  c3, c2, c1),"
//	"(0,  0,  c6, 0,  c5, c4, 0,  c3, c2, c1, 0,  0,  0,  0,  0 ),"
//	"(0,  c6, 0,  c5, c4, 0,  c3, c2, c1, 0,  0,  0,  0,  0,  0 ),"
//	"(0,  0,  0,  0,  c6, 0,  0,  c5, c4, 0,  0,  c3, c2, c1, 0 ) "
//	"))",

//-----------------------------------------------------------------------------
//
//	 adj
//
//-----------------------------------------------------------------------------

	"adj(a)",
	"adj(a)",

	"adj(((1,2),(3,4)))",
	"((4,-2),(-3,1))",

	"adj(((2,3,-2,5),(6,-2,1,4),(5,10,3,-2),(-1,2,2,3)))",
	"((-4,-177,-73,194),(-117,117,-99,-27),(310,-129,-44,-374),(-130,-51,71,-211))",

	"adj(A)",
	"adj(A)",

//-----------------------------------------------------------------------------
//
//	inv
//
//-----------------------------------------------------------------------------

	"inv(a)",
	"inv(a)",

	"invg(a)",
	"invg(a)",

	"inv(((1,2),(3,4)))",
	"((-2,1),(3/2,-1/2))",

	"inner(((1,2),(3,4)),inv(((1,2),(3,4))))",
	"((1,0),(0,1))",

	"inv(hilbert(3))",
	"((9,-36,30),(-36,192,-180),(30,-180,180))",

	"invg(hilbert(3))",
	"((9,-36,30),(-36,192,-180),(30,-180,180))",

	"inv(((a,a),(a,a)))",
	"inverse of singular matrix",

// power of tensor

	"A=((1,2),(3,4))",
	"((1,2),(3,4))",

	"inner(A,1/A)",
	"((1,0),(0,1))",

	"inner(A,A^(-1))",
	"((1,0),(0,1))",

	"inner(A,A)-A^2",
	"0",

//-----------------------------------------------------------------------------
//
//	transpose
//
//-----------------------------------------------------------------------------

	"transpose(a)",
	"transpose(a,1,2)",

	"transpose(((a,b),(c,d)))",
	"((a,c),(b,d))",

	"transpose(a,b,c)",
	"transpose(a,b,c)",

	"transpose(((a,b),(c,d)),1,2)",
	"((a,c),(b,d))",

	"transpose(((a,b,c),(d,e,f)),1,2)",
	"((a,d),(b,e),(c,f))",

	"transpose(((a,d),(b,e),(c,f)),1,2)",
	"((a,b,c),(d,e,f))",

//-----------------------------------------------------------------------------
//
//	contract
//
//-----------------------------------------------------------------------------

	"contract(a,b,c)",
	"contract(a,b,c)",

	"contract(((a,b),(c,d)))",
	"a+d",

	"contract(((1,2),(3,4)),1,2)",
	"5",

	"A=((a11,a12),(a21,a22))",
	"((a11,a12),(a21,a22))",

	"B=((b11,b12),(b21,b22))",
	"((b11,b12),(b21,b22))",

	"contract(outer(A,B),2,3)",
	"((a11*b11+a12*b21,a11*b12+a12*b22),(a21*b11+a22*b21,a21*b12+a22*b22))",

	"A=quote(A)",
	"",

	"B=quote(B)",
	"",

// rank

	"rank(A)",
	"rank(A)",

	"rank(1)",
	"0",

	"rank((a,b))",
	"1",

	"rank(((a,b),(c,d)))",
	"2",

// setup for vector identities

	"cross(u,v) = ("
	"  u[2] v[3] - u[3] v[2],"
	"  u[3] v[1] - u[1] v[3],"
	"  u[1] v[2] - u[2] v[1])",
	"",

	"div(v) = contract(d(v,(x,y,z)),1,2)",
	"",

	"grad(v) = d(v,(x,y,z))",
	"",

	"curl(f) = ("
	"  d(f[3],y) - d(f[2],z),"
	"  d(f[1],z) - d(f[3],x),"
	"  d(f[2],x) - d(f[1],y))",
	"",

	"laplacian(f) = d(d(f,x),x) + d(d(f,y),y) + d(d(f,z),z)",
	"",

//-----------------------------------------------------------------------------
//
//	gradient
//
//-----------------------------------------------------------------------------

	"d(f(x),x)",
	"d(f(x),x)",

	"d(f(x,y,z),(x,y,z))",
	"(d(f(x,y,z),x),d(f(x,y,z),y),d(f(x,y,z),z))",

	"d((f(x),g(x)),x)",
	"(d(f(x),x),d(g(x),x))",

	"grad(V())",
	"(d(V(),x),d(V(),y),d(V(),z))",

//-----------------------------------------------------------------------------
//
//	curl
//
//-----------------------------------------------------------------------------

	"curl((X(),Y(),Z()),(x,y,z))",
	"(-d(Y(),z)+d(Z(),y),d(X(),z)-d(Z(),x),-d(X(),y)+d(Y(),x))",

	"curl((X(),Y(),Z()))",
	"(-d(Y(),z)+d(Z(),y),d(X(),z)-d(Z(),x),-d(X(),y)+d(Y(),x))",

//-----------------------------------------------------------------------------
//
//	vector identities from AMA205
//
//-----------------------------------------------------------------------------

	"F=(FX(),FY(),FZ())",
	"F",

	"G=(GX(),GY(),GZ())",
	"G",

	"f=f()",
	"f",

	"g=g()",
	"g",

	"div(curl(F))",
	"0",

	"curl(grad(f))",
	"0",

	"div(grad(f))-laplacian(f)",
	"0",

	"curl(curl(F))-grad(div(F))+laplacian(F)",
	"0",

	"grad(f*g)-f*grad(g)-g*grad(f)",
	"0",

	"div(f*F)-f*div(F)-inner(grad(f),F)",
	"0",

	"curl(f*F)-f*curl(F)-cross(grad(f),F)",
	"0",

	"grad(inner(F,G))-inner(G,grad(F))-inner(F,grad(G))",
	"0",

	"grad(inner(F,G))-inner(grad(F),G)-inner(grad(G),F)-cross(G,curl(F))-cross(F,curl(G))",
	"0",

	"div(cross(F,G))-inner(G,curl(F))+inner(F,curl(G))",
	"0",

	"curl(cross(F,G))-F*div(G)+G*div(F)-inner(grad(F),G)+inner(grad(G),F)",
	"0",

//-----------------------------------------------------------------------------
//
//	simplifyfactorials
//
//-----------------------------------------------------------------------------

#if 0

	"simplifyfactorials((a+2)!/a!)",
	"(1+a)*(2+a)",

	"simplifyfactorials(a!/(a+2)!)",
	"1/((1+a)*(2+a))",

#endif

// hilbert

	"det(hilbert(6))",
	"1/186313420339200000",

// normalize angle

	"(-1)^(8/3)",
	"(-1)^(2/3)",

	"(-1)^(7/3)",
	"(-1)^(1/3)",

	"(-1)^(5/3)",
	"-(-1)^(2/3)",

	"(-1)^(4/3)",
	"-(-1)^(1/3)",

	"(-1)^(2/3)",
	"(-1)^(2/3)",

	"(-1)^(1/3)",
	"(-1)^(1/3)",

	"(-1)^(-1/3)",
	"-(-1)^(2/3)",

	"(-1)^(-2/3)",
	"-(-1)^(1/3)",

	"(-1)^(-4/3)",
	"(-1)^(2/3)",

	"(-1)^(-5/3)",
	"(-1)^(1/3)",

	"(-1)^(-7/3)",
	"-(-1)^(2/3)",

	"(-1)^(-8/3)",
	"-(-1)^(1/3)",

	// power() can return a multiply, make sure multiply() handles it

//	"-1/2*i*(-exp(-i*pi/6)+exp(i*pi/6))",
//	"1/2*(-1)^(1/3)-1/2*(-1)^(2/3)",

// from the jargon file

	"1000!",

	"40238726007709377354370243392300398571937486421071"
	"46325437999104299385123986290205920442084869694048"
	"00479988610197196058631666872994808558901323829669"
	"94459099742450408707375991882362772718873251977950"
	"59509952761208749754624970436014182780946464962910"
	"56393887437886487337119181045825783647849977012476"
	"63288983595573543251318532395846307555740911426241"
	"74743493475534286465766116677973966688202912073791"
	"43853719588249808126867838374559731746136085379534"
	"52422158659320192809087829730843139284440328123155"
	"86110369768013573042161687476096758713483120254785"
	"89320767169132448426236131412508780208000261683151"
	"02734182797770478463586817016436502415369139828126"
	"48102130927612448963599287051149649754199093422215"
	"66832572080821333186116811553615836546984046708975"
	"60290095053761647584772842188967964624494516076535"
	"34081989013854424879849599533191017233555566021394"
	"50399736280750137837615307127761926849034352625200"
	"01588853514733161170210396817592151090778801939317"
	"81141945452572238655414610628921879602238389714760"
	"88506276862967146674697562911234082439208160153780"
	"88989396451826324367161676217916890977991190375403"
	"12746222899880051954444142820121873617459926429565"
	"81746628302955570299024324153181617210465832036786"
	"90611726015878352075151628422554026517048330422614"
	"39742869330616908979684825901254583271682264580665"
	"26769958652682272807075781391858178889652208164348"
	"34482599326604336766017699961283186078838615027946"
	"59551311565520360939881806121385586003014356945272"
	"24206344631797460594682573103790084024432438465657"
	"24501440282188525247093519062092902313649327349756"
	"55139587205596542287497740114133469627154228458623"
	"77387538230483865688976461927383814900140767310446"
	"64025989949022222176590433990188601856652648506179"
	"97023561938970178600408118897299183110211712298459"
	"01641921068884387121855646124960798722908519296819"
	"37238864261483965738229112312502418664935314397013"
	"74285319266498753372189406942814341185201580141233"
	"44828015051399694290153483077644569099073152433278"
	"28826986460278986432113908350621709500259738986355"
	"42771967428222487575867657523442202075736305694988"
	"25087968928162753848863396909959826280956121450994"
	"87170124451646126037902930912088908694202851064018"
	"21543994571568059418727489980942547421735824010636"
	"77404595741785160829230135358081840096996372524230"
	"56085590370062427124341690900415369010593398383577"
	"79394109700277534720000000000000000000000000000000"
	"00000000000000000000000000000000000000000000000000"
	"00000000000000000000000000000000000000000000000000"
	"00000000000000000000000000000000000000000000000000"
	"00000000000000000000000000000000000000000000000000"
	"000000000000000000",

// float

	"float(2/3)",
	"0.666667",

	"float(hilbert(3))",
	"((1,0.5,0.333333),(0.5,0.333333,0.25),(0.333333,0.25,0.2))",

	"a=pi",
	"a",

	"float(a)",
	"3.14159",

	"a=exp(1)",
	"a",

	"float(a)",
	"2.71828",

	"a=quote(a)",
	"a",

// bondi metric

//#include "bondi.h"

// done

	// test self-referencing arg

	"f(x)=eval(x)+1",
	"",

	"f(x+1)",
	"2+x",

	// test indexed formal arg

	"f(x)=do(x[1]=3,x)",
	"",

	"x=(a,b)",
	"",

	"f(x)",
	"(3,b)",

	"x",
	"(a,b)",

	"f=quote(f)",
	"",

	"x=quote(x)",
	"",

	// last

	"a=2+3",
	"",

	"last",
	"5",

	"a=quote(a)",
	"",
};

void
test_all(void)
{
	test(__FILE__, script, sizeof(script) / sizeof (char *));
}

extern char out_buf[];
extern int out_count;

void
test(char *file, char **s, int n)
{
	int i;
	char *t;

	test_flag = 1;

	p1 = symbol(TTY);

	p1->u.sym.binding = _one;

	for (i = 0; i < n; i++) {

		logout(s[i]);
		logout("\n");

		if (s[i][0] == '#')
			continue;

		out_count = 0;

		run(s[i]);

		t = s[i];

		while (*t && *t != '=')
			t++;

		if (*t == '=') {
			i++;
			continue;
		}

		out_buf[out_count] = 0;

		t = out_buf;

		// skip leading newlines

		while (*t == '\n')
			t++;

		// remove trailing newlines

		while (out_count && out_buf[out_count - 1] == '\n')
			out_buf[--out_count] = 0;

		i++;

		if (strcmp(t, s[i]) == 0)
			continue;

		// make copy because logout clobbers out_buf

		t = strdup(t);

		logout("expected to get the following result:\n");
		logout(s[i]);
		logout("\n");

		logout("got this result instead:\n");
		logout(t);
		logout("\n");

		logout(file);
		logout("\n");

		free(t);

		errout();
	}

	test_flag = 0;
}

#if 0

char *det80 = 
"1/9903010146699347787886767841019251068775920711435760136192929763795589379494607033654082430094012086908258603804981968613136151608820622611800504364949247152051889256750844014558604691343543879795947579129772886171487843947543116738228870048023876847598769618432427621504941591150536395499386836658326675434662277436302072055981870442974761994232454738217506571538795735494601779413534251600461625745824477506058007252289680617403698140398503094557176024932815686703664456794134709042556312677902915544594836818338955188701166891317686578824305357274037082150685844584109148320916809921412548851213809695519455327726540655505169277454420920627587423212363169177948333091968613022468249106586652375080829722630087771007332483925453770621956941748035558359930335385877091781053751896858660642071814471599382383786462631002044654090027603114546212567613484702341755836354201900114217556388239820670302993429659517313842166889839017541156193534053186736884746675439833971049076846488111618762167121025674441716673159998757255502357085126757264575255781187688255894383848936724656938057360029259846234713508297716363827406726734015333472622736845442748904312890369820698622599837181346495266276218125095015097932578637105255274725208514912502591441021680579613830502633325612645696763307224839465010091190520239151025561265515299890032609042052434873379499983571131629777525985270392853542772746713408381187524566023545561481632404399740357811085425523080284209753796284171854847696406008994518245339105806382437528410858227643215090286388308883075867090151118356695848870817011106972771105506361770181746812104496969080485324062777813589243323973589166502001005271653636862637590785234535600035259939214536063067586515639757710391461569789031145994398596345687290921666653440765600880213086173089125587884998426448568473047253449471154968891768010301560334365460710581104226731177185342590329162847045276424538095625746077973964097243879002876170814148078558727750111888834039856917275740408177982477962885812538522897594572079973216868191259410014047264304537771867249628343771161217955733079170358256354890675099101225405795168810291599751593025730960964502992540214501956279919103718647780919352996524648143504356020654886727891787705434664835263033316694011140180962096866290059299507490761905682905084269683831117262258550892327768229273612760382725268090206010052572201540746148513171637929060370326633494915940219512971448543099826947754996182619411280768742233191239379867239275004417265389214370176210023269274149422153397078460803284041671799524621258453810854499305302782812315837128030587171737275744090320947537825586737865587571294447407413956860249714933893944931100367674800288520493347189171421518993187871530722732091088415909016024509810392489175293562543089496406784430803150411025796765010714296098444433542074031124282787679194264948187378502775374735489600363247335651062940790198176952578737085680579092043918513537963410017847529517902955894818773547040790597098507620452239197194687940166742246461140583561272538349344579608601669170139469634813764673891366229703167820285513587043701736259067023997133501266436388333531352678653481249213151021188703354090265982858059655851882365978955811506671602025002157147015904357961476857463328037801204373050773468190057140923088024340982123505317754654444135238949200440946797331078479237217051086739350195180598060918619152282617347104953705617533469457594275643240367457949879933465822467374553439108846106725863706545300738854102903419605745137951613315349936650332924147234101200890281257077861171726083109117738680692369444757831680000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

// ginac does this in 5.64 seconds (see misc/test-h.cc)

// this program: 2.72 seconds

// passing NULL to times() on Macintosh causes "Bus error"

void
test_h(void)
{
	struct tms x;
	unsigned int t;
	push_integer(80);
	hilbert();
	t = times(&x);
	det();
	t = times(&x) - t;
	p1 = pop();
	scan(det80);
	p2 = pop();
	if (equal(p1, p2))
		printf("result ok\n");
	else
		printf("wrong result\n");
	printf("%d.%02d seconds\n", t / 100, t % 100);

#if 0

	int r, c, n = 80;
	unsigned int t;

	gc();

	t = times(NULL);

	p1 = alloc_tensor(n * n);

	p1->u.tensor->ndim = 2;
	p1->u.tensor->dim[0] = n;
	p1->u.tensor->dim[1] = n;

	for (r = 0; r < n; r++) {
		for (c = 0; c < n; c++) {
			push_integer(r + c + 1);
			inverse();
			p1->u.tensor->elem[n * r + c] = pop();
		}
	}

	push(p1);
	det();
	p2 = pop();
	print(stdout, p2);

	// another way

	push_integer(1);
	for (r = 0; r < n; r++) {
		push_integer(r);
		factorial();
		push_integer(2);
		power();
		multiply();
		for (c = 0; c < n; c++) {
			push(p1->u.tensor->elem[n * r + c]);
			multiply();
		}
	}
	p3 = pop();

	if (equal(p2, p3))
		printf("test H passed\n");
	else
		printf("test H failed\n");

	print(stdout, p2);
	print(stdout, p3);

	t = times(NULL) - t;

	printf("%d.%02d seconds\n", t / 100, t % 100);
#endif
}

int
trandom()
{
	int t;
	while (1) {
		t = random();
		if (t)
			break;
	}
	return t;
}

void
test_num(void)
{
	int i;
	unsigned int t;
	t = times(NULL);
	for (i = 0; i < 100000; i++) {
		push_integer(random() % 1000000 + 1);
		push_integer(random() % 1000000 + 1);
		divide();
		push_integer(random() % 1000000 + 1);
		push_integer(random() % 1000000 + 1);
		divide();
		divide();
	}
	t = times(NULL) - t;
	printf("%d.%02d seconds\n", t / 100, t % 100);
}

void
test_a(void)
{
	int i;
	unsigned int t;
	struct tms tms;

	printf("timing Lewis-Wester test A (divide factorials) ");

	fflush(stdout);

	t = times(&tms);

	for (i = 1; i < 101; i++) {
		push_integer(1000 + i);
		factorial();
		push_integer(900 + i);
		factorial();
		divide();
		pop();
	}

	t = times(&tms) - t;
	printf("%d.%02d seconds\n", t / 100, t % 100);
}

void
test_b(void)
{
	int i;
	unsigned int t;
	struct tms tms;

	printf("timing Lewis-Wester test B (sum of rational numbers) ");

	fflush(stdout);

	t = times(&tms);

	push(_zero);

	for (i = 1; i < 1001; i++) {
		push_integer(i);
		inverse();
		add();
	}

	pop();

	t = times(&tms) - t;
	printf("%d.%02d seconds\n", t / 100, t % 100);
}

void
test_c(void)
{
	int i, x, y;
	unsigned int t;
	struct tms tms;

	printf("timing Lewis-Wester test C (gcd of big integers) ");

	fflush(stdout);

	x = 13 * 17 * 31;
	y = 13 * 19 * 29;

	t = times(&tms);

	for (i = 1; i < 201; i++) {
		push_integer(x);
		push_integer(300 + i % 181);
		power();
		push_integer(y);
		push_integer(200 + i % 183);
		power();
		gcd();
		p1 = pop();
	}

	t = times(&tms) - t;

	scan("53174994123961114423610399251974962981084780166115806651505844915220196792416194060680805428433601792982500430324916963290494659936522782673704312949880308677990050199363768068005367578752699785180694630122629259539608472261461289805919741933");

	p2 = pop();

	if (equal(p1, p2))
		printf("passed ");
	else
		printf("failed ");

	printf("%d.%02d seconds\n", t / 100, t % 100);
}

#define Y p3
#define T p4

// FIXME add polynomial factoring (see -k below)

void
test_d(void)
{
	int k;
	unsigned int t;
	struct tms tms;

	printf("timing Lewis-Wester test D (normalized sum of rational functions) ");

	fflush(stdout);

	scan("y");
	Y = pop();

	scan("t");
	T = pop();

	t = times(&tms);

	// sum over k from 1 to 10: k*y*t^k / (y+k*t)^k

	push(_zero);
	for (k = 1; k < 11; k++) {
		push_integer(k);
		push(Y);
		multiply();
		push(T);
		push_integer(k);
		power();
		multiply();
		push(Y);
		push_integer(k);
		push(T);
		multiply();
		add();
		push_integer(-k); // -k so it's not expanded
		power();
		multiply();
		add();
	}

	rationalize();
	eval();

	t = times(&tms) - t;

	scan(test_d_result); // see ginac.h
	eval();

	p2 = pop();
	p1 = pop();

	if (equal(p1, p2))
		printf("passed ");
	else {
		printf("\n");
		printf("p1 = \n");
		print(stdout, p1);
		printf("p2 = \n");
		print(stdout, p2);
		printf("failed ");
	}

	printf("%d.%02d seconds\n", t / 100, t % 100);
}

void
test_i(void)
{
	struct tms x;
	unsigned int t;
	printf("timing Lewis-Wester test I (invert rank 40 Hilbert) ");
	push_integer(40);
	hilbert();
	t = times(&x);
	invg();
	pop();
	t = times(&x) - t;
	printf("%d.%02d seconds\n", t / 100, t % 100);
}

void
test_k(void)
{
	struct tms tms;
	unsigned int t;
	printf("timing Lewis-Wester test K (invert rank 70 Hilbert) ");
	push_integer(70);
	hilbert();
	t = times(&tms);
	invg();
	pop();
	t = times(&tms) - t;
	printf("%d.%02d seconds\n", t / 100, t % 100);
}

#endif

