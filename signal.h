#ifndef SIGNAL_H
#define SIGNAL_H

#include <cmath>
#include <time.h>


class Signal
{
    const double PI = 3.14159265359;

public:
    Signal();
    double degrau(double );
    double seno(double ,double ,double ,double );
    double aleatorio(double , double );
    double quadrada(double ,double );
};

#endif // SIGNAL_H
