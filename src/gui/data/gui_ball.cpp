#include "gui_ball.h"
#include "model/ball.h"


GuiBall GuiBall::ball;

void  GuiBall::updateBall(){
    ball.position = Ball::getPosition();
    ball.velocity = Ball::getVelocity();
    ball.speed = Ball::getSpeed();
}

Point GuiBall::getPosition(){
    return ball.position;
}

Point GuiBall::getVelocity(){
    return ball.velocity;
}

float GuiBall::getSpeed(){
    return ball.speed;
}

GuiBall::GuiBall()
{

}



