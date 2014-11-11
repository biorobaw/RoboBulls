#include "guifield.h"
#include <iostream>
#include <QApplication>
#include "mainwindow.h"


GuiField::GuiField()
{
    Pressed = false;
//    setFlag(ItemIsMovable); // makes it movable
//    setFlag(ItemIsSelectable);
}

QRectF GuiField::boundingRect() const
{
    return QRectF(0,0,6000,4000);
}

void GuiField::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rec = boundingRect();

//    QRectF sideLines(0,0,8200,6200);
//    painter->setPen(QPen(Qt::white, 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
//    QBrush clearBrush(Qt::transparent, Qt::SolidPattern);
//    painter->fillRect(sideLines, clearBrush);
//    painter->drawRect(sideLines);

    QPen borderPen(QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    // Lines
    QLineF centerLine(3000,0, 3000,4000);
    // Center circle
    int centerDiam = rec.height() / 4;
    int centerCornerX = (rec.width()/2) - (centerDiam/2);
    int centerCornerY = (rec.height()/2) - (centerDiam/2);
    QRectF centerCircle(centerCornerX,centerCornerY, centerDiam,centerDiam);
    // Penalty areas
    int boxWidth = centerDiam/2;
    int boxHeight = centerDiam*1.3333;
    int BoxY = (rec.height()/2) - (boxHeight/2);
    int yellBoxX = rec.width()-boxWidth;
    QRectF blueBox(0,BoxY, boxWidth,boxHeight);
    QRectF yellBox(yellBoxX,BoxY,boxWidth,boxHeight);
    // Goals
    int goalThick = 40;
    int goalWidth = boxWidth/2.5;
    int goalHeight = boxHeight/1.8;
    int goalY = (rec.height()/2)-(goalHeight/2);
    int blueGoalX = goalWidth*-1;
    QPen goalPen(QPen(Qt::darkGray, goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    //int yellGoalX = rec.width();
    QRectF ourGoal(blueGoalX,goalY, goalWidth,goalHeight);
    QRectF oppGoal(rec.width(),goalY, goalWidth,goalHeight);
    // Drawing field
    QBrush brush(Qt::white, Qt::SolidPattern);
    int fieldLineThick = 40;
//    if (plain) {
//        painter->fillRect(rec,plainBrush);
//    } else {
//        painter->fillRect(rec,crossBrush);
//    }
    if (colorScheme == "Default") {
        brush = (QBrush(Qt::darkGreen, Qt::SolidPattern));
        painter->setPen(QPen(QColor::fromRgb(245,245,245,255), fieldLineThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        borderPen = (QPen(Qt::white, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        goalPen = (QPen(Qt::lightGray, goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter->fillRect(rec, brush);
    } else if (colorScheme == "Ice Rink") {
            brush = (QBrush(QColor::fromRgb(200,255,255,255), Qt::SolidPattern));
            painter->setPen(QPen(Qt::red, fieldLineThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            borderPen = (QPen(Qt::red, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            goalPen = (QPen(Qt::darkRed, goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            painter->fillRect(rec, brush);

    } else if (colorScheme == "Air Hockey") {
        brush = (QBrush(QColor::fromRgb(50,50,200,255), Qt::SolidPattern));
        painter->setPen(QPen(Qt::white, fieldLineThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        borderPen = (QPen(Qt::white, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        goalPen = (QPen(QColor::fromRgb(56,56,56,255), goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter->fillRect(rec, brush);

    } else if (colorScheme == "Basketball") {
        brush = (QBrush(QColor::fromRgb(205,133,63,255), Qt::SolidPattern));
        painter->setPen(QPen(Qt::white, fieldLineThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        borderPen = (QPen(Qt::white, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        goalPen = (QPen(QColor::fromRgb(152,0,0,255), goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter->fillRect(rec, brush);

    } else if (colorScheme == "Arctic") {
        brush = (QBrush(Qt::white, Qt::SolidPattern));
        painter->setPen(QPen(Qt::darkGray, fieldLineThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        borderPen = (QPen(Qt::darkGray, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        goalPen = (QPen(QColor::fromRgb(32,32,32,255), goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter->fillRect(rec, brush);

    } else if (colorScheme == "Game Boy") {
        brush = (QBrush(QColor::fromRgb(152,251,152,255), Qt::SolidPattern));
        painter->setPen(QPen(QColor::fromRgb(85,107,47,255), fieldLineThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        borderPen = (QPen(QColor::fromRgb(85,107,47,255), 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        goalPen = (QPen(QColor::fromRgb(37,57,0,255), goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        painter->fillRect(rec, brush);

    }
    // Drawing field lines
    painter->drawLine(centerLine);
    painter->drawEllipse(centerCircle);
    painter->drawRect(blueBox);
    painter->drawRect(yellBox);
    // Drawing Grid
    if (grid) {
        painter->setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        int longDenom = (rec.width()/2) / gridScale;
        int latiDenom = (rec.height()/2) / gridScale;
        for (int i=1; i<longDenom; i++) {
            int gridX = i*rec.width()/longDenom;
            QLineF longitude(gridX,0, gridX,4000);
            painter->drawLine(longitude);
//            std::cout << "gridX: " << gridX << "\n";
        }
        for (int j=1; j<latiDenom; j++) {
            int gridY = j*rec.height()/latiDenom;
            QLineF latitude(0,gridY, 6000,gridY);
            painter->drawLine(latitude);
//            std::cout << "gridY: " << gridY << "\n";
        }
    }
    // Drawing goals
    QBrush goalBrush(Qt::white, Qt::DiagCrossPattern);
    painter->fillRect(ourGoal,goalBrush);
    painter->fillRect(oppGoal,goalBrush);
    if (coloredGoals) {
        if (myTeam == "Blue") {
            painter->setPen(QPen(QColor::fromRgb(0,0,255,255), goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            painter->drawRect(ourGoal);
            painter->setPen(QPen(Qt::yellow, goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            painter->drawRect(oppGoal);
        } else {
            painter->setPen(QPen(Qt::yellow, goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            painter->drawRect(ourGoal);
            painter->setPen(QPen(QColor::fromRgb(0,0,255,255), goalThick, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            painter->drawRect(oppGoal);
        }
    } else {
        painter->setPen(goalPen);
        painter->drawRect(ourGoal);
        painter->drawRect(oppGoal);
    }

    painter->setPen(borderPen);
    painter->drawRect(rec);
}

void GuiField::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == false) {
        Pressed = true;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GuiField::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == false) {
        Pressed = false;
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
