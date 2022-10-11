#include "Puerta.cpp"
#include "GameTimer.h"
#include "Personajes/Personaje.h"
#include "Personajes/Explorador.h"
#include "Personajes/Carguero.h"
#include "Personajes/Topo.h"

#include <iostream>

using namespace std;

#define CANT_PUERTAS 20

#define CANT_MINEROS 3

int Puerta::totalPuertas = CANT_PUERTAS;

int main(){
    Puerta* puertaPtr = new Puerta();
    puertaPtr->printNetwork();

    GameTimer timer(120.0f);
    thread timerThread(&GameTimer::start, &timer); // inciamos el timer para el jugador azul

    cout << "Jugador Azul" << endl;
    
    thread* threadsAzules [CANT_MINEROS];

    Personaje *personajePtr;

    int minero = 0;
    char choice;
    cout << "Ingrese el número para el tipo de minero que desea crear: \n 1. Explorador\n2. Carguero\n3. Topo\n" << endl;
    streambuf* orig_buf = cout.rdbuf();
    cout.rdbuf(NULL);

    while (minero < CANT_MINEROS){
        cin >> choice;
        switch(choice){
            case '1':
                personajePtr = new Explorador(false, puertaPtr);
                break;
            case '2':
                personajePtr = new Carguero(false, puertaPtr);
                break;
            case '3':
                personajePtr = new Topo(false, puertaPtr);
                break;
            default:
                cout << "ERROR: La opción ingresada no es válida" << endl;
                continue;
        }
        threadsAzules[minero++] = new thread(&Personaje::play, personajePtr, &timerThread);
    }

    cout.rdbuf(orig_buf);

    timerThread.join();
    for(int index = 0; index < CANT_MINEROS; index++){
        threadsAzules[index]->join();
    }

    /*

    Explorador *explorador = new Explorador(false, puertaPtr);

    Explorador *explorador2 = new Explorador(true, puertaPtr);

    
    
    thread exploradorThread(&Explorador::play, explorador, &timerThread);

    thread explorador2Thread(&Explorador::play, explorador2, &timerThread);

    timerThread.join();

    exploradorThread.join();

    explorador2Thread.join();*/
}

