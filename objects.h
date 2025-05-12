#ifndef OBJECTS_H
#define OBJECTS_H
#include "functions.h"
#include "solvers.h"

struct coordinates{
    QVector<double> X;
    QVector<double> Y;
    QVector<double> Z;
    void add_values(double x,double y,double z);
};


class objects
{
private:
    QVector<double> _initial_state;
    double _initial_time;
    double _end_time;
    double _pl_mass;
    solvers* _solv;
    double _dt;
public:
    objects(){};
    objects(QVector<double> init_state,double it,double et,double pl_mass,solvers* solv);
    void set_dt(double dt);
    coordinates get_data();
};
#endif // OBJECTS_H
