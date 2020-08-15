#ifndef GUI_ROBOT_H
#define GUI_ROBOT_H

#include <QObject>
#include "model/constants.h"
#include "utilities/point.h"
#include "robot/robot.h"

//Proxy between the gui and the ball

class GuiRobot : public Robot
{
    Q_OBJECT
public:

    static GuiRobot* get(int team_id, int robot_id);
    static void connectWithModel();

    GuiRobot(QObject* parent, int team, int id); // private constructor
    void update();

    // functions for reading game related information
    int     getMoveStatus();
    QString getBehaviorName();
    bool    isControlled();

    float getOrientationInDegrees();
    QString getOrientationAsString();  // TODO: review usage of this function


    // functions to control the robot from the gui
    void setOverriden(bool new_value); // overrides the robot controller so that it can be controlled by the gui
    bool isOverriden();                // indicates whether the robot is being controlled by the gui




    // gui related functions to user input
    void select();               // robot selected by the gui to display info or control the robot
    bool selected();             // indicates whether the robot is the one selected by the gui
    static void clearSelected(); // clears the selection
    static GuiRobot* get_selected_robot(); // returns the robot selected by the gui, null if none

    int getFlipXCoordinates();

private:

    // data storing game related information
    int     move_status   = -1;        // stores the current movement status
    QString behaviorName = "";       // stores the current behavior of the robot

    // variables to control the robot from the gui
    bool  overriden = true;

    // data related to gui functions
    static GuiRobot* selected_robot; // pointer to the robot selected by the gui


    Robot* getProxy();

    // redefinition of variable in moving_object.h
    int flip_x = 1;


signals:
    void overridenChanged(bool new_value); // signal emitted when the variable overriden changes value
    void setGuiTargetVelocity(Point velocity, float angular);
    void setGuiKickSpeed(int speed = 5000);
    void setGuiDribble(bool dribble);
    void setChip(bool chip);

    void selectedChanged(GuiRobot*);       // signal emitted when a robot becomes selected or deselected
    void doubleClicked(GuiRobot*);         // signal emitted when a the graphics of a gui robot is double clicked


};

#endif // GUI_ROBOT_H
