#ifndef PLANETS_H
#define PLANETS_H


#include <iostream>
#include <map>
#include <QString>
#include <math.h>

class planets
{
private:
    double _a;
    double _e;
    double _i;
    double _Omega;
    double _omega;
    double _tau;
    double _ang_vel;
    double _T;
    double _T_sin;
    double _M;
public:
    const double M_sun = 1.989*pow(10.0,30);
    const double G = 6.674*pow(10.0,-11);
    const double a_e = 1.496*pow(10.0,11);
    const double T_earth = 365.26*3600*24;
    planets();
    planets(double a,double e,double i,double Omega,double omega,double tau,double M){
        _a = a;
        _e = e;
        _i = i;
        _Omega = Omega;
        _omega = omega;
        _tau = tau;
        _T = T_earth*sqrt(pow(_a,3));
        _ang_vel = 2*M_PI/_T;
        _T_sin = _T*T_earth/abs(_T-T_earth);
        _M = M;
    }
    planets(double a,double e,double i,double Omega,double omega,double tau,double M,double T){
        _a = a;
        _e = e;
        _i = i;
        _Omega = Omega;
        _omega = omega;
        _tau = tau;
        _T = T;
        _ang_vel = 2*M_PI/_T;
        _T_sin = _T*T_earth/abs(_T-T_earth);
        _M = M;
    }
    double get_a();
    double get_e();
    double get_i();
    double get_Omega();
    double get_omega();
    double get_tau();
    double get_T();
    double get_ang_vel();
    double get_T_sin();
    double get_M();
};

class planets_data{
public:
    std::map<QString,planets> _planets_data{{"earth", planets(1,0.0167,0,0,102.93*M_PI/180,0,5.973*pow(10,24))}, {"venus", planets(0.72,0.0068,3.39777545*M_PI/180,76.67*M_PI/180,131.77*M_PI/180,0,4.869*pow(10,24))},
                                           {"mercury", planets(0.3871,0.2056,7.00559432*M_PI/180,48.34*M_PI/180,77.457*M_PI/180,0,3.3*pow(10,23))},{"mars",planets(1.5237,0.0934,1.85181869*M_PI/180,49.71*M_PI/180,-23.917*M_PI/180,0,6.417*pow(10,23))},
                                           {"jupiter",planets(5.2028,0.0483,1.29861416*M_PI/180,100.29*M_PI/180,14.274*M_PI/180,0,1.889*pow(10,27))},
                                            {"moon",planets(384400e3,0.0548,5.145*M_PI/180,124.08*M_PI/180,318.15*M_PI/180,0,7.342e22,27.32*86400)}};
    planets_data (){}
};
#endif // PLANETS_H
