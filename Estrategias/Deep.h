#ifndef DEEP

#define DEEP 1

#include "Estrategia.h"

class Deep : public Estrategia{
    public:
        void play (thread *pThread, Personaje *minero);

        void topo(thread *pThread, Personaje *minero);
};
#endif