#include "penaltystrategy.h"
#include <cstdlib>

PenaltyStrategy::PenaltyStrategy()
{
}

void PenaltyStrategy::assignBeh()
{
    vector <Robot*> myTeam;

    beh = new PenaltyBehavior();

    GameModel * gamemodel = GameModel::getModel();

    myTeam = gamemodel->getMyTeam();

    if( ! myTeam.empty() )
        myTeam[0]->setCurrentBeh(beh);
}
