#include <iostream>
#include "matriz.h"
#include <stdexcept>
#include <iomanip>
// #include "core/vec3.h"

using namespace std;

/**
 * @brief Construtor da classe Matriz.
 * 
 * @authors Eduardo Brandt
 */
Matriz::Matriz()
{
    inicializarMatriz(0, 0);
}

/**
 * @brief Construtor da classe recebendo linhas e colunas.
 * 
 * @param linhas
 * @param colunas
 * 
 * @authors Eduardo Brandt
 */
Matriz::Matriz(int linhas, int colunas)
{
    // Chamando o método inicializarMatriz 
    inicializarMatriz(linhas, colunas);
}

/**
 * @brief Função que cria a matriz.
 * 
 * @param linhas
 * @param colunas
 * 
 * @authors Eduardo Brandt
 */
void Matriz::inicializarMatriz(int linhas, int colunas)
{
    Matriz::linhas = linhas;
    Matriz::colunas = colunas;

    m = new double *[linhas];

    for (int i = 0; i < linhas; i++)
    {
        m[i] = new double[colunas];
        for (int j = 0; j < colunas; j++)
        {
            m[i][j] = 0;
        }
    }
}

/**
 * @brief Destrutor da classe Matriz.
 * 
 * @authors Eduardo Brandt
 */
Matriz::~Matriz()
{
    // Chamando o método excluir
    excluir();
}

/**
 * @brief Método para deletar a matriz m (ponteiro para ponteiro).
 * 
 * @authors Eduardo Brandt
 */
void Matriz::excluir()
{

    if (m != nullptr)
    {
        for (int i = 0; i < linhas; i++)
        {
            // Liberando cada ponteiro que aponta para uma linha da matriz.
            delete[] m[i]; 
        }
        
        // Liberando o ponteiro que aponta para a matriz.
        delete[] m;
        m = nullptr;
    }
}

/**
 * @brief Construtor de cópia que evita que a matriz seja deletada duas vezes.
 * 
 * @authors Eduardo Brandt
 */
Matriz::Matriz(const Matriz &m) : linhas{m.linhas}, colunas{m.colunas}
{
    Matriz::m = new double *[linhas];

    for (int i = 0; i < linhas; i++)
    {
        Matriz::m[i] = new double[colunas];
        for (int j = 0; j < colunas; j++)
        {
            Matriz::m[i][j] = m.m[i][j];
        }
    }
}

/**
 * @brief Sobrecarga do operador = para atribuição de matrizes.
 * 
 * @param uma matriz
 * 
 * @authors Eduardo Brandt
 */
Matriz &Matriz::operator=(const Matriz &m2)
{
    if (linhas != m2.linhas || colunas != m2.colunas)
    {
        excluir();

        inicializarMatriz(m2.linhas, m2.colunas);

        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                m[i][j] = m2.m[i][j];
            }
        }
    }

    return *this;
}

/**
 * @brief Método para imprimir a matriz.
 * 
 * @authors Eduardo Brandt
 */
void Matriz::imprimir()
{
    for (int i = 0; i < linhas; i++)
    {
        cout << "| ";
        for (int j = 0; j < colunas; j++)
        {
            cout << m[i][j] << " ";
        }
        cout << "|"
             << "\n";
    }
    cout << "\n";
}

/**
 * @brief Verifica se duas matrizes tem a mesma ordem.
 * 
 * @param uma matriz
 * 
 * @return true ou false
 * 
 * @authors Eduardo Brandt
 */
bool Matriz::verificaOrdem(Matriz n)
{
    return linhas == n.linhas && colunas == n. colunas;
}

/**
 * @brief Método para retornar a posição de um elemento da matriz.
 * 
 * @param linha
 * @param coluna
 * 
 * @return um elemento da matriz.
 * 
 * @authors Eduardo Brandt
 */
double Matriz::getElemento(int linha, int coluna)
{
    if ((linha < 1 && linha > linhas) || (coluna < 1 && coluna > colunas))
    {
        throw out_of_range("Tentando acessar um valor que não pertence a matriz.");
    }

    return m[linha - 1][coluna - 1];
}

/**
 * @brief Método para definir o valor de um elemento da matriz em determinada posição.
 * 
 * @param linha
 * @param coluna
 * @param valor
 * 
 * @authors Eduardo Brandt
 */
void Matriz::setValor(int linha, int coluna, double valor)
{
    if ((linha < 1 && linha > linhas) || (coluna < 1 && coluna > colunas))
    {
        throw out_of_range("Tentando acessar um valor que não pertence a matriz");
    }
    m[linha - 1][coluna - 1] = valor;
}

/**
 * @brief Método para criar uma matriz aleatória.
 * 
 * @authors Eduardo Brandt
 */
void Matriz::matrizAleatoria()
{
    // Inicializa o gerador de números aleatórios.
    srand(time(NULL));
    for (int l = 0; l < linhas; ++l) {
        for (int c = 0; c < colunas; ++c) {
            m[l][c] = rand() % 100;
        }
    }
}

/**
 * @brief Método para criar a matriz identidade (se possível) de acordo com a matriz que chama o método.
 * 
 * @authors Eduardo Brandt
 */ 
void Matriz::matrizIdentidade()
{ 
    if (linhas != colunas) {
        cerr << "Erro: tentando criar uma matriz identidade de ordem diferente\n";
        exit(1);
    }
    for (int l = 0; l < linhas; ++l) {
        for (int c = 0; c < colunas; ++c) {
            m[l][c] = (l == c) ? 1 : 0;
        }
    }
}

// Sobrecarga de operadores:

/**
 * @brief Sobrecarregando o operador * para poder multiplicar uma matriz por um escalar.
 * 
 * @param um escalar
 * 
 * @authors Eduardo Brandt
 */ 
Matriz Matriz::operator*(double escalar)
{
    Matriz tmp(linhas, colunas);
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            tmp.m[i][j] = m[i][j] * escalar;
        }
    }

    return tmp;
}

/**
 * @brief Sobrecarregando o operador / para poder dividir uma matriz por um escalar.
 * 
 * @param um escalar
 * 
 * @authors Eduardo Brandt
 */ 
Matriz Matriz::operator/(double escalar)
{
    if (escalar == 0)
    {
        throw invalid_argument("Divisão por zero");
    }
    Matriz tmp(linhas, colunas);
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            tmp.m[i][j] = m[i][j] / escalar;
        }
    }

    return tmp;

}

/**
 * @brief Sobrecarregando o operador * para poder multiplicar duas matrizes.
 * 
 * @param uma matriz
 * 
 * @authors Eduardo Brandt
 */ 
Matriz Matriz::operator*(const Matriz n)
{
    // Verificar se a matriz tem a mesma quantidade de linhas que a outra tem de colunas
    if (colunas != n.linhas)
    {
        throw invalid_argument("As matrizes não podem ser multiplicadas (número de linhas diferente do de colunas).");
    }

    Matriz tmp(linhas, n.colunas);

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < n.colunas; j++)
        {
            for (int k = 0; k < colunas; k++)
            {
                tmp.m[i][j] += m[i][k] * n.m[k][j];
            }
        }
    }

    return tmp;
}

/**
 * @brief Sobrecarregando o operador + para poder somar duas matrizes.
 * 
 * @param uma matriz
 * 
 * @authors Eduardo Brandt
 */ 
Matriz Matriz::operator+(const Matriz n)
{
    if (!verificaOrdem(n))
    {
        throw invalid_argument("As matrizes precisam ter mesma ordem para serem somadas.");
    }

    Matriz tmp(linhas, colunas);

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            tmp.m[i][j] = m[i][j] + n.m[i][j];
        }
    }

    return tmp;
}

/**
 * @brief Sobrecarregando o operador - para poder subtrair duas matrizes.
 * 
 * @param uma matriz
 * 
 * @authors Eduardo Brandt
 */ 
Matriz Matriz::operator-(const Matriz n)
{
    if (!verificaOrdem(n))
    {
        throw invalid_argument("As matrizes precisam ter mesma ordem para serem subtraídas.");
    }

    Matriz tmp(linhas, colunas);

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            tmp.m[i][j] = m[i][j] - n.m[i][j];
        }
    }

    return tmp;
}

/**
 * @brief Um modo da classe matriz multiplicar um vetor.
 * 
 * @param um vetor v
 * @param uma matriz m
 * 
 * @authors Eduardo Brandt, Fernanda Martins, Monique Ellen
 */ 
// Vec3 Matriz::multiplicaMatriz(Vec3 &v, Matriz &m){
//     if (m.col()== 3){
//         Vec3 tmp{0,0,0};
//         tmp.x = v.x * m.pegarValorEm(0,0)+v.y * m.pegarValorEm(0,1)+v.z *m.pegarValorEm(0,2);
//         tmp.y = v.x * m.pegarValorEm(1,0)+v.y * m.pegarValorEm(1,1)+v.z * m.pegarValorEm(1,2);
//         tmp.z = v.x * m.pegarValorEm(2,0)+v.y * m.pegarValorEm(2,1)+v.z * m.pegarValorEm(2,2);
//         return tmp;
//     }
//     else{
//         return (v);
//     }    
// }

