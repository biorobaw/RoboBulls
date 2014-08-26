#include "defendoneonone.h"

#include "skill/gotopositionwithorientation.h"
#include "model/gamemodel.h"
#include "utilities/measurments.h"

DefendOneOnOne::DefendOneOnOne(const ParameterList& list)
{
    UNUSED_PARAM(list);
}

void DefendOneOnOne::perform(Robot *r)
{
    GameModel * gm = GameModel::getModel();
    auto goToPenalty = new Skill::GoToPositionWithOrientation(gm->getPenaltyPoint(),
                                                              Measurments::angleBetween(gm->getPenaltyPoint(), gm->getOpponentGoal()));
    goToPenalty->perform(r);


    delete goToPenalty;
}
