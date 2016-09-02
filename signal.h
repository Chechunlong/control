#ifndef SIGNAL_H
#define SIGNAL_H

#include <cmath>
#include <time.h>

#define PI 3.14159265359

class Signal
{
private:

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
