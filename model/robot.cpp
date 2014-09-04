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

void Robot::setLF(float left_forward){LF = left_forward;}

void Robot::setRF(float right_forward){RF = right_forward;}

void Robot::setLB(float left_backward){LB = left_backward;}

void Robot::setRB(float right_backward){RB = right_backward;}

void Robot::setKick(int Kick){kick = Kick;}

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

int Robot::getL(){return 0.5*(LF+LB);}

int Robot::getR(){return 0.5*(RF+RB);}

int Robot::getKick(){return kick;}

Behavior * Robot::getCurrentBeh(){return currentBehavior;}

std::string Robot::toString()
{
    stringstream ss;
    ss << "\t" << getRobotPosition().toString() << "\t ID: " << getID();
    return ss.str();
}

bool Robot::getDrible(){return drible;}

