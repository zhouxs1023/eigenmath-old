// Skydiver simulation

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void init(void);
void run(void);
void runmc(void);
double speedofsound(double);
double airdensity(double);
double gravity(double);
double randomcoeff();

//	t	simulated elapsed time (seconds)
//	dt	time increment (seconds)
//	m	mass of skydiver (kg)
//	v	velocity of skydiver (m/s)
//	h	altitude of skydiver (meters)
//	ACd	Area * Coeff. of drag (m^2)
//	vo	velocity of skydiver in previous time frame (m/s)
//	g	gravitational acceleration (m/s^2)
//	p	air density (kg/m^3)
//	Fd	Force due to drag (kg m/s^2)
//	Fg	Force due to gravity (kg m/s^2)
//	a	skydiver's acceleration (m/s^2)

double t, dt, m, v, h, vmax, hmax, ACd, vo, g, p, Fd, Fg, a, mmax;
int height, weight;

main()
{
	printf("Height, Weight, Drag, Mach\r\n");
	for (height = 60000; height <= 100000; height += 20000) {
		for (weight = 200; weight <= 400; weight += 100) {
			ACd = 0.6;
			runmc();
			ACd = 0.75;
			runmc();
			ACd = 0.9;
			runmc();
		}
	}
}

// initialize the simulation

void
init(void)
{
	h = height / 3.28084;	// convert to meters
	m = weight * 0.453592;	// convert to kg
	dt = 1;
	v = 0;
	vo = 0;
	vmax = 0;
	mmax = 0;
}

// Simulation time step

void
step(void)
{
	double s;

	g = gravity(h);
	p = airdensity(h);

	// Force due to drag, see "Drag (physics)" in Wikipedia

	Fd = -0.5 * p * v * v * ACd;

	// Force due to gravity

	Fg = m * g;

	// skydiver's net acceleration

	a = (Fd + Fg) / m;

	// update skydiver's velocity

	v = v + a * dt;

	// update skydiver's altitude (use avg. velocity)

	h = h - (v + vo) / 2 * dt;
	vo = v;

	// update vmax

	if (v > vmax)
		vmax = v;

	// update max mach

	s = speedofsound(h);

	if (v / s > mmax) {
		mmax = v / s;
		hmax = h;
	}
}

// run the monte carlo simulation

void
runmc(void)
{
	int i;
	srand(3);
	for (i = 0; i < 1000; i++) {
		init();
		for (t = 0; t < 180; t += dt) {
			step();
			if (h < 3000)
				break;
		}
		printf("%d, %d, %g, %g\r\n", height, weight, ACd, mmax);
	}
}

// calculate the air density at altitude h in meters
// see "Barometric formula" in Wikipedia

double H[7] = {0, 11000, 20000, 32000, 47000, 51000, 71000};
double P[7] = {1.2250, 0.36391, 0.08803, 0.01322, 0.00143, 0.00086, 0.000064};
double T[7] = {288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65};
double L[7] = {-0.0065, 0.0, 0.001, 0.0028, 0.0, -0.0028, -0.002};

double g0 = 9.80665;
double R = 8.31432e3;
double M = 28.9644;

double
airdensity(double h)
{
	int b;
	double x, y;
	for (b = 0; b < 6; b++) {
		if (h <= H[b])
			break;
	}
        if (L[b] == 0.0) {
                y = -g0 * M * (h - H[b]) / (R * T[b]);
                return P[b] * exp(y);
        } else {
                x = T[b] / (T[b] + L[b] * (h - H[b]));
                y = (g0 * M) / (R * L[b]);
                return P[b] * pow(x, y + 1);
        }
}

// The next function gives the speed of sound in m/s for a given altitude.
// See "Mach vs. Altitude Tables"

double S[25] = { 
761.2,
748.0,
734.6,
721.0,
707.0,
692.8,
678.3,
663.5,

660.1,
660.1,
660.1,
660.1,
660.1,
660.1,

662.0,
664.3,
666.5,
668.9,
671.1,
673.4,
675.6,
677.9,
683.7,
689.9,
696.0};

double
speedofsound(double h)
{
	int n = (int) (3.28084 * h / 5000.0 + 0.5);
	return randomcoeff() * S[n] / 2.236936;
}

// Gravity at 30,000 meters altitude = 9.72 m/s^2
// Gravity at      0 meters altitude = 9.81 m/s^2

double
gravity(double h)
{
	return 9.81 + (9.72 - 9.81) * h / 30000.0;
}

double
randomcoeff(void)
{
	double coeff;
	coeff = (double) rand() / (double) RAND_MAX;
	return 1.0 + 0.1 * (coeff - 0.5);
}
