#ifndef TOPO

#define TOPO 1

#include <thread>
#include <iostream>
#include "../Puerta.cpp"
#include "Personaje.h"

using namespace std;

class Topo : public Personaje{
    public:
        Topo(bool pJugador, Puerta *pPuerta){ // asignamos las caracteristicas especificas del topo
            jugador = pJugador;
            string jugadorStr = (jugador) ? "Rojo" : "Azul";
            name = "Topo " + jugadorStr;
            speed = 7.0f;
            capacity = 15;

            puerta = pPuerta; // asignamos la puerta y la cámara
            camara = nullptr;
            mineralAcumulado = 0;
        }

        void play(thread *pThread, Estrategia *strategy){
            strategy->topo(pThread, this);
        }
};

#endif