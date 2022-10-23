#ifndef ESTRATEGIA

#define ESTRATEGIA 1

#include "../Personajes/Personaje.h"
#include <thread>
#include <unordered_set>

using namespace std;

class Personaje;

class Estrategia{
    protected:
        Stack<int>* stack; // pila para guardar los índices y recorrer las ultimas de primero.
        int* index; // puntero que voy a usar para meter y extraer índices de la pila.
        int nextDoor; // índice a la siguiente puerta donde voy a entrar.
        unordered_set<int> camarasVisitadas; // almacenamos las puertas donde están las cámaras a donde entramos
        int mineralRecogido;

    public:
        virtual void play(thread *pThread, Personaje *personajePtr) = 0;
        virtual void topo(thread *pThread, Personaje *personajePtr) = 0;

        void moveNextDoor(Personaje *minero){
            // nos movemos hasta encontrar una cámara que tenga subárboles
            nextDoor = *stack->getFirst()->getData();
            
            // obtenemos un índice de 0 a la cantidad de puertas a las que lleva donde estamos.
            minero->setPuerta(minero->getPuerta()->getListaPuertas()->find(nextDoor)); // entramos en la puerta
            
            if (nextDoor || stack->getSize() == 1){ // si la puerta no es para devolvernos o si es para entrar a la primera puerta del origen.
                index = new int();
                *index = minero->getPuerta()->getQuantity() - 1; //salvamos la siguiente puerta a la que vamos a entrar en la pila
                stack->push(index);
            } else {
                // si la puerta en la que estamos solo tiene una puerta, el dato de index queda en 0.
                stack->pop();

                index = stack->getFirst()->getData(); // entonces popeamos para devolvernos a la puerta anterior.
                //if (*index){ // revisamos si el indice no es nulo
                *index = *index - 1; // decrementamos index
                //} else {
                    //cout << "El " << minero->getName() << " terminó de recorrer las puertas y recogió " << minero->getMineralAcumulado() << " minerales." << endl;
               //     return; // paran de recorrer las puertas.
                //}      
            }
        }
};

#endif