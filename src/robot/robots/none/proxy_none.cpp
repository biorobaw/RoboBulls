#include "proxy_none.h"

ProxyNone::ProxyNone()
{

}

void ProxyNone::sendVels(const QSet<Robot*>&){

};

bool ProxyNone::hasKicker(){
    return false;
}



bool ProxyNone::isHolonomic() {
    return true;
}

Pilot* ProxyNone::createPilot(Robot* robot){
    return new PilotDummy(robot);
}

QString ProxyNone::getName(){
    return "none";
}

void ProxyNone::close(){

}
