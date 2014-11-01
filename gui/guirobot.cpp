#include "guirobot.h"
#include <QLabel>
#include "mainwindow.h"

GuiRobot::GuiRobot()
{
    Pressed = false;
//    setFlag(ItemIsMovable); // makes it movable
    setFlag(ItemIsSelectable);
    int radius = boundingRect().width() / 2;
    setTransformOriginPoint(radius,radius);   // sets center point, around which it rotates
}

QRectF GuiRobot::boundingRect() const
{
    int diameter = 200;
//    int radius = diameter/2;
    return QRectF(0,0,diameter,diameter);
}

void GuiRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);


    QRectF rec = boundingRect();
    QRectF base(30,25,150,150);

    QBrush brush(Qt::magenta, Qt::SolidPattern);
    // ID circles colors
    QBrush topRtBrush(Qt::magenta, Qt::SolidPattern);
    QBrush topLtBrush(Qt::magenta, Qt::SolidPattern);
    QBrush lowRtBrush(Qt::magenta, Qt::SolidPattern);
    QBrush lowLtBrush(Qt::magenta, Qt::SolidPattern);

    // Setting ID circles' diameters to fractions of the robot's diameter
    int cornerDiam = boundingRect().width() / 3.8 ;
    int centerDiam = boundingRect().width() / 3 ;

    QRectF topRtCircle(0,0,cornerDiam,cornerDiam);
    QRectF topLtCircle(0,0,cornerDiam,cornerDiam);
    QRectF lowRtCircle(0,0,cornerDiam,cornerDiam);
    QRectF lowLtCircle(0,0,cornerDiam,cornerDiam);
    QRectF centerCircle(0,0,centerDiam, centerDiam);
    // Circle centerpoints based on the robot's front facing Right
    QPointF topRtpt(135,45);
    QPointF topLtpt(135,155);
    QPointF lowRtpt(45,55);
    QPointF lowLtpt(45,145);
    QPointF centerpt(95,100);
    // Setting circles to their coordinates
    topRtCircle.moveCenter(topRtpt);
    topLtCircle.moveCenter(topLtpt);
    lowRtCircle.moveCenter(lowRtpt);
    lowLtCircle.moveCenter(lowLtpt);
    centerCircle.moveCenter(centerpt);

    if(Pressed) {
        //brush.setColor(Qt::red);
    } else {
        //brush.setColor(Qt::magenta);
    }
    // Setting ID circles' colors
    if (id == 0) {
        lowLtBrush.setColor(Qt::green);
    } else if (id == 1) {
        lowLtBrush.setColor(Qt::green);
        topLtBrush.setColor(Qt::green);
    } else if (id == 2) {
        lowLtBrush.setColor(Qt::green);
        topLtBrush.setColor(Qt::green);
        topRtBrush.setColor(Qt::green);
    } else if (id == 3) {
        topRtBrush.setColor(Qt::green);
        lowLtBrush.setColor(Qt::green);
    } else if (id == 4) {
        lowRtBrush.setColor(Qt::green);
    } else if (id == 5) {
        topLtBrush.setColor(Qt::green);
        lowRtBrush.setColor(Qt::green);
    } else if (id == 6) {
        lowLtBrush.setColor(Qt::green);
        topLtBrush.setColor(Qt::green);
        topRtBrush.setColor(Qt::green);
        lowRtBrush.setColor(Qt::green);
    }
    painter->setRenderHint(QPainter::Antialiasing, true);

    // Robot body
    if (kicking) {
        painter->setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    } else if (dribling) {
        painter->setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter->setBrush(QBrush(QColor::fromRgb(255,153,0,255), Qt::SolidPattern));
    } else {
        painter->setPen(QPen(Qt::gray, 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter->setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    }
    if (enabled) {
        painter->drawRoundedRect(base,15,15);
    }

    // Robot hat
    painter->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
    if (overridden){
        painter->setBrush(QBrush(QColor::fromRgb(240,128,128,255), Qt::SolidPattern));
    }
    if (highlighted) {
        painter->setBrush(QBrush(QColor::fromRgb(0,225,225,255), Qt::SolidPattern));
    }
    if (overridden && highlighted) {
        painter->setBrush(QBrush(QColor::fromRgb(132,112,255,255), Qt::SolidPattern));
    }
    int startAngle = 50 * 16;
    int spanAngle = 260 * 16;
    painter->drawChord(rec, startAngle, spanAngle);
    // Corner ID circles
    painter->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter->setBrush(topRtBrush);
        painter->drawEllipse(topRtCircle);
    painter->setBrush(topLtBrush);
        painter->drawEllipse(topLtCircle);
    painter->setBrush(lowRtBrush);
        painter->drawEllipse(lowRtCircle);
    painter->setBrush(lowLtBrush);
        painter->drawEllipse(lowLtCircle);
    // Center circle
    if (myTeam) {
        painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    } else {
        painter->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    }
    painter->drawEllipse(centerCircle);


}


void GuiRobot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GuiRobot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void GuiRobot::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    doubleClicked = true;
    update();
    QGraphicsItem::mouseDoubleClickEvent(event);
}
