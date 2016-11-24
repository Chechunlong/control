#include "seguidor.h"

Seguidor::Seguidor()
{

    ganhoK1 = 0;
    ganhoK21 = 0;
    ganhoK22 = 0;

    tensao = 0;
    v = 0;

    polosOld[5] = {0};
    coefB = 0;
    coefC = 0;
    coefD = 0;


    matGG(0,0) = 0.993458;
    matGG(0,1) = 0.0;
    matGG(0,2) = 0.021196;
    matGG(1,0) = 0.006520;
    matGG(1,1) = 0.993458;
    matGG(1,2) = 0.000069;
    matGG(2,0) = 0.0;
    matGG(2,1) = 0.0;
    matGG(2,2) = 0.0;

    matGG2 = matGG * matGG;
    matGG3 = matGG * matGG * matGG;

    matHH(0,0) = 0.0;
    matHH(1,0) = 0.0;
    matHH(2,0) = 1.0;

    matWI(0,0) = 0.0;
    matWI(0,1) = 0.0;
    matWI(0,2) = 1.0;
    matWI(1,0) = 70.536785;
    matWI(1,1) = -7175.328891;
    matWI(1,2) = 0.0;
    matWI(2,0) = -23.512262;
    matWI(2,1) = 7222.694171;
    matWI(2,2) = 0.0;

    matArckemann(0,0) = 0.0;
    matArckemann(0,1) = 0.0;
    matArckemann(0,2) = 0.0;

    matLinhaArck(0,0) = 0.0;
    matLinhaArck(0,1) = 0.0;
    matLinhaArck(0,2) = 1.0;

    matIm(0,0) = 1.0;
    matIm(0,1) = 0.0;
    matIm(0,2) = 0.0;
    matIm(1,0) = 0.0;
    matIm(1,1) = 1.0;
    matIm(1,2) = 0.0;
    matIm(2,0) = 0.0;
    matIm(2,1) = 0.0;
    matIm(2,2) = 1.0;

    matGanhos(0,0) = 0.0;
    matGanhos(0,1) = 0.0;
    matGanhos(0,2) = 0.0;

    matAuxGanhos(0,0) = -0.497658;
    matAuxGanhos(0,1) = 151.87479;
    matAuxGanhos(0,2) = 1.0001076;
    matAuxGanhos(1,0) = -2.220E-16;
    matAuxGanhos(1,1) = -1;
    matAuxGanhos(1,2) = 1;
    matAuxGanhos(2,0) = 47.025114;
    matAuxGanhos(2,1) = 46.875111;
    matAuxGanhos(2,2) = 0.308676;
}

Seguidor::~Seguidor()
{

}

bool Seguidor::verificaPolos(double polos[5])
{
    if(polos[0] != polosOld[0] || polos[1] != polosOld[1] || polos[2] != polosOld[2] || \
            polos[3] != polosOld[3] || polos[4] != polosOld[4] )
    {
        polosOld[0] = polos[0];
        polosOld[1] = polos[1];
        polosOld[2] = polos[2];
        polosOld[3] = polos[3];
        polosOld[4] = polos[4];

        polos[0] *=-1;
        polos[1] *=-1;
        polos[2] *=-1;
        polos[3] *=-1;
        polos[4] *=-1;

        coefB = polos[0] + polos[2];
        coefC = polos[0]*polos[2] + polos[1]*polos[3]*(-1);

        coefD = coefC*polos[4];

        coefC = coefC + coefB*polos[4];
        coefB = coefB + polos[4];
//#define DEBUG_COEF_
#ifdef DEBUG_COEF_
        qDebug() << "FUNCTION calcula coeficientes";
        qDebug() << "polos";
        qDebug() << "p1 = " << polos[0] << polos[1];
        qDebug() << "ṕ2 = " << polos[2] << polos[3];
        qDebug() << "ṕ3 = " << polos[4];
        qDebug() << "coeficientes b c d";
        qDebug() << coefB << coefC << coefD;
#endif
        qDebug() << "coeficientes b c d";
        qDebug() << coefB << coefC << coefD;
        return true;
    }

    return false;
}

void Seguidor::ganhosArckemann()
{
    matQg = matGG3 + matGG2*coefB + matGG*coefC + matIm * coefD;

#define DEB_QG
#ifdef DEB_QG
    qDebug() << "@matriz Qg";
    for(int i=0; i<3; i++)
    {
       qDebug() << matQg(i,0) << ' ' << matQg(i,1) << ' ' << matQg(i,2) << ' ';

    }
#endif

    matArckemann = matLinhaArck * matWI * matQg;


    qDebug() << "@matriz Arckemann2" << matArckemann(0,0) << matArckemann(0,1) << matArckemann(0,2);
}

double Seguidor::seguidor(double nivelT1, double nivelT2, double erro, mat ganhos)
{
    /*if(verificaPolos(polos))
    {
        ganhosArckemann();        
        matGanhos = (matArckemann+matLinhaArck)*matAuxGanhos;
        ganhoK21 = matGanhos(0,0);
        ganhoK22 = matGanhos(0,1);
        ganhoK1 = matGanhos(0,2);
    }

    qDebug() << "ganhos " << ganhoK1 << ganhoK21 << ganhoK22;
    */

    qDebug() << "@ganhos " << ganhos(0,0) << ganhos(0,1) << ganhos(0,2);

    ganhoK21 = ganhos(0,0);
    ganhoK22 = ganhos(0,1);
    ganhoK1 = ganhos(0,2);
    v = v+erro;
    tensao = v*ganhoK1-ganhoK21*nivelT1-ganhoK22*nivelT2;

    return tensao;
}


mat Seguidor::getKsFromPolos(double polos[5])
{
    double coefB, coefC, coefD;
    polos[0] *=-1;
    polos[1] *=-1;
    polos[2] *=-1;
    polos[3] *=-1;
    polos[4] *=-1;

    qDebug() << "polos";
    qDebug() << polos[0] <<  polos[1] << polos[2] << polos[3] << polos[4];


    coefB = polos[0] + polos[2];
    coefC = polos[0]*polos[2] + polos[1]*polos[3]*(-1);

    coefD = coefC*polos[4];

    coefC = coefC + coefB*polos[4];
    coefB = coefB + polos[4];

    qDebug() << coefB << coefC << coefD;

    matQgTemp = matGG3 + matGG2*coefB + matGG*coefC + matIm * coefD;
    matArckemannTemp = matLinhaArck * matWI * matQgTemp;
    matGanhosTemp = (matArckemannTemp+matLinhaArck)*matAuxGanhos;

    qDebug() << "___matriz Qg";
    for(int i=0; i<3; i++)
    {
       qDebug() << matQgTemp(i,0) << ' ' << matQgTemp(i,1) << ' ' << matQgTemp(i,2) << ' ';

    }
    qDebug() << "___matriz Arckemann2" << matArckemannTemp(0,0) << matArckemannTemp(0,1) << matArckemannTemp(0,2);

    qDebug() << "## mandando " << matGanhosTemp(0,0) << matGanhosTemp(0,1) << matGanhosTemp(0,2);
    return matGanhosTemp;
}
