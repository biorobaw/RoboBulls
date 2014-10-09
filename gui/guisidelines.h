#ifndef GUISIDELINES_H
#define GUISIDELINES_H
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QDebug>

class GuiSidelines : public QGraphicsItem
{
public:
    GuiSidelines();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool plain = false;
    QString colorScheme = "Default";

};

#endif // GUISIDELINES_H
