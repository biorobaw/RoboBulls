#ifndef GUIFIELD_H
#define GUIFIELD_H
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QDebug>

class GuiField : public QGraphicsItem
{
public:
    GuiField();
    QRectF boundingRect() const;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool plain = true;
    bool grid = false;
    bool coloredGoals = false;
    QString colorScheme = "Default";

};

#endif // GUIFIELD_H
