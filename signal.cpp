#include "signal.h"
#include <cstdlib>

Signal::Signal()
{
    srand(time(NULL));
}

double Signal::degrau(double amplitude){
    return amplitude;
}

double Signal::seno(double amplitude,double time, double periodo, double offset){
    double freq = 2*Signal::PI / periodo;
    return amplitude * sin(freq*time) + offset;
}

double Signal::aleatorio(double amplitude, double periodo){
    return rand() % (int)amplitude + 1;
    //double perRand = rand() % periodo + 1;
    //return 0;
}

double Signal::quadrada(double amplitude, double offset){
    return degrau(amplitude) + offset;
}


