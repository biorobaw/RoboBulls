#ifndef VIDEO_STRATEGIES_H
#define VIDEO_STRATEGIES_H

#include "strategy/strategy.h"
#include "behavior/behavior.h"
#include "skill/kicktopointomni.h"
#include "skill/kicktopoint.h"

/*
 * OMNI-ROBOT TEST GIT BRANCH --- TEMPORARY FILE 
 * This is a temporary file meant to provide functionality for the Robocup
 * Qulification 2015 video.
 */

class Robot;

/************************************************************************/
/* BEHAVIORS */

/* OmniRandomKicker
 * A behavior that used OmniRandomKicker to kick the ball to a random point
 * around the `whoIsRecever` bot passed to it
 */

class OmniRandomKicker : public Behavior
{
public:
     OmniRandomKicker(Robot* whoIsRecever);
	~OmniRandomKicker();
	void perform(Robot* robot);
	bool hasKicked();
private:
	Robot* receiver;
	bool   bhasKicked;
    Skill::KickToPointOmni* ktp = nullptr;
};

/************************************************************************/
/* STRATEGIES */

/* VideoStrategy1
 * A strategy that coordinates two robots to continually pass and recieve 
 * the ball with each other
 */

class VideoStrategy1 : public Strategy
{
public:
	VideoStrategy1(int r1, int r2);
	void assignBeh();
	bool update();
private:
    void updateBehaviors(Robot*, Robot*, bool);
	int r1ID, r2ID;
    int timePassed;
    bool passerKicked = false;
	Robot* currentPasser, *currentRecver;
};

/************************************************************************/

#endif
