#include <random>

#ifndef RANDOM

#define RANDOM 1

using namespace std;

random_device rd; //Elementos para crear un generador de números aleatorios con una distribución uniforme.
mt19937 gen(rd()); // mersenne twister

class Random {
    public:
        // Función que retorna un número entero aleatorio en el rango de [low, high - 1];
        // Recibe el límite inferior inclusivo y el límite superior exclusivo.
        // Retorna el número aleatorio.
        static int rand_num(int low, int high){
            uniform_int_distribution<> distrib(low, high - 1); // Generador de números aleatorios.
            return distrib(gen);
        }
};

#endif