#include <assert.h>
#include <sstream>
#include <cmath>
#include "robot.h"

Robot::Robot()
{
    LF = 0; LB = 0;
    RF = 0; RB = 0;
    orientation = 0;
    kick = false;
    drible = true;
    hasBeh = false;
    currentBehavior = nullptr;
}

void Robot::setRobotPosition(Point rbtPoint){robotPosition = rbtPoint;}

void Robot::setOrientation(float ornt){orientation = ornt;}

void Robot::setID(int ID){id = ID;}

void Robot::setL(float left){LF = LB = left;}

void Robot::setR(float right){RF = RB = right;}

void Robot::setB(float back){LB = RB = back;}

void Robot::setLF(float left_forward){LF = left_forward;}

void Robot::setRF(float right_forward){RF = right_forward;}

void Robot::setLB(float left_backward){LB = left_backward;}

void Robot::setRB(float right_backward){RB = right_backward;}

void Robot::setTeam(bool which) { team = which; }

void Robot::setKick(bool Kick){kick = Kick;}

void Robot::setDrible(bool drible){
    this->drible = drible;
}

void Robot::setCurrentBeh(Behavior *currentBeh)
{
    currentBehavior = currentBeh;
    hasBeh = true;
}

void Robot::clearCurrentBeh()
{
    hasBeh = false;
    if(currentBehavior != nullptr) {
        delete currentBehavior;
        this->currentBehavior = nullptr;
    }
}

void Robot::setVelocity(Point vel) { velocity = vel; }

Point Robot::getRobotPosition(){return robotPosition;}

float Robot::getOrientation(){return orientation;}

int Robot::getID(){return id;}

int Robot::getL(){return LF;}

int Robot::getR(){return RF;}

int Robot::getB(){return RB;}    //RB and RL are the same for three-wheel

int Robot::getLF(){ return LF; }

int Robot::getRF(){ return RF; }

int Robot::getLB(){ return LB; }

int Robot::getRB(){ return RB; }

int Robot::getKick(){return kick;}

bool Robot::isOnMyTeam() { return team; }

Behavior * Robot::getCurrentBeh(){return currentBehavior;}

Point Robot::getVelocity() { return velocity; }

float Robot::getSpeed()
{
    Point vel = getVelocity();
    return sqrt((vel.x * vel.x) + (vel.y * vel.y));
}

robotType Robot::type()
{
    assert(id >= 0 && id < 10);
    return robotIDTypes[id];
}

std::string Robot::toString()
{
    stringstream ss;
    ss << "\t" << getRobotPosition().toString() << "\t ID: " << getID();
    return ss.str();
}

bool Robot::getDrible(){return drible;}

