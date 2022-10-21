#ifndef ESTRATEGIA

#define ESTRATEGIA 1

#include "../Personajes/Personaje.h"
#include <thread>

using namespace std;

class Personaje;

class Estrategia{
    public:
        virtual void play(thread *pThread, Personaje *personajePtr) = 0;
        virtual void topo(thread *pThread, Personaje *personajePtr) = 0;
};

#endif