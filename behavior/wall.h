#ifndef WALL_H
#define WALL_H

#include "genericmovementbehavior.h"
#include "model/gamemodel.h"
#include "utilities/region/defencearea.h"
#include "utilities/comparisons.h"
#include "gui/guiinterface.h"
#include "skill/kicktopointomni.h"

class Wall : public GenericMovementBehavior
{
public:
    Wall();
    ~Wall();
    void perform(Robot *) override;
    bool isFinished() override;
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
    void calcWallPoints();
    bool spaceForGoalKick();

    // Variables for clearing the ball
    static bool being_cleared;
    Point kickPoint;
    Skill::KickToPointOmni* kick_skill;
    bool shouldClear(Robot*);
    bool shouldStopClearing(Robot*);
};

#endif // WALL_H
