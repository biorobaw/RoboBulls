#ifndef GUIBOTLABEL_H
#define GUIBOTLABEL_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>

//GuiBotLabel is the ID and ball indication label on the robot on the field.
//(GuiRobot on the FieldPanel)

class GuiBotLabel : public QGraphicsItem
{
public:
    GuiBotLabel(int team);

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int id;
    int team;
    bool hidden = true;
    bool mainTeam = true;
};

#endif // GUIBOTLABEL_H

