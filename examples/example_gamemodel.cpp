//This is an example showing how to use GameModel functions

#include "model/gamemodel.h"

int main()
{
    //Get the ball's current position on the field
    Point ballPosition = gameModel->getBallPoint();

    //Get all robots currently on My Team (TEAM_BLUE or TEAM_YELLOW)
    std::vector<Robot*>& team = gameModel->getMyTeam();
    for(Robot* robot : team) {
        std::cout << robot->getID() << std::endl;
    }

    //Find a robot on a team
    Robot* robot_5 = gameModel->findMyTeam(5);
    if(robot_5) {
        std::cout << "Robot with id 5 is currently on team, at position " 
                  << robot_5->getRobotPosition()->toString() << std::endl;
    }
}
