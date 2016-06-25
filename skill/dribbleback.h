#ifndef DRIBBLEBACK_H
#define DRIBBLEBACK_H

#include "skill.h"
#include "utilities/point.h"
#include "movement/gotoposition.h"
#include "model/gamemodel.h"

namespace Skill
{

class DribbleBack : public Skill
{
public:
    DribbleBack(Point& target);
    DribbleBack(Point* target);
    bool perform(Robot*) override;
private:
    Point* target;
    Point grasp_point;
    Movement::GoToPosition move_skill;
    enum {move_to_ball, grasp, move_back} state = move_to_ball;
    float prev_vel = 0;
};

}


#endif // DRIBBLEBACK_H
