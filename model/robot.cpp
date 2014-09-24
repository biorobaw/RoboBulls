#include <assert.h>
#include "robot.h"

Robot::Robot()
{
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

Point Robot::getRobotPosition(){return robotPosition;}

float Robot::getOrientation(){return orientation;}

int Robot::getID(){return id;}

int Robot::getL(){return LF;}

int Robot::getR(){return RF;}

int Robot::getB(){return RB;}	//RB and RL are the same for three-wheel

int Robot::getLF(){ return LF; }

int Robot::getRF(){ return RF; }

int Robot::getLB(){ return LB; }

int Robot::getRB(){ return RB; }

int Robot::getKick(){return kick;}

Behavior * Robot::getCurrentBeh(){return currentBehavior;}

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

