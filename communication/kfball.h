#ifndef KFILTER_H
#define KFILTER_H
#include "kalman/ekfilter.hpp"
#include "model/gamemodel.h"
#include <time.h>
#include <deque>

// Set up in accordance with:
// http://kalman.sourceforge.net/doc/example.html#expage

class KFBall : public Kalman::EKFilter<double,1,true,true,false>
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

/* Simulator Settings
Physics
    World
        65
        False
        0.015
        9.8
    Ball
        0.044
        0.1
        1.0
        0.5
        0.1
        0.004
        0.004

Communication


*/
