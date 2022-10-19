#ifndef FLASH

#define FLASH

#include <iostream>
#include <thread>
#include "../Personajes/Personaje.h"
#include "../Estructuras/List.h"

using namespace std;

/*  Esta estrategia consiste en ir lo más rápido posible, ir 
    solamente a los hijos de la cámara raíz y caminar la menor distancia.
*/
class Flash : public Estrategia{
    public:
        play(thread pThread, Personaje *minero){
            Stack* stack = new Stack();

            while (pThread->joinable()){
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;
                if (!minero->getPuerta()->getCamara() || (!minero->getSubCamara()->getRight() && !minero->getSubCamara()->getLeft())){
                    // nos movemos hasta encontrar una cámara que tenga subárboles
                    int* index = new int();
                    *index = minero->getPuerta()->getQuantity() - 1;
                    if (*index){ // si la puerta en la que estamos tiene más de una puerta, salvamos la puerta a la que vamos a entrar en la pila.
                        stack->push(index);
                    } else {
                         // si la puerta en la que estamos solo tiene una puerta, el dato de index queda en 0.
                        index = stack->pop(); // entonces popeamos para devolvernos a la puerta anterior.
                        *index--; // decrementamos index
                        if (*index == 0 && stack->size() > 1){ // hacemos esto para no devolvernos a la puerta por donde entramos.
                            index = stack->pop(); // entonces popeamos para devolvernos a la puerta anterior.
                            *index--; // decrementamos index
                        }
                    }
                    
                    // obtenemos un índice de 0 a la cantidad de puertas a las que lleva donde estamos.
                    minero->setPuerta(minero->getPuerta()->getListaPuertas()->find(*index)); // entramos en la puerta
                    //cout << index << " " << puerta->getId() << endl;
                } else {
                    minero->setCamara(minero->getPuerta()->getCamara());
                    cout << "El " << minero->getName() << " encontró una cámara en la puerta " << minero->getPuerta()->getId() << endl;
                    try{
                        int distL = minero->getSubCamara()->getLeft();
                    }
                    catch(Exception e){
                        distL = 100;
                    }
                    try{
                        int distR = minero->getSubCamara()->getRight();
                    }
                    catch(Exception e){
                        distR = 100;
                    }
                    if(distL <= distR){
                        minero->setSubCamara(minero->getSubCamara()->getLeft());
                        this_thread::sleep_for(seconds(distL/minero->getSpeed());
                        
                    } else {

                    }
                }
                this_thread::sleep_for(seconds(1));
            }
        }

        topo(thread pThread, Personaje *pPersonaje){

        }
};

#endif