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
    vector <Robot*> myTeam;

    beh = new StopBehavior();

    GameModel * gamemodel = GameModel::getModel();
    myTeam = gamemodel->getMyTeam();

    for(unsigned int i=0; i<myTeam.size(); i++)
    {
        myTeam[i]->setCurrentBeh(beh);
    }
}
