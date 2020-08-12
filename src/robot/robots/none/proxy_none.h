#ifndef ROBOTNONE_H
#define ROBOTNONE_H

#include "../../robot_proxy.h"
#include "../../navigation/pilots/pilot_dummy.h"

class ProxyNone : public RobotProxy
{
public:
    ProxyNone();
    void sendVels(const QSet<Robot*>&) override;

    bool hasKicker() override;
    bool isHolonomic() override;
    Pilot* createPilot(Robot* robot) override;

};

#endif // ROBOTNONE_H
