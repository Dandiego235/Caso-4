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
#include "Estrategias/RandomStrat.h"
#include <iostream>
#include <thread>

using namespace std;

#define CANT_PUERTAS 15

#define CANT_MINEROS 3

#define CANT_JUGADORES 2

int Puerta::totalPuertas = CANT_PUERTAS;

int main(){
    Puerta* puertaPtr = new Puerta();
    puertaPtr->printNetwork();

    bool turnos[] = {false, true};

    Personaje* personajesJug [2][CANT_PUERTAS];

    GameTimer* gameTimers[2];

    thread* timerThreads[2];

    string jugador;

    thread* threadsJugadores[2][3];

    int scores[2];

    Personaje *personajePtr;

    Estrategia *estrategiaPtr;

    for (int index = 0; index < CANT_JUGADORES; index++){
        int minero = 0;
        char choice;
        streambuf* orig_buf;
        jugador = turnos[index] ? "Rojo" : "Azul";
        cout << "Jugador " << jugador << endl;
        gameTimers[index] = new GameTimer(120.0f);
        thread timerThread(&GameTimer::start, gameTimers[index]);

        while (minero < CANT_MINEROS){
            cout << "Ingrese el número para el tipo de minero que desea crear: \n1. Explorador\n2. Carguero\n3. Topo\n" << endl;
            orig_buf = cout.rdbuf(); // desactivamos la salida estandar para que los mensajes de los mineros no estorben.
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
            cout.rdbuf(orig_buf); // restauramos la salida estandar
            cout << "Ingrese la estrategia para " << personajePtr->getName() << endl;
            cout << "1. SpeedRun\n2. Deep\n3. Conservador\n4. RandomStrat" << endl;
            orig_buf = cout.rdbuf();
            cout.rdbuf(NULL); // desactivamos la salida estandar para que los mensajes de los mineros no estorben.
            cin >> choice;
            switch(choice){
                case '1':
                    estrategiaPtr = new SpeedRun();
                    break;
                case '2':
                    estrategiaPtr = new Deep();
                    break;
                case '3':
                    estrategiaPtr = new Conservador();
                    break;
                case '4':
                    estrategiaPtr = new RandomStrat();
                    break;
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
        orig_buf = cout.rdbuf();
        cout.rdbuf(NULL);
        scores[index] = 0;  // desactivamos la salida estandar para que los mensajes de los mineros no estorben.
        for(int personaje = 0; personaje < CANT_MINEROS; personaje++){
            threadsJugadores[index][personaje]->join();
            scores[index] += personajesJug[index][personaje]->getMineralAcumulado();
        }
        cout.rdbuf(orig_buf);
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
}

