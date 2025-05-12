#include "planets.h"

planets::planets()
{
    _a = 0;
    _i = 0;
    _e = 0;
    _Omega = 0;
    _omega = 0;
    _tau = 0;
}
double planets::get_a(){
    return _a;
}
double planets::get_e(){
    return _e;
}
double planets::get_Omega(){
    return _Omega;
}
double planets::get_omega(){
    return _omega;
}

double planets::get_i(){
    return _i;
}

double planets::get_tau(){
    return _tau;
}
double planets::get_T(){
    return _T;
}
double planets::get_ang_vel(){
    return _ang_vel;
}
double planets::get_T_sin(){
    return _T_sin;
}
double planets::get_M(){
    return _M;
}
