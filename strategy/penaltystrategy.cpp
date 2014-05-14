#include "penaltystrategy.h"
#include <cstdlib>
#include <typeinfo>

PenaltyStrategy::PenaltyStrategy()
{
}

void PenaltyStrategy::assignBeh()
{
    GameModel * gamemodel = GameModel::getModel();
    vector <Robot*> myTeam;
    myTeam = gamemodel->getMyTeam();
    beh = new PenaltyBehavior();


    if( ! myTeam.empty())
    {
        if (! (myTeam[0]->hasBeh))
        {

            myTeam[0]->setCurrentBeh(beh);
        }
        else
        {
            if (typeid(*(myTeam[0]->getCurrentBeh())) != typeid(PenaltyBehavior))
            {
                myTeam[0]->setCurrentBeh(beh);
            }
            else if (typeid(*(myTeam[0]->getCurrentBeh())) == typeid(PenaltyBehavior))
            {
                myTeam[0]->setCurrentBeh(myTeam[0]->getCurrentBeh());
            }
        }

    }

}
