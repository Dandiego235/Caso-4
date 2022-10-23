#include "Puerta.cpp"
#include "GameTimer.h"
#include "Personajes/Personaje.h"
#include "Personajes/Explorador.h"
#include "Personajes/Carguero.h"
#include "Personajes/Topo.h"
#include "Estrategias/Estrategia.h"
#include "Estrategias/SpeedRun.h"
#include "Estrategias/Conservador.h"
#include "Estrategias/Deep.h"
//#include "Estrategias/TryHard.h"
#include <iostream>
#include <thread>

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

    bool turnos[] = {false, true};

    Personaje* personajesJug [2][CANT_PUERTAS];

    GameTimer* gameTimers[2];

    thread* timerThreads[2];

    string jugador;

    thread* threadsJugadores[2][3];

    int scores[2];

    Personaje *personajePtr;

    Estrategia *estrategiaPtr;

    /*GameTimer timer(120.0f);
    thread timerThread(&GameTimer::start, &timer); // inciamos el timer para el jugador azul

    cout << "Jugador Azul" << endl;
    
    thread* threadsAzules [CANT_MINEROS];

    Personaje *personajePtr;

    Personaje* personajesAzules[CANT_MINEROS];
    unordered_map<char, Estrategia*> estrategias;

    Estrategia *estrategiaPtr = new SpeedRun();
    estrategias['1'] = estrategiaPtr;*/

    /*estrategiaPtr = new Conservador();
    estrategias['2'] = estrategiaPtr;

    estrategiaPtr = new Deep();
    estrategias['3'] = estrategiaPtr;

    estrategiaPtr = new TryHard();
    estrategias['4'] = estrategiaPtr;*/

    for (int index = 0; index < 2; index++){
        int minero = 0;
        char choice;
        streambuf* orig_buf;
        jugador = turnos[index] ? "Rojo" : "Azul";
        cout << "Jugador " << jugador << endl;
        gameTimers[index] = new GameTimer(120.0f);

        thread timerThread(&GameTimer::start, gameTimers[index]);

        while (minero < CANT_MINEROS){
            cout << "Ingrese el número para el tipo de minero que desea crear: \n1. Explorador\n2. Carguero\n3. Topo\n" << endl;
            orig_buf = cout.rdbuf();
            cout.rdbuf(NULL);
            cin >> choice;
            switch(choice){
                case '1':
                    personajePtr = new Explorador(turnos[index], puertaPtr);
                    break;
                case '2':
                    personajePtr = new Carguero(turnos[index], puertaPtr);
                    break;
                case '3':
                    personajePtr = new Topo(turnos[index], puertaPtr);
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
            personajesJug[index][minero] = personajePtr;
            threadsJugadores[index][minero++] = new thread(&Personaje::play, personajePtr, &timerThread, estrategiaPtr);
            cout.rdbuf(orig_buf);
        }

        cout.rdbuf(orig_buf);
        timerThread.join();

        scores[index] = 0;
        for(int personaje = 0; personaje < CANT_MINEROS; personaje++){
            threadsJugadores[index][personaje]->join();
            scores[index] += personajesJug[index][personaje]->getMineralAcumulado();
        }
    }
    

    cout << "Los azules consiguieron " << scores[0] << " minerales." << endl;
    cout << "Los rojos consiguieron " << scores[1] << " minerales." << endl;

    if (scores[0] > scores[1]){
        cout << "¡Ganó el jugador azul!" << endl;
    } else if (scores[1] > scores[0]){
        cout << "¡Ganó el jugador rojo!" << endl;
    } else {
        cout << "Los jugadores han empatado." << endl;
    }

    /*Explorador *explorador = new Explorador(false, puertaPtr);

    Explorador *explorador2 = new Explorador(true, puertaPtr);

    
    
    thread exploradorThread(&Explorador::play, explorador, &timerThread);

    thread explorador2Thread(&Explorador::play, explorador2, &timerThread);

    timerThread.join();

    exploradorThread.join();

    explorador2Thread.join();*/
}

