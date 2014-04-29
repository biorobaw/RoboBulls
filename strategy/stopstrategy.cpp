#include "stopstrategy.h"
#include <iostream>

using namespace std;

StopStrategy::StopStrategy()
{
}

void StopStrategy::assignBeh()
{
    vector <Robot*> team;

    beh = new StopBehavior();

    GameModel * gamemodel = GameModel::getModel();

    //TODO: should be assigning behaviours
//    cout<<"Assigning beh in stop strategy!!!"<<endl;
    //control.stop(comm, NXTrobot);

    // Assign stop behaviour to every robot
        //Get myteam from model
            // For every robot in my team
                // Assign new stopbehavior

    team = gamemodel->getMyTeam();

     for(int i=0; i<team.size(); i++)
     {
         team[i]->setCurrentBeh(beh);
     }
}
