#ifndef GUITEAMS_H
#define GUITEAMS_H

#include <string>
using std::string;

class GuiTeams
{
public:
    static void update();
    static string get_robot_type(int team_id);
    static string get_controller(int team_id);
    static string get_strategy(int team_id);

private:
    static string robot_type[2];
    static string controller[2];
    static string strategy[2];

};

#endif // GUITEAMS_H
