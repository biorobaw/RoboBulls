#include <stdlib.h>
#include <cmath>
#include "model/robot.h"
#include "measurments.h"

float Measurments::distance(const Point& p1, const Point& p2) {
    float dy = p2.y - p1.y;
    float dx = p2.x - p1.x;
    return sqrt((dy * dy) + (dx * dx));
}
float Measurments::distance(const Point& p1, Robot* r1) {
    return Measurments::distance(p1, r1->getPosition());
}
float Measurments::distance(Robot* r1, const Point& p1) {
    return Measurments::distance(r1->getPosition(), p1);
}
float Measurments::distance(Robot* r1, Robot* r2) {
    return Measurments::distance(r1->getPosition(), r2->getPosition());
}


Point Measurments::midPoint(const Point& a, const Point& b){
    return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}
Point Measurments::midPoint(const Point& p1, Robot* r1) {
    return Measurments::midPoint(p1, r1->getPosition());
}
Point Measurments::midPoint(Robot* r1, const Point& p1) {
    return Measurments::midPoint(r1->getPosition(), p1);
}
Point Measurments::midPoint(Robot* r1, Robot* r2) {
    return Measurments::midPoint(r1->getPosition(), r2->getPosition());
}


float Measurments::angleBetween(const Point& p1, const Point& p2) {
    //Behold a perfect application of arc tangent.
    return atan2(p2.y - p1.y, p2.x - p1.x);
}
float Measurments::angleBetween(const Point& p1, Robot* r1) {
    return Measurments::angleBetween(p1, r1->getPosition());
}
float Measurments::angleBetween(Robot* r1, const Point& p1) {
    return Measurments::angleBetween(r1->getPosition(), p1);
}
float Measurments::angleBetween(Robot* r1, Robot* r2) {
    return Measurments::angleBetween(r1->getPosition(), r2->getPosition());
}

bool Measurments::isClose(const Point& p1, const Point& p2, float tol) {
    return (fabs(p1.y - p2.y) <= tol) && (fabs(p2.x - p1.x) <= tol);
}
bool Measurments::isClose(const Point& p1, Robot* r1, float tol) {
    return Measurments::isClose(p1, r1->getPosition(), tol);
}
bool Measurments::isClose(Robot* r1, const Point& p1, float tol) {
    return Measurments::isClose(r1->getPosition(), p1, tol);
}
bool Measurments::isClose(Robot* r1, Robot* r2, float tol) {
    return Measurments::isClose(r1->getPosition(), r2->getPosition(), tol);
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
    float sinA = sin(angle1);
    float cosA = cos(angle1);
    float sinB = sin(angle2);
    float cosB = cos(angle2);
    return atan2(sinA*cosB+sinB*cosA, cosA*cosB-sinA*sinB);
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
    return abs((y2-y1)*p0.x - (x2-x1)*p0.y - x1*y2 + x2*y1) / Measurments::distance(LStart, LEnd);
}


Point Measurments::linePoint(const Point& p0, const Point& LStart, const Point& LEnd)
{
    //See https://pantherfile.uwm.edu/ericskey/www/231material/hws19.pdf
    float y2 = LEnd.y;
    float y1 = LStart.y;
    float x2 = LEnd.x;
    float x1 = LStart.x;
    float m = (y2 - y1) / (x2 - x1);
    float b = m * (-x1) + y1;
    float x = (p0.x + m*p0.y - b*m) / (m*m + 1);
    float y = (m*p0.x + m*m*p0.y + b) / (m*m + 1);
    return Point(x, y);
}
