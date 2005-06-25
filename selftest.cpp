#include "stdafx.h"

#include "defs.h"

extern void test_abs(void);
extern void test_arccos(void);
extern void test_arccosh(void);
extern void test_arcsin(void);
extern void test_arcsinh(void);
extern void test_arctan(void);
extern void test_arctanh(void);
extern void test_ceiling(void);
extern void test_charpoly(void);
extern void test_coeff(void);
extern void test_condense(void);
extern void test_contract(void);
extern void test_cos(void);
extern void test_cosh(void);
extern void test_denominator(void);
extern void test_derivative(void);
extern void test_display(void);
extern void test_divisors(void);
extern void test_eigen(void);
extern void test_expcos(void);
extern void test_expsin(void);
extern void test_factor_number(void);
extern void test_factorpoly(void);
extern void test_for(void);
extern void test_floor(void);
extern void test_index(void);
extern void test_inner(void);
extern void test_integral(void);
extern void test_isprime(void);
extern void test_lcm(void);
extern void test_log(void);
extern void test_madd(void);
extern void test_mdiv(void);
extern void test_mgcd(void);
extern void test_mmod(void);
extern void test_mmul(void);
extern void test_mod(void);
extern void test_mpow(void);
extern void test_mprime(void);
extern void test_mroot(void);
extern void test_msub(void);
extern void test_multiply(void);
extern void test_numerator(void);
extern void test_outer(void);
extern void test_power(void);
extern void test_product(void);
extern void test_prog(void);
extern void test_rationalize(void);
extern void test_roots(void);
extern void test_scan(void);
extern void test_simplify(void);
extern void test_sin(void);
extern void test_sinh(void);
extern void test_sum(void);
extern void test_tan(void);
extern void test_tanh(void);
extern void test_taylor(void);
extern void test_tensor(void);
extern void test_test(void);
extern void test_trace(void);
extern void test_transpose(void);
extern void test_user_func(void);
extern void test_wedge(void);
extern void test_zero(void);

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
	//test_display();
	test_power();
	test_factor_number();
	test_test();
	test_all();
	test_user_func();
	test_tensor();

	test_abs();
	test_ceiling();
	test_charpoly();
	test_condense();
	test_contract();
	test_denominator();
	test_derivative();
	test_expcos();
	test_expsin();
	test_gcd();
	test_factorpoly();
	test_floor();
	test_for();
	test_inner();
	test_lcm();
	test_log();
	test_mod();
	test_numerator();
	test_outer();
	test_product();
	test_prog();
	test_rationalize();
	test_sum();
	test_taylor();
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
