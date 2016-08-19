#ifndef SIGNAL_H
#define SIGNAL_H

#include <cmath>
#include <time.h>

class Signal
{
private:
    const double PI = 3.14159265359;

public:
    Signal();
    double degrau(double tensao, double offset);
    double seno(double tensao, double time, double periodo, double offset);
    double aleatorio(double ampMax, double ampMin);
    double quadrada(double tensao, double time, double periodo, double offset);
    double serra(double tensao, double time, double periodo, double offset);
    double periodoAleatorio(double durMax, double durMin);
};

#endif // SIGNAL_H
