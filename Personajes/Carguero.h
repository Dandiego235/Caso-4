#ifndef CARGUERO

#define CARGUERO 1

#include <thread>
#include <iostream>
#include "../Puerta.cpp"
#include "Personaje.h"

using namespace std;

class Carguero : public Personaje{
    public:
        Carguero(bool pJugador, Puerta *pPuerta){
            jugador = pJugador;
            string jugadorStr = (jugador) ? "Rojo" : "Azul";
            name = "Carguero " + jugadorStr;
            speed = 10;
            capacity = 40;

            puerta = pPuerta; // asignamos la puerta y la cámara
            camara = nullptr;
        }

        void play(){
            while (true){
                
                cout << "El " << name << " está en la Puerta " << puerta->getId() << endl;
                if (!puerta->getCamara()){
                    int index = Random::rand_num(0,puerta->getQuantity());
                    // obtenemos un índice de 0 a la cantidad de puertas a las que lleva donde estamos.
                    puerta = puerta->getListaPuertas()->find(index); // entramsos en la puerta
                    //cout << index << " " << puerta->getId() << endl;
                } else {
                    camara = puerta->getCamara();
                    cout << "El " << name << " encontró una cámara en la puerta " << puerta->getId() << endl;
                    int index = Random::rand_num(0,puerta->getQuantity());
                    // obtenemos un índice de 0 a la cantidad de puertas a las que lleva donde estamos.

                    puerta = puerta->getListaPuertas()->find(index); // entramsos en la puerta
                    //cout << index << " " << puerta->getId() << endl;
                }
                this_thread::sleep_for(seconds(1));
            }
        }
};

#endif