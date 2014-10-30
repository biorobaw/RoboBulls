#ifndef GUIDRAWLINE_H
#define GUIDRAWLINE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>

class GuiDrawLine : public QGraphicsItem
{
public:
    GuiDrawLine();
    QRectF boundingRect() const;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int x1;
    int y1;
    int x2;
    int y2;
//    std::string color;

};

#endif // GUIDRAWLINE_H
