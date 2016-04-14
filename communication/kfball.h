#ifndef KFILTER_H
#define KFILTER_H
#include "kalman/ekfilter.hpp"
#include "model/gamemodel.h"
#include <time.h>
#include <deque>

// Set up in accordance with:
// http://kalman.sourceforge.net/doc/example.html#expage

class KFBall : public Kalman::EKFilter<double,1>
{
public:
    KFBall();
protected:
    void makeA();
    void makeH();
    void makeV();
    void makeR();
    void makeW();
    void makeQ();
    void makeProcess();
    void makeMeasure();
    double a, T;

    void printTimeStep();
    time_t prev_clock;
    std::deque<time_t> intervals;
};

#endif // KF_H
