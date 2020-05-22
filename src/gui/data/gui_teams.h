#ifndef GUI_TEAMS_H
#define GUI_TEAMS_H

#include <string>
using std::string;

class GuiTeams
{
public:
    static void updateTeams();
    static string get_robot_type(int team_id);
    static string get_controller(int team_id);
    static string get_strategy(int team_id);

private:
    static string robot_type[2];
    static string controller[2];
    static string strategy[2];

};

#endif // GUI_TEAMS_H
