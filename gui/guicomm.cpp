#include "guicomm.h"
#include <iostream>

GuiComm::GuiComm(int interval, QObject* parent)
    : QThread(parent)
    , i(0)
    , inc(-1)
    , intvl(interval)
    , timer(0)
{
    ;
}

GuiComm::~GuiComm()
{
    if(timer != 0)
        delete timer;
}

void GuiComm::run(void)
{
    if(timer == 0)
    {
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(count()));
    }
    timer->start(intvl);
    exec();
}

void GuiComm::count(void)
{
    if((i >= 100) or ( i <= 0))
        inc = -inc;

    i += inc;
    emit valueChanged(i);
}
