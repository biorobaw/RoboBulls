#include "my_kalman_filter.h"

// Movement distance between detections within which the ball is said to be stationary
#define B_STOP_THRESH 5.0


//#if SIMULATED
//    #define ACCEL -0.03
//    #define TIME_STEP 1.0/55.0
//    #define QVEL 0.5
//    #define QPOS 0.5
//    #define RVEL 10.0
//    #define RPOS 5.0
//#else
    #define ACCEL -10.0
    #define TIME_STEP 0.0112    // Determined using printTimeStep()
    #define QVEL 1.0
    #define QPOS 1.0
    #define RVEL 0.00001
    #define RPOS 0.00001
//#endif

MyKalmanFilter::MyKalmanFilter():a(ACCEL), T(TIME_STEP)
{
    // setDim(x, u, w, z, v)
    setDim(4, 4, 4, 4, 4);

    MyKalmanFilter::Matrix P0;
    P0.resize(4,4);
    P0(1,1) = 500;
    P0(2,2) = 100;
    P0(3,3) = 500;
    P0(4,4) = 100;

    MyKalmanFilter::Vector x(4);
    x(1) = 0.0;
    x(2) = 0;
    x(3) = 0.0;
    x(4) = 0;

    init(x,P0);


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
void MyKalmanFilter::makeProcess()
{
    Vector x_(x.size());

//    printTimeStep();

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
void MyKalmanFilter::makeMeasure()
{
    z(1)= x(1);
    z(2)= x(2);
    z(3)= x(3);
    z(4)= x(4);
}

// State Vector Jacobian
void MyKalmanFilter::makeA()
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
void MyKalmanFilter::makeW()
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
void MyKalmanFilter::makeH()
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
void MyKalmanFilter::makeV()
{
    V(1,1) = 1.0;
    V(2,2) = 1.0;
    V(3,3) = 1.0;
    V(4,4) = 1.0;
}

// Process Noise Covariance
void MyKalmanFilter::makeQ()
{
    Q(1,1) = QVEL;
    Q(2,2) = QPOS;
    Q(3,3) = QVEL;
    Q(4,4) = QPOS;
}

// Measurement Noise Covariance
void MyKalmanFilter::makeR()
{
    R(1,1) = RVEL;
    R(2,2) = RPOS;
    R(3,3) = RVEL;
    R(4,4) = RPOS;
}

void MyKalmanFilter::printTimeStep()
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


void MyKalmanFilter::newObservation(Point new_position){

    //TODO: verify if the required velocity is the last filtered average
    Vector z(4);
    z(1) = velocity_average.x;
    z(2) = new_position.x;
    z(3) = velocity_average.y;
    z(4) = new_position.y;

    Vector state = getX(); // get current state
    step(state,z);         // update state
    state = getX();        // get state after update


    Point new_filtered_position = Point(state(2),state(4));


    // GuiInterface* gui = GuiInterface::getGuiInterface();
    // gui->drawPath(b_pos, b_pos + avg_vel, 0.1);

    // calculate velocity since last observation, why do we do this?, see previous comment
    // old code assumed 0.0193s between message
    // TODO: get actual delta T between measurements insteaad of assumed value
    Point v = (new_filtered_position - last_filtered_position)/0.0193;

    // recalculate average velocity (incremental method)
    Point oldest_v = velocity_history[-1];
    velocity_average += ( v - oldest_v )/velocity_history.getSize();

    // push new velocity to the list
    velocity_history.push(v);
    last_filtered_position = new_filtered_position;





}

Point MyKalmanFilter::getPosition(){
    return last_filtered_position;
}

Point MyKalmanFilter::getVelocity(){
    Vector state = getX();
    double b_vel = hypot(state(1), state(3));
    Point v = Point(state(1),state(3));
    return b_vel < B_STOP_THRESH ? Point(0, 0) : Point(state(1), state(3));
}
