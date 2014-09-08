#include "penaltybehavior.h"

PenaltyBehavior::PenaltyBehavior(const ParameterList& list)
	: GenericMovementBehavior(list)
{
    UNUSED_PARAM(list);
    pb = moving;
}

/**
 * @brief PenaltyBehavior::perform Behavior that goes to the penalty point and kicks the ball once.
 * @param myRobot : The robot to perform the behavior on (usually myTeam[0])
 */
void PenaltyBehavior::perform(Robot * myRobot)
{
    GameModel *model = GameModel::getModel();
    float myAngle = Measurments::angleBetween(myRobot->getRobotPosition(),model->getPenaltyPoint());

    switch(pb)
    {
    case moving:
		{
			setMovementTargets(model->getBallPoint(), myAngle);
			
			if(!(Measurments::isClose(myRobot->getRobotPosition(),model->getBallPoint(),100)))
			{
				GenericMovementBehavior::perform(myRobot);
			#if PENALTY_BEHAVIOR_DEBUG
				cout <<"moving performed!"<<endl;
			#endif
			}
			else
				pb = kicking;
		}
        break;
    case kicking:
		{
			Skill::Kick kick;
			kick.perform(myRobot);
		#if PENALTY_BEHAVIOR_DEBUG
			cout<<"kicking performed!"<<endl;
		#endif
			pb = idling;
		}
        break;
    case idling:
		{
			Skill::Stop stop;
			stop.perform(myRobot);
		#if PENALTY_BEHAVIOR_DEBUG
			cout<<"idling performed!"<<endl;
		#endif
		}
        break;
    }
}
