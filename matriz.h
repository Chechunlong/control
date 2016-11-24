#ifndef MATRIZ_H
#define MATRIZ_H

#include <assert.h>
#include <stdlib.h>

#include <QDebug>


class Matriz
{
public:
    double** Mat_Aloc(int linhas, int colunas)
    {
        double **mat = NULL;

        mat = (double**)calloc(linhas, sizeof(double));

        assert(mat != NULL);

        for(int i=0; i<linhas; i++)
        {
            mat[i] = (double*)calloc(colunas, sizeof(double));
            assert(mat[i] != NULL);
        }

        return mat;
    }

    double** Mat_Free(int linhas, int colunas, double** mat)
    {
        if(mat == NULL) return NULL;

        for(int i=0; i<linhas; i++)
        {
            free(mat[i]);
        }
        free(mat);

        return NULL;
    }

    void Mat_Sum(double **matSum, double **matB, double** matC, int linhas, int colunas)
    {
        for(int i=0; i<linhas; i++)
        {
            for(int j=0; j<colunas; j++)
            {
                matSum[i][j] = matB[i][j] + matC[i][j];
            }
        }
    }

    void Mat_Qg(double **matQg, double **matG, double **matG2, double **matG3, double **matI, double escalarB, double escalarC, double escalarD, \
                int linhas, int colunas)
    {
#ifdef DEB_QG_
        qDebug() << "matQg +++++";
        qDebug() << escalarB << escalarC << escalarD;
        qDebug() << "--------";
#endif
        for(int i=0; i<linhas; i++)
        {
            for(int j=0; j<colunas; j++)
            {
                matQg[i][j] = matG3[i][j] + escalarB*matG2[i][j] + escalarC*matG[i][j] + escalarD*matI[i][j];

#define DEB_QG_
#ifdef DEB_QG_
                if(i==0 && j == 0) {
            qDebug() << matQg[i][0] << matQg[i][1] << matQg[i][2];

            qDebug() << "############" << matG3[0][0] << escalarB*matG2[0][0] << escalarC*matG[0][0] << escalarD*matI[0][0];
            //qDebug() << "############" << matG3[0][0] << 1*matG2[0][0] << 1*matG[0][0] << 1*matI[0][0];
            qDebug() << "@@@@@@" << matQg[0][0];
            qDebug() <<matG3[i][j] + escalarB*matG2[i][j] + escalarC*matG[i][j] + escalarD*matI[i][j];
                }
#endif
            }

        }
    }

    void Mat_Mult(double **matResul, double **matrizA, int linhasMatrizA, int colunasMatrizA, \
        double **matrizB, int linhasMatrizB, int colunasMatrizB)
    {

        assert(colunasMatrizA == linhasMatrizB);

        double aux;

        for(int i=0; i<linhasMatrizA; i++)
        {
            for(int j=0; j<colunasMatrizB; j++)
            {
                matResul[i][j]=0.0;
                aux=0;
                for(int k=0; k<linhasMatrizB; k++)
                {
                    aux += matrizA[i][k] * matrizB[k][j];
                }
                matResul[i][j]=aux;
            }
        }
    }
};

#endif // MATRIZ_H
