#ifndef KFILTER_H
#define KFILTER_H
#include "kalman/ekfilter.hpp"
#include <time.h>
#include <deque>
#include <src/utilities/circular_buffer.h>
#include <src/utilities/point.h>

#define VEL_HIST_SIZE 20

/*!
 * @file
 * @author Muhaimen Shamsi
 * @brief Kalman filter to reduce noise in ball detection.
 * In addition to reducing noise, should also produce a prediction
 * of where the ball is when it is blocked from the camera by robots.
 *
 * Set up in accordance with:
 * http://kalman.sourceforge.net/doc/example.html#expage
 *
 * @todo Check Tigers Manheim's source code for improvemnt.
 * @todo Allow auto-tuning.
 */

class MyKalmanFilter : public Kalman::EKFilter<double,1,true,true,false>
{
public:
    MyKalmanFilter();
    void newObservation(Point p);
    Point getPosition();
    Point getVelocity();
private:
     CircularBuffer<Point> velocity_history = CircularBuffer<Point>(VEL_HIST_SIZE,Point(0,0));
     Point velocity_average = Point(0,0);
     Point last_filtered_position = Point(0,0);

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
