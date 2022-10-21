#ifndef FLASH

#define FLASH

#include <iostream>
#include <thread>
#include "../Personajes/Personaje.h"
#include "../Estructuras/List.h"
#include <unordered_set>
#include <chrono>

using namespace std;

/*  Esta estrategia consiste en ir lo más rápido posible, ir 
    solamente a los hijos de la cámara raíz y caminar la menor distancia.
    Solamente va a una subcamara si tiene una distancia menor a 50 metros.
*/


class SpeedRun : public Estrategia{
    private:
        Stack<int>* stack; // pila para guardar los índices y recorrer las ultimas de primero.
        int* index; // puntero que voy a usar para meter y extraer índices de la pila.
        int nextDoor; // índice a la siguiente puerta donde voy a entrar.
        unordered_set<int> camarasVisitadas; // almacenamos las puertas donde están las cámaras a donde entramos
        int mineralRecogido;

    public:
        void play(thread *pThread, Personaje *minero){
            stack = new List<int>();
            index = new int();
            *index = minero->getPuerta()->getQuantity() - 1; // metemos el índice de la última puerta de la puerta origen.
            stack->push(index);
            
            while (pThread->joinable()){
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    
                    //cout << index << " " << puerta->getId() << endl;

                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    /*cout << "Izquierda: " << minero->readMineral(minero->getSubCamara()->getLeft()) <<
                    " Distancia: " << minero->getSubCamara()->getLeft()->getDistancia() << 
                    " Derecha: " << minero->readMineral(minero->getSubCamara()->getRight()) <<
                     " Distancia: " << minero->getSubCamara()->getRight()->getDistancia() << endl;*/

                    SubCamara* left = minero->getSubCamara()->getLeft();
                    SubCamara* right = minero->getSubCamara()->getRight();

                    if (left){ // si tiene hijo izquierdo
                        if (left->getDistancia() <= 50 && minero->readMineral(left)){
                            // si la camara tiene una distancia menor a 51 y tiene mineral.

                            cout << "El " << minero->getName() << " está caminando " << left->getDistancia() << endl;

                            this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                            // esperamos a que llegue a la subcamara.

                            mineralRecogido = minero->takeMineral(left, minero->getCapacity());
                            // recoge todo lo que puede cargar.

                            cout << "El " << minero->getName() << " recogió " << mineralRecogido << " de minerales" << endl;

                            this_thread::sleep_for(duration<float>(left->getDistancia()/minero->getSpeed()));
                            // esperamos a que regrese.

                            if (pThread->joinable()){ // si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                            }
                        }
                    }
                    
                    if (right){ // si tiene hijo derecho.
                        if (right->getDistancia() <= 50 && minero->readMineral(right)){
                            // si la distancia es menor que 50 y el nodo tiene mineral.
                            cout << "El " << minero->getName() << " está caminando " << right->getDistancia() << endl;

                            this_thread::sleep_for(duration<float>(right->getDistancia()/minero->getSpeed()));
                            // esperamos a que termine.

                            mineralRecogido = minero->takeMineral(right, minero->getCapacity());
                            // recoge todo lo que puede cargar

                            cout << "El " << minero->getName() << " recogió " << mineralRecogido << " de minerales" << endl;

                            this_thread::sleep_for(duration<float>(right->getDistancia()/minero->getSpeed()));
                            // esperamos a que regrese.

                            if (pThread->joinable()){
                                minero->incMineralAcumulado(mineralRecogido); // si todavía puedo trabajar
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                            }
                        }
                    }
                    // AGREGAR CAMARA VISITADA
                    camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.

                }
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
            
            while (pThread->joinable()){
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;
                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.

                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " encontró una cámara en la puerta " << minero->getPuerta()->getId() << endl;

                    /*cout << "Izquierda: " << minero->readMineral(minero->getSubCamara()->getLeft()) <<
                    " Distancia: " << minero->getSubCamara()->getLeft()->getDistancia() << 
                    " Derecha: " << minero->readMineral(minero->getSubCamara()->getRight()) <<
                     " Distancia: " << minero->getSubCamara()->getRight()->getDistancia() << endl;*/

                    SubCamara* left = minero->getSubCamara()->getLeft();
                    SubCamara* right = minero->getSubCamara()->getRight();
                    if (left){
                        if (left->getDistancia() <= 50 && minero->readMineral(left)){
                            // si la camara tiene una distancia menor a 51 y tiene mineral.

                            cout << "El " << minero->getName() << " está caminando " << left->getDistancia() << endl;

                            this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                            // esperamos a que llegue

                            mineralRecogido = minero->takeMineral(left, minero->getCapacity());
                            // el minero recoge todo lo que puede.

                            cout << "El " << minero->getName() << " recogió " << mineralRecogido << " de minerales" << endl;

                            if (right->getDistancia() < left->getDistancia()){
                                // si la distancia de la cámara hermana es menor, nos cambiamos de rama para durar menos.
                                this_thread::sleep_for(duration<float>(right->getDistancia()/minero->getSpeed()));
                            } else {
                                this_thread::sleep_for(duration<float>(left->getDistancia()/minero->getSpeed()));
                            }

                            if (pThread->joinable()){
                                minero->incMineralAcumulado(mineralRecogido); // si todavia podemos trabajar, incrementa el numero de mineral recogido.
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                            }
                        }
                    }
                    if (right){
                        if (right->getDistancia() <= 50 && minero->readMineral(right)){
                            // si la distancia es menor que 51 y el nodo tiene mineral.

                            cout << "El " << minero->getName() << " está caminando " << right->getDistancia() << endl;
                            
                            this_thread::sleep_for(duration<float>(right->getDistancia()/minero->getSpeed()));
                            // esperamos a que llegue

                            mineralRecogido = minero->takeMineral(right, minero->getCapacity());
                            // el topo recoge todo lo que puede.

                            cout << "El " << minero->getName() << " recogió " << mineralRecogido << " de minerales" << endl;
                            
                            if (left->getDistancia() < right->getDistancia()){
                                // si la distancia de la cámara hermana es menor, nos cambiamos de rama.
                                this_thread::sleep_for(duration<float>(left->getDistancia()/minero->getSpeed()));
                                cout << "El " << minero->getName() << " se pasó de cámara." << endl;
                            } else {
                                this_thread::sleep_for(duration<float>(right->getDistancia()/minero->getSpeed()));
                            }

                            if (pThread->joinable()){
                                minero->incMineralAcumulado(mineralRecogido);
                            }

                            cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                        }
                    }
                    // AGREGAR CAMARA VISITADA
                    camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.

                }
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