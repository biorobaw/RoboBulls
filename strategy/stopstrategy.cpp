#include <iostream>
#include "stopstrategy.h"
#include "behavior/behaviorassignment.h"
#include "behavior/obstacleavoidbehavior.h"

using namespace std;

StopStrategy::StopStrategy()
{
}

void StopStrategy:: assignBeh()
{
//    GameModel * gamemodel = GameModel::getModel();

//    myTeam = gamemodel->getMyTeam();

    BehaviorAssignment<StopBehavior> stopAssignment;
    stopAssignment.assignBeh();

}
