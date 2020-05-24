#include "graphics_field.h"
#include <QPainter>

#include "gui/data/gui_field.h"
#include "gui/style_sheets/color_palettes.h"


GraphicsField::GraphicsField()
{


    // default values
    setX(-( GuiField::fieldHalfLength()-100));
    setY(-(GuiField::fieldHalfWidth()-100)); // Y seems to be 100 off (?)
    setZValue(1);
    grid = false;
}

QRectF GraphicsField::boundingRect() const
{
    return QRectF(0,0,GuiField::fieldLength(),GuiField::fieldWidth());
}





void GraphicsField::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rec = boundingRect();



    auto& pallete = field_pallete_maps[colorScheme];
    painter->setPen(pallete.defaultPen);
    painter->fillRect(rec, pallete.brush);


    // Mid line
    QLineF centerLine(GuiField::fieldHalfLength(),0,GuiField::fieldHalfLength(),GuiField::fieldWidth());

    // Center circle
    int centerDiam    = rec.height() / 4;
    int centerCornerX = (rec.width()/2)  - (centerDiam/2);
    int centerCornerY = (rec.height()/2) - (centerDiam/2);
    QRectF centerCircle(centerCornerX,centerCornerY, centerDiam,centerDiam);

    // Penalty areas
    int boxWidth = centerDiam/2;
    int boxHeight = centerDiam*1.3333;
    int BoxY = (rec.height()/2) - (boxHeight/2);
    int yellBoxX = rec.width()-boxWidth;
    QRectF blueBox(0,BoxY, boxWidth,boxHeight);
    QRectF yellBox(yellBoxX,BoxY,boxWidth,boxHeight);


    // Drawing field lines
    painter->drawLine(centerLine);
    painter->drawEllipse(centerCircle);
    painter->drawRect(blueBox);
    painter->drawRect(yellBox);
    // Drawing Grid
    if (grid) {
        painter->setPen(blackLinePen);
        int longDenom = (rec.width()/2) / gridScale;
        int latiDenom = (rec.height()/2) / gridScale;
        for (int i=1; i<longDenom; i++) {
            int gridX = i*rec.width()/longDenom;
            painter->drawLine(QLineF(gridX,0, gridX,rec.height()));
        }
        for (int j=1; j<latiDenom; j++) {
            int gridY = j*rec.height()/latiDenom;
            painter->drawLine(QLineF(0,gridY, rec.width(),gridY));
        }
    }

    // Goals
    int goalWidth  = boxWidth/2.5;
    int goalHeight = boxHeight/1.8;
    int goalY = (rec.height()/2)-(goalHeight/2);
    int blueGoalX = goalWidth*-1;
    QRectF left_goal(blueGoalX,goalY, goalWidth,goalHeight);
    QRectF right_goal(rec.width(),goalY, goalWidth,goalHeight);

    // Drawing goals first backgorund, then lines
    painter->fillRect(left_goal,goal_area_brush);
    painter->fillRect(right_goal,goal_area_brush);

    bool left_is_blue = GuiField::getTeamOnSide(FIELD_SIDE_NEGATIVE) == ROBOT_TEAM_BLUE;
    painter->setPen(left_is_blue ? blueLinePen : yellowLinePen);
    painter->drawRect(left_goal);
    painter->setPen(!left_is_blue ? blueLinePen : yellowLinePen);
    painter->drawRect(right_goal);

    // draw outter lines
    painter->setPen(pallete.borderPen);
    painter->drawRect(rec);
}

