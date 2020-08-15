#include <assert.h>
#include <sstream>
#include <cmath>
#include "robot.h"
#include "model/team.h"
#include <iostream>
#include "strategy/behaviors/refstop.h"
#include "robot/robot_proxy.h"
#include "robot/navigation/robot_pilot.h"
#include "robot/navigation/pilots/pilot_dummy.h"
#include "model/game_state.h"
#include "model/team.h"

using std::cerr, std::cout, std::endl;

// ============================================================
// ====== Constructor and basic info ==========================
// ============================================================


Robot::Robot(int team, int id) :
    RobotLowLevelControls(nullptr),
    id(id),
    team_id(team),
    behavior(nullptr),
    pilot(new PilotDummy(this))
{

}

Robot::~Robot(){
}


void Robot::setRobotProxy(RobotProxy* proxy){
   has_kicker = proxy->hasKicker();
   is_holonomic = proxy->isHolonomic();
   delete pilot; // delete old pilot
   pilot = proxy->createPilot(this);

}

int Robot::getID() { return id; }

bool  Robot::hasKicker(){
    return has_kicker;
};


std::string Robot::toString()
{
    std::stringstream ss;
    ss << "\t" << getPosition().toString() << "\t ID66666: " << getID();
    return ss.str();
}

// ============================================================
// ================ TEAM RELATED ==============================
// ============================================================



int Robot::getTeamId(){
    return team_id;
}

RobotTeam* Robot::getTeam(){
    return RobotTeam::teams[team_id];
}

QSet<Robot*>& Robot::getOpponentRobots(){
    return getTeam()->getGameState()->getFieldRobots(1-team_id);
}

Robot* Robot::getOpponentRobot(int r_id){
    return getTeam()->getGameState()->getRobot(1-team_id,r_id);
}

int Robot::getRole(){
    return team_role;
}

void Robot::setRole(int role){
    team_role = role;
}

bool Robot::isGoalie(){
    return team_role == ROLE_GOALIE;
}


// ============================================================
// ============= Low Level Control ============================
// ============================================================





void Robot::useOverridenControls(bool ignore){
    use_overriden_controls = ignore;
}

// ============================================================
// ============= HIGH Level Control ===========================
// ============================================================


bool Robot::ignoreController(){
    return use_overriden_controls;
}



Behavior* Robot::getBehavior(){
    return behavior;
}

void Robot::setBehavior(Behavior *currentBeh)
{
    behavior = currentBeh;
}

bool Robot::hasBehavior() {
    return behavior != nullptr ;
}

void Robot::performBehavior(){
    if(use_overriden_controls) return;
    if(behavior != nullptr) {
        behavior->perform();
    }
    pilot->executeCommands();
}

void Robot::clearBehavior()
{
    if(behavior != nullptr) {
        delete behavior;
        this->behavior = nullptr;
    }
}



//Pilot* Robot::getPilot(){
//    return pilot;
//}

void Robot::goToPose(CmdGoToPose& newCommand){
    pilot->setNewCommand(newCommand);
}

bool Robot::completedGoToPoseCmd(){
    return pilot->finishedCommand();
}


bool Robot::hasBall(){
    return has_ball;
}

void Robot::setHasBall(bool val){
    has_ball = val;
}


// ============================================================
// ============= LOE Level Control ============================
// ============================================================


RobotLowLevelControls* Robot::getOverridenController(){
    return overriden_controls;
}
RobotLowLevelControls* Robot::getActiveController(){
    return use_overriden_controls ? overriden_controls : this;
}

// ============================================================
// =============== PRIVATE FUNCTIONS ==========================
// ============================================================



void Robot::setID(int ID){id = ID;}

void Robot::setTeam(int which) { team_id = which; }












