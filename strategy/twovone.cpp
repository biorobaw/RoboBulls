#include "twovone.h"

TwoVOne::TwoVOne()
{

}

void TwoVOne::assignBeh()
{
    gm = GameModel::getModel();
    bp = gm->getBallPoint();
    ogp = gm->getOpponentGoal();
    mgp = gm->getMyGoal();

    r0=gm->getMyTeam().at(0)->getRobotPosition();
    r1=gm->getMyTeam().at(1)->getRobotPosition();
#if SIMULATED==1
    r2=gm->getMyTeam().at(2)->getRobotPosition();
    r3=gm->getMyTeam().at(3)->getRobotPosition();
    r4=gm->getMyTeam().at(4)->getRobotPosition();
    r5=gm->getMyTeam().at(5)->getRobotPosition();
#endif

    Robot * robot_0 = gm->getMyTeam().at(0);
    Robot * robot_1 = gm->getMyTeam().at(1);

    static float rgd = gm->getMyGoal().x/(abs)(gm->getMyGoal().x); //Relative friendly goal direction

    BehaviorAssignment<PositionForKickoff> bKickOff(true);

    if(Region::goalScored())
    {
        bKickOff.assignBeh();
    }

    else
    {
        //Define Regions
        //Assign Strategic Value
        //Add region to deque of regions
        #if SIMULATED==1
            Region * right_defense = new Region(0,3000*rgd,-2000,r4.y+150);
            strategicRegions.push_back(right_defense);

            Region * mid_defense = new Region(0,3000*rgd,r2.y-150,r4.y+150);
            strategicRegions.push_back(mid_defense);

            Region * left_defense = new Region(0,3000*rgd,2000,r2.y-150);
            strategicRegions.push_back(left_defense);
        #endif

        Region * left_offense = new Region(0,-3000*rgd,2000,0);
        strategicRegions.push_back(left_offense);

        Region * right_offense = new Region(0,-3000*rgd,-2000,0);
        strategicRegions.push_back(right_offense);

        sort(strategicRegions.begin(),strategicRegions.end(),[](Region* i,Region* j){
            return i->getStrategicValue()>j->getStrategicValue();
        });

        //Create Behaviors
        BehaviorAssignment<AttackMain> bAttactMain(true);
        BehaviorAssignment<AttackSupport> bAttackSupport(true);
        BehaviorAssignment<DefendCloseToBall> bMidDefender(true);
        BehaviorAssignment<DefendCloseToBall> bRightDefender(true);
        BehaviorAssignment<DefendCloseToBall> bLeftDefender(true);
        BehaviorAssignment<DefendFarFromBall> bGoalie(true);

        BehaviorAssignment<SendBallToRegion> bSendBall(true);
        bSendBall.setBehParam("region", strategicRegions.front());


        //Assign attacker behaviors
        if((left_offense->contains(bp)||right_offense->contains(bp)))
        {
            if(Measurments::distance(r0, bp)
               < Measurments::distance(r1, bp))
            {
                bAttactMain.assignBeh(robot_0);
                bAttackSupport.assignBeh(robot_1);
            }
            else
            {
                bAttactMain.assignBeh(robot_1);
                bAttackSupport.assignBeh(robot_0);
            }
        }
        else
        {
            bKickOff.assignBeh(robot_0);
            bKickOff.assignBeh(robot_1);
        }

        //Assign defender behaviors
        #if SIMULATED==1
            bGoalie.assignBeh(gm->getMyTeam().at(5));

            if(left_defense->contains(bp))
                bSendBall.assignBeh(gm->getMyTeam().at(2));
            else
                bKickOff.assignBeh(gm->getMyTeam().at(2));

            if(mid_defense->contains(bp))
                bSendBall.assignBeh(gm->getMyTeam().at(3));
            else
                bKickOff.assignBeh(gm->getMyTeam().at(3));

            if(right_defense->contains(bp))
                bSendBall.assignBeh(gm->getMyTeam().at(4));
            else
                bKickOff.assignBeh(gm->getMyTeam().at(4));

            delete mid_defense;
            delete left_defense;
            delete right_defense;
        #endif
        delete left_offense;
        delete right_offense;
    }
}

void TwoVOne::assignStrategicValue(Region * region)
{
    int value = 0;

    //10 * Max Distance squared as a fraction of distance from goal squared
    value += 40000000/int(((ogp.x-region->centre().x)*(ogp.x-region->centre().x)+(ogp.y-region->centre().y)*(ogp.y-region->centre().y)));

    //Number of teammates
    value += region->numOfTeammates();

    //Number of opponents
    value -= region->numOfOpponents();

    region->setStrategicValue(value);
}
