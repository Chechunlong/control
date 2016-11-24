#ifndef SEGUIDOR_H
#define SEGUIDOR_H

#include "matriz.h"
#include <QDebug>


#include <armadillo>

using namespace arma;

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

    mat::fixed<3,3> matGG; /* 3x3 */
    mat::fixed<3,3> matGG2; /* 3x3 */
    mat::fixed<3,3> matGG3; /* 3x3 */
    mat::fixed<3,1> matHH; /* 3x1 */
    mat::fixed<3,3> matWI; /* 3x3 */
    mat::fixed<1,3> matArckemann; /* 1x3*/
    mat::fixed<1,3> matLinhaArck; /* 1x3*/
    mat::fixed<3,3> matQg; /* 3x3 */
    mat::fixed<3,3> matIm; /* 3x3 */
    mat::fixed<1,3> matGanhos; /* 1x3*/
    mat::fixed<3,3> matAuxGanhos; /* 3x3 */

    bool verificaPolos(double polos[5]);
    void ganhosArckemann();
public:
    Seguidor();
    ~Seguidor();

    double seguidor(double nivelT1, double nivelT2, double setPoint, double polos[5]);
};

#endif // SEGUIDOR_H
