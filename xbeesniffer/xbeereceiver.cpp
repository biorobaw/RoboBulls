#include "xbeereceiver.h"
#include <stdio.h>
#include <assert.h>


using namespace std;

XbeeReceiver::XbeeReceiver()
{
    int ret = xbee.Open("/dev/xbee",57600);

    if (ret != 1) { // If an error occured...
        printf ("Error while opening port. Permission problem ?\n"); // ... display a message ...
        exit(1);
        //return Ret;// ... quit the application
    }
    else{
        printf ("Serial port opened successfully !\n");
    }
}

void XbeeReceiver::receive()
{
    char c;
    while (true) {
        int received = xbee.ReadChar(&c, 0);
        if (received > 0) {
            if (c == '~' || c == char(250)) {
                cout << endl;
                cout << '~' << " ";
            } else if (c == '$' || c == char(255)) {
                cout << '$' << " ";
            } else {
                cout << (int)(c) << " ";
            }
        }
    }
}

