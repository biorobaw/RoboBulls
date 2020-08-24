#include "proxy_none.h"

ProxyNone::ProxyNone()
{

}

void ProxyNone::sendCommands(const QSet<Robot*>&){

};

bool ProxyNone::hasKicker(){
    return false;
}



bool ProxyNone::isHolonomic() {
    return true;
}

RobotPilot* ProxyNone::createPilot(Robot* robot){
    return new PilotDummy(robot);
}

QString ProxyNone::getName(){
    return "none";
}

void ProxyNone::close(){

}
