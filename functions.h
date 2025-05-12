#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QVector>
#include <planets.h>


class functions
{
public:
    functions(){};
    virtual QVector<double> func(QVector<double> state,double pl_mass){
        return state;
    };

};

class gravitation_func:public functions{
public:
    virtual QVector<double> func(QVector<double> state,double pl_mass);
};
#endif // FUNCTIONS_H
