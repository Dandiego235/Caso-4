#ifndef DEEP

#define DEEP 1

#include "Estrategia.h"
#include <iostream>
#include <thread>
#include "../Personajes/Personaje.h"
#include "../Estructuras/List.h"
#include <unordered_set>
#include <chrono>

using namespace std;

// va hasta el final de la cámara
class Deep : public Estrategia{
    private:
        Stack<int>* stack; // pila para guardar los índices y recorrer las ultimas de primero.
        int* index; // puntero que voy a usar para meter y extraer índices de la pila.
        int nextDoor; // índice a la siguiente puerta donde voy a entrar.
        unordered_set<int> camarasVisitadas; // almacenamos las puertas donde están las cámaras a donde entramos
        int mineralRecogido;
        bool change;
        int mineralAgarrado;
    public:
        void play(thread *pThread, Personaje *minero){
            stack = new List<int>();
            index = new int();
            *index = minero->getPuerta()->getQuantity() - 1; // metemos el índice de la última puerta de la puerta origen.
            stack->push(index);
            mineralRecogido = 0;
            
            while (pThread->joinable()){
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    
                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();

                        if (left && !right){ // si existe un hijo en una dirección, pero en la otra no, se va por la dirección en la que sí existe
                            cout << "El " << minero->getName() << " está caminando " << left->getDistancia() << endl;
                            this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(left);
                        } else if(!left && right){
                            cout << "El " << minero->getName() << " está caminando " << right->getDistancia() << endl;
                            this_thread::sleep_for(chrono::duration<float>(right->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(right);
                        } else if(right && left){ // si existen ambas direcciones, busca para ver cuál tiene mayor distancia y se va a esa, porque es más profunda
                            if (left->getDistancia() >= right->getDistancia()){ 
                                cout << "El " << minero->getName() << " está caminando " << left->getDistancia() << endl;
                                this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(left);
                            } else{ 
                                cout << "El " << minero->getName() << " está caminando " << right->getDistancia() << endl;
                                this_thread::sleep_for(chrono::duration<float>(right->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(right);
                            }
                        } else { // Si la cámara no tiene hijos, ya llegó a la subcámara más profunda del árbol
                            while (minero->getSubCamara()->getParent() != nullptr){ // mientras no haya salido de la cámara
                                if ((minero->getSubCamara()->getParent() != nullptr)){
                                    if (minero->readMineral(minero->getSubCamara()->getParent()) == 0){ // si el nodo padre tiene 0 minerales, se muere el minero
                                        cout << "El " << minero->getName() << " se sofocó" << endl;
                                        return;
                                    }
                                }
                                if (mineralRecogido > minero->getCapacity()){ // si el mineral recogido sobrepasa la capacidad, lo iguala a la capacidad
                                    mineralRecogido = minero->getCapacity();
                                } else {
                                    // Si no, recoge todo lo que puede cargar.
                                    
                                    mineralAgarrado = minero->takeMineral(minero->getSubCamara(), minero->getCapacity(), mineralRecogido, minero->getCapacity());
                                    mineralRecogido += mineralAgarrado;
                                    cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;
                                }
                                // espera el tiempo que dura en recorrer la distancia para llegar al padre.
                                this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(minero->getSubCamara()->getParent());          
                            }
                            break; // termina el ciclo de while
                        }
                        if (pThread->joinable()){ // si todavía puede trabajar.
                            minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                            cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                        } else {
                            return;
                        }
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
            change = false;
            mineralRecogido = 0;
            
            while (pThread->joinable()){
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    
                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();

                        if (left && !right){ // si existe un hijo en una dirección, pero en la otra no, se va por la dirección en la que sí existe
                            cout << "El " << minero->getName() << " está caminando " << left->getDistancia() << endl;
                            this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(left);
                        } else if(!left && right){
                            cout << "El " << minero->getName() << " está caminando " << right->getDistancia() << endl;
                            this_thread::sleep_for(chrono::duration<float>(right->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(right);
                        } else if(right && left){ // si existen ambas direcciones, busca para ver cuál tiene mayor distancia y se va a esa, porque es más profunda
                            if (left->getDistancia() >= right->getDistancia()){ 
                                cout << "El " << minero->getName() << " está caminando " << left->getDistancia() << endl;
                                this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(left);
                            } else{ 
                                cout << "El " << minero->getName() << " está caminando " << right->getDistancia() << endl;
                                this_thread::sleep_for(chrono::duration<float>(right->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(right);
                            }
                        } else { // Si la cámara no tiene hijos, ya llegó a la subcámara más profunda del árbol
                            while (minero->getSubCamara()->getParent() != nullptr){ // mientras no haya salido de la cámara
                                if ((minero->getSubCamara()->getParent() != nullptr)){
                                    if (minero->readMineral(minero->getSubCamara()->getParent()) == 0){ // si el nodo padre tiene 0 minerales, se muere el minero
                                        cout << "El " << minero->getName() << " se sofocó" << endl;
                                        return;
                                    }
                                }
                                if (mineralRecogido > minero->getCapacity()){ // si el mineral recogido sobrepasa la capacidad, lo iguala a la capacidad
                                    mineralRecogido = minero->getCapacity();
                                } else {
                                    // Si no, recoge todo lo que puede cargar.
                                    mineralAgarrado = minero->takeMineral(minero->getSubCamara(), minero->getCapacity(), mineralRecogido, minero->getCapacity());
                                    mineralRecogido += mineralAgarrado;
                                    cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;
                                }
                                
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
                                // espera el tiempo que dura en recorrer la distancia para llegar al padre.
                                this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(minero->getSubCamara()->getParent());          
                            }
                            break; // termina el ciclo de while
                        }
                        if (pThread->joinable()){ // si todavía puede trabajar.
                            minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                            cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                        } else {
                            return;
                        }
                    }
                }
                // AGREGAR CAMARA VISITADA
                camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.
                
                // nos movemos hasta encontrar una cámara que tenga subárboles
                nextDoor = *stack->getFirst()->getData();
                
                // obtenemos un índice de 0 a la cantidad de puertas a las que lleva donde estamos.
                minero->setPuerta(minero->getPuerta()->getListaPuertas()->find(nextDoor)); // entramos en la puerta
                change = false;
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
};
#endif