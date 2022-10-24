#ifndef CONSERVADOR

#define CONSERVADOR 1

#include "Estrategia.h"
#include <iostream>
#include <thread>
#include "../Personajes/Personaje.h"
#include "../Estructuras/List.h"
#include <unordered_set>
#include <chrono>

// agarra todo excepto 1, para no matar esas cámaras. Se queda en la misma cámara hasta agarrar todo el mineral.
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
                if (nextDoor == -1){ // si ya terminó de recorrer las puertas, se sale.
                    return;
                }
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    

                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    while (pThread->joinable()){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();
                        SubCamara * newNodo = nullptr; // se almacena la subcámara donde se va a ir
                        if (minero->getSubCamara()->getParent() == nullptr){ // si el minero llegó a la raíz
                            if (pThread->joinable()){ // revisa si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                                mineralRecogido = 0;
                            } else {
                                return;
                            }
                            if (minero->readMineral(left) == 1 && minero->readMineral(right) == 1){ // si ya recorrió los dos subárboles principales, los dos están en 1.
                                break;
                            }
                        } else {
                            if (minero->readMineral(minero->getSubCamara()->getParent()) == 0){ // si el nodo padre tiene 0 minerales, se muere el minero
                                cout << "El " << minero->getName() << " se sofocó" << endl;
                                return;
                            }
                        }
                        
                        
                        
                        // Si la capacidad del minero ya se llenó o si el minero ya recogió los dos subárboles de la subcámara, se devuelve al nodo padre
                        if (mineralRecogido == minero->getCapacity() || (minero->readMineral(left) == 1 && minero->readMineral(right) == 1)){
                            if (minero->readMineral(left) == 1 && minero->readMineral(right) == 1){
                                // si los dos hijos están en uno, agarra todo lo que pueda de la subcamara donde está.
                                mineralAgarrado = minero->leaveOne(minero->getSubCamara(), mineralRecogido);
                                mineralRecogido += mineralAgarrado;
                                cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;              
                            }
                            if ((minero->getSubCamara()->getParent())){
                                cout << "El " << minero->getName() << " está devolviéndose " << minero->getSubCamara()->getDistancia() << endl;
                                this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(minero->getSubCamara()->getParent());
                                // nos devolvemos al padre.
                            } else{
                                break;
                            }
                            continue;
                        } else {
                            // Si no, recoge todo lo que puede cargar.
                            mineralAgarrado = minero->leaveOne(minero->getSubCamara(), mineralRecogido);
                            mineralRecogido += mineralAgarrado;
                            cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;              
                        } // revisa si existe un nodo izquierdo
                        if (left){
                            if (minero->readMineral(left) > 1){  // si existe, revisa si ya se recorrió o no
                                if (mineralRecogido == minero->getCapacity()){ // si no, revisa si tiene espacio para ir ahí
                                    continue; // si no, se devuelve
                                }
                                newNodo = left;
                            }
                        } // realiza lo mismo, pero con el lado derecho
                        if(right && newNodo != left) { // revisa si el newNodo no se ha modificado para ser el left
                            if (minero->readMineral(right) > 1){
                                if (mineralRecogido == minero->getCapacity()){
                                    continue;
                                }
                                newNodo = right;
                            }
                        } 
                        if (newNodo){
                            minero->setSubCamara(newNodo);
                            // si no es hoja, entramos en el nodo establecido.
                        } else{
                             // si llegamos a un nodo hoja, nos devolvemos.
                            newNodo = minero->getSubCamara();
                            minero->setSubCamara(newNodo->getParent());
                        }
                        cout << "El " << minero->getName() << " está caminando " << newNodo->getDistancia() << endl;
                        this_thread::sleep_for(chrono::duration<float>(newNodo->getDistancia()/minero->getSpeed()));                        
                        continue;                         
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
            mineralRecogido = 0; // se reinicia el material recogido por el minero
            change = false;
            
            
            while (pThread->joinable()){
                if (nextDoor == -1){ // si ya terminó de recorrer las puertas, se sale.
                    return;
                }
                cout << "El " << minero->getName() << " está en la Puerta " << minero->getPuerta()->getId() << endl;

                if (minero->getPuerta()->getCamara() && camarasVisitadas.find(minero->getPuerta()->getId()) == camarasVisitadas.end()){
                    // si la puerta tiene una camara que no he visitado ya.
                    

                    minero->setCamara(minero->getPuerta()->getCamara()); // la camara del minero es donde entró

                    cout << "El " << minero->getName() << " entró en la cámara de la puerta " << minero->getPuerta()->getId() << endl;

                    while (true){
                        SubCamara* left = minero->getSubCamara()->getLeft();
                        SubCamara* right = minero->getSubCamara()->getRight();
                        SubCamara * newNodo = nullptr; // se almacena la subcámara donde se va a ir
                        if (minero->getSubCamara()->getParent() == nullptr){ // si el minero llegó a la raíz
                            if (pThread->joinable()){ // revisa si todavía puede trabajar.
                                minero->incMineralAcumulado(mineralRecogido); // incrementa la cantidad de mineral recogido.
                                cout << "El " << minero->getName() << " devolvió " << mineralRecogido << endl;
                                mineralRecogido = 0;
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
                                    // si estamos en la camara derecha del padre.
                                    if (minero->getSubCamara()->getParent()->getLeft() != nullptr){
                                        if (minero->getSubCamara()->getParent()->getLeft()->getDistancia() < minero->getSubCamara()->getDistancia()){
                                            minero->setSubCamara(minero->getSubCamara()->getParent()->getLeft());
                                            // el topo se pasa a la camara izquierda si la distancia es menor.
                                        }
                                    } else {
                                        minero->setSubCamara(minero->getSubCamara()->getParent());
                                    }
                                } else{
                                    // si estamos en la camara izquierda del padre.
                                    if (minero->getSubCamara()->getParent()->getRight() != nullptr){
                                        if (minero->getSubCamara()->getParent()->getRight()->getDistancia() < minero->getSubCamara()->getDistancia()){
                                            minero->setSubCamara(minero->getSubCamara()->getParent()->getRight());
                                        }
                                    } else {
                                        minero->setSubCamara(minero->getSubCamara()->getParent());
                                        // si no puede pasarse, se regresa por el padre.
                                    }
                                }
                                change = true; // el change se pone en true para señalar que se vino por el mejor nodo.
                                continue;
                            }
                            if (minero->readMineral(left) == 1 && minero->readMineral(right) == 1){
                                // si los dos hijos están en uno, agarra todo lo que pueda de la subcamara donde está.
                                mineralAgarrado = minero->leaveOne(minero->getSubCamara(), mineralRecogido);
                                mineralRecogido += mineralAgarrado;
                                cout << "El " << minero->getName() << " recogió " << mineralAgarrado << " de minerales" << endl;              
                            }
                            if ((minero->getSubCamara()->getParent())){
                                cout << "El " << minero->getName() << " está devolviéndose " << minero->getSubCamara()->getDistancia() << endl;
                                this_thread::sleep_for(chrono::duration<float>(minero->getSubCamara()->getDistancia()/minero->getSpeed()));
                                minero->setSubCamara(minero->getSubCamara()->getParent());
                                // nos devolvemos al padre.
                            } else {
                                break;
                            }
                            continue;
                        } else {
                            // Si no, recoge todo lo que puede cargar.
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
                        if (newNodo){
                            minero->setSubCamara(newNodo); // si existe, se mueve a la otra subcamara.
                        } else{
                             // si llegamos a una hoja
                            newNodo = minero->getSubCamara();
                            minero->setSubCamara(newNodo->getParent());
                        }
                        cout << "El " << minero->getName() << " está caminando " << newNodo->getDistancia() << endl;
                        this_thread::sleep_for(chrono::duration<float>(newNodo->getDistancia()/minero->getSpeed()));
                        continue;
                    }
                    // AGREGAR CAMARA VISITADA
                    camarasVisitadas.insert(minero->getPuerta()->getId()); // metemos la puerta donde esta la camara.
                }
                moveNextDoor(minero);
            }
        }
};
#endif