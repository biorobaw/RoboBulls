#ifndef _GRAPHICS_FIELD_
#define _GRAPHICS_FIELD_
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QDebug>




//GuiField is the background of the field itself, behind the FieldPanel

class GraphicsField : public QGraphicsItem
{
public:
    GraphicsField();
    QRectF boundingRect() const;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString colorScheme = "Default";
    bool grid = false;
    int gridScale = 100;


};

#endif // _GRAPHICS_FIELD_
