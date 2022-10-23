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
    /*private:
        Stack<int>* stack; // pila para guardar los índices y recorrer las ultimas de primero.
        int* index; // puntero que voy a usar para meter y extraer índices de la pila.
        int nextDoor; // índice a la siguiente puerta donde voy a entrar.
        unordered_set<int> camarasVisitadas; // almacenamos las puertas donde están las cámaras a donde entramos
        int mineralRecogido;*/

    public:
        void play(thread *pThread, Personaje *minero){
            stack = new List<int>();
            index = new int();
            *index = minero->getPuerta()->getQuantity() - 1; // metemos el índice de la última puerta de la puerta origen.
            stack->push(index);
            
            while (pThread->joinable()){
                if (nextDoor == -1){
                    return;
                }
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
                    SubCamara* children[] = {left, right};

                    for (int index = 0; index < 2; index++){
                        if (children[index]){
                            if (children[index]->getDistancia() <= 50 && minero->readMineral(children[index])){
                                // si la camara tiene una distancia menor a 51 y tiene mineral.

                                cout << "El " << minero->getName() << " está caminando " << children[index]->getDistancia() << endl;

                                this_thread::sleep_for(chrono::duration<float>(left->getDistancia()/minero->getSpeed()));
                                // esperamos a que llegue a la subcamara.

                                mineralRecogido = minero->leaveOne(children[index], 0);
                                // recoge todo lo que puede cargar, pero deja 1 para no matar las camaras.

                                cout << "El " << minero->getName() << " recogió " << mineralRecogido << " de minerales" << endl;

                                this_thread::sleep_for(chrono::duration<float>(children[index]->getDistancia()/minero->getSpeed()));
                                // esperamos a que regrese.

                                if (pThread->joinable()){ // si todavía puede trabajar.
                                    minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                    cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                                }
                            }
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
            
            while (pThread->joinable()){
                if (nextDoor == -1){ // si ya terminó de recorrer las puertas, se sale.
                    return;
                }
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
                    SubCamara* children[] = {left, right};

                    for (int index = 0; index < 2; index++){
                        if (children[index]){
                            if (children[index]->getDistancia() <= 50 && minero->readMineral(children[index])){
                                // si la camara tiene una distancia menor a 51 y tiene mineral.

                                cout << "El " << minero->getName() << " está caminando " << children[index]->getDistancia() << endl;

                                this_thread::sleep_for(chrono::duration<float>(children[index]->getDistancia()/minero->getSpeed()));
                                // esperamos a que llegue

                                mineralRecogido = minero->leaveOne(children[index], 0);
                                // el topo recoge todo lo que puede, pero deja 1 para no matar las camaras.

                                cout << "El " << minero->getName() << " recogió " << mineralRecogido << " de minerales" << endl;

                                int indexHermana = !index;

                                if (children[indexHermana]->getDistancia() < children[index]->getDistancia()){
                                    // si la distancia de la hermana es menor, bajo por la otra camara.
                                    cout << " El " << minero->getName() << " se pasó de cámara." << endl;

                                    int mineralHermana = minero->leaveOne(children[index], mineralRecogido);
                                    // el topo recoge todo lo que puede, pero deja 1 para no matar las camaras.
                                    mineralRecogido += mineralHermana;
                                    cout << "El " << minero->getName() << " recogió " << mineralHermana << " minerales de la hermana" << endl;

                                    cout << "El " << minero->getName() << " está caminando " << children[indexHermana]->getDistancia() << endl;
                                    this_thread::sleep_for(chrono::duration<float>(children[indexHermana]->getDistancia()/minero->getSpeed()));
                                    // bajamos por la hermana
                                } else {
                                    this_thread::sleep_for(chrono::duration<float>(children[index]->getDistancia()/minero->getSpeed()));
                                    // bajamos por la normal.
                                }

                                if (pThread->joinable()){
                                    minero->incMineralAcumulado(mineralRecogido); // si todavia podemos trabajar, incrementa el numero de mineral recogido.
                                    cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                                } else {
                                    return;
                                }
                            }
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