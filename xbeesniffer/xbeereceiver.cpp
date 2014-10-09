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

void XbeeReceiver::receive(){
    char * c = new char[1];
    char * p = new char[10];
    int i = 0;

    while (true){
        int received = xbee.ReadChar(c, 0);
        if (received > 0){
            if (*c == '~'){
                i = 0;
                cout << endl;
                cout << '~' << " ";
            } else if (*c == '$') {
                cout << '$' << " ";
                // Not kicking
                assert (i != 9 || p[6] == 0);
            } else {
                cout << (int)(*c) << " ";
            }

            p[i] = *c;
            i++;
        }


    }

}

