#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include "model/robot/robot.h"
#include "measurements.h"
#include <algorithm>

float Measurements::mag(const Point& p)
{
    return hypot(p.x, p.y);
}

float Measurements::distance(const Point& p1, const Point& p2) {
    float dy = p2.y - p1.y;
    float dx = p2.x - p1.x;
    return sqrt((dy * dy) + (dx * dx));
}
float Measurements::distance(const Point& p1, Robot* r1) {
    return Measurements::distance(p1, *r1);
}
float Measurements::distance(Robot* r1, const Point& p1) {
    return Measurements::distance(*r1, p1);
}
float Measurements::distance(Robot* r1, Robot* r2) {
    return Measurements::distance(*r1, *r2);
}


Point Measurements::midPoint(const Point& a, const Point& b){
    return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}
Point Measurements::midPoint(const Point& p1, Robot* r1) {
    return Measurements::midPoint(p1, *r1);
}
Point Measurements::midPoint(Robot* r1, const Point& p1) {
    return Measurements::midPoint(*r1, p1);
}
Point Measurements::midPoint(Robot* r1, Robot* r2) {
    return Measurements::midPoint(*r1, *r2);
}


float Measurements::angleBetween(const Point& p1, const Point& p2) {
    //Behold a perfect application of arc tangent.
    return atan2(p2.y - p1.y, p2.x - p1.x);
}
float Measurements::angleBetween(const Point& p1, Robot* r1) {
    return Measurements::angleBetween(p1, *r1);
}
float Measurements::angleBetween(Robot* r1, const Point& p1) {
    return Measurements::angleBetween(*r1, p1);
}
float Measurements::angleBetween(Robot* r1, Robot* r2) {
    return Measurements::angleBetween(*r1, *r2);
}

bool Measurements::isClose(const Point& p1, const Point& p2, float tol) {

    return (fabs(p1.y - p2.y) <= tol) && (fabs(p2.x - p1.x) <= tol);
}
bool Measurements::isClose(const Point& p1, Robot* r1, float tol) {
    return Measurements::isClose(p1, *r1, tol);
}
bool Measurements::isClose(Robot* r1, const Point& p1, float tol) {
    return Measurements::isClose(*r1, p1, tol);
}
bool Measurements::isClose(Robot* r1, Robot* r2, float tol) {
    return Measurements::isClose(*r1, *r2, tol);
}


float Measurements::slope(Point p1, Point p2)
{
    return atan2((p1.y-p2.y),(p1.x-p2.x));
}


float Measurements::angleDiff(float angle1, float angle2){
    return Point(angle1-angle2).angle();
}


float Measurements::angleSum(float angle1, float angle2)
{
    return Point(angle1+angle2).angle();
}


bool Measurements::angleInRange(float angle, float start, float end)
{
    end = (end - start) < 0.0f ? end - start + 2*M_PI : end - start;
    angle = (angle - start) < 0.0f ? angle - start + 2*M_PI : angle - start;
    return (angle < end);
}


bool Measurements::isClose(float angle1, float angle2, float tol)
{
    return fabs(Measurements::angleDiff(angle1, angle2)) < tol;
}


float Measurements::lineDistance(const Point& p0, const Point& LStart, const Point& LEnd)
{
    auto delta = LEnd-LStart;
    return abs((p0-LEnd).wedge(delta))/delta.norm();
}


Point Measurements::lineSegmentPoint(const Point& p0, const Point& LStart, const Point& LEnd)
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


float Measurements::lineSegmentDistance(const Point& p0, const Point& LStart, const Point& LEnd)
{
    return distance(p0, lineSegmentPoint(p0,LStart,LEnd));
}


bool Measurements::noObstaclesInPath(const std::vector<Point>& obstacles, const Point& A, const Point& B, const int& tolerance)
{
    for(const Point& obstacle : obstacles)
        if(lineSegmentDistance(obstacle, A, B) < tolerance)
            return false;
    return true;
}

Robot* Measurements::robotInPath(const std::vector<Robot*>& robots, const Point& A, const Point& B, const int& tolerance)
{
    for(Robot* r : robots)
        if(lineSegmentDistance(*r, A, B) < tolerance)
            return r;
    return nullptr;
}

Point Measurements::unitVector(const Point& vector)
{
    return vector/mag(vector);
}
