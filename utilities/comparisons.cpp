#include <algorithm>
#include "include/config/globals.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"

using namespace std::placeholders;

namespace Comparisons
{

struct idFilter {
    std::function<bool(int, int)> compare_function;
    int compare_id;
    idFilter(int id) : compare_id(id)
        { }
    bool operator()(Robot* robot) {
        return compare_function(robot->getID(), compare_id);
    }
};

struct idEqual : public idFilter {
    idEqual(int id) : idFilter(id)
    { compare_function = std::equal_to<int>{}; }
};

struct idNotEqual : public idFilter {
    idNotEqual(int id) : idFilter(id)
    { compare_function = std::not_equal_to<int>{}; }
};


//Unary operators
bool IgnorablePredicate::operator()(Robot* robot) {
    try {
        return isNotIgnored(robot) and robotCompareFn(robot, nullptr);
    } catch( ... ) {
        return isNotIgnored(robot);
    }
}
bool IgnorablePredicate::operator()(const Point& point) {
    return pointCompareFn(point, Point(0,0));
}

//Binary operators
bool IgnorablePredicate::operator()(Robot* a, Robot* b) {
    try {
        return isNotIgnored(a) and isNotIgnored(b) and robotCompareFn(a, b);
    } catch( ... ) {
        return isNotIgnored(a);
    }
}
bool IgnorablePredicate::operator()(const Point& a, const Point& b) {
    return pointCompareFn(a, b);
}

//Ignore Functions
IgnorablePredicate& IgnorablePredicate::ignore_if(std::function<bool(Robot*)> function) {
    ignoreFunctions[nextIgnoreSpot++] = function;
    return *this;
}
IgnorablePredicate& IgnorablePredicate::ignoreID(int id) {
    return ignore_if(idEqual(id));
}
IgnorablePredicate& IgnorablePredicate::ignoreIDNot(int id) {
    return ignore_if(idNotEqual(id));
}
IgnorablePredicate& IgnorablePredicate::ignoreID(Robot* r) {
    return ignoreID(r->getID());
}
IgnorablePredicate& IgnorablePredicate::ignoreIDNot(Robot* r) {
    return ignoreIDNot(r->getID());
}
IgnorablePredicate& IgnorablePredicate::ignoreIDs(std::initializer_list<int> ids) {
    for(int id : ids)
        ignore_if(idEqual(id));
    return *this;
}
IgnorablePredicate& IgnorablePredicate::ignoreIDs(std::initializer_list<Robot*> robs) {
    for(Robot* rob : robs)
        ignore_if(idEqual(rob->getID()));
    return *this;
}
IgnorablePredicate& IgnorablePredicate::ignoreIDsNot(std::initializer_list<int> ids) {
    for(int id : ids)
        ignore_if(idNotEqual(id));
    return *this;
}

//MyTeam/OpTeam/AnyTeam helper functions
Robot* IgnorablePredicate::maxMyTeam() {
    auto& myTeam = gameModel->getMyTeam();
    return *max(myTeam);
}

Robot* IgnorablePredicate::maxOpTeam() {
    auto& opTeam = gameModel->getOponentTeam();
    return *max(opTeam);
}

Robot* IgnorablePredicate::maxAnyTeam() {
    return std::max(maxMyTeam(), maxOpTeam(), *this);
}

Robot* IgnorablePredicate::minMyTeam() {
    auto& myTeam = gameModel->getMyTeam();
    return *min(myTeam);
}

Robot* IgnorablePredicate::minOpTeam() {
    auto& opTeam = gameModel->getOponentTeam();
    return *min(opTeam);
}

Robot* IgnorablePredicate::minAnyTeam() {
    return std::min(minMyTeam(), minMyTeam(), *this);
}

Robot* IgnorablePredicate::anyMyTeam() {
    auto& myTeam = gameModel->getMyTeam();
    auto it = any(myTeam);
    return it != myTeam.end() ? *it : NULL;
}

Robot* IgnorablePredicate::anyOpTeam() {
    auto& opTeam = gameModel->getOponentTeam();
    auto it = any(opTeam);
    return it != opTeam.end() ? *it : NULL;
}

Robot* IgnorablePredicate::anyAnyTeam() {
    Robot* r = anyMyTeam();
    if(r) return r;
    r = anyOpTeam();
    if(r) return r;
    return NULL;
}

void IgnorablePredicate::setCompareFunction() { }


bool IgnorablePredicate::isNotIgnored(Robot* robot) {
    for(int i = 0; i != nextIgnoreSpot; ++i)
        if(ignoreFunctions[i](robot))
            return false;
    return true;
}

/****************************************************************/


pred_id::pred_id(int whichId) {
    ignoreIDNot(whichId);
}
pred_id::pred_id(std::initializer_list<int> ids) {
    ignoreIDs(ids);
}

pred_idNot::pred_idNot(int whichId) {
    ignoreID(whichId);
}
pred_idNot::pred_idNot(std::initializer_list<int> ids) {
    ignoreIDs(ids);
}



/****************************************************************/

static bool distanceCompareRobotFn(Robot* a, Robot* b, const Point& pt) {
    return Measurments::distance(a, pt) < Measurments::distance(b, pt);
}
static bool distanceComparePointFn(const Point& a, const Point& b, const Point& pt) {
    return Measurments::distance(a, pt) < Measurments::distance(b, pt);
}

pred_distance::pred_distance(const Point& testPoint)
    : pred_testPoint(testPoint) {
        robotCompareFn = std::bind(distanceCompareRobotFn, _1, _2, pred_testPoint);
        pointCompareFn = std::bind(distanceComparePointFn, _1, _2, pred_testPoint);
    }
pred_distance::pred_distance(Robot* robot)
    : pred_distance(robot->getRobotPosition())
    { }

pred_distanceBall::pred_distanceBall()
    : pred_distance(gameModel->getBallPoint())
    { }
pred_distanceMyGoal::pred_distanceMyGoal()
    : pred_distance(gameModel->getMyGoal())
    { }
pred_distanceOpGoal::pred_distanceOpGoal()
    : pred_distance(gameModel->getOpponentGoal())
    { }

/****************************************************************/

//Robot on My Team, not ID 5, closest to the robot farthest from the ball
//Robot* r = Comparisons::distance(*Comparisons::distanceBall().maxMyTeam()).ignoreID(GOALIE_ID).minMyTeam();

/****************************************************************/

bool isFacingPointCompareFn(Robot* robot, Robot* null, const Point& pred_Point, float pred_tolerance,
                            std::function<bool(float,float)> compare_function) {
    (void)(null);
    Point robPos     = robot->getRobotPosition();
    float robAngle   = robot->getOrientation();
    float angleTween = Measurments::angleBetween(robPos, pred_Point);
    float angleDiff  = abs( Measurments::angleDiff(angleTween, robAngle) );
    return compare_function(angleDiff, pred_tolerance);
}

//Base class constructors
_isFacingPoint::_isFacingPoint(const Point& testPoint, float tol)
    : pred_testPoint(testPoint)
    , pred_tolerance(tol)
    { }
_isFacingPoint::_isFacingPoint(Robot* testRobot, float tol)
    : _isFacingPoint(testRobot->getRobotPosition(), tol)
    { }

void pred_isFacingPoint::setCompareFunction() {
    compare_function = std::less<float>{};
    robotCompareFn = std::bind
            (isFacingPointCompareFn, _1, _2, pred_testPoint, pred_tolerance, compare_function);
}
void pred_isNotFacingPoint::setCompareFunction() {
    compare_function = std::greater<float>{};
    robotCompareFn = std::bind
            (isFacingPointCompareFn, _1, _2, pred_testPoint, pred_tolerance, compare_function);
}


/****************************************************************/

static bool pointOutsideCompareFnPt(const Point& testPoint, compareFunction f) {
    if(f(abs(testPoint.x), FIELD_WIDTH*0.966))
        return true;
    if(f(abs(testPoint.y), FIELD_LENGTH*0.95))
        return true;
    return false;
}

static bool pointOutsideCompareFnRob(Robot* r, std::function<bool(float,float)> f) {
    return pointOutsideCompareFnPt(r->getRobotPosition(), f);
}

pred_isPointOutsideField::pred_isPointOutsideField() {
}

void pred_isPointOutsideField::setCompareFunction() {
    compare_function = std::greater<float>{};
    pointCompareFn = std::bind(pointOutsideCompareFnPt, _1, compare_function);
    robotCompareFn = std::bind(pointOutsideCompareFnRob, _1, compare_function);
}

void pred_isPointInsideField::setCompareFunction() {
    compare_function = std::less<float>{};
    pointCompareFn = std::bind(pointOutsideCompareFnPt, _1, compare_function);
    robotCompareFn = std::bind(pointOutsideCompareFnRob, _1, compare_function);
}


/****************************************************************/

bool isDistGreaterCompareFnPt(const Point& a, const Point& null, const Point& b, float dist,
                       compareFunction f) {
    (void)(null);
    return f(Measurments::distance(a, b), dist);
}

bool isDistGreaterCompareFnRb(Robot* a, Robot* null, const Point& b, float dist, compareFunction f) {
    (void)(null);
    return isDistGreaterCompareFnPt(a->getRobotPosition(), Point(0,0), b, dist, f);
}

pred_isDistanceToGreater::pred_isDistanceToGreater(const Point& a, float dist)
    : pred_testPoint(a)
    , pred_testDist(dist)
    { }

pred_isDistanceToGreater::pred_isDistanceToGreater(Robot* a, float dist)
    : pred_isDistanceToGreater(a->getRobotPosition(), dist)
    { }

void pred_isDistanceToGreater::setCompareFunction() {
    compare_function = std::greater<float>{};
    pointCompareFn = std::bind(isDistGreaterCompareFnPt, _1, _2, pred_testPoint, pred_testDist,
                           compare_function);
    robotCompareFn = std::bind(isDistGreaterCompareFnRb, _1, _2, pred_testPoint, pred_testDist,
                           compare_function);
}

void pred_isDistanceToLess::setCompareFunction() {
    compare_function = std::less<float>{};
    pointCompareFn = std::bind(isDistGreaterCompareFnPt, _1, _2, pred_testPoint, pred_testDist,
                           compare_function);
    robotCompareFn = std::bind(isDistGreaterCompareFnRb, _1, _2, pred_testPoint, pred_testDist,
                           compare_function);
}

/*******************************************************************/

bool isFacingPoint(Robot* a, Robot* b, float tol) {
    return isFacingPoint(a, b->getRobotPosition(), tol);
}
bool isFacingPoint(Robot* a, const Point& b, float tol) {
    return isFacingPointCompareFn(a, nullptr, b, tol, std::less<float>{});
}
bool isNotFacingPoint(Robot* a, Robot* b, float tol) {
    return isNotFacingPoint(a, b->getRobotPosition(), tol);
}
bool isNotFacingPoint(Robot* a, const Point& b, float tol) {
    return isFacingPointCompareFn(a, nullptr, b, tol, std::greater<float>{});
}
bool isPointOutsideField(const Point& a) {
    return pointOutsideCompareFnPt(a, std::greater<float>{});
}
bool isPointOutsideField(Robot* a) {
    return pointOutsideCompareFnRob(a, std::greater<float>{});
}
bool isPointInsideField(const Point& a) {
    return pointOutsideCompareFnPt(a, std::less<float>{});
}
bool isPointInsideField(Robot* a) {
    return pointOutsideCompareFnRob(a, std::less<float>{});
}
bool isDistanceToGreater(Robot* a, Robot* b, float tol) {
    return isDistanceToGreater(a->getRobotPosition(), b->getRobotPosition(), tol);
}
bool isDistanceToGreater(const Point& a, const Point& b, float tol) {
    return isDistGreaterCompareFnPt(a, Point(), b, tol, std::greater<float>{});
}
bool isDistanceToGreater(Robot* a, const Point& b, float tol) {
    return isDistanceToGreater(a->getRobotPosition(), b, tol);
}
bool isDistanceToGreater(const Point& a, Robot* b, float tol) {
    return isDistanceToGreater(a, b->getRobotPosition(), tol);
}
bool isDistanceToLess(Robot* a, Robot* b, float tol) {
    return isDistanceToLess(a->getRobotPosition(), b->getRobotPosition(), tol);
}
bool isDistanceToLess(const Point& a, const Point& b, float tol) {
    return isDistGreaterCompareFnPt(a, Point(), b, tol, std::less<float>{});
}
bool isDistanceToLess(Robot* a, const Point& b, float tol) {
    return isDistanceToLess(a->getRobotPosition(), b, tol);
}
bool isDistanceToLess(const Point& a, Robot* b, float tol) {
    return isDistanceToLess(a, b->getRobotPosition(), tol);
}


}
