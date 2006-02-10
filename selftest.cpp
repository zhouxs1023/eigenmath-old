#include "stdafx.h"
#include "defs.h"

char logbuf[1000];
static FILE *logfile;
static jmp_buf jbuf;

void
selftest(void)
{
#if 0 // don't use log file
#ifdef MAC
	static char buf[144];   
	strcpy(buf, getenv("HOME"));
	strcat(buf, "/Desktop/Eigenmath.out");
	logfile = fopen(buf, "w");
#else
	logfile = fopen("selftest.txt", "w");
#endif
#endif

	if (setjmp(jbuf))
		return;

	symbol(BAKE)->u.sym.binding = symbol(BAKE);

	// test bignum arithmetic

	test_madd();
	test_msub();
	test_mmul();
	test_mdiv();
	test_mmod();
	test_mprime();

	test_quickfactor();
	test_mgcd();
	test_mpow();
	test_mroot();
	test_multiply();
	test_scan();
	test_power();
	test_factor_number();
	test_test();
	test_all();
	test_user_func();
	test_tensor();

	test_bake();

	test_abs();
	test_adj();
	test_arg();
	test_besselj();
	test_bessely();
	test_carac();
	test_ceiling();
	test_cofactor();
	test_condense();
	test_contract();
	test_convolution();
	test_denominator();
	test_derivative();
	test_dirac();
	test_erf();
	test_erfc();
	test_expcos();
	test_expsin();
	test_gcd();
	test_factorpoly();
	test_floor();
	test_for();
//	test_fourier();
	test_gamma();
	test_imag();
	test_inner();
	test_lcm();
	test_log();
	test_mag();
	test_mod();
	test_numerator();
	test_outer();
	test_polar();
	test_product();
	test_prog();
	test_quotient();
	test_rationalize();
	test_real();
	test_rect();
	test_sgn();
	test_sum();
	test_taylor();
	test_tchebychevT();
	test_tchebychevU();
	test_trace();
	test_transpose();
	test_zero();

	test_hermite();
	test_laguerre();
	test_legendre();
	test_binomial();
	test_divisors();
	test_coeff();
	test_sin();
	test_cos();
	test_tan();
	test_sinh();
	test_cosh();
	test_tanh();
	test_arcsin();
	test_arcsinh();
	test_arccos();
	test_arccosh();
	test_arctan();
	test_arctanh();
	test_wedge();
	test_index();
	test_isprime();
	test_integral();
	test_simplify();
	test_roots();
	test_eigen();

	logout("OK, all tests passed.\n");

	if (logfile)
		fclose(logfile);
}

void
logout(char *s)
{
	printstr(s);
//	if (logfile)
//		fprintf(logfile, "%s", s);
}

void
errout(void)
{
	logout("\n");
//	if (logfile)
//		fclose(logfile);
	longjmp(jbuf, 1);
}

int test_flag;
extern int out_count;
extern void run(char *);

char *script[] = {

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
	"Stop: divide by zero",

	"1.0/0.0",
	"Stop: divide by zero",

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
//	integral
//
//-----------------------------------------------------------------------------

	"integral(a,x)-a*x",
	"0",

	"integral(a*b,x)-a*b*x",
	"0",

	"integral(x,x)-1/2*x^2",
	"0",

	"integral(a*x,x)-1/2*a*x^2",
	"0",

	"integral(a*b*x,x)",
	"1/2*a*b*x^2",

	"integral(a+b,x)-a*x-b*x",
	"0",

	"integral(1/x,x)",
	"log(x)",

	"integral(x^a,x)",
	"x^(1+a)/(1+a)",

	"integral(exp(x),x)",
	"exp(x)",

	"integral(exp(a*x),x)",
	"exp(a*x)/a",

	"integral(exp(a*x+b),x)-exp(b+a*x)/a",
	"0",

	"integral(x*exp(A*x^2+B),x)-exp(B+A*x^2)/(2*A)",
	"0",

	"integral(log(x),x)+x-x*log(x)",
	"0",

	"integral(log(a*x+b),x)+x-x*log(b+a*x)-b*log(b+a*x)/a",
	"0",

	"integral(sin(x),x)",
	"-cos(x)",

	"integral(cos(x),x)",
	"sin(x)",

	"integral(sin(x)*cos(x),x)",		// 318
	"1/2*sin(x)^2",

	"integral(sin(a*x)*cos(a*x),x)",	// 318
	"sin(a*x)^2/(2*a)",

	// integral w/o 2nd arg

	"integral(1+x+x^2+x^3)-(x+1/2*x^2+1/3*x^3+1/4*x^4)",
	"0",

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
	"0",

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
	"Stop: inverse of singular matrix",

// power of tensor

	"A=((1,2),(3,4))",
	"((1,2),(3,4))",

	"inner(A,1/A)",
	"((1,0),(0,1))",

	"inner(A,A^(-1))",
	"((1,0),(0,1))",

	"inner(A,A)-A^2",
	"((0,0),(0,0))",

	"A=quote(A)",
	"",

// rank

	"rank(A)",
	"0",

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
	"(0,0,0)",

	"div(grad(f))-laplacian(f)",
	"0",

	"curl(curl(F))-grad(div(F))+laplacian(F)",
	"(0,0,0)",

	"grad(f*g)-f*grad(g)-g*grad(f)",
	"(0,0,0)",

	"div(f*F)-f*div(F)-inner(grad(f),F)",
	"0",

	"curl(f*F)-f*curl(F)-cross(grad(f),F)",
	"(0,0,0)",

	"grad(inner(F,G))-inner(G,grad(F))-inner(F,grad(G))",
	"(0,0,0)",

	"grad(inner(F,G))-inner(grad(F),G)-inner(grad(G),F)-cross(G,curl(F))-cross(F,curl(G))",
	"(0,0,0)",

	"div(cross(F,G))-inner(G,curl(F))+inner(F,curl(G))",
	"0",

	"curl(cross(F,G))-F*div(G)+G*div(F)-inner(grad(F),G)+inner(grad(G),F)",
	"(0,0,0)",

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

	// test self-referencing arg

	"f(x)=eval(x)+1",
	"",

	"f(x+1)-(2+x)",
	"0",

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

	// equality of tensors

	"testeq((1,2),(1,2))",
	"1",

	"testeq((1,2),(1,3))",
	"0",

	// the "check" function with tensor arg

	"check((1,2)=(1,2))",
	"",

	// nil

	"nil",
	"nil",
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
