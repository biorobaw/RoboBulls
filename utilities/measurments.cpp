#include <stdlib.h>
#include "measurments.h"

float Measurments::distance(const Point& p1, const Point& p2)
{
	float dy = p2.y - p1.y;
	float dx = p2.x - p1.x;
    return sqrt((dy * dy) + (dx * dx));
}


Point Measurments::midPoint(const Point& a, const Point& b)
{
    return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}


float Measurments::angleBetween(const Point& p1, const Point& p2)
{
    //Behold a perfect application of arc tangent.
    return atan2(p2.y - p1.y, p2.x - p1.x);
}


bool Measurments::isClose(const Point& p1, const Point& p2, float tol)
{
    return (fabs(p1.y - p2.y) <= tol) && (fabs(p2.x - p1.x) <= tol);
}


float Measurments::slope(Point p1, Point p2)
{
    return atan2((p1.y-p2.y),(p1.x-p2.x));
}


float Measurments::angleDiff(float angle1, float angle2){
    // Convert angles to unitary complex numbers z1 and z2
    float r1 = cos(angle1);
    float i1 = sin(angle1);
    float r2 = cos(angle2);
    float i2 = sin(angle2);
    // Conjugate z1
    i1 = -i1;
    // Multiply to get z3
    float r3 = r1*r2 - i1 * i2;
    float i3 = i1*r2 + r1 * i2;
    // Get the argument
    float arg = atan2(i3, r3);

    return arg;
}


float Measurments::angleSum(float angle1, float angle2)
{
    float sAlpha = sin(angle1);
    float cBeta = cos(angle2);
    float cAlpha = cos(angle1);
    float sBeta = sin(angle2);
    float result = sAlpha*cBeta + cAlpha*sBeta;
    return asin(result);
}


bool Measurments::isClose(float angle1, float angle2, float tol)
{
	return abs(Measurments::angleDiff(angle1, angle2)) < tol;
}


float Measurments::lineDistance(const Point& p0, const Point& LStart, const Point& LEnd)
{
	float y2 = LEnd.y;
	float y1 = LStart.y;
	float x2 = LEnd.x;
	float x1 = LStart.x;

	return abs((y2-y1)*p0.x - (x2-x1)*p0.y - x1*y2 + x2*y1)
			/ Measurments::distance(LStart, LEnd);
}

