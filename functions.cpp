#include "functions.h"



QVector<double> gravitation_func::func(QVector<double> state,double pl_mass){
    planets_data pl_dat;
    double G = 6.67430e-11;
    double mu = G * pl_mass;

    int n = state.size();
    QVector<double> values(n);

    if (n % 2 == 0) {
        double r_squared = 0;
        for (int i = 0; i < n / 2; ++i) {
            values[i] = state[n / 2 + i]; // dx/dt = v
            r_squared += state[i] * state[i];
        }

        double r = std::sqrt(r_squared);
        double r_cubed = r * r_squared;

        for (int i = n / 2; i < n; ++i) {
            values[i] = -mu * state[i - n / 2] / r_cubed;
        }
    }

    return values;
}
