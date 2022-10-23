#ifndef TRYHARD

#define TRYHARD 1

#include "Estrategia.h"

// mayor cantidad en menor tiempo. Si no puede seguir bajando a tiempo, se sale de la cámara
class TryHard : public Estrategia{
    public:
        void play (thread *pThread, Personaje *minero);

        void topo(thread *pThread, Personaje *minero);
};
#endif