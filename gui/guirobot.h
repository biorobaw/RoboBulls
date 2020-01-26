#ifndef GUIROBOT_H
#define GUIROBOT_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QPaintEvent>

class MainWindow;
//A GuiRobot is a robot on the field (FieldPanel)

class GuiRobot : public QGraphicsItem   // inheriting QGraphicsItem
{
public:
    GuiRobot(MainWindow* dash, int team, int id);

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int id;
    int team;
    bool Pressed = false;
    bool highlighted = false;
    bool kicking = false;
    bool dribling = false;
    bool icon = false;
    bool doubleClicked = false;
    bool enabled = true;
    bool overridden = false;
//    std::string myTeam = "Blue";

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    MainWindow* dash;


};

#endif // GUIROBOT_H
