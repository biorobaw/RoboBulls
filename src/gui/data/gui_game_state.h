#ifndef GUIGAMESTATE_H
#define GUIGAMESTATE_H

#include <QString>
#include "model/game_state.h"
#include "gui_robot.h"


class GuiGameState : public GameState
{
public:
    GuiGameState();

    void update();
    QString getRefereeCommandAsStr();

    static GuiGameState* get();
    static GuiRobot* getRobot(int team_id, int robot_id);

private:
    static GuiGameState* gui_game_state;

    GuiRobot* gui_robots[2][MAX_ROBOTS_PER_TEAM];

};

#endif // GUIGAMESTATE_H
