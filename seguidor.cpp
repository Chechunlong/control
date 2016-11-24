#include "seguidor.h"

Seguidor::Seguidor()
{

    ganhoK1 = 0;
    ganhoK21 = 0;
    ganhoK22 = 0;
    erro = 0;
    tensao = 0;
    v = 0;

    polosOld[5] = {0};
    coefB = 0;
    coefC = 0;
    coefD = 0;

    matGG = Mat_Aloc(3,3);
    matHH = Mat_Aloc(3,1);
    matWI = Mat_Aloc(3,3);
    matArckemann = Mat_Aloc(1,3);
    matLinhaArck = Mat_Aloc(1,3);
    matQg = Mat_Aloc(3,3);
    matIm = Mat_Aloc(3,3);
    matGanhos = Mat_Aloc(1,3);
    matAuxGanhos = Mat_Aloc(3,3);

    matGG[0][0] = 0.993458;
    matGG[0][1] = 0.0;
    matGG[0][2] = 0.021196;
    matGG[1][0] = 0.006520;
    matGG[1][1] = 0.993458;
    matGG[1][2] = 0.000069;
    matGG[2][0] = 0.0;
    matGG[2][1] = 0.0;
    matGG[2][2] = 0.0;

    matHH[0][0] = 0.0;
    matHH[1][0] = 0.0;
    matHH[2][0] = 1.0;

    matWI[0][0] = 0.0;
    matWI[0][1] = 0.0;
    matWI[0][2] = 1.0;
    matWI[1][0] = 70.536785;
    matWI[1][1] = -7175.328891;
    matWI[1][2] = 0.0;
    matWI[2][0] = -23.512262;
    matWI[2][1] = 7222.694171;
    matWI[2][2] = 0.0;

    matArckemann[0][0] = 0.0;
    matArckemann[0][1] = 0.0;
    matArckemann[0][2] = 0.0;

    matLinhaArck[0][0] = 0.0;
    matLinhaArck[0][1] = 0.0;
    matLinhaArck[0][2] = 1.0;

    matIm[0][0] = 1.0;
    matIm[0][1] = 0.0;
    matIm[0][2] = 0.0;
    matIm[1][0] = 0.0;
    matIm[1][1] = 1.0;
    matIm[1][2] = 0.0;
    matIm[2][0] = 0.0;
    matIm[2][1] = 0.0;
    matIm[2][2] = 1.0;

    matGanhos[0][0] = 0.0;
    matGanhos[0][1] = 0.0;
    matGanhos[0][2] = 0.0;

    matAuxGanhos[0][0] = -0.4976584;
    matAuxGanhos[0][1] = 151.87479;
    matAuxGanhos[0][2] = 1.0001076;
    matAuxGanhos[1][0] = -2.220E-16;
    matAuxGanhos[1][1] = -1;
    matAuxGanhos[1][2] = 1;
    matAuxGanhos[2][0] = 47.025114;
    matAuxGanhos[2][1] = 46.875111;
    matAuxGanhos[2][2] = 0.3086763;
}

Seguidor::~Seguidor()
{
    matGG = Mat_Free(3,3,matGG);
    matHH = Mat_Free(3,1,matHH);
    matWI = Mat_Free(3,3,matWI);
    matArckemann = Mat_Free(1,3,matArckemann);
    matLinhaArck = Mat_Free(1,3,matLinhaArck);
    matQg = Mat_Free(3,3,matQg);
    matIm = Mat_Free(3,3,matIm);
    matGanhos = Mat_Free(1,3,matGanhos);
    matAuxGanhos = Mat_Free(3,3,matAuxGanhos);
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
        return true;
    }

    return false;
}

void Seguidor::ganhosArckemann()
{
    Mat_Qg(matQg, matGG, matIm, coefB, coefC, coefD, 3, 3);
    Mat_Mult(matArckemann, matLinhaArck, 1, 3, matWI,3,3);
    Mat_Mult(matArckemann, matArckemann, 1, 3, matQg,3,3);
    qDebug() << matArckemann[0][0] << matArckemann[0][1] << matArckemann[0][2];
}

double Seguidor::seguidor(double nivelT1, double nivelT2, double setPoint, double polos[5])
{
    if(verificaPolos(polos))
    {
        ganhosArckemann();
        Mat_Sum(matGanhos, matArckemann, matLinhaArck, 1, 3);
        Mat_Mult(matGanhos, matGanhos, 1, 3, matAuxGanhos, 3, 3);

        ganhoK21 = matGanhos[0][1];
        ganhoK22 = matGanhos[0][0];
        ganhoK1 = matGanhos[0][2];
    }

    erro = setPoint - nivelT2;
    v = v+erro;
    tensao = v*ganhoK1-ganhoK21*nivelT1-ganhoK22*nivelT2;

    return tensao;

    return 6;
}
