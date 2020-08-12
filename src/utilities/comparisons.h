#ifndef COMPARISONS_H
#define COMPARISONS_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <functional>
#include <algorithm>
#include <math.h>
#include "utilities/point.h"
#include <array>
#include <set>
#include <QSet>

class Robot;
class RobotTeam;
class Ball;


/*! @brief Defines comparison predicates and information querying functions.
 * @details `Comparisons` is a framework for extendable, flexible
 * predicate logic functionality.
 * Comparisons defines a set of commonly-used predicates and
 * comparison functions to query information on <i>containers</i> of Robot* or
 * Point. These functions are meant to be used with the max_element
 * and min_element STL algorithms, or standalone. <br><br>
 * A commonly-used query we need, for example, is to find the robot closest to the ball.
 * Using Comparisons: <br>
 * <i>Robot* best = Comparisons::distanceBall().minMyTeam();</i><br><br>
 * It can also be chained to provide complex queries easily:<br>
 * //Robot on My Team, not the goalie, closest to the robot farthest from the ball<br>
 * <i>Robot* r = Comparisons::distance(*Comparisons::distanceBall().maxMyTeam()).ignoreID(GOALIE_ID).minMyTeam();</i>
 * <br><br>Comparisons defines many object functors like these for various
 * min/max comparisons on containers, along with ignore functions to
 * not consider certain IDs in these comparisons. */

namespace Comparisons
{

typedef std::function<bool(float,float)> compareFunction;

//! @brief Base class predicate that includes some "ignore" criteria
class Predicate
{
public:
    //! @name Unary operators
    //! @{
    //! @brief Invoked on function need unary predicates, such as find_if
    bool operator()(Robot* robot);
    bool operator()(const Point& point);
    //! @}

    //! @name Binary operators
    //! @{
    //! @brief Invoked on function need binary predicates, such as max_element
    bool operator()(Robot* a, Robot* b);
    bool operator()(const Point& a, const Point& b);
    //! @}

    /*! @name Ignore Functions
     * @{
     * @brief Used to ignore robots matching an ID or a predicate. Are chainable.
     * Use these functions to not consider a robot when querying a predicate. */
    Predicate& ignore_if(std::function<bool(Robot*)> function);
    Predicate& ignoreID(int id);
    Predicate& ignoreIDNot(int id);
    Predicate& ignoreID(Robot* r);
    Predicate& ignoreIDNot(Robot* r);
    Predicate& ignoreIDs(std::initializer_list<int> ids);
    Predicate& ignoreIDs(std::initializer_list<Robot*> robs);
    Predicate& ignoreIDsNot(std::initializer_list<int> ids);
    //! @}

    //! @name Boolean Container Queries
    //! @{
    //! @details Helpers to query the predicate on a Container of Robot* or Point

    //! @brief Returns true if *this holds on <i>any of</i> the elements of the container c
    template<typename Container>
    bool any_of(Container& c) {
        return std::any_of(std::begin(c), std::end(c), *this);
    }
    //! @brief Returns true if *this holds on <i>all of</i> the elements of the container c
    template<typename Container>
    bool all_of(Container& c) {
        return std::all_of(std::begin(c), std::end(c), *this);
    }
    //! @brief Returns true if *this holds on <i>none of</i> the elements of the container c
    template<typename Container>
    bool none_of(Container& c) {
        return std::none_of(std::begin(c), std::end(c), *this);
    }
    //! @}


    //! @name Iterator Container Queries
    //! @{
    //! @details Helpers to query an element by predicate on a Container of Robot* or Point

    //! @brief Returns the maximum element in container C based on the predicate
    template<typename Container>
    typename Container::iterator max(Container& c) {
        return std::max_element(std::begin(c), std::end(c), *this);
    }
    //! @brief Returns the minimum element in container C based on the predicate
    template<typename Container>
    typename Container::iterator min(Container& c) {
        return std::min_element(std::begin(c), std::end(c), *this);
    }
    //! @brief Returns the first element element in container C for which the predicate returns true
    template<typename Container>
    typename Container::iterator any(Container& c) {
        return std::find_if(std::begin(c), std::end(c), *this);
    }
    //! @}

    /*! @name GameModel helper functions
     * @{
     * @brief Helper functions to query the container queries on GameModel teams */
    Robot* maxInSet(QSet<Robot*>& robots);
    Robot* maxInTeam(RobotTeam* team);
    Robot* minInSet(QSet<Robot*>& robots);
    Robot* minInTeam(RobotTeam* team);
    //! @}

    virtual void setCompareFunction();

protected:
    std::function<bool(Robot*, Robot*)> robotCompareFn;
    std::function<bool(const Point&, const Point&)> pointCompareFn;
    std::function<bool(float, float)> compare_function;

private:
    std::array<std::function<bool(Robot*)>, 4> ignoreFunctions;
    int nextIgnoreSpot = 0;
    bool isNotIgnored(Robot* robot);
};

//! @cond

/****************************************************************/

struct pred_id : public Predicate
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

struct pred_distance : public Predicate
{
    pred_distance(const Point& testPoint);
    pred_distance(Robot* robot);
protected:
    Point pred_testPoint;
};

struct pred_distanceBall : public pred_distance {
    pred_distanceBall(Point ball_point);
};



/****************************************************************/


//Robot on My Team, not the goalie, closest to the robot farthest from the ball
//Robot* r = Comparisons::distance(*Comparisons::distanceBall().maxMyTeam())
//              .ignoreID(GOALIE_ID).minMyTeam();

/****************************************************************/

//Default "is facing" tolerance (a 75 degree cone)
#define _IFP_TOL (5*M_PI/12)

struct _isFacingPoint : public Predicate
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


struct pred_isPointOutsideField : public Predicate
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

struct pred_isDistanceToGreater : public Predicate
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

//! @endcond

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
PREDICATE_FUNCTION(pred_id,                  id)
PREDICATE_FUNCTION(pred_idNot,               idNot)

/*! @{
 * @name In addition to functions, Comparisons provides measurements-style \
 * standalone math querying functions<br> */
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
//!@}

#undef _IFP_TOL

}

#endif
