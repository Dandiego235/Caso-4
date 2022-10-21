#ifndef CONSERVADOR

#define CONSERVADOR 1

#include "Estrategia.h"

class Conservador : public Estrategia{
    public:
        void play (thread *pThread, Personaje *minero);

        void topo(thread *pThread, Personaje *minero);
};
#endif