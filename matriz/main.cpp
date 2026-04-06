#include <iostream>

#include "matriz.h"
using namespace std;

void testeMatriz()
{
    Matriz m(2, 3);
    m.matrizAleatoria();

    cout << "Matriz M:\n";
    m.imprimir();

    Matriz n(3, 2);
    n.matrizAleatoria();

    cout << "Matriz N:\n";
    n.imprimir();

    Matriz A(3,3);
    A.matrizAleatoria();

    cout << "Matriz A:\n";
    A.imprimir();

    Matriz B(3, 3);
    B.matrizAleatoria();

    cout << "Matriz B:\n";
    B.imprimir();

    Matriz MxN = m * n;
    Matriz dobroM = m * 2;
    Matriz metadeM = m / 2;
    Matriz somaAB = A + B;
    Matriz subAB = A - B;    

    cout << "M * 2:\n";
    dobroM.imprimir();

    cout << "M / 2:\n";
    metadeM.imprimir();

    cout << "MxN:\n";
    MxN.imprimir();

    cout << "A + B:\n";
    somaAB.imprimir();

    cout << "A - B:\n";
    subAB.imprimir();
}

int main(int argc, char const *argv[])
{
    testeMatriz();

    return 0;
}
