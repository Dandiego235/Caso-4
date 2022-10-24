#ifndef RANDOMSTRAT

#define RANDOMSTRAT 1

#include "Estrategia.h"
#include "../RandomGen.cpp"

using namespace std;

// estrategia que toma decisiones aleatorias para recoger minerales, bajar por las camaras y salirse de la camara.
class RandomStrat : public Estrategia{
    public:
        void play (thread *pThread, Personaje *minero){
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

                    SubCamara* paths[3];

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();
                        SubCamara* parent = minero->getSubCamara()->getParent();
                        paths[0] = left;
                        paths[1] = right;
                        paths[2] = parent;

                        if (!minero->getSubCamara()->getParent()){ // si llegue a la raiz, entrego el mineral que he acumulado.
                            if (pThread->joinable()){ // si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                if (mineralRecogido){
                                    cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                                }
                                mineralRecogido = 0;
                                int salir = Random::rand_num(0, 5); // tenemos un chance de un veinte porciento de salir de la camara
                                if (!salir){
                                    break;
                                }
                                int side = Random::rand_num(0, 2);
                                if (side){
                                    minero->setSubCamara(right);
                                } else {
                                    minero->setSubCamara(left);
                                }
                                cout << "El " << minero->getName() << " está caminando " << minero->getSubCamara()->getDistancia() << endl;
                                this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                continue;
                            } else {
                                return;
                            }
                        } else {
                            if (minero->readMineral(parent) != 0){ // si el padre no está vacío.
                                int direccion;
                                if (mineralRecogido != minero->getCapacity()){
                                    mineralAgarrado = minero->takeMineral(minero->getSubCamara(), Random::rand_num(0, minero->getCapacity() + 1), 
                                    mineralRecogido, minero->getCapacity());
                                    mineralRecogido += mineralAgarrado;

                                    cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " minerales." << endl;

                                    if (mineralRecogido != minero->getCapacity() && minero->readMineral(minero->getSubCamara()) != 0){
                                        do{
                                            direccion = Random::rand_num(0,3); // generamos una direccion del 0 al 2
                                        } while (!paths[direccion]);
                                        // si la subcamara quedó con un nullptr, intentamos conseguir un puntero no nulo. 
                                    } else {
                                        direccion = 2;
                                        // si el minero vacio la camara se devuelve al padre.
                                    }
                                } else {
                                    direccion = 2;
                                    // si el minero esta lleno, se devuelve hasta la raiz.
                                }
                                if (direccion != 2){
                                       minero->setSubCamara(paths[direccion]);
                                    cout << "El " << minero->getName() << " está caminando " << minero->getSubCamara()->getDistancia() << endl;
                                    this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed())); 
                                } else {
                                    cout << "El " << minero->getName() << " está caminando " << minero->getSubCamara()->getDistancia() << endl;
                                    this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                    minero->setSubCamara(parent);
                                    // nos regresamos al padre. 
                                }
                            } else { // si el nodo padre tiene 0 minerales, se muere el minero
                                cout << "El " << minero->getName() << " se sofocó" << endl;
                                return;
                            }
                        }
                    }
                    // AGREGAR CAMARA VISITADA
                    camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.        
                }
                nextDoor = Random::rand_num(0, minero->getPuerta()->getQuantity()); // ingresamos a una puerta aleatoria.
                minero->setPuerta(minero->getPuerta()->getListaPuertas()->find(nextDoor));
            }
        }

        void topo(thread *pThread, Personaje *minero){
            stack = new List<int>();
            index = new int();
            *index = minero->getPuerta()->getQuantity() - 1; // metemos el índice de la última puerta de la puerta origen.
            stack->push(index);
            int mineralAgarrado = 0;
            bool change;

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

                    SubCamara* paths[3];

                    change = false;

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();
                        SubCamara* parent = minero->getSubCamara()->getParent();
                        paths[0] = left;
                        paths[1] = right;
                        paths[2] = parent;

                        if (!minero->getSubCamara()->getParent()){ // si llegue a la raiz, entrego el mineral que he acumulado.
                            if (pThread->joinable()){ // si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                if (mineralRecogido){
                                    cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                                }
                                mineralRecogido = 0;
                                int salir = Random::rand_num(0, 5); // tenemos un chance de un veinte porciento de salir de la camara
                                if (!salir){
                                    break;
                                }
                                int side = Random::rand_num(0, 2);
                                if (side){
                                    minero->setSubCamara(right);
                                } else {
                                    minero->setSubCamara(left);
                                }
                                cout << "El " << minero->getName() << " está caminando " << minero->getSubCamara()->getDistancia() << endl;
                                this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                continue;
                            } else {
                                return;
                            }
                        } else {
                            if (minero->readMineral(parent) != 0){ // si el padre no está vacío.
                                int direccion;
                                if (mineralRecogido != minero->getCapacity()){
                                    mineralAgarrado = minero->takeMineral(minero->getSubCamara(), Random::rand_num(0, minero->getCapacity() + 1), 
                                    mineralRecogido, minero->getCapacity());
                                    mineralRecogido += mineralAgarrado;

                                    cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " minerales." << endl;

                                    if (mineralRecogido != minero->getCapacity() && minero->readMineral(minero->getSubCamara()) != 0){
                                        do{
                                            direccion = Random::rand_num(0,3); // generamos una direccion del 0 al 2
                                        } while (!paths[direccion]);
                                        // si la subcamara quedó con un nullptr, intentamos conseguir un puntero no nulo. 
                                    } else {
                                        direccion = 2;
                                        // si el minero vacio la camara se devuelve al padre.
                                    }
                                } else {
                                    direccion = 2;
                                    // si el minero esta lleno, se devuelve hasta la raiz.
                                }
                                if (direccion != 2){
                                    if(!change && !Random::rand_num(0,3)){ // el topo va a tener un chance de un 33% de pasarse de camara
                                        if (parent->getLeft() == minero->getSubCamara()){
                                            if (parent->getRight()){
                                                // si el padre tiene hijo derecho, se pasa. si no, continúa con la dirección que había salido.
                                                minero->setSubCamara(parent->getRight());
                                                cout << "El " << minero->getName() << " se pasó a la cámara derecha." << endl;
                                                // nos pasamos a la camara derecha si estamos en la izquierda
                                                change = true; // sí cambió
                                                continue;
                                            }
                                        } else {
                                            if (parent->getLeft()){
                                                // si el padre tiene hijo izquierdo, se pasa. si no, continúa con la dirección que había salido.
                                                minero->setSubCamara(parent->getLeft());
                                                cout << "El " << minero->getName() << " se pasó a la cámara izquierda." << endl;
                                                // nos pasamos a la camara izquierda si estamos en la derecha
                                                change = true; // sí cambió
                                                continue;
                                            }
                                        }
                                    }
                                    minero->setSubCamara(paths[direccion]);
                                    cout << "El " << minero->getName() << " está caminando " << minero->getSubCamara()->getDistancia() << endl;
                                    this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed())); 
                                } else {
                                    cout << "El " << minero->getName() << " está caminando " << minero->getSubCamara()->getDistancia() << endl;
                                    this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                    minero->setSubCamara(parent);
                                    // nos regresamos al padre. 
                                }
                            } else { // si el nodo padre tiene 0 minerales, se muere el minero
                                cout << "El " << minero->getName() << " se sofocó" << endl;
                                return;
                            }
                        }
                    }
                    // AGREGAR CAMARA VISITADA
                    camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.        
                }
                nextDoor = Random::rand_num(0, minero->getPuerta()->getQuantity()); // ingresamos a una puerta aleatoria.
                minero->setPuerta(minero->getPuerta()->getListaPuertas()->find(nextDoor));
            }
        }
};
#endif