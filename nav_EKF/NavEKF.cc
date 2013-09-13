
#include "NavEKF.h"
#include <cmath>
#include <iostream>

using namespace std;

NavEKF::NavEKF() 
{
	setDim(4, 1, 2, 2, 2);
	Period = 0.2;
	Gravity = 9.8;
	Bfriction = 0.35;
	Portance = 3.92;
	Mass = 1000;
}

void NavEKF::makeBaseA()
{
	A(1,1) = 1.0;
	// A(1,2) = Period - Period*Period*Bfriction/Mass*x(2);
	A(1,3) = 0.0;
	A(1,4) = 0.0;

	A(2,1) = 0.0;
	// A(2,2) = 1 - 2*Period*Bfriction/Mass*x(2);
	A(2,3) = 0.0;
	A(2,4) = 0.0;

	A(3,1) = 0.0;
	// A(3,2) = Period*Period*Portance/Mass*x(2);
	A(3,3) = 1.0;
	A(3,4) = Period;

	A(4,1) = 0.0;
	// A(4,2) = 2*Period*Portance/Mass*x(2);
	A(4,3) = 0.0;
	A(4,4) = 1.0;
}

void NavEKF::makeA()
{
	// A(1,1) = 1.0;
	A(1,2) = Period - Period*Period*Bfriction/Mass*x(2);
	// A(1,3) = 0.0;
	// A(1,4) = 0.0;

	// A(2,1) = 0.0;
	A(2,2) = 1 - 2*Period*Bfriction/Mass*x(2);
	// A(2,3) = 0.0;
	// A(2,4) = 0.0;

	// A(3,1) = 0.0;
	A(3,2) = Period*Period*Portance/Mass*x(2);
	// A(3,3) = 1.0;
	// A(3,4) = Period;

	// A(4,1) = 0.0;
	A(4,2) = 2*Period*Portance/Mass*x(2);
	// A(4,3) = 0.0;
	// A(4,4) = 1.0;
}

void NavEKF::makeBaseW()
{
	W(1,1) = 0.0;
	W(1,2) = 0.0;
	W(2,1) = 1.0;
	W(2,2) = 0.0;
	W(3,1) = 0.0;
	W(3,2) = 0.0;
	W(4,1) = 0.0;
	W(4,2) = 1.0;
}

void NavEKF::makeBaseQ()
{
	Q(1,1) = 0.01*0.01;
	Q(1,2) = 0.01*0.01/10.0;
	Q(2,1) = 0.01*0.01/10.0;
	Q(2,2) = 0.01*0.01;
}

void NavEKF::makeBaseH()
{
	// H(1,1) = -x(3)/(x(1)*x(1)+x(3)*x(3));
	H(1,2) = 0.0;
	// H(1,3) = x(1)/(x(1)*x(1)+x(3)*x(3));
	H(1,4) = 0.0;

	// H(2,1) = x(1)/sqrt(x(1)*x(1)+x(3)*x(3));
	H(2,2) = 0.0;
	// H(2,3) = x(3)/sqrt(x(1)*x(1)+x(3)*x(3));
	H(2,4) = 0.0;
}

void NavEKF::makeH()
{
	H(1,1) = -x(3)/(x(1)*x(1)+x(3)*x(3));
	// H(1,2) = 0.0;
	H(1,3) = x(1)/(x(1)*x(1)+x(3)*x(3));
	// H(1,4) = 0.0;

	H(2,1) = x(1)/sqrt(x(1)*x(1)+x(3)*x(3));
	// H(2,2) = 0.0;
	H(2,3) = x(3)/sqrt(x(1)*x(1)+x(3)*x(3));
	// H(2,4) = 0.0;
}

void NavEKF::makeBaseV()
{
	V(1,1) = 1.0;
	V(2,2) = 1.0;
}

void NavEKF::makeBaseR()
{
	R(1,1) = 0.01*0.01;
	R(2,2) = 50*50;
}

void NavEKF::makeProcess()
{
	Vector x_(x.size());
	x_(1) = x(1) + x(2)*Period + (Period*Period)/2*(u(1)/Mass - Bfriction/Mass*x(2)*x(2));
	x_(2) = x(2) + (u(1)/Mass - Bfriction/Mass*x(2)*x(2))*Period;
	x_(3) = x(3) + x(4)*Period + (Period*Period)/2*(Portance/Mass*x(2)*x(2)-Gravity);
	x_(4) = x(4) + (Portance/Mass*x(2)*x(2)-Gravity)*Period;
	x.swap(x_);
}

void NavEKF::makeMeasure()
{
	z(1)=atan2(x(3), x(1));
	z(2)=sqrt(x(1)*x(1)+x(3)*x(3));
}

