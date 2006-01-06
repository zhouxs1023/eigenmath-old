//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	convolution product
//
//	Input:		tos-2		x	(can be a symbol or expr)
//
//				tos-1		y	(can be a symbol or expr)
//
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
int h1, h2, h3, h4;

static void yconvolution(void);

void
eval_convolution(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	convolution();
}

void
convolution(void)
{
	save();
	yconvolution();
	restore();
}

#define X p1
#define Y p2

static void
yconvolution(void)
{
	Y = pop();
	X = pop();
	push(Y);
	guess();
	p4=pop();
	pop();
	push(X);
	guess();
	p3=pop();
	pop();


	if (iszero(X) || iszero(Y)) {
		push_integer(0);
		return;
	}
	
	if (car(X) == symbol(DIRAC)) {
		push(Y);
		return;
	}

	if (car(Y) == symbol(DIRAC)) {
		push(X);
		return;
	}

	if (car(X) == symbol(DERIVATIVE)) {
		push_symbol(CONVOLUTION);
		push(cadr(X));
		push(Y);
		push(caddr(X));
		derivative();
		list(3);
		return;
	}

	if (car(Y) == symbol(DERIVATIVE)) {
		push_symbol(CONVOLUTION);
		push(cadr(Y));
		push(X);
		push(caddr(Y));
		derivative();
		list(3);
		return;
	}

	if (car(X) == symbol(ADD)) {
		X = cdr(X);
		push(car(X));
		push(Y);
		convolution();
		X = cdr(X);
		while (iscons(X)) {
			push(car(X));
			push(Y);
			convolution();
			add();
			X = cdr(X);
		}
		return;
	}

	if (car(Y) == symbol(ADD)) {
		Y = cdr(Y);
		push(X);
		push(car(Y));
		convolution();
		Y = cdr(Y);
		while (iscons(Y)) {
			push(X);
			push(car(Y));
			convolution();
			add();
			Y = cdr(Y);
		}
		return;
	}

	if (car(X) == symbol(MULTIPLY) && p3 == p4) {

		h1 = tos;

		p5 = cdr(X);
		while (iscons(p5)) {
			if (!find(car(p5), p3))
				push(car(p5));
			p5 = cdr(p5);
		}

		h2 = tos;

		p5 = cdr(X);
		while (iscons(p5)) {
			if (find(car(p5), p3))
				push(car(p5));
			p5 = cdr(p5);
		}

		if (tos - h2 == 0) {
			push(one);
			push(Y);
			convolution();}
		else {
			multiply_all(tos - h2);
			X = pop();
			push(X);
			push(Y);
			convolution();
		}

		multiply_all(tos - h1);
		return;
	}


	if (car(Y) == symbol(MULTIPLY) && p3 == p4) {

		h3 = tos;

		p5 = cdr(Y);
		while (iscons(p5)) {
			if (!find(car(p5), p3))
				push(car(p5));
			p5 = cdr(p5);
		}

		h4 = tos;

		p5 = cdr(Y);
		while (iscons(p5)) {
			if (find(car(p5), p3))
				push(car(p5));
			p5 = cdr(p5);
		}

		if (tos - h4 == 0) {
			push(X);
			push(one);
			convolution();}
		else {
			multiply_all(tos - h4);
			Y = pop();
			push(X);
			push(Y);
			convolution();
		}

		multiply_all(tos - h3);
		return;
	}

// 	if (expomode ==1) {
// 		push(X);
// 		fourier();
// 		push(Y);
// 		fourier();
// 		multiply();
// 		invfourier();
// 		return;
// 	}

	push_symbol(CONVOLUTION);
	push(X);
	push(Y);
	list(3);
	return;
	

}


static char *s[] = {
	"convolution(dirac(x),f(x))",
	"f(x)",
	
	"eval(convolution(d(dirac(x),x),f(x)))",
	"d(f(x),x)",
	
};

void
test_convolution(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
