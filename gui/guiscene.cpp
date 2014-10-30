#include "guiscene.h"
#include <QGraphicsSceneMouseEvent>

GuiScene::GuiScene(QObject *parent) : QGraphicsScene(parent)
{
}

void GuiScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    mousePoint = mouseEvent->scenePos();
}
