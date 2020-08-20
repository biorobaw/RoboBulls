#define _USE_MATH_DEFINES
#include <cmath>
#include "defencearea.h"
#include "model/field.h"
#include "configuration/constants.h"

DefenceArea::DefenceArea(int side) :
    side_sign(side == FIELD_SIDE_POSITIVE ? 1 : -1),
    positive_area( Field::HALF_FIELD_LENGTH - Field::DEF_AREA_WIDTH,
                  -Field::DEF_AREA_LENGTH/2,
                   Field::HALF_FIELD_LENGTH,
                   Field::DEF_AREA_LENGTH),
    area( side_sign * positive_area.min_x,
         -Field::DEF_AREA_LENGTH/2,
          side_sign * positive_area.max_x,
          Field::DEF_AREA_LENGTH)
{


}


bool DefenceArea::contains(const Point& p)
{
    return area.contains(p);
}

bool DefenceArea::contains(Point p, const float tol)
{
    // to simplify we use positive side rectancle
    // thus, symetrize if negative defense area
    p.x *= side_sign;

    // add tolerance  // dont we also want to expand y directions?
    positive_area.min_x -=tol;

    // calculate result
    bool result = positive_area.contains(p);

    // remove tolerance
    positive_area.min_x +=tol;

    // return result
    return result;

}

void DefenceArea::draw()
{
    area.draw();
}

void DefenceArea::expelPoint(Point& p)
{
    //   delta_x             obs weareae_area for
    //    <->                simplicity but use positive_area
    // |-----  ^
    // |     | | delta_y
    // |  .  | V            if delta_y >= delta_x:
    // |  p  |                  move point to outter vertical line
    // |-----| y=0          else
    // |     |                  move point to upper or lower line according to sign(y)
    // |     |
    // |     |
    // |-----

    // to simplify we use positive side rectancle
    // thus, symetrize if negative defense area
    p.x *= side_sign;

    if(positive_area.contains(p)){
        int delta_y = positive_area.max_y - abs(p.y);
        int delta_x = p.x - positive_area.min_x;

        if( delta_y >= delta_x){
            p.x = positive_area.min_x;
        } else p.y = p.y > 0 ? positive_area.max_y : positive_area.min_y;

    }

    // undo symmetrization:
    p.x *= side_sign;

}

std::vector<Point> DefenceArea::intersectSegment(Point PA, Point PB)
{
    return area.intersectSegment(PA,PB);

}

std::vector<Point> DefenceArea::intersectLine(Point PA, Point PB)
{
    return area.intersectLine(PA,PB);
}

