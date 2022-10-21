#include "Puerta.cpp"
#include "GameTimer.h"
#include "Personajes/Personaje.h"
#include "Personajes/Explorador.h"
#include "Personajes/Carguero.h"
#include "Personajes/Topo.h"
#include "Estrategias/Estrategia.h"
#include "Estrategias/SpeedRun.h"
//#include "Estrategias/Conservador.h"
//#include "Estrategias/Deep.h"
//#include "Estrategias/TryHard.h"
#include <iostream>
#include <thread>
#include <unordered_map>

using namespace std;

#define CANT_PUERTAS 15

#define CANT_MINEROS 3

int Puerta::totalPuertas = CANT_PUERTAS;

int main(){
    Puerta* puertaPtr = new Puerta();
    puertaPtr->printNetwork();

    /*Personaje *personajePtr;
    personajePtr = new Explorador(false, puertaPtr);

    GameTimer timer(120.0f);
    thread timerThread(&GameTimer::start, &timer); // inciamos el timer para el jugador azul

    Estrategia *strategy = new SpeedRun();
    thread *mineroThread = new thread(&Personaje::play, personajePtr, &timerThread, strategy);

    timerThread.join();
    mineroThread->join();*/

    GameTimer timer(120.0f);
    thread timerThread(&GameTimer::start, &timer); // inciamos el timer para el jugador azul

    cout << "Jugador Azul" << endl;
    
    thread* threadsAzules [CANT_MINEROS];

    Personaje *personajePtr;

    Personaje* personajesAzules[CANT_MINEROS];
    unordered_map<char, Estrategia*> estrategias;

    Estrategia *estrategiaPtr = new SpeedRun();
    estrategias['1'] = estrategiaPtr;

    /*estrategiaPtr = new Conservador();
    estrategias['2'] = estrategiaPtr;

    estrategiaPtr = new Deep();
    estrategias['3'] = estrategiaPtr;

    estrategiaPtr = new TryHard();
    estrategias['4'] = estrategiaPtr;*/

    int minero = 0;
    char choice;
    streambuf* orig_buf;

    while (minero < CANT_MINEROS){
        cout << "Ingrese el número para el tipo de minero que desea crear: \n1. Explorador\n2. Carguero\n3. Topo\n" << endl;
        orig_buf = cout.rdbuf();
        cout.rdbuf(NULL);
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
        cout.rdbuf(orig_buf);
        cout << "Ingrese la estrategia para " << personajePtr->getName() << endl;
        cout << "1. SpeedRun\n2. Deep\n3. Conservador\n4. TryHard" << endl;
        orig_buf = cout.rdbuf();
        cout.rdbuf(NULL);
        cin >> choice;
        switch(choice){
            case '1':
                estrategiaPtr = new SpeedRun();
                break;
            /*case '2':
                personajePtr = new Carguero(false, puertaPtr);
                break;
            case '3':
                personajePtr = new Topo(false, puertaPtr);
                break;*/
            default:
                cout << "ERROR: La opción ingresada no es válida" << endl;
                continue;
        }
        personajesAzules[minero] = personajePtr;
        threadsAzules[minero++] = new thread(&Personaje::play, personajePtr, &timerThread, estrategiaPtr);
        cout.rdbuf(orig_buf);
    }

    cout.rdbuf(orig_buf);
    timerThread.join();
    int scoreAzul = 0;
    for(int index = 0; index < CANT_MINEROS; index++){
        threadsAzules[index]->join();
        scoreAzul += personajesAzules[index]->getMineralAcumulado();
    }

    cout << "Los azules consiguieron " << scoreAzul << " minerales." << endl;
    /*Explorador *explorador = new Explorador(false, puertaPtr);

    Explorador *explorador2 = new Explorador(true, puertaPtr);

    
    
    thread exploradorThread(&Explorador::play, explorador, &timerThread);

    thread explorador2Thread(&Explorador::play, explorador2, &timerThread);

    timerThread.join();

    exploradorThread.join();

    explorador2Thread.join();*/
}

