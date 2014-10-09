#ifndef GUIBOTLABEL_H
#define GUIBOTLABEL_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>

class GuiBotLabel : public QGraphicsItem
{
public:
    GuiBotLabel();

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int id;
    bool hidden = true;
    bool myTeam = true;


};

#endif // GUIBOTLABEL_H
