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


Robot::Robot(int id, int team) :
    id(id),
    team_id(team),
    behavior(nullptr),
    kick_speed(0),
    dribble(false),
    pilot(new PilotDummy(this))
{
    assignBeh<RefStop>();

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
    return false;
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

float Robot::getTargetAngularSpeed(){
    return  target_angular_speed;
}
Point Robot::getTargetVelocity(){
    return target_velocity;
}
float Robot::getTargetSpeed(){
    return target_velocity.norm();
}
int   Robot::getKickSpeed() {
    return kick_speed;
}
bool  Robot::getDribble(){
    return dribble;
}
bool  Robot::getChip(){
    return chip;
}

/*! @brief Set power in m/s for the robot to kick
 * @details *Do not use;* use Skil::Kick instead
 * Sets the initial kick velocity for the robot in m/s
 * @see setDrible */
void Robot::setTargetVelocity(Point velocity, float angular_speed){
    target_velocity = velocity;
    target_angular_speed = angular_speed;
}
void Robot::setKickSpeed(int speed){
    kick_speed = speed;
}
void Robot::setDribble(bool on){
    dribble = on; }
void Robot::setChip(bool on){
    chip = on;
}

// ============================================================
// ============= HIGH Level Control ===========================
// ============================================================

Pilot* Robot::getPilot(){ return pilot;}

Behavior* Robot::getBehavior(){ return behavior; }
bool Robot::hasBehavior() { return behavior != nullptr ; }
void Robot::performBehavior(){
    if(behavior!=nullptr) {
        behavior->perform();
    }
    getPilot()->executeCommands();
}
void Robot::clearBehavior()
{
    if(behavior != nullptr) {
        delete behavior;
        this->behavior = nullptr;
    }
}


void Robot::goToPose(CmdGoToPose& newCommand){
    pilot->setNewCommand(newCommand);
}
bool Robot::completedGoToPoseCmd(){
    return pilot->finishedCommand();
}


bool Robot::hasBall(){
    return has_ball;
}


// ============================================================
// =============== PRIVATE FUNCTIONS =========================
// ============================================================



void Robot::setID(int ID){id = ID;}

void Robot::setTeam(int which) { team_id = which; }

void Robot::setCurrentBeh(Behavior *currentBeh)
{
    behavior = currentBeh;
}










