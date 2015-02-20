#include "teststrategy.h"
#include "behavior/behaviorassignment.h"
#include "skill/stop.h"
#include "skill/kicktopoint.h"
#include "skill/kicktopointomni.h"
#include "skill/kick.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/movetype.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"

/************************************************************************/

class KickBeh : public Behavior
{
    Skill::KickToPointOmni* ktpo;
public:
    KickBeh() {
        ktpo = new Skill::KickToPointOmni(Point(0,0));
    }

    void perform(Robot * robot) override
    {
        ktpo->perform(robot);
    }
};

bool TestStrategy::update()
{
    return false;
}

void TestStrategy::assignBeh()
{
    gameModel->findMyTeam(8)->assignBeh<KickBeh>();
}
