/* Simplify factorials

This script:

	F(n,k) = k binomial(n,k)
	(F(n,k)+F(n,k-1))/F(n+1,k)

Generates the following output:

       k! n!             n! (1 - k + n)!              k! n!
 -------------------- + -------------------- - ----------------------
 (-1 + k)! (1 + n)!     (1 + n)! (-k + n)!     k (-1 + k)! (1 + n)!

The algorithm that works for simplifying this is:

	Simplify each term, then simplify the sum

"Simplify each term" yields the following:

   k       1 - k + n       1
------- + ----------- - -------
 1 + n       1 + n       1 + n

Then the final simplify yields:

   n
-------
 1 + n

*/

#include "stdafx.h"
#include "defs.h"
static void simfac_term(void);
static int yysimfac(int);

void
simfac(void)
{
	int h;

	save();

	p1 = pop();

	if (car(p1) == symbol(ADD)) {
		h = tos;
		p1 = cdr(p1);
		while (p1 != nil) {
			push(car(p1));
			simfac_term();
			p1 = cdr(p1);
		}
		addk(tos - h);
		p1 = pop();
		if (find(p1, symbol(FACTORIAL))) {
			push(p1);
			if (car(p1) == symbol(ADD)) {
				condense();
				simfac_term();
			}
		} else {
			push(p1);
			simplify();
		}
	} else {
		push(p1);
		simfac_term();
	}

	restore();
}

static void
simfac_term(void)
{
	int h;

	save();

	p1 = pop();

	// if not a product of factors then done

	if (car(p1) != symbol(MULTIPLY)) {
		push(p1);
		restore();
		return;
	}

	// push all factors

	h = tos;
	p1 = cdr(p1);
	while (p1 != nil) {
		push(car(p1));
		p1 = cdr(p1);
	}

	// keep trying until no more to do

	while (yysimfac(h))
		;

	multiply_all_noexpand(tos - h);

	restore();
}

// try all pairs of factors

static int
yysimfac(int h)
{
	int i, j;

	for (i = h; i < tos; i++) {
		p1 = stack[i];
		for (j = h; j < tos; j++) {
			if (i == j)
				continue;
			p2 = stack[j];

			//	n! / n		->	(n - 1)!

			if (car(p1) == symbol(FACTORIAL)
			&& car(p2) == symbol(POWER)
			&& isminusone(caddr(p2))
			&& equal(cadr(p1), cadr(p2))) {
				push(cadr(p1));
				push(one);
				subtract();
				factorial();
				stack[i] = pop();
				stack[j] = one;
				return 1;
			}

			//	n / n!		->	1 / (n - 1)!

			if (car(p2) == symbol(POWER)
			&& isminusone(caddr(p2))
			&& caadr(p2) == symbol(FACTORIAL)
			&& equal(p1, cadadr(p2))) {
				push(p1);
				push(one);
				subtract();
				factorial();
				reciprocate();
				stack[i] = pop();
				stack[j] = one;
				return 1;
			}

			//	(n + 1) n!	->	(n + 1)!

			if (car(p2) == symbol(FACTORIAL)) {
				push(p1);
				push(cadr(p2));
				subtract();
				p3 = pop();
				if (isplusone(p3)) {
					push(p1);
					factorial();
					stack[i] = pop();
					stack[j] = one;
					return 1;
				}
			}

			//	1 / ((n + 1) n!)	->	1 / (n + 1)!

			if (car(p1) == symbol(POWER)
			&& isminusone(caddr(p1))
			&& car(p2) == symbol(POWER)
			&& isminusone(caddr(p2))
			&& caadr(p2) == symbol(FACTORIAL)) {
				push(cadr(p1));
				push(cadr(cadr(p2)));
				subtract();
				p3 = pop();
				if (isplusone(p3)) {
					push(cadr(p1));
					factorial();
					reciprocate();
					stack[i] = pop();
					stack[j] = one;
					return 1;
				}
			}

			//	(n + 1)! / n!	->	n + 1

			//	n! / (n + 1)!	->	1 / (n + 1)

			if (car(p1) == symbol(FACTORIAL)
			&& car(p2) == symbol(POWER)
			&& isminusone(caddr(p2))
			&& caadr(p2) == symbol(FACTORIAL)) {
				push(cadr(p1));
				push(cadr(cadr(p2)));
				subtract();
				p3 = pop();
				if (isplusone(p3)) {
					stack[i] = cadr(p1);
					stack[j] = one;
					return 1;
				}
				if (isminusone(p3)) {
					push(cadr(cadr(p2)));
					reciprocate();
					stack[i] = pop();
					stack[j] = one;
					return 1;
				}
			}
		}
	}

	return 0;
}
