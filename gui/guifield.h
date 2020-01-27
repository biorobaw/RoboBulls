#ifndef GUIFIELD_H
#define GUIFIELD_H
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QDebug>


//GuiField is the background of the field itself, behind the FieldPanel

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
    int gridScale = 100;
    bool Pressed = false;
    bool highlighted = false;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released

};

#endif // GUIFIELD_H
