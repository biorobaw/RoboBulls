#ifndef POINT_H
#define POINT_H
#include <string>

//! @addtogroup everydayuse
//! @{

/*! @brief 2D Point Class
 * @details A Point is a 2D vector in space. Typically used to represent positions
 * on the Robot Field
 */
class Point
{
public:
    Point() : x(0), y(0) { }
    Point(float nx, float ny) : x(nx), y(ny) { }

    //! @name Operator Overloads
    //! @{
    bool	operator==(const Point& rhs);
    bool	operator!=(const Point& rhs);
    Point& 	operator=(const Point& rhs);
	Point&	operator+=(const Point& rhs);
	Point&	operator-=(const Point& rhs);
	Point&	operator*=(float rhs);
	Point&	operator/=(float rhs);
    Point	operator+(const Point& rhs) const;
    Point	operator-(const Point& rhs) const;
    Point   operator*(float rhs) const;
    Point   operator/(float rhs) const;
    //! @}
	
    std::string toString() const;
public:
    float x;    //!< The X coordinate
    float y;    //!< The Y coordinate
};

//! @}

#endif // POINT_H
