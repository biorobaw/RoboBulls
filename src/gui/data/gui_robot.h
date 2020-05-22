#ifndef GUI_ROBOT_H
#define GUI_ROBOT_H

#include "utilities/point.h"
#include "model/constants.h"
#include <QString>
#include <QObject>
#include <string>
#include <QtCore>
using std::string;

class GuiRobot : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(bool overriden READ isOverriden WRITE setOverride)
public:
    static GuiRobot proxies[2][MAX_ROBOTS_PER_TEAM];
    static void init_static_data();
    static void updateRobots();

    bool overriden = true;
    bool isOverriden(){return overriden;}
    void setOverride(bool val){overriden=true;}

    static GuiRobot* get_selected_robot();

    // function related to robot measurments
    bool    isInField(); // indicates whether a robot is in the field (independently if it is controlled)
    bool    isControlled(); // indicates whether we control the robot
    bool    hasBall();
    int     getMoveStatus();
    string  getBehaviorName();
    Point   getCurrentPosition();
    float   getOrientation();
    QString getOrientationAsString();
    int     getCurrentSpeed();     // actual speed
    Point   getVelocityCommand();  // target velocity
    int     getSpeedCommand();     // target speed
    bool    isDribbling();
    bool    isKicking();



    // functions to control the robot from the gui
    void  setManualVelocity(Point vxy, float angular);
    void  setKick(float power = 5.0);
    void  setDribble(bool dribble);


    // functions related to user input
    bool selected();
    bool doubleClicked();
    bool select(bool signal); // returns true if it could be selected
    void doubleClick();
    static void clearSelected(bool signal);


    int id;
    int team;
    bool visible        = true;


private:

    static GuiRobot* selected_robot;
    static GuiRobot* double_clicked_robot;

    bool  is_in_field   = false;
    bool  is_controlled = false; // whether we are controlling the robot
    bool  has_ball      = false;
    int   move_status   = -1;
    Point previous_pos = Point(0,0);
    Point current_pos  = Point(0,0);
    int   previous_speed = 0; // actually, its delta distance
    int   current_speed  = 0; // actually, its delta distance
    Point velocity_command = Point(0,0); // last command set (but may not have been setnt yet)
    int   speed_command = 0; //millimeters per second
    float current_orientation = 0;
    string behaviorName = "";

    bool dribling = false;
    bool kicking = false;

    GuiRobot(QObject* parent = 0);
    void initProxy(int team, int id);


//signals:
//    void overridenChanged(bool new_value);

};

#endif // GUI_ROBOT_H
