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
    GuiBotLabel();

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int id;
    bool hidden = true;
    bool mainTeam = true;
    std::string myTeam = "Blue";


};

#endif // GUIBOTLABEL_H
