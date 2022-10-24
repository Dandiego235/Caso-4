#ifndef EXPLORADOR

#define EXPLORADOR 1

#include <thread>
#include <iostream>
#include "../Puerta.cpp"
#include "Personaje.h"
#include "../Estrategias/Estrategia.h"

using namespace std;

class Explorador : public Personaje{
    public:
        Explorador(bool pJugador, Puerta *pPuerta){  // asignamos las caracteristicas especificas del explorador
            jugador = pJugador;
            string jugadorStr = (jugador) ? "Rojo" : "Azul";
            name = "Explorador " + jugadorStr;
            speed = 20.0f;
            capacity = 8;

            puerta = pPuerta; // asignamos la puerta y la cámara
            camara = nullptr;
            mineralAcumulado = 0;
        }

        void play(thread *pThread, Estrategia *strategy){
            strategy->play(pThread, this);
        }
};

#endif