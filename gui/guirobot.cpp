#include "guirobot.h"
#include <QLabel>

GuiRobot::GuiRobot()
{
    Pressed = false;
//    setFlag(ItemIsMovable); // makes it movable
    int radius = boundingRect().width() / 2;
    setTransformOriginPoint(radius,radius);   // sets center point, around which it rotates
}

QRectF GuiRobot::boundingRect() const
{
    int diameter = 200;
    int radius = diameter/2;
    return QRectF(0,0,diameter,diameter);
}

void GuiRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);


    QRectF rec = boundingRect();

    QBrush brush(Qt::magenta, Qt::SolidPattern);
    // ID circles colors
    QBrush topRtBrush(Qt::magenta, Qt::SolidPattern);
    QBrush topLtBrush(Qt::magenta, Qt::SolidPattern);
    QBrush lowRtBrush(Qt::magenta, Qt::SolidPattern);
    QBrush lowLtBrush(Qt::magenta, Qt::SolidPattern);

    // Setting ID circles' diameters to fractions of the robot's diameter
    int cornerDiam = boundingRect().width() / 3.5 ;
    int centerDiam = boundingRect().width() / 3 ;

    QRectF topRtCircle(0,0,cornerDiam,cornerDiam);
    QRectF topLtCircle(0,0,cornerDiam,cornerDiam);
    QRectF lowRtCircle(0,0,cornerDiam,cornerDiam);
    QRectF lowLtCircle(0,0,cornerDiam,cornerDiam);
    QRectF centerCircle(0,0,centerDiam, centerDiam);
    // Circle centerpoints based on the robot's front facing Right
    QPointF topRtpt(140,45);
    QPointF topLtpt(140,155);
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
    }

    // Robot body
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
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
    // Label
//    painter->setFont(QFont("Arial", 50, QFont::Bold));
//    painter->drawText(10,10,"test");


}

void GuiRobot::subCircle(QPainter *painter, QBrush topLft, QBrush topRt, QBrush lowLft, QBrush lowRt)
{

}


void GuiRobot::paintEvent(QPaintEvent *)
{
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
