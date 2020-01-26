#include <algorithm>
#include "parameters/field.h"
#include "model/game_state.h"
#include "utilities/comparisons.h"
#include "model/ball.h"

using namespace std::placeholders;

namespace Comparisons
{

//! @cond

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

//! @endcond

//Unary operators
bool Predicate::operator()(Robot* robot) {
    try {
        return isNotIgnored(robot) and robotCompareFn(robot, nullptr);
    } catch( ... ) {
        return isNotIgnored(robot);
    }
}
bool Predicate::operator()(const Point& point) {
    return pointCompareFn(point, Point(0,0));
}

//Binary operators
bool Predicate::operator()(Robot* a, Robot* b) {
    try {
        if(!isNotIgnored(a))
            return false;
        if(!isNotIgnored(b))
            return true;
        return robotCompareFn(a,b);
    } catch( ... ) {
        return isNotIgnored(a);
    }
}

bool Predicate::operator()(const Point& a, const Point& b) {
    return pointCompareFn(a, b);
}

//Ignore Functions
Predicate& Predicate::ignore_if(std::function<bool(Robot*)> function) {
    ignoreFunctions[nextIgnoreSpot++] = function;
    return *this;
}
Predicate& Predicate::ignoreID(int id) {
    return ignore_if(idEqual(id));
}
Predicate& Predicate::ignoreIDNot(int id) {
    return ignore_if(idNotEqual(id));
}
Predicate& Predicate::ignoreID(Robot* r) {
    return ignoreID(r->getID());
}
Predicate& Predicate::ignoreIDNot(Robot* r) {
    return ignoreIDNot(r->getID());
}
Predicate& Predicate::ignoreIDs(std::initializer_list<int> ids) {
    for(int id : ids)
        ignore_if(idEqual(id));
    return *this;
}
Predicate& Predicate::ignoreIDs(std::initializer_list<Robot*> robs) {
    for(Robot* rob : robs)
        ignore_if(idEqual(rob->getID()));
    return *this;
}
Predicate& Predicate::ignoreIDsNot(std::initializer_list<int> ids) {
    for(int id : ids)
        ignore_if(idNotEqual(id));
    return *this;
}



Robot* Predicate::maxInSet(std::set<Robot*>& robots){
    return *max(robots);
}

Robot* Predicate::maxInTeam(Team* team){
    return maxInSet(team->getRobots());
}

Robot* Predicate::minInSet(std::set<Robot*>& robots){
    return *min(robots);
}

Robot* Predicate::minInTeam(Team* team){
    return minInSet(team->getRobots());
}


void Predicate::setCompareFunction() { }


bool Predicate::isNotIgnored(Robot* robot) {
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
    return Measurements::distance(a, pt) < Measurements::distance(b, pt);
}
static bool distanceComparePointFn(const Point& a, const Point& b, const Point& pt) {
    return Measurements::distance(a, pt) < Measurements::distance(b, pt);
}

pred_distance::pred_distance(const Point& testPoint)
    : pred_testPoint(testPoint) {
        robotCompareFn = std::bind(distanceCompareRobotFn, _1, _2, pred_testPoint);
        pointCompareFn = std::bind(distanceComparePointFn, _1, _2, pred_testPoint);
    }
pred_distance::pred_distance(Robot* robot)
    : pred_distance(robot->getPosition())
    { }

pred_distanceBall::pred_distanceBall()
    : pred_distance(Ball::getPosition())
    { }
pred_distanceMyGoal::pred_distanceMyGoal()
    : pred_distance(gameState->getMyGoal())
    { }
pred_distanceOpGoal::pred_distanceOpGoal()
    : pred_distance(gameState->getOppGoal())
    { }

/****************************************************************/

//Robot on My Team, not ID 5, closest to the robot farthest from the ball
//Robot* r = Comparisons::distance(*Comparisons::distanceBall().maxMyTeam()).ignoreID(GOALIE_ID).minMyTeam();

/****************************************************************/

//! @cond
bool isFacingPointCompareFn(Robot* robot, Robot*, const Point& pred_Point, float pred_tolerance,
                            compareFunction compare_function) {
    Point robPos     = robot->getPosition();
    float robAngle   = robot->getOrientation();
    float angleTween = Measurements::angleBetween(robPos, pred_Point);
    float angleDiff  = fabs( Measurements::angleDiff(angleTween, robAngle) );
    return compare_function(angleDiff, pred_tolerance);
}
//! @endcond

//Base class constructors
_isFacingPoint::_isFacingPoint(const Point& testPoint, float tol)
    : pred_testPoint(testPoint)
    , pred_tolerance(tol)
    { }
_isFacingPoint::_isFacingPoint(Robot* testRobot, float tol)
    : _isFacingPoint(testRobot->getPosition(), tol)
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

//! @cond
static bool pointOutsideCompareFnPt(const Point& testPoint, compareFunction f) {
    if(f(abs(testPoint.x), HALF_FIELD_LENGTH)
    || f(abs(testPoint.y), HALF_FIELD_WIDTH))
        return true;
    return false;
}
static bool pointOutsideCompareFnRob(Robot* r, std::function<bool(float,float)> f) {
    return pointOutsideCompareFnPt(r->getPosition(), f);
}
//! @endcond

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
//! @cond
bool isDistGreaterCompareFnPt(const Point& a, const Point&, const Point& b, float dist, compareFunction f) {
    return f(Measurements::distance(a, b), dist);
}
bool isDistGreaterCompareFnRb(Robot* a, Robot*, const Point& b, float dist, compareFunction f) {
    return isDistGreaterCompareFnPt(a->getPosition(), Point(0,0), b, dist, f);
}
//! @endcond

pred_isDistanceToGreater::pred_isDistanceToGreater(const Point& a, float dist)
    : pred_testPoint(a)
    , pred_testDist(dist)
    { }

pred_isDistanceToGreater::pred_isDistanceToGreater(Robot* a, float dist)
    : pred_isDistanceToGreater(a->getPosition(), dist)
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
    return isFacingPoint(a, b->getPosition(), tol);
}
bool isFacingPoint(Robot* a, const Point& b, float tol) {
    return isFacingPointCompareFn(a, nullptr, b, tol, std::less<float>{});
}
bool isNotFacingPoint(Robot* a, Robot* b, float tol) {
    return isNotFacingPoint(a, b->getPosition(), tol);
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
    return isDistanceToGreater(a->getPosition(), b->getPosition(), tol);
}
bool isDistanceToGreater(const Point& a, const Point& b, float tol) {
    return isDistGreaterCompareFnPt(a, Point(), b, tol, std::greater<float>{});
}
bool isDistanceToGreater(Robot* a, const Point& b, float tol) {
    return isDistanceToGreater(a->getPosition(), b, tol);
}
bool isDistanceToGreater(const Point& a, Robot* b, float tol) {
    return isDistanceToGreater(a, b->getPosition(), tol);
}
bool isDistanceToLess(Robot* a, Robot* b, float tol) {
    return isDistanceToLess(a->getPosition(), b->getPosition(), tol);
}
bool isDistanceToLess(const Point& a, const Point& b, float tol) {
    return isDistGreaterCompareFnPt(a, Point(), b, tol, std::less<float>{});
}
bool isDistanceToLess(Robot* a, const Point& b, float tol) {
    return isDistanceToLess(a->getPosition(), b, tol);
}
bool isDistanceToLess(const Point& a, Robot* b, float tol) {
    return isDistanceToLess(a, b->getPosition(), tol);
}


}
