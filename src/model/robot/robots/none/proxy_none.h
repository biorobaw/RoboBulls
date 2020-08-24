#ifndef ROBOTNONE_H
#define ROBOTNONE_H

#include "../../robot_implementation.h"
#include "../../navigation/pilots/pilot_dummy.h"

class ProxyNone : public RobotImplementation
{
    Q_OBJECT
public:
    ProxyNone();
    void sendCommands(const QSet<Robot*>&) override;

    bool hasKicker() override;
    bool isHolonomic() override;
    RobotPilot* createPilot(Robot* robot) override;

    QString getName() override;

protected:
    void close() override;

};

#endif // ROBOTNONE_H
