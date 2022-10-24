#ifndef CAMARA

#define CAMARA 1

#include "SubCamara.h"
#include "RandomGen.cpp"
#include <iostream>

#include "Estructuras/AVL_Tree.cpp"

#define MAX_DISTANCE 720
#define MAX_CAMARAS 680

using namespace std;

class Camara{
    private:
        AVL_Tree* arbol; // arbol de subcámaras.
        int totalMineral; // total de mineral para todo el tunel
        int maxDistancia; // distancia máxima de un subárbol del tunel
        int distanciaGen; // distancia generada con cada subcámara
        int cantMineral; // cantidad de mineral generada para cada subcámara
        int mineralGenerado; // cantidad de mineral generado hasta el momento
        int contador; // contador para la cantidad de subcámaras generadas.
    public:
        Camara(){
            arbol = new AVL_Tree();

            totalMineral = Random::rand_num(200, 1000); // generamos un número máximo de minerales que puede haber en la camara
            maxDistancia = 0;
            mineralGenerado = 0;
            contador = 0;
    
            cantMineral = 1;
            distanciaGen = 10;
            while (contador < MAX_CAMARAS && mineralGenerado < totalMineral && maxDistancia < MAX_DISTANCE){
                cantMineral = Random::rand_num(1, 20); //generamos la cantidad de mineral en la camara.
                distanciaGen = Random::rand_num(10, 80);
                if (mineralGenerado + cantMineral > totalMineral){
                    cantMineral = totalMineral - mineralGenerado;
                    // si nos pasamos de la cantidad de mineral, simplemente cogemos lo que falta para llegar al límite
                }
                SubCamara* subcamara = new SubCamara(distanciaGen, cantMineral);
                AVL_Node* newNode = arbol->AVL_insert(subcamara); // encontramos el nodo que contiene a la subcamara.
                if (!newNode){ // si no se agregó borramos la cámara generada.
                    delete(subcamara);
                    continue;
                }

                subcamara->setPosition(newNode); // le asignamos su posición en el arbol.
                int distanciaTotal = subcamara->getDistanciaTotal();
                if (distanciaTotal > MAX_DISTANCE){ // si nos pasamos de los 720 metros, borramos el nodo y volvemos a intentarlo
                    arbol->AVL_delete(subcamara);
                    newNode = nullptr;
                    delete(subcamara);
                    continue;
                }

                // si todo salió bien, puedo incrementar la cantidad de mineral generado.
                mineralGenerado += cantMineral;
                maxDistancia = (distanciaTotal > maxDistancia) ? distanciaTotal : maxDistancia;
                contador++;
            }
        }

        
        AVL_Node* getRaiz(){
            return arbol->getRaiz();
        }
        
        AVL_Tree* getArbol(){
            return arbol;
        }

};

#endif
