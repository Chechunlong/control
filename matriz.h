#ifndef MATRIZ_H
#define MATRIZ_H

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

    void Mat_Sum(double **matSum, double **matA, double **matB, double** matC, double **matD, int linhas, int colunas)
    {
        for(int i=0; i<linhas; i++)
        {
            for(int j=0; j<colunas; j++)
            {
                matSum[i][j] = matA[i][j] + matB[i][j] + matC[i][j] + matD[i][j];
            }
        }
    }

    void Mat_Qg(double **matSum, double **matA, double escalarA, double **matB, double escalarB, \
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

    void Mat_MultEscalar(double **matResul, double **mat, int linhas, int colunas, double escalar)
    {
        for(int i=0; i<linhas; i++)
        {
            for(int j=0; j<colunas; j++)
            {
                matResul[i][j] = mat[i][j] * escalar;
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

