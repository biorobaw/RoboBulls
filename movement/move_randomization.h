#ifndef MOVE_RANDOMIZATION_H
#define MOVE_RANDOMIZATION_H

/* Movement Randomization
 * Contains structures and routines for eliminating the collision deadlocks
 * when more than one robot moves to a single point. To do this, target points
 * are randomized if they will cause a colision deadlock, so robots will move 
 * to a general area instead of one spcific point.
 */
class Robot;
class Point;

namespace Movement
{
namespace Randomization
{
    //Adds a Robot's target point to consideration for avoidance
    void addTargetPoint(Robot* robot, const Point& robotTarget);
    
    //Updates all given target points to hopefully avoid all same-point problems
    void  update();
    
    //Rectrieve a target point fixed so where no robot is going there
    Point getUpdatedTargetPoint(Robot* robot);
}
}

#endif
