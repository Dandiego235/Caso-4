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

// va hasta el final de la cámara, a la subcámara más profunda y luego pasa a otra cámara distinta.
class Deep : public Estrategia{
    private:
        bool change;

    public:
        void play(thread *pThread, Personaje *minero){
            stack = new List<int>();
            index = new int();
            *index = minero->getPuerta()->getQuantity() - 1; // metemos el índice de la última puerta de la puerta origen.
            stack->push(index);
            int mineralAgarrado = 0;

            while (pThread->joinable()){
                if (nextDoor == -1){ // si ya terminó de recorrer las puertas, se sale.
                    return;
                }
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    
                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    mineralRecogido = 0;

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();

                        if (left && !right){
                            // si existe un hijo en una dirección, pero en la otra no, se va por la dirección en la que sí existe
                            cout << "El " << minero->getName() << " está caminando " << left->getDistancia() << endl;
                            this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(left);

                        } else if(!left && right){
                            cout << "El " << minero->getName() << " está caminando " << right->getDistancia() << endl;
                            this_thread::sleep_for(chrono::duration<float>(right->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(right);

                        } else if(right && left){
                            // si existen ambas direcciones, busca para ver cuál tiene mayor distancia y se va a esa, porque es más profunda
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
                                if (minero->readMineral(minero->getSubCamara()->getParent()) == 0){
                                    // si el nodo padre tiene 0 minerales, se muere el minero
                                    cout << "El " << minero->getName() << " se sofocó" << endl;
                                    return;
                                }

                                // recoge todo lo que puede cargar.
                                mineralAgarrado = minero->takeMineral(minero->getSubCamara(), minero->getCapacity(), mineralRecogido, minero->getCapacity());
                                mineralRecogido += mineralAgarrado;

                                cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;

                                // espera el tiempo que dura en recorrer la distancia para llegar al padre.
                                this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(minero->getSubCamara()->getParent());          
                            }
                            if (pThread->joinable()){ // si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                            } else {
                                return;
                            }
                            break; // termina el ciclo de while
                        }
                    }
                    // AGREGAR CAMARA VISITADA
                    camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.        
                }
                moveNextDoor(minero);
            }
        }
    

        void topo(thread *pThread, Personaje *minero){
            stack = new List<int>();
            index = new int();
            *index = minero->getPuerta()->getQuantity() - 1; // metemos el índice de la última puerta de la puerta origen.
            stack->push(index);
            int mineralAgarrado = 0;

            while (pThread->joinable()){
                if (nextDoor == -1){ // si ya terminó de recorrer las puertas, se sale.
                    return;
                }
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    
                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    mineralRecogido = 0;

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();

                        if (left && !right){
                        // si existe un hijo en una dirección, pero en la otra no, se va por la dirección en la que sí existe
                            cout << "El " << minero->getName() << " está caminando " << left->getDistancia() << endl;
                            this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(left);

                        } else if(!left && right){
                            cout << "El " << minero->getName() << " está caminando " << right->getDistancia() << endl;
                            this_thread::sleep_for(chrono::duration<float>(right->getDistancia()/minero->getSpeed()));
                            minero->setSubCamara(right);

                        } else if(right && left){
                            // si existen ambas direcciones, busca para ver cuál tiene mayor distancia y se va a esa, porque es más profunda
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
                                if (minero->readMineral(minero->getSubCamara()->getParent()) == 0){
                                    // si el nodo padre tiene 0 minerales, se muere el topo
                                    cout << "El " << minero->getName() << " se sofocó" << endl;
                                    return;
                                }
                                    
                                mineralAgarrado = minero->takeMineral(minero->getSubCamara(), minero->getCapacity(), mineralRecogido, minero->getCapacity());
                                mineralRecogido += mineralAgarrado;

                                cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;

                                if (!change){
                                    if(minero->getSubCamara()->getParent()->getRight() == minero->getSubCamara()){
                                        // si estamos en el hijo derecho de la camara
                                        if (minero->getSubCamara()->getParent()->getLeft()){
                                            // si tiene hijo izquierdo
                                            if (minero->getSubCamara()->getParent()->getLeft()->getDistancia() < minero->getSubCamara()->getDistancia()){
                                                // si la distancia del izquierdo es menor, se pasa
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
                                
                                //}
                                // espera el tiempo que dura en recorrer la distancia para llegar al padre.
                                this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(minero->getSubCamara()->getParent());          
                            }
                        
                            if (pThread->joinable()){ // si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                            } else {
                                return;
                            }
                            break; // termina el ciclo de while
                        }
                    }
                    // AGREGAR CAMARA VISITADA
                    camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.        
                }
                moveNextDoor(minero);
            }
        }
};
#endif