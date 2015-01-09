#include <cstdlib>
#include "model/robot.h"
#include "utilities/point.h"
#include "include/config/tolerances.h"
#include "include/config/globals.h"
#include "utilities/measurments.h"
#include "utilities/comparisons.h"
#include "movement/move_randomization.h"

/************************************************************************/
/* USER CONFIGURATION */

/* Defines the number of calls to Randomization::update (called every time 
 * a Move object is performed on a robot with obstacle avoidance) that must 
 * take place before randomization movement is actually updated. This is
 * done for efficiency purposes
 */
#define ROBOT_RANDOM_UPDATE_COUNT        40
/* Defines the radius that the algorithm tries to places the points away from
 * each other. ROBOT_SIZE is a good choice, but it is customizable here.
 */
#define ROBOT_RANDOM_DIFF_RADIUS        (ROBOT_SIZE)
/* Defines the number of tries that the algorithm will loop through to resolve
 * all point conflicts. Because this uses randomization, there is of course a chance
 * the algorithm will never finish, so a secondary "try count" is needed.
 */
#define ROBOT_RANDOM_TRY_COUNT            99

/************************************************************************/
/* Implementation */


namespace Movement
{
namespace Randomization
{
    //Stored current targets of each robot
    static Point currentRobotTargetPoints[10];
    //Counter to only update namespace slecectively
    static int   randomizationUpdateCount = 0;
    
    
    static std::pair<int,int> 
    getNextPointConflict() 
    {
        for(int i = 0;   i != 10; ++i) {
        for(int j = i+1; j != 10; ++j) {
            const Point& a = currentRobotTargetPoints[i];
            const Point& b = currentRobotTargetPoints[j];
            if(Measurments::isClose(a, b, ROBOT_RANDOM_DIFF_RADIUS))
                return std::move(std::make_pair(i,j));
        }
        }
        return std::move(std::make_pair(-1,-1));
    }
    
    static void sanitizePoint(Point& pt)
    {
        pt.x = Measurments::clamp(pt.x, -2900.0f, 2900.0f);
        pt.y = Measurments::clamp(pt.y, -1900.0f, 1900.0f);
    }
    

    void addTargetPoint(Robot* robot, const Point& robotTarget)
    {
        currentRobotTargetPoints[robot->getID()] = robotTarget;
    }
    
    /* Attempts to resolve all robot destination-point 
     * conflicts by moving them away from each other.
     */
    void update()
    {
        if(++randomizationUpdateCount < ROBOT_RANDOM_UPDATE_COUNT)
            return;
        randomizationUpdateCount = 0;

        int tryCount = 0;
        std::pair<int,int> nextConflict;
        while((nextConflict = getNextPointConflict()).first != -1 
               and tryCount < ROBOT_RANDOM_TRY_COUNT) 
        {
            Point& a = currentRobotTargetPoints[nextConflict.first];
            Point& b = currentRobotTargetPoints[nextConflict.second];
            int multiplier  = 1;
            float direction = (std::rand() % 360) * M_PI/180;
            
            while(Measurments::isClose(a, b, ROBOT_RANDOM_DIFF_RADIUS)
                  and not(Comparisons::isPointOutsideField(b))) 
            {
                b.x += ROBOT_RANDOM_DIFF_RADIUS * cos(direction) * multiplier;
                b.y += ROBOT_RANDOM_DIFF_RADIUS * sin(direction) * multiplier;
                multiplier += 1;
            }
            
            if(Comparisons::isPointOutsideField(b))
                sanitizePoint(b);
                
            ++tryCount;
        }
    }
    
    Point getUpdatedTargetPoint(Robot* robot)
    {
        return currentRobotTargetPoints[robot->getID()];
    }
}
}
