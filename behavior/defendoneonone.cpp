#include "defendoneonone.h"
#include "model/gamemodel.h"
#include "utilities/measurments.h"

DefendOneOnOne::DefendOneOnOne(const ParameterList& list)
	: GenericMovementBehavior(list)
{
    UNUSED_PARAM(list);
}

void DefendOneOnOne::perform(Robot *r)
{
    GameModel * gm = GameModel::getModel();
	
	setMovementTargets(gm->getPenaltyPoint(), 
		Measurments::angleBetween(gm->getPenaltyPoint(), gm->getOpponentGoal()));
		
	GenericMovementBehavior::perform(r);
}
