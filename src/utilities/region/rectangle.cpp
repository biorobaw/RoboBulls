#include "rectangle.h"
#include "model/robot/robot.h"
#include <algorithm>

Rectangle::Rectangle()
{
}


Rectangle::Rectangle(float X1,float Y1,float X2,float Y2)
{
    min_x = std::min(X1,X2);
    max_x = std::max(X1,X2);
    min_y = std::min(Y1,Y2);
    max_y = std::max(Y1,Y2);

}

bool Rectangle::contains(const Point& point)
{
    return point.x >= min_x && point.x <= max_x && point.y >= min_y && point.y <= max_y;
}

//See http://stackoverflow.com/questions/1585525/
bool Rectangle::containsLine(const Point& p0, const Point& p1)
{
    float x1 = p0.x;
    float x2 = p1.x;
    float y1 = p0.y;
    float y2 = p1.y;

    //Completely outside
    if ((x1 <= min_x && x2 <= min_x) ||
        (y1 <= min_y && y2 <= min_y) ||
        (x1 >= max_x && x2 >= max_x) ||
        (y1 >= max_y && y2 >= max_y))
        return false;

    float m = (y2 - y1) / (x2 - x1);

    float y = m * (min_x - x1) + y1;
    if (y > min_y && y < max_y) return true;

    y = m * (max_x - x1) + y1;
    if (y > min_y && y < max_y) return true;

    float x = (min_y - y1) / m + x1;
    if (x > min_x && x < max_x) return true;

    x = (max_y - y1) / m + x1;
    if (x > min_x && x < max_x) return true;

    return false;
}

int Rectangle::numOfRobots(std::set<Robot*>& robots)
{

    int number_of_robots = 0;

    for (Robot * robot : robots)
    {
        if(Rectangle::contains(*robot))
        {
           number_of_robots++;
        }
    }
    return number_of_robots;
}







Point Rectangle::centre()
{
    return Point((min_x+max_x)/2,(min_y+max_y)/2);
}

std::string Rectangle::toString()
{
    std::stringstream ss;

    ss << "minX, maxX, minY, maxY: "
    << min_x << ", " << max_x << ", "
    << min_y << ", " << max_y << std::endl;

    return ss.str();
}

void Rectangle::draw()
{
    for(int x = min_x; x <= max_x; x+=10)
        for(int y = min_y; y <= max_y; y+=10)
            if(this->contains(Point(x,y)))
                GuiInterface::getGuiInterface()->drawPoint(Point(x,y));
}

namespace {
    // nameless namespace for constants and functions required
    // by Cohen-sutherland line clipping algorithm
    // original source: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

    typedef int OutCode;

    const int INSIDE = 0; // 0000
    const int LEFT = 1;   // 0001
    const int RIGHT = 2;  // 0010
    const int BOTTOM = 4; // 0100
    const int TOP = 8;    // 1000

    OutCode ComputeOutCode(const Point& p, Rectangle* r)
    {
        OutCode code;
        code = INSIDE;          // initialised as being inside of [[clip window]]
        if (p.x < r->min_x)      code |= LEFT;   // to the left of clip window
        else if (p.x > r->max_x) code |= RIGHT;  // to the right of clip window

        if (p.y < r->min_y)      code |= BOTTOM; // below the clip window
        else if (p.y > r->max_y) code |= TOP;    // above the clip window
        return code;
    }

}


std::vector<Point> Rectangle::intersectSegment(Point p0, Point p1)
{
    std::vector<Point> intersection;

    // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
    OutCode outcode0 = ComputeOutCode(p0, this);
    OutCode outcode1 = ComputeOutCode(p1, this);
    bool accept = false;

    while (true) {
        if (!(outcode0 | outcode1)) {
            // bitwise OR is 0: both points inside window; trivially accept and exit loop
            accept = true;
            break;
        } else if (outcode0 & outcode1) {
            // bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
            // or BOTTOM), so both must be outside window; exit loop (accept is false)
            break;
        } else {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            double x, y;

            // At least one endpoint is outside the clip rectangle; pick it.
            OutCode outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

            // Now find the intersection point;
            // use formulas:
            //   slope = (y1 - y0) / (x1 - x0)
            //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
            //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
            // No need to worry about divide-by-zero because, in each case, the
            // outcode bit being tested guarantees the denominator is non-zero
            if (outcodeOut & TOP) {           // point is above the clip window
                x = p0.x + (p1.x - p0.x) * (max_y - p0.y) / (p1.y - p0.y);
                y = max_y;
            } else if (outcodeOut & BOTTOM) { // point is below the clip window
                x = p0.x + (p1.x - p0.x) * (min_y - p0.y) / (p1.y - p0.y);
                y = min_y;
            } else if (outcodeOut & RIGHT) {  // point is to the right of clip window
                y = p0.y + (p1.y - p0.y) * (max_x - p0.x) / (p1.x - p0.x);
                x = max_x;
            } else if (outcodeOut & LEFT) {   // point is to the left of clip window
                y = p0.y + (p1.y - p0.y) * (min_x - p0.x) / (p1.x - p0.x);
                x = min_x;
            }

            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcodeOut == outcode0) {
                p0.x = x;
                p0.y = y;
                outcode0 = ComputeOutCode(p0,this);
            } else {
                p1.x = x;
                p1.y = y;
                outcode1 = ComputeOutCode(p1,this);
            }
        }
    }
    if (accept) {
        intersection.push_back(p0);
        if(p1!=p0) intersection.push_back(p1);

    }
    return intersection;
}


std::vector<Point> Rectangle::intersectLine(Point p0, Point p1){

    std::vector<Point> intersection;

    // check p0 and p1 form a line:
    if(p0 == p1){
        // we only have a point, check if its inside the rectangle and return result
        if(contains(p0)) intersection.push_back(p0);
        return intersection;
    }

    // get the vertexes of the poligon
    Point points[4] = {
        Point(min_x,min_y),
        Point(min_x,max_y),
        Point(max_x,max_y),
        Point(max_x,min_y)
    };

    Point perpen = Point(p0.y-p1.y, p1.x-p0.x); // get vector perpendicular to the line

    float zero_height = p0.dot(perpen);

    float relative_heights[4];
    for(int i=0; i<4; i++) relative_heights[i] = points[i].dot(perpen) - zero_height;

    // try to intersect each segment:
    for(int i=0; i<4; i++){
        int j = (i+1)%4;
        auto& hi = relative_heights[i];
        auto& hj = relative_heights[j];

        // if line doesnt intersect segment, skip segment
        // note : intersections with the vertices are handled only when the index is i to avoid double processing
        if(( hj > 0 && hi > 0 ) || // both points are above the line
           ( hj < 0 && hi < 0 ) || // both points are below the line
             hj == 0 ) // this case is left for next cycle
            continue; // the point will be processed on next cycle

        // Get the intersection by interpolating:
        hi = abs(hi);
        hj = abs(hj);
        auto inter = (points[i]*hj + points[j]*hi)/(hi+hj);
        intersection.push_back(inter);
    }

}











