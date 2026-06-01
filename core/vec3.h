#pragma once
#include <iostream>
using namespace std;

/**
 * brief Uma classe para representação de vetores no Espaço
 **/
class Vec3 
{
    public:
        
        /**
         * Construtor sem argumentos
         **/
        Vec3();

        /**
         * Construtor com dados do vetor
         * > param x um float para a coordenada x
         * > param y um float para a coordenada y
         * > param z um float para a coordenada z
         **/
        Vec3(float x, float y, float z);

        /**
         * Construtor com pontos
         * > a[] é o ponto inicial do vetor
         * > b[] é o ponto final do vetor
         **/
        Vec3(float a[], float b[]);
        
        /**
         * Construtor de cópia
         * > param v uma referência constante (&) para um vetor
         **/
        Vec3(const Vec3 &v);
        
        /**
         *operação que retorna o modulo do vetor
         **/
        float modulo();

        /**
         *operação que retorna o versor do vetor
         **/
        Vec3 versor();

        /**
         * operação que retorna a projeção deste vetor em outro vetor
         * > param v uma referência (&) para um vetor
         **/
        Vec3 projetar(Vec3 &v);

        /**
         * calcular o produto escalar deste vetor com outro vetor
         * > param v uma referência constante (&) para um vetor
         **/
        float produto_escalar(const Vec3 &v);

        /**
         * multiplica vetor por escaçar
         * > param v uma referência (&) para um vetor
         **/
        Vec3 multiEscalar(float escalar);
        
        /**
         * calcular o produto vetorial deste vetor com outro vetor
         * > param v uma referência constante (&) para um vetor
         **/
        Vec3 produto_vetorial(const Vec3 &v);

        /**
         * calcular o produto misto entre este vetor e outros dois vetores
         * > param v1 uma referência constante (&) para um vetor
         * > param v1 uma referência constante (&) para um vetor
         **/
        float produto_misto(const Vec3 &v1, const Vec3 &v2);

        /**
         * verificar se dois vetores são proporcionais
         * > param v uma referência constante (&) para um vetor
         **/
        bool verificar_proporcionalidade(const Vec3 &v);

        /**
         * calcular o angulo entre este vetor e outro vetor
         * > param v uma referência (&) para um vetor
         **/
        float angulo_entre_vetores(Vec3 &v);

        /**
         * Operador de soma e atribuição
         * > param v uma referência constante (&) para um vetor
         **/
        Vec3 operator+=(const Vec3 &v);

         /**
         * Operador de soma 
         * > param v uma referência constante (&) para um vetor
         **/
        Vec3 operator+(const Vec3 &v);

        /**
         * Operador de subtração e atribuição
         * > param v uma referência constante (&) para um vetor
         **/
        Vec3 operator-=(const Vec3 &v);

         /**
         * Operador de subtração 
         * > param v uma referência constante (&) para um vetor
         **/
        Vec3 operator-(const Vec3 &v);

        float x; /**< Valor da coordenada x */
        float y; /**< Valor da coordenada y */
        float z; /**< Valor da coordenada z */
};

/**
 * Função para impressão da classe vetor utilizando um ostrem
 * param out um ostrem
 * > param v uma referência constante para o vetor a ser impresso
 */
ostream & operator<< (ostream &out, const Vec3 &v);