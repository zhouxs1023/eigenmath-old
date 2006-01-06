# Makefile for building Eigenmath on Linux machines.
#
# 1. Unpack the tarball.
#
#	$ tar zxf eigenmath-linux.tar.gz
#
# 2. Compile it.
#
#	$ cd eigenmath
#	$ make
#
# 3. Test it.
#
#	$ ./math
#	> --test
#
# 4. Press control-c to quit.
#
#	> ^C
#	$
#
# 5. Scripts can be run from the command line.
#
#	$ ./math StaticSphericalMetric
#
#	$ ./math script1 script2 script3
#
# 6. See eigenmath.sourceforge.net for more.


# CXX and CXXFLAGS are used by make's implicit rule for compiling C++ files.
# divby1billion() in mstr.c doesn't work with -O2
CXXFLAGS = -Wall -Wuninitialized -O -DLINUX

objects = madd.o mmul.o mprime.o mgcd.o mpow.o mroot.o mcmp.o mstr.o mscan.o mmodpow.o \
qadd.o qsub.o qmul.o qdiv.o qpow.o \
sin.o cos.o tan.o arcsin.o arccos.o arctan.o \
sinh.o cosh.o tanh.o arcsinh.o arccosh.o arctanh.o \
abs.o mod.o roots.o eigen.o simplify.o for.o isprime.o index.o wedge.o zero.o \
rationalize.o prog.o lcm.o floor.o ceiling.o condense.o userfunc.o find.o \
init.o primetab.o bignum.o symbol.o run.o atomize.o pollard.o outer.o inner.o \
is.o clear.o expcos.o expsin.o coeff.o log.o append.o list.o cons.o guess.o \
main.o misc.o data.o alloc.o stack.o scan.o print.o eval.o subst.o gcd.o \
factor.o quickfactor.o factorpoly.o tensor.o contract.o transpose.o \
derivative.o integral.o prime.o dsolve.o distill.o display.o add.o multiply.o \
power.o factorial.o sum.o product.o det.o inv.o hilbert.o taylor.o binomial.o \
trace.o charpoly.o hermite.o laguerre.o legendre.o degree.o divisors.o \
vectorize.o variables.o test.o selftest.o numerator.o denominator.o simfac.o \
besselj.o bessely.o carac.o convolution.o dirac.o erf.o erfc.o summarize.o \
fourier.o gamma.o heaviside.o invfourier.o sgn.o cofactor.cpp \
tchebychevT.o tchebychevU.o arg.o imag.o mag.o real.o rect.o

math : $(objects)
	$(CXX) -o math $(objects) -lm

$(objects) : defs.h prototypes.h
