#include <stdlib.h>
#include <cmath>
#include "model/robot.h"
#include "measurements.h"

float Measurements::mag(const Point& p)
{
    return hypot(p.x,p.y);
}

float Measurements::distance(const Point& p1, const Point& p2) {
    float dy = p2.y - p1.y;
    float dx = p2.x - p1.x;
    return sqrt((dy * dy) + (dx * dx));
}
float Measurements::distance(const Point& p1, Robot* r1) {
    return Measurements::distance(p1, r1->getPosition());
}
float Measurements::distance(Robot* r1, const Point& p1) {
    return Measurements::distance(r1->getPosition(), p1);
}
float Measurements::distance(Robot* r1, Robot* r2) {
    return Measurements::distance(r1->getPosition(), r2->getPosition());
}


Point Measurements::midPoint(const Point& a, const Point& b){
    return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}
Point Measurements::midPoint(const Point& p1, Robot* r1) {
    return Measurements::midPoint(p1, r1->getPosition());
}
Point Measurements::midPoint(Robot* r1, const Point& p1) {
    return Measurements::midPoint(r1->getPosition(), p1);
}
Point Measurements::midPoint(Robot* r1, Robot* r2) {
    return Measurements::midPoint(r1->getPosition(), r2->getPosition());
}


float Measurements::angleBetween(const Point& p1, const Point& p2) {
    //Behold a perfect application of arc tangent.
    return atan2(p2.y - p1.y, p2.x - p1.x);
}
float Measurements::angleBetween(const Point& p1, Robot* r1) {
    return Measurements::angleBetween(p1, r1->getPosition());
}
float Measurements::angleBetween(Robot* r1, const Point& p1) {
    return Measurements::angleBetween(r1->getPosition(), p1);
}
float Measurements::angleBetween(Robot* r1, Robot* r2) {
    return Measurements::angleBetween(r1->getPosition(), r2->getPosition());
}

bool Measurements::isClose(const Point& p1, const Point& p2, float tol) {
    return (fabs(p1.y - p2.y) <= tol) && (fabs(p2.x - p1.x) <= tol);
}
bool Measurements::isClose(const Point& p1, Robot* r1, float tol) {
    return Measurements::isClose(p1, r1->getPosition(), tol);
}
bool Measurements::isClose(Robot* r1, const Point& p1, float tol) {
    return Measurements::isClose(r1->getPosition(), p1, tol);
}
bool Measurements::isClose(Robot* r1, Robot* r2, float tol) {
    return Measurements::isClose(r1->getPosition(), r2->getPosition(), tol);
}


float Measurements::slope(Point p1, Point p2)
{
    return atan2((p1.y-p2.y),(p1.x-p2.x));
}


float Measurements::angleDiff(float angle1, float angle2){
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


float Measurements::angleSum(float angle1, float angle2)
{
    float sinA = sin(angle1);
    float cosA = cos(angle1);
    float sinB = sin(angle2);
    float cosB = cos(angle2);
    return atan2(sinA*cosB+sinB*cosA, cosA*cosB-sinA*sinB);
}


bool Measurements::isClose(float angle1, float angle2, float tol)
{
    return abs(Measurements::angleDiff(angle1, angle2)) < tol;
}


float Measurements::lineDistance(const Point& p0, const Point& LStart, const Point& LEnd)
{
	float y2 = LEnd.y;
	float y1 = LStart.y;
	float x2 = LEnd.x;
	float x1 = LStart.x;
    return abs((y2-y1)*p0.x - (x2-x1)*p0.y - x1*y2 + x2*y1) / Measurements::distance(LStart, LEnd);
}


Point Measurements::linePoint(const Point& p0, const Point& LStart, const Point& LEnd)
{
    // Return minimum distance between the line segment and point p0
    const double l2 = pow((LStart.x - LEnd.x),2) + pow((LStart.y - LEnd.y),2);  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) return LStart;   // v == w case
    // Consider the line extending the segment, parameterized as start + t (end - start).
    // We find projection of point p onto the line.
    // It falls where t = [(p0-start) . (end-start)] / |end-start|^2
    // We clamp t from [0,1] to handle points outside the line segment.
    Point A = p0 - LStart, B = LEnd - LStart;
    const double t = std::max(0.0, std::min(1.0, (A.x*B.x + A.y*B.y) / l2));
    Point projection = LStart + (LEnd - LStart) * t;  // Projection falls on the segment
    return projection;
}
