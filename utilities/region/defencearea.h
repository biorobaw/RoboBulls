#ifndef DEFENCEAREA_H
#define DEFENCEAREA_H

#include "include/config/globals.h"
#include "rectangle.h"
#include "sector.h"
#include "iostream"


class DefenceArea : public Region
{
public:
    DefenceArea();
    bool contains(const Point &) override;
    void draw() override;
private:
    // Consult SSL Rule book for clarification of the following regions
    static Rectangle r;
    static Sector s1, s2;
};

#endif // DEFENCEAREA_H
