#include "haltstrategy.h"
#include "skill/skill.h"
#include "behavior/behaviorassignment.h"
#include "behavior/simplebehaviors.h"

HaltStrategy::HaltStrategy()
{
}

void HaltStrategy::assignBeh()
{
    BehaviorAssignment<SimpleBehaviors> haltAssignment;
    haltAssignment.setSingleAssignment(true);
    haltAssignment.assignBeh();
}
