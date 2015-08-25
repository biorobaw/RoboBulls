#ifndef REGION_H
#define REGION_H
#include "point.h"
#include "model/gamemodel.h"
#include "model/robot.h"

/*! @brief `Region` is a utility class to define a 2D region (rectangle) on the field.
 *
 * A Region is defined in terms of two Points, given in the constructor as X1, X2, Y1, Y2.
 * The Region class contains some useful information querying functions once
 * the region is defined, and is used sparsely throughout the code. */

class Region
{
public:
    Region();

    /*! @brief Constructs region with a top-left point (X1,Y1) and
     * bottom-left point (X2,Y2)
     * \param X1 The left edge of the field
     * \param Y1 The top edge of the region
     * \param X2 the right edge of the region
     * \param Y2 The bottom edge of the region */
    Region(float X1,float X2,float Y1,float Y2);

    /*! @brief Returns true if a point is inside the rectangular region */
    bool contains(const Point& point);

    /*! @brief Returns true if the line from p0 and p1 passes through the region */
    bool containsLine(const Point& p0, const Point& p1);

    /*! @brief Returns the sum of numOfOpponents and numOfTeammates.
     * \see numOfOpponents
     * \see numOfTeammates */
    int numOfRobots();

    /*! @brief Returns number of opponents in the region.*/
    int numOfOpponents();

    /*! @brief Returns number of teammates in the region. */
    int numOfTeammates();

    /*! @brief Excludes the robots in the vectors passed passed in. */
    int numOfRobots(vector <Robot *> ignoreOpponents,vector <Robot *>ignoreTeammates);

    /*! @brief Returns the calculated center of the Region */
    Point centre();

    /*! @brief Returns true if the ball is in either goal-post. */
    static bool goalScored();

    /*! @brief Returns a string representation of the Region */
    string toString();

    /*! @brief Associate a numeric strategic value with the region */
    void setStrategicValue(int);

    /*! @brief Retrieves a numeric strategic value with the region */
    int getStrategicValue();

public:
    static Region lowerLeftRegion;  //!< The lower-left (negative X) quarter of the field
    static Region topLeftRegion;    //!< Top-left quarter of the field
    static Region topRightRegion;   //!< The top-right quarter (positive X) of the field
    static Region lowerRightRegion; //!< The lower-right quarter of the field
    static Region goalLeftRegion;   //!< Encompassing the goalie box on the -x side of the field
    static Region goalRightRegion;  //!< Encompassing the goalie box on the +x side of the field

private:
    float minX, maxX, minY, maxY;
    int strategic_value;
};

#endif // REGION_H


