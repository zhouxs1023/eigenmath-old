// simplify factorials

#include "stdafx.h"
#include "defs.h"
static void ysimfac(void);
static int yysimfac(int);

void
simfac(void)
{
	save();
	ysimfac();
	restore();
}

static void
ysimfac(void)
{
	int h;

	p1 = pop();

	// if sum then try condense

	if (car(p1) == symbol(ADD)) {
		push(p1);
		condense();
		p1 = pop();
	}

	// if not a product of factors then done

	if (car(p1) != symbol(MULTIPLY)) {
		push(p1);
		return;
	}

	// push all factors

	h = tos;
	p1 = cdr(p1);
	while (p1 != nil) {
		push(car(p1));
		p1 = cdr(p1);
	}

	// keep going until done

	while (yysimfac(h))
		;

	multiply_all(tos - h);
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
		}
	}

	return 0;
}
