#ifndef SOLVERS_H
#define SOLVERS_H

#include <iostream>
#include <QVector>
#include "functions.h"
#include "planets.h"

class solvers
{
    functions * _f;
public:
    solvers(){}
    virtual QVector<double> solverRightPart(double t,QVector<double> state,double h,double pl_mass){
        return state;
    }
    virtual QVector<double> solverRightPart(double t,QVector<double> state,double h){
        return state;
    }
    void set_f(functions* f){
        _f = f;
    }
    functions* get_f(){
        return _f;
    }
};

class eulersolver3d:public solvers{
public:
    eulersolver3d(){}
    virtual QVector<double> solverRightPart(double t,QVector<double> state,double h,double pl_mass);
};
class rksolver3d:public solvers{
public:
    rksolver3d(){}
    virtual QVector<double> solverRightPart(double t,QVector<double> state,double h,double pl_mass);
};


class keplersolver3d: public solvers{
public:
    keplersolver3d(){}
    double kepler_solve(double e,double M){
        double E = M;
        for(int i=0;i<50;i++) E = e*E+M;
        return E;
    }
    virtual QVector<double> solverRightPart(double t,QVector<double> state,double h,double pl_mass);
};

#endif // SOLVERS_H
