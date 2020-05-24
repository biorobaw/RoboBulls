#ifndef _GRAPHICS_BALL_
#define GUIBALLDRAWER_H

#include <QGraphicsItem>

// Graphics element to draw the ball

class GraphicsBall : public QGraphicsItem
{
public:

    // Constructor, draw_at_origin indicates whether the graphics
    // should be drawn in the ball coordinates or in the origin
    GraphicsBall(bool draw_at_origin);
    static void setColor(QString color); // sets the color

    // required QGraphicsItem functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    bool draw_at_origin = false; // variable indicating whether to draw the ball at the origin or at the ball's coordinates
    static QBrush brush;         // brush for filling the ball
    static QPen   pen;           // brush for the borders of the ball

};

#endif // _GRAPHICS_BALL_
