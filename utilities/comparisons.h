#ifndef COMPARISONS_H
#define COMPARISONS_H

#include <functional>
#include <algorithm>
#include <math.h>

#include "model/gamemodel.h"
#include "model/robot.h"
#include "utilities/point.h"
#include "include/config/globals.h"

namespace Comparisons
{

typedef std::function<bool(float,float)> compareFunction;

//Generic predicate that includes some "ignore" criteria
struct IgnorablePredicate
{
    //Unary operators
    bool operator()(Robot* robot);
    bool operator()(const Point& point);

    //Binary operators
    bool operator()(Robot* a, Robot* b);
    bool operator()(const Point& a, const Point& b);

    //Ignore Functions
    IgnorablePredicate& ignore_if(std::function<bool(Robot*)> function);
    IgnorablePredicate& ignoreID(int id);
    IgnorablePredicate& ignoreIDNot(int id);
    IgnorablePredicate& ignoreID(Robot* r);
    IgnorablePredicate& ignoreIDNot(Robot* r);
    IgnorablePredicate& ignoreIDs(std::initializer_list<int> ids);
    IgnorablePredicate& ignoreIDs(std::initializer_list<Robot*> robs);
    IgnorablePredicate& ignoreIDsNot(std::initializer_list<int> ids);

    //Helper Query Functions
    //Helpers to query the predicate on a Container of Robot* or Point
    template<typename Container>
    bool any_of(Container& c) {
        return std::any_of(std::begin(c), std::end(c), *this);
    }
    template<typename Container>
    bool all_of(Container& c) {
        return std::all_of(std::begin(c), std::end(c), *this);
    }
    template<typename Container>
    bool none_of(Container& c) {
        return std::none_of(std::begin(c), std::end(c), *this);
    }

    //Helpers to query the min/max/any value (iterator) on a Container
    template<typename Container>
    typename Container::iterator max(Container& c) {
        return std::max_element(std::begin(c), std::end(c), *this);
    }
    template<typename Container>
    typename Container::iterator min(Container& c) {
        return std::min_element(std::begin(c), std::end(c), *this);
    }
    template<typename Container>
    typename Container::iterator any(Container& c) {
        return std::find_if(std::begin(c), std::end(c), *this);
    }

    //MyTeam/OpTeam/AnyTeam helper functions
    Robot* maxMyTeam();
    Robot* maxOpTeam();
    Robot* maxAnyTeam();
    Robot* minMyTeam();
    Robot* minOpTeam();
    Robot* minAnyTeam();
    Robot* anyMyTeam();
    Robot* anyOpTeam();
    Robot* anyAnyTeam();

public:
    virtual void setCompareFunction();

protected:
    std::function<bool(Robot*, Robot*)> robotCompareFn;
    std::function<bool(const Point&, const Point&)> pointCompareFn;
    std::function<bool(float, float)> compare_function;

private:
    std::array<std::function<bool(Robot*)>, 10> ignoreFunctions;
    int nextIgnoreSpot = 0;

    bool isNotIgnored(Robot* robot);
};

/****************************************************************/

struct pred_id : public IgnorablePredicate
{
    pred_id(int);
    pred_id(std::initializer_list<int>);
protected:
    pred_id(){}
};

struct pred_idNot : public pred_id
{
    pred_idNot(int);
    pred_idNot(std::initializer_list<int>);
};

/****************************************************************/

/****************************************************************/

struct pred_distance : public IgnorablePredicate
{
    pred_distance(const Point& testPoint);
    pred_distance(Robot* robot);
protected:
    Point pred_testPoint;
};

struct pred_distanceBall : public pred_distance {
    pred_distanceBall();
};
struct pred_distanceMyGoal : public pred_distance {
    pred_distanceMyGoal();
};
struct pred_distanceOpGoal : public pred_distance {
    pred_distanceOpGoal();
};


/****************************************************************/


//Robot on My Team, not ID 5, closest to the robot farthest from the ball
//Robot* r = Comparisons::distance(*Comparisons::distanceBall().maxMyTeam()).ignoreID(5).minMyTeam();

/****************************************************************/

//Default "is facing" tolerance (a 75 degree cone)
#define _IFP_TOL (5*M_PI/12)

struct _isFacingPoint : public IgnorablePredicate
{
    _isFacingPoint(const Point&, float tol = _IFP_TOL);
    _isFacingPoint(Robot*,       float tol = _IFP_TOL);
protected:
    Point  pred_testPoint;
    float  pred_tolerance;
};

struct pred_isFacingPoint : public _isFacingPoint {
    using _isFacingPoint::_isFacingPoint;
    void setCompareFunction() override;
};

struct pred_isNotFacingPoint : public _isFacingPoint {
    using _isFacingPoint::_isFacingPoint;
    void setCompareFunction() override;
};


/****************************************************************/


struct pred_isPointOutsideField : public IgnorablePredicate
{
    pred_isPointOutsideField();
    void setCompareFunction() override;
};

struct pred_isPointInsideField : public pred_isPointOutsideField
{
    using pred_isPointOutsideField::pred_isPointOutsideField;
    void setCompareFunction() override;
};


/****************************************************************/


bool isDistGreaterCompareFnPt(const Point&, const Point&, const Point&, float, compareFunction f);
bool isDistGreaterCompareFnRb(Robot*, Robot*, const Point&, float, compareFunction f);

struct pred_isDistanceToGreater : public IgnorablePredicate
{
    pred_isDistanceToGreater(Robot*, float);
    pred_isDistanceToGreater(const Point&, float);
    void setCompareFunction() override;
protected:
    Point pred_testPoint;
    float pred_testDist;
};

struct pred_isDistanceToLess : public pred_isDistanceToGreater
{
    using pred_isDistanceToGreater::pred_isDistanceToGreater;
    void setCompareFunction() override;
};


/****************************************************************/


#define PREDICATE_FUNCTION(name, function_name) \
    template<typename... Args, typename = \
        typename std::enable_if<std::is_constructible<name, Args...>::value>::type> \
    name function_name(Args... args) { \
        static_assert(std::is_constructible<name, Args...>::value, \
            "No match for construction with these arguments"); \
        name d(args...); \
        d.setCompareFunction(); \
        return std::move(d); \
    }

PREDICATE_FUNCTION(pred_isFacingPoint,       isFacingPoint)
PREDICATE_FUNCTION(pred_isNotFacingPoint,    isNotFacingPoint)
PREDICATE_FUNCTION(pred_isPointOutsideField, isPointOutsideField)
PREDICATE_FUNCTION(pred_isPointInsideField,  isPointInsideField)
PREDICATE_FUNCTION(pred_isDistanceToGreater, isDistanceToGreater)
PREDICATE_FUNCTION(pred_isDistanceToLess,    isDistanceToLess)
PREDICATE_FUNCTION(pred_distance,            distance)
PREDICATE_FUNCTION(pred_distanceBall,        distanceBall)
PREDICATE_FUNCTION(pred_distanceMyGoal,      distanceMyGoal)
PREDICATE_FUNCTION(pred_distanceOpGoal,      distanceOpGoal)
PREDICATE_FUNCTION(pred_id,                  id)
PREDICATE_FUNCTION(pred_idNot,               idNot)

bool isFacingPoint(Robot*, Robot*, float tol = _IFP_TOL);
bool isFacingPoint(Robot*, const Point&, float tol = _IFP_TOL);
bool isNotFacingPoint(Robot*, Robot*, float = _IFP_TOL);
bool isNotFacingPoint(Robot*, const Point&, float = _IFP_TOL);
bool isPointOutsideField(const Point&);
bool isPointOutsideField(Robot*);
bool isPointInsideField(const Point&);
bool isPointInsideField(Robot*);
bool isDistanceToGreater(Robot*, Robot*, float);
bool isDistanceToGreater(const Point&, const Point&, float);
bool isDistanceToGreater(Robot*, const Point&, float);
bool isDistanceToGreater(const Point&, Robot*, float);
bool isDistanceToLess(Robot*, Robot*, float);
bool isDistanceToLess(const Point&, const Point&, float);
bool isDistanceToLess(Robot*, const Point&, float);
bool isDistanceToLess(const Point&, Robot*, float);

#undef _IFP_TOL

}

#endif
