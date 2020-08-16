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


Robot* Robot::setRobotProxy(RobotProxy* proxy){
   has_kicker = proxy->hasKicker();
   is_holonomic = proxy->isHolonomic();
   delete pilot; // delete old pilot
   pilot = proxy->createPilot(this);
   return this;
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

Robot* Robot::setRole(int role){
    team_role = role;
    return this;
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


bool Robot::ignoresController(){
    return use_overriden_controls;
}

GameState* Robot::getGameState(){
    return game_state;
}
Robot* Robot::setGameState(GameState* state){
    game_state = state;
    return this;
}



Behavior* Robot::getBehavior(){
    return behavior;
}

Robot* Robot::setBehavior(Behavior *currentBeh)
{
    behavior = currentBeh;
    return this;
}

bool Robot::hasBehavior() {
    return behavior != nullptr ;
}

Robot* Robot::performBehavior(){
    if(use_overriden_controls) return this;
    if(behavior != nullptr) {
        behavior->perform();
    }
    pilot->executeCommands();
    return this;
}

Robot* Robot::clearBehavior()
{
    if(behavior != nullptr) {
        delete behavior;
        this->behavior = nullptr;
    }
    return this;
}



//Pilot* Robot::getPilot(){
//    return pilot;
//}

Robot* Robot::goToPose(CmdGoToPose& newCommand){
    pilot->setNewCommand(newCommand);
    return this;
}

bool Robot::completedGoToPoseCmd(){
    return pilot->finishedCommand();
}


bool Robot::hasBall(){
    return has_ball;
}

Robot* Robot::setHasBall(bool val){
    has_ball = val;
    return this;
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













