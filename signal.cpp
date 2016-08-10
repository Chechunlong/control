#include "signal.h"
#include <cstdlib>

Signal::Signal()
{
    srand(time(NULL));
}

double Signal::degrau(double tensao, double offset)
{
    return tensao+offset;
}

double Signal::seno(double tensao, double time, double periodo, double offset)
{
    double frequencia = (2*PI) / periodo;
    double sinal =  sin(frequencia*time)*tensao + offset;
    return sinal;
}

double Signal::aleatorio()
{
    double signal = ((double)rand() / ((double)RAND_MAX + 1) * 8) - 4.0;

    return signal;
}

double Signal::quadrada(double tensao, double time, double periodo, double offset)
{
    double frequencia = (2*PI)/periodo;
    double sinal =  sin(frequencia*time)*tensao;

    if(sinal>=0) sinal = tensao + offset;
    else sinal = -tensao + offset;

    return sinal;
}

double Signal::serra(double tensao, double time, double periodo, double offset)
{
    return (tensao * 2 * time)/periodo - tensao + offset;
}

double Signal::periodoAleatorio()
{
    return ((double)rand() / ((double)RAND_MAX + 1) * 10);
}


