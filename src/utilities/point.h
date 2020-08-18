#ifndef POINT_H
#define POINT_H
#include <string>
#include <QObject>


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
    Point(const Point&);
    Point(float nx, float ny) : x(nx), y(ny) { }
    Point(float angle);
    virtual ~Point();


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
	
    // 2d vector operations

    float norm() const;  // returns the norm of the vector
    float norm2() const; // returns the norm squared
    float dot(const Point& rhs) const;    // returns the dot product
    float wedge(const Point& rhs) const;  // returns the value of the wedge product (not considering the basis)
    float angle() const;
    Point perpen() const; // returns the perpendicular vector of same size using right hand rule
    Point complexMultiplication(const Point& rhs) const;
    std::string toString() const;
public:
    float x;    //!< The X coordinate
    float y;    //!< The Y coordinate
};

QDebug operator<<(QDebug debug, const Point& p);

Q_DECLARE_METATYPE(Point);

//! @}

#endif // POINT_H
