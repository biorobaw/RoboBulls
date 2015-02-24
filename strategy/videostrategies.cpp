#include <stdlib.h>
#include "behavior/ballreceiver.h"
#include "skill/stop.h"
#include "model/gamemodel.h"
#include "strategy/videostrategies.h"
#include "utilities/debug.h"
#include "utilities/comparisons.h"

/************************************************************************/
/* BEHAVIORS */

static int X_DEV = 500;
static int Y_DEV = 1000;
static int RECV_DIST = 1500;
static int RECV_TIME = 1000;

OmniRandomKicker::OmniRandomKicker(Robot* whoIsRecever)
	: receiver(whoIsRecever)
    , bhasKicked(false)
{
    debug::registerVariable("x_dev", &X_DEV);
    debug::registerVariable("y_dev", &Y_DEV);
    debug::registerVariable("recv_d", &RECV_DIST);
    debug::registerVariable("recv_t", &RECV_TIME);
}
	
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
        Point offset = Point(-X_DEV+rand()%(2*X_DEV), -Y_DEV+rand()%(2*Y_DEV));
        Point myGoal = gameModel->getMyGoal();
        Point opgoal = gameModel->getOpponentGoal();
        Point less = std::min(myGoal, opgoal, Comparisons::distance(receiver));
        ktp = new Skill::KickToPointOmni(less + offset);
	} else {
		if(ktp->perform(robot)) {
			bhasKicked = true;
		}
	}
}

/************************************************************************/
/* VIDEO STRATEGY 1 */

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
		
        if( Measurments::distance(gameModel->getBallPoint(), currentRecver) < RECV_DIST or
            ++timePassed > RECV_TIME)
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
