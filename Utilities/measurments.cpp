#include <stdlib.h>
#include "measurments.h"

float Measurments::distance(const Point& p1, const Point& p2)
{
    return sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
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


int closestPoint(const Point& p1, const Point* array, int n, Point* out)
{
    int ret = 0, i;
    float closestDist = Measurments::distance(p1, array[0]);

    for(i = 1; i != n; ++i) {
        float newDist = Measurments::distance(p1, array[i]);

        if(newDist < closestDist) {
            closestDist = newDist;
            ret = i;
        }
    }

    if(out != NULL) *out = array[i];

    return ret;
}

float Measurments::slop(Point p1, Point p2)
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
//    // Find the complementary, just in case the returned angle by atan2 is not the smaller one
//    float argComp;
//    if (arg > 0)
//        argComp = -(2 * M_PI - arg);
//    else
//        argComp = 2 * M_PI + arg;
//    // Return the minimum in absolute value
//    if (abs(arg) < abs(argComp))
//        return arg;
//    else
//        return argComp;
    return arg;
}

float Measurments::angleSum(float angle1, float angle2){
    // Convert angles to unitary complex numbers z1 and z2
    float r1 = cos(angle1);
    float i1 = sin(angle1);
    float r2 = cos(angle2);
    float i2 = sin(angle2);

    // Multiply to get z3
    float r3 = r1*r2 - i1 * i2;
    float i3 = i1*r2 + r1 * i2;
    // Get the argument
    float arg = atan2(i3, r3);
//    // Find the complementary, just in case the returned angle by atan2 is not the smaller one
//    float argComp;
//    if (arg > 0)
//        argComp = -(2 * M_PI - arg);
//    else
//        argComp = 2 * M_PI + arg;
//    // Return the minimum in absolute value
//    if (abs(arg) < abs(argComp))
//        return arg;
//    else
//        return argComp;
    return arg;
}
