#include "communication/kfball.h"
#include "include/config/simulated.h"

#if SIMULATED
#define DECEL -1.0
#define TIME_STEP 1.0/55.0
#else

#endif
KFBall::KFBall():a(DECEL), T(TIME_STEP)
{
    // setDim(x, u, w, z, v)
    setDim(4, 2, 2, 2, 2);
}

// State Vector
void KFBall::makeProcess()
{
    Vector x_(x.size());

    // vel_x
    x_(1) = x(1);
    if(fabs(x(1)) - fabs(a) < 0)
        x_(1) = 0;
    else
        x_(1) += fabs(x(1))/x(1) * a * T;

    // pos_x
    x_(2) = x(2) + x(1) * T + 0.5 * a * T * T;

    // vel_y
    x_(3) = x(3);
    if(fabs(x(3)) - fabs(a) < 0)
        x_(3) = 0;
    else
        x_(3) += fabs(x(3))/x(3) * a * T;

    // pos_y
    x_(4) = x(4) + x(3) * T + 0.5 * a * T * T;

    x.swap(x_);
}

// Measurements
void KFBall::makeMeasure()
{
    z(1)= x(2);
    z(2)= x(4);
}

// State Vector Jacobian
void KFBall::makeA()
{
    A(1,1) = 1.0;
    A(1,2) = 0.0;
    A(1,3) = 0.0;
    A(1,4) = 0.0;

    A(2,1) = T;
    A(2,2) = 1.0;
    A(2,3) = 0.0;
    A(2,4) = 0.0;

    A(3,1) = 0.0;
    A(3,2) = 0.0;
    A(3,3) = 1.0;
    A(3,4) = 0.0;

    A(4,1) = 0.0;
    A(4,2) = 0.0;
    A(4,3) = T;
    A(4,4) = 1.0;
}

// Measurement Noise derivative
void KFBall::makeW()
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

// Process Noise derivatives
void KFBall::makeH()
{
    H(1,1) = 0.0;
    H(1,2) = 1.0;
    H(1,3) = 0.0;
    H(1,4) = 0.0;

    H(2,1) = 0.0;
    H(2,2) = 0.0;
    H(2,3) = 0.0;
    H(2,4) = 1.0;
}

// Measurement Noise derivatives
void KFBall::makeV()
{
    V(1,1) = 1.0;
    V(1,2) = 0.0;
    V(2,1) = 0.0;
    V(2,2) = 1.0;
}

// Process Noise Covariance
void KFBall::makeQ()
{
    Q(1,1) = 100.0;
    Q(1,2) = 0.0;
    Q(2,1) = 0.0;
    Q(2,2) = 100.0;
}

// Measurement Noise Covariance
void KFBall::makeR()
{
    R(1,1) = 100.0;
    R(1,2) = 0.0;
    R(2,1) = 0.0;
    R(2,2) = 100.0;
}
