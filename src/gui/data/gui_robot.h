#ifndef GUI_ROBOT_PROXY_H
#define GUI_ROBOT_PROXY_H

#include "utilities/point.h"
#include "model/constants.h"
#include <QString>
#include <string>
using std::string;

class GuiRobot
{
public:
    static GuiRobot proxies[2][MAX_ROBOTS_PER_TEAM];
    static void init_static_data();
    static void updateRobots();

    static int selected_robot;
    static int selected_team;
    static GuiRobot* get_selected_robot();

    Point getCurrentPosition();
    float getOrientation();
    int   getCurrentSpeed();
    Point getVelocityCommand();
    int   getSpeedCommand();
    QString getOrientationAsString();
    string getBehaviorName();



    void  setManualVelocity(Point vxy, float angular);
    void  setKick(float power = 5.0);
    void  setDribble(bool dribble);
    bool  hasProxy();

    bool isDribbling();
    bool isKicking();

    int id;
    int team;
    bool Pressed        = false;
    bool highlighted    = false;
    bool doubleClicked  = false;
    bool visible        = true;
    bool overridden     = false;
    bool selected       = false;

private:

    bool  is_in_field = false;
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

    GuiRobot();
    void initProxy(int team, int id);

};

#endif // GUI_ROBOT_PROXY_H
