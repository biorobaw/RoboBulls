#include "communication/kfball.h"
#include "include/config/simulated.h"

#if SIMULATED
#define ACCEL -9.0
#define TIME_STEP 1.0/55.0
#define QVAL 0.05
#define RVAL 10.0
#else
#define ACCEL -10.0
#define TIME_STEP 0.0193    // Determined using printTimeStep()
#define QPOS 0.5    // Was 0.5
#define QVEL 1    // Was 0.5
#define RPOS 15.0   // Was 15.0
#define RVEL 10.0   // Was 15.0
#endif

KFBall::KFBall():a(ACCEL), T(TIME_STEP)
{
    // setDim(x, u, w, z, v)
    setDim(4, 4, 4, 4, 4);
}

int sign(double x)
{
    if(x > 0)
        return 1;
    else if(x < 0)
        return -1;
    else
        return 0;
}

// State Vector
void KFBall::makeProcess()
{
    Vector x_(x.size());

    //printTimeStep();

    // vel_x
    if(fabs(x(1)) - fabs(a * T) >= 0)
        x_(1) = x(1) + sign(x(1)) * a * T;
    else
        x_(1) = x(1) * 0.95;

    // pos_x
    x_(2) = x(2) + x(1) * T + sign(x(1)) * 0.5 * a * T * T;

    // vel_y
    if(fabs(x(3)) - fabs(a * T) >= 0)
        x_(3) = x(3) + sign(x(3)) * a * T;
    else
        x_(3) = x(3) * 0.95;

    // pos_y
    x_(4) = x(4) + x(3) * T + sign(x(3)) * 0.5 * a * T * T;

    x.swap(x_);
}

// Measurements
void KFBall::makeMeasure()
{
    z(1)= x(1);
    z(2)= x(2);
    z(3)= x(3);
    z(4)= x(4);
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
    for (size_t i = 1; i <= W.ncol(); ++i)
        for (size_t j = 1; j <= W.nrow(); ++j)
            W(i,j) = 0.0;

    W(1,1) = 1.0;
    W(2,2) = 1.0;
    W(3,3) = 1.0;
    W(4,4) = 1.0;
}

// Process Noise derivatives
void KFBall::makeH()
{
    for (size_t i = 1; i <= H.ncol(); ++i)
        for (size_t j = 1; j <= H.nrow(); ++j)
            H(i,j) = 0.0;

    H(1,1) = 1.0;
    H(2,2) = 1.0;
    H(3,3) = 1.0;
    H(4,4) = 1.0;
}

// Measurement Noise derivatives
void KFBall::makeV()
{
    V(1,1) = 1.0;
    V(2,2) = 1.0;
    V(3,3) = 1.0;
    V(4,4) = 1.0;
}

// Process Noise Covariance
void KFBall::makeQ()
{
    Q(1,1) = QVEL;
    Q(2,2) = QPOS;
    Q(3,3) = QVEL;
    Q(4,4) = QPOS;
}

// Measurement Noise Covariance
void KFBall::makeR()
{
    R(1,1) = RVEL;
    R(2,2) = RPOS;
    R(3,3) = RVEL;
    R(4,4) = RPOS;
}

void KFBall::printTimeStep()
{
    // Store the time steps in a deque
    time_t curr_clock = clock();
    intervals.push_back(curr_clock - prev_clock);
    prev_clock = curr_clock;

    // Don't store too many records
    size_t n_records = 120;
    if(intervals.size() > n_records)
        intervals.pop_front();

    // Calculate Average
    time_t time_step = 0;
    for(time_t t: intervals)
        time_step += t;
    time_step /= n_records;

    std::cout << "Time Step: " << time_step/(double)CLOCKS_PER_SEC << std::endl;
}
