#include "gotoposebasic.h"


GoToPoseBasic::GoToPoseBasic()
{
}

threeWheelVels GoToPoseBasic::calcWheelVels(Robot * rob, double x_goal, double y_goal, double theta_goal)
{
    //Current Position
    double x_current = rob->getRobotPosition().x;
    double y_current = rob->getRobotPosition().y;
    double theta_current = rob->getOrientation();

    cout << "X: " << x_current << endl;
    cout << "Y: " << y_current << endl;
    cout << "Theta: " << theta_current *180/M_PI<< endl;

    //Robot Physical Properties
    double axle_length = 100;  //distance from center of mass to wheel
    double wheel_radius = 20;
    double wheel_offset = M_PI/6;

    //Transformation Matrix
    double t_matrix[3][3] = { { -sin(wheel_offset+theta_current),	  cos(wheel_offset+theta_current),	  axle_length },
                              { -sin(wheel_offset-theta_current),	 -cos(wheel_offset-theta_current),	  axle_length },
                              {               cos(theta_current),                  sin(theta_current),    axle_length }};

    //Cartesian Velocity Matrix
    double cartVels[3][1] = {{x_goal-x_current},
                             {y_goal-y_current},
                             {theta_current-theta_goal}};

    //Result Matrix
    double wheelVels[3][1] = {{0},
                              {0},
                              {0}};

    multiply_3x3_3x1(t_matrix,cartVels,wheelVels);

    threeWheelVels results;

    results.L = wheelVels[0][0]/wheel_radius;
    results.R = -wheelVels[1][0]/wheel_radius;
    results.B = wheelVels[2][0]/wheel_radius;

    //Normalize wheel velocities
    if (abs(results.L)>100)
    {
        results.B=(100/results.L)*results.B;
        results.R=(100/results.L)*results.R;
        results.L=100;
    }
    if (abs(results.R)>100)
    {
        results.B=(100/results.R)*results.B;
        results.L=(100/results.R)*results.L;
        results.R=100;
    }
    if (abs(results.B)>100)
    {
        results.L=(100/results.B)*results.L;
        results.R=(100/results.B)*results.R;
        results.B=100;
    }

    cout << "WheelL: " << results.L << endl;
    cout << "WheelR: " << -results.R << endl;
    cout << "WheelB: " << results.B << endl;

    return results;
}
