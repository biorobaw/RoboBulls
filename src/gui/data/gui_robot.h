#ifndef GUI_ROBOT_H
#define GUI_ROBOT_H

#include "utilities/point.h"
#include "model/constants.h"
#include <QString>
#include <QObject>
#include <QtCore>

//Proxy between the gui and the ball

class GuiRobot : public QObject
{
    Q_OBJECT
public:

    // Static functions to get the robots and to update their values
    static GuiRobot* get(int team, int robot);
    static void updateRobots();



    // ID of team/robot being controlled
    const int id;
    const int team;


    // functions for reading game related information
    bool    isInField();    // indicates whether a robot is in the field (independently if it is controlled)
    bool    isControlled(); // indicates whether our software controls the robot
    bool    hasBall();
    int     getMoveStatus();
    QString  getBehaviorName();
    Point   getCurrentPosition();
    float   getOrientation();
    QString getOrientationAsString();  // TODO: review usage of this function
    int     getCurrentSpeed();     // actual  speed
    int     getSpeedCommand();     // desired speed
    Point   getVelocityCommand();  // desired velocity
    bool    isDribbling();         // indicates whether the bot is dribbling
    bool    isKicking();           // indicates whether the bot is kicking



    // functions to control the robot from the gui
    void setOverriden(bool new_value); // overrides the robot controller so that it can be controlled by the gui
    bool isOverriden();                // indicates whether the robot is being controlled by the gui
    void  setManualVelocity(Point vxy, float angular);
    void  setKick(float power = 5.0);
    void  setDribble(bool dribble);


    // gui related functions to user input
    void select();               // robot selected by the gui to display info or control the robot
    bool selected();             // indicates whether the robot is the one selected by the gui
    static void clearSelected(); // clears the selection
    static GuiRobot* get_selected_robot(); // returns the robot selected by the gui, null if none





private:


    // data storing game related information
    bool  is_in_field   = false;     // stores whether the robot is the field
    bool  is_controlled = false;     // stores whether our software controlls the robot
    bool  has_ball      = false;     // stores whether the robot is in possesion of the ball
    int   move_status   = -1;        // stores the current movement status
    QString behaviorName = "";       // stores the current behavior of the robot
    Point previous_pos = Point(0,0); // stores the previous position of the robot
    Point current_pos  = Point(0,0); // stores the current position of the robot
    float current_orientation = 0;   // stores the current orientation of the robot
    int   previous_speed = 0;        // stores previous delta distance
    int   current_speed  = 0;        // stores current delta distance
    Point velocity_command = Point(0,0); // stores the desired velocity of the robot
    int   speed_command = 0;         // stores the desired speed of the robot

    // data to control the robot from the gui
    bool dribling = false;
    bool kicking = false;
    bool overriden = true;


    // data related to gui functions
    static GuiRobot* selected_robot; // pointer to the robot selected by the gui

    // Array of gui robots + initializer and constructor
    static GuiRobot*** robots;  // Statically initialized to size [2][MAX_ROBOTS_PER_TEAM][1]
    static GuiRobot*** init_robots();
    GuiRobot(QObject* parent, int team, int id);


signals:
    void overridenChanged(bool new_value);
    void selectedChanged(GuiRobot*);
    void doubleClicked(GuiRobot*);


};

#endif // GUI_ROBOT_H
