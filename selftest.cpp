// self test functions

#include "stdafx.h"
#include "defs.h"
#include "selftest.h"

static jmp_buf jbuf;

void
selftest(void)
{
	if (setjmp(jbuf))
		return;

#if SELFTEST

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
	test_user_func();
	test_tensor();

	test_bake();

	test(__FILE__, s, sizeof (s) / sizeof (char *)); // "s" is in selftest.h

	test_abs();
	test_adj();
	test_arg();
	test_besselj();
	test_bessely();
	test_carac();
	test_ceiling();
	test_choose();
	test_circexp();
	test_clock();
	test_cofactor();
	test_condense();
	test_contract();
	test_convolution();
	test_defint();
	test_denominator();
	test_derivative();
	test_dirac();
	test_erf();
	test_erfc();
	test_expcos();
	test_expsin();
	test_factorpoly();
	test_float();
	test_floor();
	test_for();
	test_gamma();
	test_gcd();
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
	test_quotient();
	test_rationalize();
	test_real();
	test_rect();
	test_sgn();
	test_sum();
	test_taylor();
	test_tchebychevT();
	test_tchebychevU();
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
	test_index();
	test_isprime();
	test_integral();
	test_simplify();
	test_roots();
	test_eigen();

#endif

	mini_test();

	logout("OK, all tests passed.\n");
}

void
logout(char *s)
{
	printstr(s);
}

void
errout(void)
{
	logout("\n");
	longjmp(jbuf, 1);
}

void
test(char *file, char **s, int n)
{
	int i;
	char *t;

	test_flag = 1;

	clear_symbols();

	defn();

	run("e=quote(e)");

	for (i = 0; i < n; i++) {

		logout(s[i]);
		logout("\n");

		if (s[i][0] == '#')
			continue;

		out_count = 0;

		run(s[i]);

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
