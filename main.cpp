#include "Puerta.cpp"
#include <iostream>

using namespace std;

#define CANT_PUERTAS 20

int Puerta::totalPuertas = CANT_PUERTAS;

int main(){
    Puerta* puertaPtr = new Puerta();
    puertaPtr->printNetwork();
}
