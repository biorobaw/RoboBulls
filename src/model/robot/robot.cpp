#include <assert.h>
#include <sstream>
#include <cmath>
#include "robot.h"
#include "model/team/team.h"
#include <iostream>
#include "model/team/controllers/normal_game/behaviors/refstop.h"
#include "model/robot/robot_implementation.h"
#include "model/robot/navigation/robot_pilot.h"
#include "model/robot/navigation/pilots/pilot_dummy.h"
#include "model/game_state.h"
#include "model/team/team.h"

using std::cerr, std::cout, std::endl;

// ============================================================
// ====== Constructor and basic info ==========================
// ============================================================


Robot::Robot(int team, int id) :
    RobotLowLevelControls(nullptr,this),
    id(id),
    team_id(team)
{

}

Robot::~Robot(){
}


Robot* Robot::specifyImplementation(RobotImplementation* impl){
   has_kicker = impl->hasKicker();
   is_holonomic = impl->isHolonomic();
   if(pilot!=nullptr) delete pilot; // delete old pilot
   pilot = impl->createPilot(this);
   return this;
}

int Robot::getId() { return id; }

bool  Robot::hasKicker(){
    return has_kicker;
};


std::string Robot::toString()
{
    std::stringstream ss;
    ss << "\t" << Point::toString() << "\t ID66666: " << getId();
    return ss.str();
}

// ============================================================
// ================ TEAM RELATED ==============================
// ============================================================



int Robot::getTeamId(){
    return team_id;
}

RobotTeam* Robot::getTeam(){
    return team;
}

Robot* Robot::setTeam(RobotTeam *t){
    team = t;
    return this;
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





void Robot::setUseOverridenControls(bool ignore){
    use_overriden_controls = ignore;
}

// ============================================================
// ============= HIGH Level Control ===========================
// ============================================================


bool Robot::useOverridenControls(){
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

Robot* Robot::runControlCycle(){
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
        Behavior* temp = behavior;
        this->behavior = nullptr;
        delete temp;
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
// ============= LOW Level Control ============================
// ============================================================


RobotLowLevelControls* Robot::getOverridenController(){
    return overriden_controls;
}
RobotLowLevelControls* Robot::getActiveController(){
    return use_overriden_controls ? overriden_controls : this; // this
}













