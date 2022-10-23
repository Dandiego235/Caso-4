#ifndef CONSERVADOR

#define CONSERVADOR 1

#include "Estrategia.h"
#include <iostream>
#include <thread>
#include "../Personajes/Personaje.h"
#include "../Estructuras/List.h"
#include <unordered_set>
#include <chrono>

// agarra todo excepto 1, para no matar esas cámaras.
class Conservador : public Estrategia{
    private:
        bool change;
        int mineralAgarrado; //  aquí se almacena el mineral agarrado en una sola subcámara
        
    public:
        void play(thread *pThread, Personaje *minero){
            stack = new List<int>();
            index = new int();
            *index = minero->getPuerta()->getQuantity() - 1; // metemos el índice de la última puerta de la puerta origen.
            stack->push(index);
            mineralRecogido = 0; // se reinicia el material recogido por el minero
            
            
            while (pThread->joinable()){
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    

                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();
                        SubCamara * newNodo; // se almacena la subcámara donde se va a ir
                        if (minero->getSubCamara()->getParent() == nullptr){ // si el minero llegó a la raíz
                            if (pThread->joinable()){ // revisa si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                            } else {
                                return;
                            }
                            if (minero->readMineral(left) == 1 && minero->readMineral(right) == 1){ // si ya recorrió los dos subárboles principales
                                break;
                            }
                        }
                        if ((minero->getSubCamara()->getParent() != nullptr)){
                            if (minero->readMineral(minero->getSubCamara()->getParent()) == 0){ // si el nodo padre tiene 0 minerales, se muere el minero
                                cout << "El " << minero->getName() << " se sofocó" << endl;
                                return;
                            }
                        }
                        
                        
                        
                        // Si la capacidad del minero ya se llenó o si el minero ya recogió los dos subárboles de la subcámara, se devuelve al nodo padre
                        if (mineralRecogido >= minero->getCapacity() || (minero->readMineral(left) == 1 && minero->readMineral(right) == 1)){ // si el mineral recogido sobrepasa la capacidad, lo iguala a la capacidad
                            this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(minero->getSubCamara()->getParent());
                            continue;
                        } else {
                            // Si no, recoge todo lo que puede cargar.
                            /*if (minero->readMineral(minero->getSubCamara()) < minero->getCapacity()){
                                mineralAgarrado = minero->takeMineral(minero->getSubCamara(), minero->readMineral(minero->getSubCamara())-1, mineralRecogido, minero->getCapacity());
                            } else {
                                mineralAgarrado = minero->takeMineral(minero->getSubCamara(), minero->getCapacity(), mineralRecogido, minero->getCapacity());
                            }*/
                            mineralAgarrado = minero->leaveOne(minero->getSubCamara(), mineralRecogido);
                            mineralRecogido += mineralAgarrado;
                            cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;              
                        } // revisa si existe un nodo izquierdo
                        if (left){
                            if (minero->readMineral(left) != 1){  // si existe, revisa si ya se recorrió o no
                                if (mineralRecogido == minero->getCapacity()){ // si no, revisa si tiene espacio para ir ahí
                                    continue; // si no, se devuelve
                                }
                                newNodo = left;
                            }
                        } // realiza lo mismo, pero con el lado derecho
                        if(right && newNodo != left) { // revisa si el newNodo no se ha modificado para ser el left
                            if (minero->readMineral(right) != 1){
                                if (mineralRecogido == minero->getCapacity()){
                                    continue;
                                }
                                newNodo = right;
                            }
                        } 
                        if (!newNodo){
                            newNodo = minero->getSubCamara();
                            minero->setSubCamara(newNodo->getParent());
                            
                        } else{
                            minero->setSubCamara(newNodo);
                        }
                        cout << "El " << minero->getName() << " está caminando " << newNodo->getDistancia() << endl;
                        this_thread::sleep_for(chrono::duration<float>(newNodo->getDistancia()/minero->getSpeed()));                        
                        continue;                         
                    }
                }
                // AGREGAR CAMARA VISITADA
                camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.
            
                // nos movemos hasta encontrar una cámara que tenga subárboles
                nextDoor = *stack->getFirst()->getData();
                
                // obtenemos un índice de 0 a la cantidad de puertas a las que lleva donde estamos.
                minero->setPuerta(minero->getPuerta()->getListaPuertas()->find(nextDoor)); // entramos en la puerta
                mineralRecogido = 0;
                if (nextDoor || stack->getSize() == 1){ // si la puerta no es para devolvernos o si es para entrar a la primera puerta del origen.
                    index = new int();
                    *index = minero->getPuerta()->getQuantity() - 1; //salvamos la siguiente puerta a la que vamos a entrar en la pila
                    stack->push(index);
                } else {
                    // si la puerta en la que estamos solo tiene una puerta, el dato de index queda en 0.
                    stack->pop();

                    index = stack->getFirst()->getData(); // entonces popeamos para devolvernos a la puerta anterior.
                    if (*index){ // revisamos si el indice no es nulo
                        *index = *index - 1; // decrementamos index
                    } else {
                        //cout << "El " << minero->getName() << " terminó de recorrer las puertas y recogió " << minero->getMineralAcumulado() << " minerales." << endl;
                        return; // paran de recorrer las puertas.
                    }      
                }
            }
        }
    

        void topo(thread *pThread, Personaje *minero){
            stack = new List<int>();
            index = new int();
            *index = minero->getPuerta()->getQuantity() - 1; // metemos el índice de la última puerta de la puerta origen.
            stack->push(index);
            mineralRecogido = 0; // se reinicia el material recogido por el minero
            change = false;
            
            
            while (pThread->joinable()){
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    

                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();
                        SubCamara * newNodo; // se almacena la subcámara donde se va a ir
                        if (minero->getSubCamara()->getParent() == nullptr){ // si el minero llegó a la raíz
                            if (pThread->joinable()){ // revisa si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                            } else {
                                return;
                            }
                            
                            if (minero->readMineral(left) == 1 && minero->readMineral(right) == 1){ // si ya recorrió los dos subárboles principales
                                break;
                            }
                        }
                        if ((minero->getSubCamara()->getParent() != nullptr)){
                            if (minero->readMineral(minero->getSubCamara()->getParent()) == 0){ // si el nodo padre tiene 0 minerales, se muere el minero
                                cout << "El " << minero->getName() << " se sofocó" << endl;
                                return;
                            }
                        }
                        
                        // Si la capacidad del minero ya se llenó o si el minero ya recogió los dos subárboles de la subcámara, se devuelve al nodo padre
                        if (mineralRecogido >= minero->getCapacity() || (minero->readMineral(left) == 1 && minero->readMineral(right) == 1)){ // si el mineral recogido sobrepasa la capacidad, lo iguala a la capacidad
                            if (!change){
                                if(minero->getSubCamara()->getParent()->getRight() == minero->getSubCamara()){
                                    if (minero->getSubCamara()->getParent()->getLeft() != nullptr){
                                        if (minero->getSubCamara()->getParent()->getLeft()->getDistancia() < minero->getSubCamara()->getDistancia()){
                                            minero->setSubCamara(minero->getSubCamara()->getParent()->getLeft());
                                        }
                                    } else {
                                        minero->setSubCamara(minero->getSubCamara()->getParent());
                                    }
                                } else{
                                    if (minero->getSubCamara()->getParent()->getRight() != nullptr){
                                        if (minero->getSubCamara()->getParent()->getRight()->getDistancia() < minero->getSubCamara()->getDistancia()){
                                            minero->setSubCamara(minero->getSubCamara()->getParent()->getRight());
                                        }
                                    } else {
                                        minero->setSubCamara(minero->getSubCamara()->getParent());
                                    }
                                }
                                change = true;
                                continue;
                            }
                            this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(minero->getSubCamara()->getParent());
                            continue;
                        } else {
                            // Si no, recoge todo lo que puede cargar.
                            /*if (minero->readMineral(minero->getSubCamara()) <= minero->getCapacity()){
                                mineralAgarrado = minero->takeMineral(minero->getSubCamara(), minero->readMineral(minero->getSubCamara())-1, mineralRecogido, minero->getCapacity());
                            } else {
                                mineralAgarrado = minero->takeMineral(minero->getSubCamara(), minero->getCapacity(), mineralRecogido, minero->getCapacity());
                            }*/
                            mineralAgarrado = minero->leaveOne(minero->getSubCamara(), mineralRecogido);
                            mineralRecogido += mineralAgarrado;
                            
                            cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;              
                        } // revisa si existe un nodo izquierdo
                        if (left){
                            if (minero->readMineral(left) != 1){  // si existe, revisa si ya se recorrió o no
                                if (mineralRecogido == minero->getCapacity()){ // si no, revisa si tiene espacio para ir ahí
                                    continue; // si no, se devuelve
                                }
                                newNodo = left;
                            }
                        } // realiza lo mismo, pero con el lado derecho
                        if(right && newNodo != left) { // revisa si el newNodo no se ha modificado para ser el left
                            if (minero->readMineral(right) != 1){
                                if (mineralRecogido == minero->getCapacity()){
                                    continue;
                                }
                                newNodo = right;
                            }
                        } 
                        if (!newNodo){
                            newNodo = minero->getSubCamara(); // si no puede continuar por el derecho, se devuelve al padre.
                            minero->setSubCamara(newNodo->getParent());
                            
                        } else{
                            minero->setSubCamara(newNodo); // si existe, se mueve a la otra subcamara.
                        }
                        cout << "El " << minero->getName() << " está caminando " << newNodo->getDistancia() << endl;
                        this_thread::sleep_for(chrono::duration<float>(newNodo->getDistancia()/minero->getSpeed()));
                        continue;                         
                    }
                }
                // AGREGAR CAMARA VISITADA
                camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.
            
                // nos movemos hasta encontrar una cámara que tenga subárboles
                nextDoor = *stack->getFirst()->getData();
                
                // obtenemos un índice de 0 a la cantidad de puertas a las que lleva donde estamos.
                minero->setPuerta(minero->getPuerta()->getListaPuertas()->find(nextDoor)); // entramos en la puerta
                mineralRecogido = 0;
                change = false;
                if (nextDoor || stack->getSize() == 1){ // si la puerta no es para devolvernos o si es para entrar a la primera puerta del origen.
                    index = new int();
                    *index = minero->getPuerta()->getQuantity() - 1; //salvamos la siguiente puerta a la que vamos a entrar en la pila
                    stack->push(index);
                } else {
                    // si la puerta en la que estamos solo tiene una puerta, el dato de index queda en 0.
                    stack->pop();

                    index = stack->getFirst()->getData(); // entonces popeamos para devolvernos a la puerta anterior.
                    if (*index){ // revisamos si el indice no es nulo
                        *index = *index - 1; // decrementamos index
                    } else {
                        //cout << "El " << minero->getName() << " terminó de recorrer las puertas y recogió " << minero->getMineralAcumulado() << " minerales." << endl;
                        return; // paran de recorrer las puertas.
                    }      
                }
            }
        }
};
#endif