#pragma once
// #include "utils/Vec3.h"
#include <iostream>
using namespace std;

class Matriz
{
private:
    double **m;

public:
    // Atributos de linhas e colunas
    int linhas;
    int colunas;

    // Construtor vazio
    Matriz();        
    // Construtor que inicializa com 0                
    Matriz(int linhas, int colunas);
    // Construtor de cópia
    Matriz(const Matriz &m2);        

    // Destrutor
    ~Matriz();

    void inicializarMatriz(int linhas, int colunas);

    void excluir();

    // Operadores
    Matriz &operator=(const Matriz &m2);

    Matriz operator*(double escalar);
    Matriz operator/(double escalar);
    Matriz operator*(const Matriz n);
    Matriz operator+(const Matriz n);
    Matriz operator-(const Matriz n);

    // Métodos
    // Gera uma matriz aleatória 
    void matrizAleatoria();
    // Imprime a matriz
    void imprimir();
    // Retorna o valor de uma posição da matriz
    double getElemento(int linha, int coluna);
    // Define o valor de uma posição da matriz
    void setValor(int linha, int coluna, double valor);
    // Compara duas matrizes retornando true para mesma ordem e false para ordens diferentes
    bool verificaOrdem(Matriz n);
    // Cria uma matriz identidade da ordem da matriz
    void matrizIdentidade();
};

