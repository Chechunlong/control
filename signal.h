#ifndef SIGNAL_H
#define SIGNAL_H

#include <cmath>
#include <time.h>


class Signal
{
    static const double PI = 3.14;

public:
    Signal();
    double degrau(double amplitude);
    double seno(double amplitude,double time,double periodo, double offset);
    double aleatorio(double amplitude, double periodo);
    double quadrada(double amplitude,double offset);


};

#endif // SIGNAL_H
