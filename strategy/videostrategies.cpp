#include <stdlib.h>
#include "behavior/ballreceiver.h"
#include "skill/stop.h"
#include "model/gamemodel.h"
#include "strategy/videostrategies.h"

/************************************************************************/
/* BEHAVIORS */

OmniRandomKicker::OmniRandomKicker(Robot* whoIsRecever)
	: receiver(whoIsRecever)
    , bhasKicked(false)
	{ }
	
OmniRandomKicker::~OmniRandomKicker()
{
	delete ktp;
}
	
bool OmniRandomKicker::hasKicked() 
{
	return this->bhasKicked;
}
	
void OmniRandomKicker::perform(Robot* robot)
{
	if(ktp == nullptr) {
        Point offset = Point(-500+rand()%1000, -1000+rand()%2000);
		Point recvr  = receiver->getRobotPosition();
        ktp = new Skill::KickToPoint(recvr + offset);
	} else {
		if(ktp->perform(robot)) {
			bhasKicked = true;
		}
	}
}

/************************************************************************/
/* VIDEO STRATEGY 1 */

#define TIME_PASS_MAX 1000

VideoStrategy1::VideoStrategy1(int r1, int r2)
	: r1ID(r1)
	, r2ID(r2)
    , timePassed(0)
	{ }

void VideoStrategy1::assignBeh() 
{
	Robot* r1 = gameModel->findMyTeam(r1ID);
	Robot* r2 = gameModel->findMyTeam(r2ID);

	if(r1 and r2)  {
		this->currentPasser = r1;
		this->currentRecver = r2;
        this->currentPasser->assignBeh<OmniRandomKicker>(currentRecver);
	}	
}

void VideoStrategy1::updateBehaviors(Robot* r1, Robot* r2, bool r2Receiving)
{
    //Keeps both robots on a singe side of the field
    Point r1Target = (r1->getID() == r1ID) ? gameModel->getOpponentGoal() : gameModel->getMyGoal();

    //R1 is the robot always retreating
    r1->assignBeh<GenericMovementBehavior>(r1Target);

    if(r2Receiving) {
        r2->assignBeh<BallReceiver>();
    } else {
        r2->assignBeh<OmniRandomKicker>(r1);
    }
}

bool VideoStrategy1::update() 
{
    OmniRandomKicker* ORK =  dynamic_cast<OmniRandomKicker*>(currentPasser->getCurrentBeh());

    if(ORK != nullptr && !passerKicked)
        passerKicked = ORK->hasKicked();

	//Once the passer has kicked...
    if( passerKicked )
	{
		//Stop the passer from moving and make the receiver start moving to recieve
        updateBehaviors(currentPasser, currentRecver, true);
		
        if( Measurments::distance(gameModel->getBallPoint(), currentRecver) < 1500 or
            ++timePassed > TIME_PASS_MAX)
		{
			//Swap roles and assign the passer (the previous receiver) to kick the ball
			std::swap(currentPasser, currentRecver);
            updateBehaviors(currentRecver, currentPasser, false);

            //The current passer has no longer kicked, and we reset time
            passerKicked = false;
            timePassed = 0;
		}
	}
	
	return false;
}

/************************************************************************/
