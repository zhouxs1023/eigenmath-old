static char *help_rational_arithmetic[] = {
	"1/2 + 1/3",
	"float",
};

static char *help_factor_polynomial[] = {
	"factor(125 x^3 - 1)",
};

static char *help_factor_number[] = {
	"factor(900)",
};

static char *help_arccos[] = {
	"xrange=(-1,1)",
	"yrange=(0,pi)",
	"draw(arccos(x))",
};

static char *help_arcsin[] = {
	"xrange=(-1,1)",
	"yrange=(-pi/2,pi/2)",
	"draw(arcsin(x))",
};

static char *help_arctan[] = {
	"draw(arctan(x))",
};

static char *help_cos[] = {
	"draw(cos(x))",
};

static char *help_sin[] = {
	"draw(sin(x))",
};

static char *help_tan[] = {
	"xrange=(-pi,pi)",
	"draw(tan(x))",
};

#define HELP(x) do_help(x, sizeof x / sizeof (char *))

static void do_help(char **, int);
