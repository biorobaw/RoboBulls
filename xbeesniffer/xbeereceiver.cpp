#include "xbeereceiver.h"
#include <stdio.h>

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

void XbeeReceiver::receive(){
    char * c = new char[1];

    while (true){
        int received = xbee.ReadChar(c, 0);
        if (received > 0){
            if (*c == '~'){
                cout << endl;
                cout << '~' << " ";
            } else if (*c == '$') {
                cout << '$' << " ";
            } else {
                cout << (int)(*c) << " ";
            }
        }

    }

}

