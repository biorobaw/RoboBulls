#ifndef GUISCENE_H
#define GUISCENE_H

#include <QGraphicsScene>

class GuiScene : public QGraphicsScene
{
public:
    GuiScene(QObject *parent = 0);

    QPointF mousePoint;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

#endif // GUISCENE_H
