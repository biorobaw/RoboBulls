#include <QCoreApplication>

#include "xbeereceiver.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    XbeeReceiver xr;

    xr.receive();

    return a.exec();
}
