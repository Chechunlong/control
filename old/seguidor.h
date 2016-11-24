#ifndef SEGUIDOR_H
#define SEGUIDOR_H

#include <assert.h>
#include <stdlib.h>
#include <cmath>



#include <QDebug>

class Seguidor
{
private:
    double ganhoK1 = 0;
    double ganhoK21 = 0;
    double ganhoK22 = 0;

    double tensao = 0;
    double erro = 0;
    double v = 0;

    double polos1Old[2] = {0.0};
    double polos2Old[2] = {0.0};
    double polos3Old = 0;
    /* Coeficientes do polinomio qc(G) */
    double coefB = 0, coefC = 0, coefD = 0;

    double **matGG; /* 3x3 */
    double **matHH; /* 3x1 */
    double **matWI; /* 3x3 */
    double **matArckemann; /* 1x3*/
    double **matLinhaArck; /* 1x3*/
    double **matQg; /* 3x3 */

    double **matIm; /* 3x3 */

    double **matGanhos; /* 1x3*/
    double **matAuxGanhos; /* 3x3 */

    double** Mat_Aloc(int linhas, int colunas);
    double** Mat_Free(int linhas, int colunas, double** mat);
    void Mat_Sum(double **matSum, double **matB, double** matC, int linhas, int colunas);
    void Mat_Qg(double **matSum, double **matA, double escalarA, double **matB, double escalarB, \
                 double** matC, double escalarC, double **matD, double escalarD, int linhas, int colunas);
    void Mat_Mult(double **matResul, double **matrizA, int linhasMatrizA, int colunasMatrizA, \
        double **matrizB, int linhasMatrizB, int colunasMatrizB);

    /* Calcula coeficientes do polinomio qc(G) */
    bool verificaPolos(double polo1[2], double polo2[2], double polo3);
    void calculaCoeficientes(double polo1[2], double polo2[2], double polo3);
    void calculaQg();
    void ganhosArckemann();
public:
    Seguidor();
    ~Seguidor();

    double seguidor(double setPoint, double nivelT1, double nivelT2, double polo1[2], double polo2[2], double polo3);
};

#endif // SEGUIDOR_H
