#ifndef ESTRATEGIA

#define ESTRATEGIA 1

#include "Personajes/Personaje.h"

class Estrategia{
    public:
        public virtual void play(thread *pThread, Personaje *personajePtr) = 0;
        public virtual void topo(thread *pThread, Personaje *personajePtr) = 0;
};

#endif