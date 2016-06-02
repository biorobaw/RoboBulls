#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "region.h"

#include <sstream>
#include <cmath>
#include <vector>

#include "model/robot.h"
#include "model/gamemodel.h"

class Rectangle : public Region
{
public:
    Rectangle();

    /*! @brief Constructs rectangle with a bottom-left point (X1,Y1) and
     * top-right point (X2,Y2)
     */
    Rectangle(float X1, float Y1, float X2, float Y2);

    /*! @brief Returns true if a point is inside the rectangular region */
    bool contains(const Point& point) override;

    /*! @brief Displays the region on the GUI */
    void draw() override;

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
    int numOfRobots(std::vector<Robot*>& ignoreOpponents,
                    std::vector<Robot*>& ignoreTeammates);

    /*! @brief Returns the calculated center of the Region */
    Point centre();

    /*! @brief Returns a string representation of the Region */
    std::string toString();

    float min_x, max_x, min_y, max_y;
};

#endif // RECTANGLE_H
