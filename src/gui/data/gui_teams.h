#ifndef GUI_TEAMS_H
#define GUI_TEAMS_H

#include <QString>

//Proxy between the gui and the ball

class GuiTeams
{
public:
    static void updateTeams();  // function to update the data of the teams
    static QString get_robot_type(int team_id); // returns the robot type used by the team
    static QString get_controller(int team_id); // returns the controller used by the team
    static QString get_strategy(int team_id);   // returns the current strategy of the team
    static bool is_controlled(int team_id);

private:
    static QString robot_type[2]; // stores the robot types used by each team
    static QString controller[2]; // stores the controllers used by each team
    static QString strategy[2];   // stores the current strategy of each team

};

#endif // GUI_TEAMS_H
