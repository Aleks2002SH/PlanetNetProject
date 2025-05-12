#include "solvers.h"


QVector<double> eulersolver3d::solverRightPart(double t,QVector<double> state,double h,double pl_mass){
    int n = state.size();
    QVector<double> values = get_f()->func(state,pl_mass);
    for(int i =0;i<n;i++){
        values[i] = values[i]*h+state[i];
    }
    return values;
}


QVector<double> rksolver3d::solverRightPart(double t,QVector<double> state,double h,double pl_mass){
    int n = state.size();
    QVector<double> values_1 = get_f()->func(state,pl_mass);
    QVector<double> tmp_values(n);
    for(int i =0;i<n;i++){
        tmp_values[i] = values_1[i]*h+state[i];
    }
    tmp_values=get_f()->func(tmp_values,pl_mass);
    QVector<double> values(n);
    for(int i =0;i<n;i++){
        values[i] = state[i] + h/2*(tmp_values[i]+values_1[i]);
    }
    return values;
}


QVector<double> keplersolver3d::solverRightPart(double t,QVector<double> state,double h,double pl_mass){
    planets_data pl_dat;
    planets pl = pl_dat._planets_data.at("");
    double e = pl.get_e();
    double a = pl.get_a();
    double p = a*(1-pow(e,2));
    double M = sqrt(pl.G*pl.M_sun/pow(a*pl.a_e,3))*(t-pl.get_tau());
    double E = kepler_solve(e,M);
    double nu = 2*atan((sqrt(1+e)/sqrt(1-e))*tan(E/2));
    double u = pl.get_omega()+nu;
    double r = p/(1+e*cos(nu));
    double mu = pl.M_sun*pl.G;
    double vr = sqrt(mu/p)*e*sin(nu);
    double vn = sqrt(mu/p)*(1+e*cos(nu));
    QVector<double> new_state = {r*(cos(pl.get_Omega())*cos(u)-sin(pl.get_Omega())*sin(u)*cos(pl.get_i())),
                                     r*(sin(pl.get_Omega())*cos(u)+cos(pl.get_Omega())*sin(u)*cos(pl.get_i())),
                                 r*sin(u)*sin(pl.get_i()),
                                vr*(cos(pl.get_Omega())*cos(u)-sin(pl.get_Omega())*sin(u)*cos(pl.get_i()))-vn*(cos(pl.get_Omega())*sin(u)+sin(pl.get_Omega())*cos(u)*cos(pl.get_i())),
                                vr*(sin(pl.get_Omega())*cos(u)+cos(pl.get_Omega())*sin(u)*cos(pl.get_i()))-vn*(sin(pl.get_Omega())*sin(u)-cos(pl.get_Omega())*cos(u)*cos(pl.get_i())),
                                vr*sin(u)*sin(pl.get_i())+vn*cos(u)*sin(pl.get_i())};
    return new_state;
}

