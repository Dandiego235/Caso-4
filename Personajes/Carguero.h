#ifndef CARGUERO

#define CARGUERO 1

#include <thread>
#include <iostream>
#include "../Puerta.cpp"
#include "Personaje.h"

using namespace std;

class Carguero : public Personaje{
    public:
        Carguero(bool pJugador, Puerta *pPuerta){ // asignamos las caracteristicas especificas del carguero
            jugador = pJugador;
            string jugadorStr = (jugador) ? "Rojo" : "Azul";
            name = "Carguero " + jugadorStr;
            speed = 10.0f;
            capacity = 40;

            puerta = pPuerta; // asignamos la puerta y la cámara
            camara = nullptr;
            mineralAcumulado = 0;
        }

        void play(thread *pThread, Estrategia *strategy){
            strategy->play(pThread, this);
        }
};

#endif