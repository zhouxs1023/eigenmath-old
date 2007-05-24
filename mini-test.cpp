// mini test for distribution builds

#include "stdafx.h"
#include "defs.h"

static char *s[] = {

	// static spherical metric

	"gdd=((-exp(2*Phi(r)),0,0,0),(0,exp(2*Lambda(r)),0,0),(0,0,r^2,0),(0,0,0,r^2*sin(theta)^2))",
	"",

	"X=(t,r,theta,phi)",
	"",

	"guu=inv(gdd)",
	"",

	"gddd=d(gdd,X)",
	"",

	"GAMDDD=1/2*(gddd+transpose(gddd,2,3)-transpose(transpose(gddd,2,3),1,2))",
	"",

	"GAMUDD=contract(outer(guu,GAMDDD),2,3)",
	"",

	"T1=d(GAMUDD,X)",
	"",

	"T2=contract(outer(GAMUDD,GAMUDD),2,4)",
	"",

	"RUDDD=transpose(T1,3,4)-T1+transpose(T2,2,3)-transpose(transpose(T2,2,3),3,4)",
	"",

	"RDD=contract(RUDDD,1,3)",
	"",

	"R=contract(contract(outer(guu,RDD),2,3),1,2)",
	"",

	"GDD=RDD-1/2*gdd*R",
	"",

	"Gtt=1/r^2*exp(2 Phi(r)) d(r (1 - exp(-2 Lambda(r))),r)",
	"",

	"Grr=-1/r^2*exp(2*Lambda(r))*(1-exp(-2*Lambda(r)))+2/r*d(Phi(r),r)",
	"",

	"Gthetatheta=r^2*exp(-2*Lambda(r))*(d(d(Phi(r),r),r)+d(Phi(r),r)^2+d(Phi(r),r)/r-d(Phi(r),r)*d(Lambda(r),r)-d(Lambda(r),r)/r)",
	"",

	"Gphiphi=sin(theta)^2*Gthetatheta",
	"",

	"T=((Gtt,0,0,0),(0,Grr,0,0),(0,0,Gthetatheta,0),(0,0,0,Gphiphi))",
	"",

	"GDD-T",
	"((0,0,0,0),(0,0,0,0),(0,0,0,0),(0,0,0,0))",
};

void
mini_test(void)
{
	test(__FILE__, s, sizeof (s) / sizeof (char *));
}
