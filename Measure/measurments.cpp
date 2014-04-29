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
