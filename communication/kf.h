#ifndef KFILTER_H
#define KFILTER_H
#include "kalman/ekfilter.hpp"
#include "model/gamemodel.h"

class KF : public Kalman::EKFilter<double,1>
{
public:
    KF();
protected:
    void makeA(){};   // Jacobian Matrix
    void makeB(){};   // Input Matrix
    void makeH(){};   //
    void makeV(){};
    void makeW(){};
    void makeQ(){};   // Process Noise Covariance
    void makeR();   // Measurement Noise Covariance

    double decel_friction, period;
};

typedef KF::Vector Vector;
typedef KF::Matrix Matrix;

#endif // KF_H
