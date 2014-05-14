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
                cout<<"The current beh is not a penalty behavior!"<<endl;
                myTeam[0]->setCurrentBeh(beh);
            }
            else if (typeid(*(myTeam[0]->getCurrentBeh())) == typeid(PenaltyBehavior))
            {
                cout<<"The current beh is a penalty behavior!"<<endl;
                myTeam[0]->setCurrentBeh(myTeam[0]->getCurrentBeh());
            }
        }

//        if (! (myTeam[0]->hasBeh))
//            myTeam[0]->setCurrentBeh(beh);
//        else if ((myTeam[0]->hasBeh))
//        {
//            if (typeid(myTeam[0]->getCurrentBeh()) != typeid(PenaltyBehavior()))
//                myTeam[0]->setCurrentBeh(beh);
//        }
//        if (typeid(myTeam[0]->getCurrentBeh()) != typeid(beh))
//            myTeam[0]->setCurrentBeh(beh);
    }
//        myTeam[0]->setCurrentBeh(beh);
        // does myTeam[0] has a PenaltyBehavior already
            // Then dont do anything
        // Else
            // setTheCurrentBehavior to beh
}
