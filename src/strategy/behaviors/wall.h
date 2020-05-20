#ifndef WALL_H
#define WALL_H

#include "genericmovementbehavior.h"
#include "utilities/region/defencearea.h"
#include "utilities/comparisons.h"
#include "gui/gui_interface.h"
#include "../skills/kicktopointomni.h"


/*!
 * @file
 * @author Muhaimen Shamsi
 * @brief This behavior will move the robot to a position on a wall of defenders along the defence area.
 * Ideally, there will always be at least two robots assigned this behavior during the game to assist
 * the goalkeeper.
 *
 * \i "Night gathers, and our watch begins."
 */

class Wall : public GenericMovementBehavior
{
public:
    Wall(Robot* robot);
    ~Wall();
    void perform() override;
    bool isFinished() override;

    string getName() override;

private:
    enum {wall, clear} state = wall;
    int id = -1;

    // Variables for maintaining a wall of defenders
    struct WallPoint
    {
        Point position = Point(0,0);
        int id = -1;
    };

    static WallPoint wall_array[2];
    bool occupying = false;
    void calcWallPoints(Robot*);
    bool spaceForGoalKick();

    // Variables for clearing the ball
    static bool being_cleared;
    Point kickPoint;
    Skill::KickToPointOmni* kick_skill;
    bool shouldClear(Robot*);
    bool shouldStopClearing(Robot*);
};

#endif // WALL_H
