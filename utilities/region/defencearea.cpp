#include "defencearea.h"

DefenceArea::DefenceArea(bool team) :team(team)
{
    if(team == OUR_TEAM)    // Initialize our defence area
    {
        r = Rectangle(-HALF_FIELD_LENGTH,
                        -DEF_AREA_OFFSET,
                        -HALF_FIELD_LENGTH + DEF_AREA_RADIUS,
                        DEF_AREA_OFFSET);

        s1 = Sector(Point(-HALF_FIELD_LENGTH, DEF_AREA_OFFSET),
                            DEF_AREA_RADIUS,
                            0, M_PI_2);

        s2 = Sector(Point(-HALF_FIELD_LENGTH, -DEF_AREA_OFFSET),
                            DEF_AREA_RADIUS,
                            -M_PI_2, 0);
    }
    else    // initialize opponent defence area
    {
        r = Rectangle(HALF_FIELD_LENGTH,
                        -DEF_AREA_OFFSET,
                        HALF_FIELD_LENGTH - DEF_AREA_RADIUS,
                        DEF_AREA_OFFSET);

        s1 = Sector(Point(HALF_FIELD_LENGTH, DEF_AREA_OFFSET),
                            DEF_AREA_RADIUS,
                            M_PI_2, M_PI);

        s2 = Sector(Point(HALF_FIELD_LENGTH, -DEF_AREA_OFFSET),
                            DEF_AREA_RADIUS,
                            -M_PI, -M_PI_2);
    }
}


bool DefenceArea::contains(const Point& p)
{
    return r.contains(p) || s1.contains(p) || s2.contains(p);
}

bool DefenceArea::contains(const Point& p, const float tol)
{
    // Expand to tolerance
    if(team == OUR_TEAM)
        r.max_x = -HALF_FIELD_LENGTH + DEF_AREA_RADIUS + tol;
    else
        r.min_x = HALF_FIELD_LENGTH - DEF_AREA_RADIUS - tol;

    s1.r = DEF_AREA_RADIUS+tol;
    s2.r = DEF_AREA_RADIUS+tol;

    bool result = r.contains(p) || s1.contains(p) || s2.contains(p);

    s1.r = DEF_AREA_RADIUS;
    s2.r = DEF_AREA_RADIUS;

    if(team == OUR_TEAM)
        r.max_x = -HALF_FIELD_LENGTH + DEF_AREA_RADIUS;
    else
        r.min_x = HALF_FIELD_LENGTH - DEF_AREA_RADIUS;

    return result;
}

void DefenceArea::draw()
{
    r.draw();
    s1.draw();
    s2.draw();
}

void DefenceArea::expelPoint(Point& point)
{
    if(r.contains(point))
    {
        if(team == OUR_TEAM)
            point.x = -FIELD_LENGTH/2 + (DEF_AREA_RADIUS+DEF_AREA_TOL);
        else
            point.x =  FIELD_LENGTH/2 - (DEF_AREA_RADIUS+DEF_AREA_TOL);
    }
    else if(s1.contains(point))
    {
        // Circle info
        float p = s1.c.x, q = s1.c.y, r = s1.r + DEF_AREA_TOL;

        // Line info
        float m = (point.y - q)/(point.x - p);
        if(m == 0)
        {
            point.y = s1.c.y + DEF_AREA_RADIUS + DEF_AREA_TOL;
            return;
        }
        float c = q - m*p;

        // Intersection Equation Variables
        float A = m*m + 1;
        float B = 2*(m*c - m*q - p);
        float C = q*q - r*r + p*p - 2*c*q + c*c;

        if(team == OUR_TEAM)
            point.y = m*((-B + sqrt(B*B-4*A*C))/(2*A)) + c;
        else
            point.y = m*((-B - sqrt(B*B-4*A*C))/(2*A)) + c;

        point.x = p + (point.y-q)/m;
    }
    else if(s2.contains(point))
    {
        // Circle info
        float p = s2.c.x, q = s2.c.y, r = s2.r + DEF_AREA_TOL;

        // Line info
        float m = (point.y - q)/(point.x - p);
        if(m == 0)
        {
            point.y = s2.c.y - DEF_AREA_RADIUS - DEF_AREA_TOL;
            return;
        }
        float c = q - m*p;

        // Intersection Equation Variables
        float A = m*m + 1;
        float B = 2*(m*c - m*q - p);
        float C = q*q - r*r + p*p - 2*c*q + c*c;

        if(team == OUR_TEAM)
            point.y = m*((-B + sqrt(B*B-4*A*C))/(2*A)) + c;
        else
            point.y = m*((-B - sqrt(B*B-4*A*C))/(2*A)) + c;

        point.x = p + (point.y-q)/m;
    }
}

std::vector<Point> DefenceArea::lineSegmentIntercepts(const Point& PA, const Point& PB)
{
    std::vector<Point> intercepts;

    // Check stretch
    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    float x1 = PA.x, y1 = PA.y;
    float x2 = PB.x, y2 = PB.y;

    float x3 = FIELD_LENGTH/2 - DEF_AREA_RADIUS - DEF_AREA_TOL, y3 = DEF_AREA_OFFSET;
    float x4 = FIELD_LENGTH/2 - DEF_AREA_RADIUS - DEF_AREA_TOL, y4 = -DEF_AREA_OFFSET;

    if (team == OUR_TEAM)
    {
        x3 *= -1;
        x4 *= -1;
    }

    float Px_num = (x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4);
    float Py_num = (x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4);
    float dem = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

    if(dem != 0)    // Lines are not parallel
    {
        // Check if intersection is within stretch
        float Py = Py_num/dem;
        if(Py < DEF_AREA_OFFSET && Py > -DEF_AREA_OFFSET)
        {
            // Check if intersection is within line segment
            float Px = Px_num/dem;
            Point PC(Px,Py);

            float dist_diff = Measurements::distance(PA,PC)
                            + Measurements::distance(PC,PB)
                            - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                intercepts.push_back(Point(Px, Py));
        }
    }

    // Check top sector
    // Line info
    float m = (PA.y - PB.y)/(PA.x - PB.x);
    float c = PB.y - m*PB.x;

    // http://math.stackexchange.com/questions/228841/how-do-i-calculate-the-intersections-of-a-straight-line-and-a-circle
    // Circle info
    float p = s1.c.x, q = s1.c.y, r = s1.r + DEF_AREA_TOL;

    // Intersection Equation Variables
    float A = m*m + 1;
    float B = 2*(m*c - m*q - p);
    float C = q*q - r*r + p*p - 2*c*q + c*c;

    float det = B*B - 4*A*C;
    if(det >= 0)
    {
        float x1 = (-B + sqrt(det))/(2*A);
        float y1 = m*x1 + c;

        float x2 = (-B - sqrt(det))/(2*A);
        float y2 = m*x2 + c;

        // Check if within correct sector
        if(team == OUR_TEAM)
        {
            Rectangle rec(-HALF_FIELD_LENGTH, DEF_AREA_OFFSET,
                        -HALF_FIELD_LENGTH+DEF_AREA_RADIUS+DEF_AREA_TOL, DEF_AREA_OFFSET+DEF_AREA_RADIUS+DEF_AREA_TOL);

            // Check if within line segment
            Point PC(x1,y1);
            float dist_diff = Measurements::distance(PA,PC)
                            + Measurements::distance(PC,PB)
                            - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                if(rec.contains(PC))
                    intercepts.push_back(PC);

            PC = Point(x2,y2);
            dist_diff = Measurements::distance(PA,PC)
                      + Measurements::distance(PC,PB)
                      - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                if(rec.contains(Point(x2,y2)))
                    intercepts.push_back(Point(x2, y2));
        }
        else
        {
            Rectangle rec(HALF_FIELD_LENGTH-DEF_AREA_RADIUS-DEF_AREA_TOL, DEF_AREA_OFFSET,
                        HALF_FIELD_LENGTH, DEF_AREA_OFFSET+DEF_AREA_RADIUS+DEF_AREA_TOL);

            // Check if within line segment
            Point PC(x1,y1);
            float dist_diff = Measurements::distance(PA,PC)
                            + Measurements::distance(PC,PB)
                            - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                if(rec.contains(PC))
                    intercepts.push_back(PC);

            PC = Point(x2,y2);
            dist_diff = Measurements::distance(PA,PC)
                      + Measurements::distance(PC,PB)
                      - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                if(rec.contains(PC))
                    intercepts.push_back(PC);
        }
    }

    // Check bottom sector
    // http://math.stackexchange.com/questions/228841/how-do-i-calculate-the-intersections-of-a-straight-line-and-a-circle
    // Circle info
    p = s2.c.x, q = s2.c.y, r = s2.r+DEF_AREA_TOL;

    // Intersection Equation Variables
    A = m*m + 1;
    B = 2*(m*c - m*q - p);
    C = q*q - r*r + p*p - 2*c*q + c*c;

    det = B*B - 4*A*C;
    if(det >= 0)
    {
        float x1 = (-B + sqrt(det))/(2*A);
        float y1 = m*x1 + c;

        float x2 = (-B - sqrt(det))/(2*A);
        float y2 = m*x2 + c;

        if(team == OUR_TEAM)
        {
            Rectangle rec(-HALF_FIELD_LENGTH, -DEF_AREA_OFFSET-DEF_AREA_RADIUS-DEF_AREA_TOL,
                        -HALF_FIELD_LENGTH+DEF_AREA_RADIUS+DEF_AREA_TOL, -DEF_AREA_OFFSET);

            // Check if within line segment
            Point PC(x1,y1);
            float dist_diff = Measurements::distance(PA,PC)
                            + Measurements::distance(PC,PB)
                            - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                if(rec.contains(PC))
                    intercepts.push_back(PC);

            PC = Point(x2,y2);
            dist_diff = Measurements::distance(PA,PC)
                      + Measurements::distance(PC,PB)
                      - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                if(rec.contains(PC))
                    intercepts.push_back(PC);
        }
        else
        {
            Rectangle rec(HALF_FIELD_LENGTH-DEF_AREA_RADIUS-DEF_AREA_TOL, -DEF_AREA_OFFSET-DEF_AREA_RADIUS-DEF_AREA_TOL,
                        HALF_FIELD_LENGTH, -DEF_AREA_OFFSET);

            // Check if within line segment
            Point PC(x1,y1);
            float dist_diff = Measurements::distance(PA,PC)
                            + Measurements::distance(PC,PB)
                            - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                if(rec.contains(PC))
                    intercepts.push_back(PC);

            PC = Point(x2,y2);
            dist_diff = Measurements::distance(PA,PC)
                      + Measurements::distance(PC,PB)
                      - Measurements::distance(PA,PB);

            if(-0.1 <= dist_diff && dist_diff <= 0.1)
                if(rec.contains(PC))
                    intercepts.push_back(PC);
        }
    }

    return intercepts;
}

std::vector<Point> DefenceArea::lineIntercepts(const Point& PA, const Point& PB)
{
    std::vector<Point> intercepts;

    // Check stretch
    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    float x1 = PA.x, y1 = PA.y;
    float x2 = PB.x, y2 = PB.y;

    float x3 = FIELD_LENGTH/2 - DEF_AREA_RADIUS - DEF_AREA_TOL, y3 = DEF_AREA_OFFSET;
    float x4 = FIELD_LENGTH/2 - DEF_AREA_RADIUS - DEF_AREA_TOL, y4 = -DEF_AREA_OFFSET;

    if (team == OUR_TEAM)
    {
        x3 *= -1;
        x4 *= -1;
    }

    float Px_num = (x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4);
    float Py_num = (x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4);
    float dem = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

    if(dem != 0)    // Lines are not parallel
    {
        // Check if intersection is within stretch
        float Px = Px_num/dem;
        float Py = Py_num/dem;
        if(Py < DEF_AREA_OFFSET && Py > -DEF_AREA_OFFSET)
            intercepts.push_back(Point(Px, Py));
    }

    // Check top sector
    // Line info
    float m = (PA.y - PB.y)/(PA.x - PB.x);
    float c = PB.y - m*PB.x;

    // http://math.stackexchange.com/questions/228841/how-do-i-calculate-the-intersections-of-a-straight-line-and-a-circle
    // Circle info
    float p = s1.c.x, q = s1.c.y, r = s1.r + DEF_AREA_TOL;

    // Intersection Equation Variables
    float A = m*m + 1;
    float B = 2*(m*c - m*q - p);
    float C = q*q - r*r + p*p - 2*c*q + c*c;

    float det = B*B - 4*A*C;
    if(det >= 0)
    {
        float x1 = (-B + sqrt(det))/(2*A);
        float y1 = m*x1 + c;

        float x2 = (-B - sqrt(det))/(2*A);
        float y2 = m*x2 + c;

        // Check if within correct sector
        if(team == OUR_TEAM)
        {
            Rectangle rec(-HALF_FIELD_LENGTH, DEF_AREA_OFFSET,
                        -HALF_FIELD_LENGTH+DEF_AREA_RADIUS+DEF_AREA_TOL, DEF_AREA_OFFSET+DEF_AREA_RADIUS+DEF_AREA_TOL);

            // Check if within line segment
            Point PC(x1,y1);
            if(rec.contains(PC))
                intercepts.push_back(PC);

            PC = Point(x2,y2);
            if(rec.contains(Point(x2,y2)))
                intercepts.push_back(Point(x2, y2));
        }
        else
        {
            Rectangle rec(HALF_FIELD_LENGTH-DEF_AREA_RADIUS-DEF_AREA_TOL, DEF_AREA_OFFSET,
                        HALF_FIELD_LENGTH, DEF_AREA_OFFSET+DEF_AREA_RADIUS+DEF_AREA_TOL);

            // Check if within line segment
            Point PC(x1,y1);
            if(rec.contains(PC))
                intercepts.push_back(PC);

            PC = Point(x2,y2);
            if(rec.contains(PC))
                intercepts.push_back(PC);
        }
    }

    // Check bottom sector
    // http://math.stackexchange.com/questions/228841/how-do-i-calculate-the-intersections-of-a-straight-line-and-a-circle
    // Circle info
    p = s2.c.x, q = s2.c.y, r = s2.r+DEF_AREA_TOL;

    // Intersection Equation Variables
    A = m*m + 1;
    B = 2*(m*c - m*q - p);
    C = q*q - r*r + p*p - 2*c*q + c*c;

    det = B*B - 4*A*C;
    if(det >= 0)
    {
        float x1 = (-B + sqrt(det))/(2*A);
        float y1 = m*x1 + c;

        float x2 = (-B - sqrt(det))/(2*A);
        float y2 = m*x2 + c;

        if(team == OUR_TEAM)
        {
            Rectangle rec(-HALF_FIELD_LENGTH, -DEF_AREA_OFFSET-DEF_AREA_RADIUS-DEF_AREA_TOL,
                        -HALF_FIELD_LENGTH+DEF_AREA_RADIUS+DEF_AREA_TOL, -DEF_AREA_OFFSET);

            // Check if within line segment
            Point PC(x1,y1);
            if(rec.contains(PC))
                intercepts.push_back(PC);

            PC = Point(x2,y2);
            if(rec.contains(PC))
                intercepts.push_back(PC);
        }
        else
        {
            Rectangle rec(HALF_FIELD_LENGTH-DEF_AREA_RADIUS-DEF_AREA_TOL, -DEF_AREA_OFFSET-DEF_AREA_RADIUS-DEF_AREA_TOL,
                        HALF_FIELD_LENGTH, -DEF_AREA_OFFSET);

            // Check if within line segment
            Point PC(x1,y1);
            if(rec.contains(PC))
                intercepts.push_back(PC);

            PC = Point(x2,y2);
            if(rec.contains(PC))
                intercepts.push_back(PC);
        }
    }

    return intercepts;
}

