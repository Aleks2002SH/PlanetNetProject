#include "objects.h"

void coordinates::add_values(double x,double y,double z){
    X.push_back(x);
    Y.push_back(y);
    Z.push_back(z);
}


objects::objects(QVector<double> init_state,double it,double et,double pl_mass,solvers* solv){
    _initial_state = init_state;
    _initial_time = it;
    _end_time = et;
    _pl_mass = pl_mass;
    _solv = solv;
}
void objects::set_dt(double dt){
    _dt = dt;
}

coordinates objects::get_data(){
    coordinates answer;
    gravitation_func f;
    double h = _dt;/*/(_end_time - _initial_time);*/
    for(double t = _initial_time;t<_end_time;t+=_dt){
        _solv->set_f(&f);
         _initial_state = _solv->solverRightPart(t,_initial_state,h,_pl_mass);
        answer.add_values(_initial_state[0],_initial_state[1],_initial_state[2]);
    }
    return answer;
}
