#include "seguidor.h"

double** Seguidor::Mat_Aloc(int linhas, int colunas)
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

double** Seguidor::Mat_Free(int linhas, int colunas, double** mat)
{
    if(mat == NULL) return NULL;

    for(int i=0; i<linhas; i++)
    {
        free(mat[i]);
    }
    free(mat);

    return NULL;
}

void Seguidor::Mat_Sum(double **matSum, double **matB, double** matC, int linhas, int colunas)
{
    for(int i=0; i<linhas; i++)
    {
        for(int j=0; j<colunas; j++)
        {
            matSum[i][j] = matB[i][j] + matC[i][j];
        }
    }
}

void Seguidor::Mat_Qg(double **matSum, double **matA, double escalarA, double **matB, double escalarB, \
             double** matC, double escalarC, double **matD, double escalarD, int linhas, int colunas)
{
    for(int i=0; i<linhas; i++)
    {
        for(int j=0; j<colunas; j++)
        {
            matSum[i][j] = escalarA*matA[i][j] + escalarB*matB[i][j] + escalarC*matC[i][j] + escalarD*matD[i][j];
        }
    }
}

void Seguidor::Mat_Mult(double **matResul, double **matrizA, int linhasMatrizA, int colunasMatrizA, \
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

bool Seguidor::verificaPolos(double polo1[2], double polo2[2], double polo3)
{
    if( (polo1[0] != polos1Old[0]) || (polo1[1] != polos1Old[1]) || \
            (polo2[0] != polos2Old[0]) || (polo2[1] != polos2Old[1]) || \
            polo3 != polos3Old )
    {
        polos1Old[0] = polo1[0];
        polos1Old[1] = polo1[1];
        polos2Old[0] = polo2[0];
        polos2Old[1] = polo2[1];
        polos3Old = polo3;

        calculaCoeficientes(polo1, polo2, polo3);

        return true;
    }
    else
    {
        return false;
    }
}

void Seguidor::calculaCoeficientes(double polo1[2], double polo2[2], double polo3)
{
    polo1[0] *= -1;
    polo1[1] *= -1;

    polo2[0] *= -1;
    polo2[1] *= -1;

    polo3 *= -1;

    coefB = polo1[1] + polo2[1];
    coefC = polo1[0]*polo2[0] + polo1[1]*polo2[1]*(-1);

    coefD = coefC*polo3;

    coefB += polo3;
    coefC += coefC*polo3;

    qDebug() << "FUNCTION calcula coeficientes";
    qDebug() << "polos";
    qDebug() << "p1 = " << polo1[0] << polo1[1];
    qDebug() << "ṕ2 = " << polo2[0] << polo2[1];
    qDebug() << "ṕ3 = " << polo3;
    qDebug() << "coeficientes b c d";
    qDebug() << coefB << coefC << coefD;
}

void Seguidor::calculaQg()
{
    Mat_Qg(matQg, matGG, 1, matGG, coefB, matGG, coefC, matIm, coefD, 3, 3);

    qDebug() << "FUNCTION calculaQg";
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++)
            qDebug() << matAuxGanhos[i][j];
        qDebug() << "\n";
    }

}

void Seguidor::ganhosArckemann()
{
    qDebug() << "vai calcular Qg";
    calculaQg();

    Mat_Mult(matArckemann, matLinhaArck, 1, 3, matWI,3,3);
    qDebug() << "ganhos";
    qDebug() << matArckemann[0][0] << matArckemann[0][1] << matArckemann[0][2];
    Mat_Mult(matArckemann, matArckemann, 1, 3, matQg,3,3);

    qDebug() << "FUNCTION calcula coeficientes";
    qDebug() << "ganhos2";
    qDebug() << matArckemann[0][0] << matArckemann[0][1] << matArckemann[0][2];
}

Seguidor::Seguidor()
{
    matGG = Mat_Aloc(3,3);
    matHH = Mat_Aloc(3,1);
    matWI = Mat_Aloc(3,3);
    matArckemann = Mat_Aloc(1,3);
    matLinhaArck = Mat_Aloc(1,3);
    matQg = Mat_Aloc(3,3);

    matIm = Mat_Aloc(3,3);

    matGanhos = Mat_Aloc(1,3);
    matAuxGanhos = Mat_Aloc(3,3);

    qDebug() << "aaaaaaaaaa";

    /*-------------------------------------*/

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

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            qDebug() << matAuxGanhos[i][j];




    /*

-1->ganhos^-1
 ans  =

  - 0.4976584    151.87479    1.0001076
  - 2.220D-16  - 1.           1.
    47.025114    46.875111    0.3086763

-1->ganhos = [-0.006542 0 0.021196; 0.00652 -0.006542 0.000069 ; 0.00652 0.993458 0.000069 ]
 ganhos  =

  - 0.006542    0.          0.021196
    0.00652   - 0.006542    0.000069
    0.00652     0.993458    0.000069
    */
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


double Seguidor::seguidor(double setPoint, double nivelT1, double nivelT2, double polo1[2], double polo2[2], double polo3)
{

    if(verificaPolos(polo1, polo2, polo3))
    {
        ganhosArckemann();

        /*
        Mat_Sum(matGanhos, matArckemann, matLinhaArck, 1, 3);
        Mat_Mult(matGanhos, matGanhos, 1, 3, matAuxGanhos, 3, 3);

        ganhoK21 = matGanhos[0][0];
        ganhoK22 = matGanhos[0][1];
        ganhoK1 = matGanhos[0][2];
        */
    }

   // ganhoK21 = matGanhos[0][0];
   // ganhoK22 = matGanhos[0][1];
   // ganhoK1 = matGanhos[0][2];



    return 2;

    erro = setPoint - nivelT2;
    v = v+erro;
    tensao = v*ganhoK1-ganhoK21*nivelT1-ganhoK22*nivelT2;

    return tensao;


}


