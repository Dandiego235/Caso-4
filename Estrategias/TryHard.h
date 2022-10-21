#ifndef TRYHARD

#define TRYHARD 1

#include "Estrategia.h"

class TryHard : public Estrategia{
    public:
        void play (thread *pThread, Personaje *minero);

        void topo(thread *pThread, Personaje *minero);
};
#endif