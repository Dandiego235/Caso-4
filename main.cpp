#include "Puerta.cpp"
#include "GameTimer.h"
#include "Personajes/Personaje.h"
#include "Personajes/Explorador.h"

#include <iostream>

using namespace std;

#define CANT_PUERTAS 20

int Puerta::totalPuertas = CANT_PUERTAS;

int main(){
    Puerta* puertaPtr = new Puerta();
    puertaPtr->printNetwork();

    GameTimer timer(120.0f);

    Explorador *explorador = new Explorador(false, puertaPtr);

    Explorador *explorador2 = new Explorador(true, puertaPtr);

    thread timerThread(&GameTimer::start, &timer);
    
    thread exploradorThread(&Explorador::play, explorador, &timerThread);

    thread explorador2Thread(&Explorador::play, explorador2, &timerThread);

    timerThread.join();

    exploradorThread.join();

    explorador2Thread.join();
}

