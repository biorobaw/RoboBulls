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
<<<<<<< HEAD
    char c;
    char p[10];
    int i = 0;

    while (true){
        int received = xbee.ReadChar(&c, 0);
        if (received > 0){
            if (c == '~'){
                i = 0;
                cout << endl;
                cout << '~' << " ";
            } else if (c == '$') {
=======
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
>>>>>>> 67f6d320b6a48c12f5f11751171467ab5d2d2697
                cout << '$' << " ";
                // Not kicking
                assert (i != 9 || p[6] == 0);
            } else {
<<<<<<< HEAD
                cout << (int)(c) << " ";
            }

            p[i] = c;
=======
                cout << (int)(*c) << " ";
            }

            p[i] = *c;
>>>>>>> 67f6d320b6a48c12f5f11751171467ab5d2d2697
            i++;
        }


    }

}

