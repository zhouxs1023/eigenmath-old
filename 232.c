// Barometric formula from Wikipedia

#include <stdio.h>
#include <math.h>

double h[7] = {
	0.0,
	11000.0,
	20000.0,
	32000.0,
	47000.0,
	51000.0,
	71000.0,
};

double rho[7] = {
	1.2250,
	0.36391,
	0.08803,
	0.01322,
	0.00143,
	0.00086,
	0.000064,
};

double T[7] = {
	288.15,
	216.65,
	216.65,
	228.65,
	270.65,
	270.65,
	214.65,
};

double L[7] = {
	-0.0065,
	0.0,
	0.001,
	0.0028,
	0.0,
	-0.0028,
	-0.002,
};

double g0 = 9.80665;
double R = 8.31432e3;
double M = 28.9644;

double
air_density(double hh)
{
	int b;
	double x, y;
	for (b = 0; b < 6; b++)
		if (hh < h[b])
			break;
	if (L[b] == 0.0) {
		y = -g0 * M * (hh - h[b]) / (R * T[b]);
		return rho[b] * exp(y);
	} else {
		x = T[b] / (T[b] + L[b] * (hh - h[b]));
		y = (g0 * M) / (R * L[b]);
		return rho[b] * pow(x, y + 1);
	}
}
