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
        AVL_Tree* arbol;
        int totalMineral;
        int maxDistancia;
        int distanciaGen;
        int cantMineral;
        int mineralGenerado;
        int contador;
    public:
        Camara(){
            arbol = new AVL_Tree();

            totalMineral = Random::rand_num(200, 1000); // generamos un número máximo de minerales que puede haber en la camara
            maxDistancia = 0;
            mineralGenerado = 0;
            contador = 0;

            //cout << totalMineral << endl;            
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
                if (distanciaTotal > MAX_DISTANCE){ // si nos pasamos de los 720 metros, borramos el nodo.
                    arbol->AVL_delete(subcamara);
                    newNode = nullptr;
                    delete(subcamara);
                    continue;
                }

                // si todo salió bien, puedo incrementar la cantidad de mineral generado.
                mineralGenerado += cantMineral;
                maxDistancia = (distanciaTotal > maxDistancia) ? distanciaTotal : maxDistancia;
                contador++;
                cout << contador << " Mineral: " << cantMineral << " Distancia: " << distanciaGen << endl;
            }
            //arbol->printTree();
            cout << contador << " Mineral generado: " << mineralGenerado << " Maxdistance: " << maxDistancia << endl;
        }

        
        AVL_Node* getRaiz(){
            return arbol->getRaiz();
        }
        
        AVL_Tree* getArbol(){
            return arbol;
        }

};


/*int main(){
    Camara *camara = new Camara();

    camara->getArbol()->printTree();


   //Constructing tree 

    SubCamara* root = arbol->AVL_insert(40);
    arbol->AVL_insert(20);
    arbol->printTree();
    arbol->AVL_insert(10);
    arbol->printTree();
    arbol->AVL_insert(30);
    arbol->printTree();
    arbol->AVL_insert(25);
    arbol->printTree();
    arbol->AVL_insert(60);
    arbol->printTree();
    arbol->AVL_insert(45);
    arbol->printTree();
    arbol->AVL_insert(42);
    arbol->printTree();
    arbol->AVL_insert(52);
    arbol->printTree();
    arbol->AVL_insert(50);
    arbol->printTree();
    arbol->AVL_insert(55);
    arbol->printTree();
    arbol->AVL_insert(75);
    arbol->printTree();
    arbol->AVL_insert(70);
    arbol->printTree();
    arbol->AVL_insert(80);
    arbol->printTree();
    arbol->AVL_insert(85);

    arbol->printTree();
    cout << "Preorder traversal of the above AVL tree is:\n"<<endl;
    arbol->PREORDER(arbol->getRaiz());


    //Insert the node 7

    root = arbol->AVL_delete(45);
    cout << "Preorder traversal of the above AVL tree after deletion of 25 is:\n"<<endl;
    arbol->PREORDER(arbol->getRaiz());
    arbol->printTree();


    SubCamara *nodo = arbol->searchI(10);
    cout << nodo->getHeight() << " " << nodo->getPotencial()  << " " << nodo->getParent()->getPotencial() << endl;
    
}*/

#endif
