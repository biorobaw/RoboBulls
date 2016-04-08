#include "communication/kf.h"

KF::KF()
{
    setDim(6,2,2,2,2);
//    Vector x_init = {0,0,0,0,0,0};
//    Matrix p_init = {3,0,0,3};
//    init(x_init, p_init);
}



void KF::makeR()
{
    R(1,1) = 3.0;
    R(1,2) = 0.0;
    R(2,1) = 0.0;
    R(2,2) = 3.0;
}
