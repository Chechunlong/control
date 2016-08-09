#include "signal.h"
#include <cstdlib>

Signal::Signal()
{
    srand(time(NULL));
}

double Signal::degrau(double tensao){
    return tensao;
}

double Signal::seno(double tensao,double time, double periodo, double offset){
    double freq = 2*Signal::PI / periodo;
    return tensao * sin(freq*time) + offset;
}

double Signal::aleatorio(double tensao, double periodo){
    return rand() % (int)tensao + 1;
}

double Signal::quadrada(double tensao, double offset){

    return degrau(tensao) + offset;
}


