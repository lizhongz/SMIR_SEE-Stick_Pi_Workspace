#ifndef _NAV_EKF_H_
#define _NAM_EKF_H_

#include "kalman/ekfilter.hpp"


class NavEKF : public Kalman::EKFilter<double, 1, false, false, false> 
{
public:
	NavEKF();

protected:
	void makeBaseA();
	void makeBaseH();
	void makeBaseV();
	void makeBaseR();
	void makeBaseW();
	void makeBaseQ();

	void makeA();
	void makeProcess();
	void makeMeasure();

};

typedef NavEKF::Vector Vector;
typedef NavEKF::Matrix Matrix;

#endif
