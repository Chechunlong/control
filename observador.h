#ifndef OBSERVADOR_H
#define OBSERVADOR_H

#include <vector>

#include <assert.h>
#include <stdlib.h>

#include <QDebug>
#include <iostream>
class Observador {

private:
    /*double matG[2][2] = {
        {0.993458,0.0},
        {0.006520,0.993458}};
    double matG2[2][2] = {
    };
    double matI[2][2] = {
        {1, 0},
        {0, 1} };
    double matH[2][1] = {
        {0.021196},
        {0.000069}};
    double matC[1][2] = {
        {0, 1}
    };

    double polo1[2] = {0};
    double polo2[2] = {0};
    double matqLG[2][2] = {0};
    double matL[2][2] = {0};

    */


        //double[][] calculQLG(double polo1[2], double polo2[2]){
        //    22 22
        //}
        // qLG = matG^2 + matG*(polo1+polo2) + polo1*polo2*I
        // L = qLG * Wo^-1*[0  1]^T
    double b;
    double c;

    double nivelObs;

    double **matGlA; /* 2x2 */
    double **matGlB; /* 2x2 */
    double **matGlC; /* 2x2 */

    double **matG; /* 2x2 */
    double **matI; /* 2x2 */
    double **matH; /* 2x1 */
    double **matWoInv; /* 2x2 */
    double **matQL; /* 2x2 */
    double **matL; /* 2x1 */

    double **matC; /*1x2*/


    double **matColSL;

    double **matXObs; /* 2x1 x observador*/
    double yObs; /* Y observador */

    double** Mat_Aloc(int linhas, int colunas);
    double** Mat_Free(int linhas, int colunas, double** mat);
    double** Mat_Mult(double **matrizA, int linhasMatrizA, int colunasMatrizA, \
                      double **matrizB, int linhasMatrizB, int colunasMatrizB);
    double** Mat_MultEscalar(double **mat, int linhas, int colunas, int escalar);
    double** Mat_Sum(double **matA, double **matB, int linhas, int colunas);
    double** Mat_Sum(double **matA, double **matB, double **matC, int linhas, int colunas);

    void geraMatQL(double b, double c);
    void geraMatL(double b, double c);
public:
    Observador();
    ~Observador();

    double getNivelObs();

    double calculaObservador(double tensao, double y, double polo1[], double polo2[]); /* y é o nivel do tanque */
};

#endif // OBSERVADOR_H
