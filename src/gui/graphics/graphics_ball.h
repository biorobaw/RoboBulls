#ifndef GUIBALLDRAWER_H
#define GUIBALLDRAWER_H

#include <QGraphicsItem>
class GuiBall;

class GraphicsBall : public QGraphicsItem
{
public:

    static QBrush brush;
    static QPen   pen;
    static void setColor(QString color);



    GraphicsBall(bool is_icon);
    GuiBall* ball;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    bool is_icon = false;

};

#endif
