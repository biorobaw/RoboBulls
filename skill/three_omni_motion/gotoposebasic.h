#ifndef GOTOPOSEBASIC_H
#define GOTOPOSEBASIC_H

struct threeWheelVels{
    int L,R,B;
};

class GoToPoseBasic
{
public:
    GoToPoseBasic();
    threeWheelVels calcWheelVels(double x_goal, double y_goal, double theta_goal);
};

#endif // GOTOPOSEBASIC_H
