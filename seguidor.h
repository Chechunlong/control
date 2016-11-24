#ifndef SEGUIDOR_H
#define SEGUIDOR_H

#include "matriz.h"
#include <QDebug>

class Seguidor : public Matriz
{
    double polosOld[5];
    double coefB, coefC, coefD;

    double ganhoK1;
    double ganhoK21;
    double ganhoK22;
    double erro;
    double tensao;
    double v;

    double **matGG; /* 3x3 */
    double **matHH; /* 3x1 */
    double **matWI; /* 3x3 */
    double **matArckemann; /* 1x3*/
    double **matLinhaArck; /* 1x3*/
    double **matQg; /* 3x3 */
    double **matIm; /* 3x3 */
    double **matGanhos; /* 1x3*/
    double **matAuxGanhos; /* 3x3 */

    bool verificaPolos(double polos[5]);
    void ganhosArckemann();
public:
    Seguidor();
    ~Seguidor();

    double seguidor(double nivelT1, double nivelT2, double setPoint, double polos[5]);
};

#endif // SEGUIDOR_H
