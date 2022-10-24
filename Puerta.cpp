#include "Estructuras/List.h"
#ifndef PUERTA

#define PUERTA 1

#include "Camara.cpp"
#include "RandomGen.cpp"
#include <iostream>

using namespace std;

class Puerta {
    private:
        static int totalPuertas;
        static int contadorPuertas;

        int quantity;
        int id;

        List<Puerta> *listaPuertas;
        Camara *camara;
        static Queue<Puerta> *colaPuertas; // cola polimorfica

        void generarCamara(){
            int random = Random::rand_num(0, 3); // generamos la probabilidad de encontrar cámara
            if (random < 2){ // si es menor que 2, tiene una probabilidad de un 66% de generar cámara.
                camara = new Camara();
            } else {
                camara = nullptr;
            }
        }

        void generarRed(){
            while( totalPuertas != 0 && !colaPuertas->isEmpty()){
                Puerta* puertaPtr = colaPuertas->dequeue(); // sacamos la puerta a la cual le vamos a generar las puertas.
                int doorQuantity = Random::rand_num(1, 4); // generamos una cantidad del 1 al 3 porque ya tenemos la puerta que generó a esta puerta.
                if (totalPuertas < doorQuantity){
                    doorQuantity = totalPuertas; // si nos pasamos del total, vamos a generar las que faltan.
                }
                totalPuertas -= doorQuantity;
                for (int cantDoor = 0; cantDoor < doorQuantity; cantDoor++){
                    Puerta* newPuerta = new Puerta(puertaPtr, contadorPuertas++); // meter con segundo constructor
                    puertaPtr->getListaPuertas()->add(newPuerta); // le agregamos la nueva puerta a la puerta al que le generamos puertas.
                    colaPuertas->enqueue(newPuerta); // encolamos la nueva puerta generada para luego generar sus puertas.
                }
                puertaPtr->setQuantity(doorQuantity + 1); // establecemos la cantidad de puertas. + 1 porque ya tiene la puerta que la generó.
            }
            totalPuertas = contadorPuertas - 1;
        }
    public:
        Puerta(){ // constructor para la primer puerta del 
            listaPuertas = new List<Puerta>();
            generarCamara(); // creamos la camara
            quantity = Random::rand_num(2, 5); // el 5 es exclusivo
            id = contadorPuertas++; // primera puerta tiene ID 0.
            if (totalPuertas < quantity){ // genera las puertas que faltan para llegar al total.
                quantity = totalPuertas;
            }
            totalPuertas -= quantity;
            for (int cantDoor = 0; cantDoor < quantity; cantDoor++){
                Puerta* puertaPtr = new Puerta(this, contadorPuertas++); // meter con otro constructor
                listaPuertas->add(puertaPtr); // la añadimos a la lista de puertas que lleva al puerta origen.
                colaPuertas->enqueue(puertaPtr); // encolamos las puertas generadas para generarles puertas después.
            }
            generarRed();
        }

        Puerta(Puerta *pPuerta, int pId){ // constructor de puertas generadas después de la primera.
            // pPuerta es la puerta que generó la nueva puerta
            id = pId;
            listaPuertas = new List<Puerta>();
            listaPuertas->add(pPuerta);
            generarCamara(); // creamos la camara
            quantity = 1; // ya tiene la puerta que la generó.
        }

        int getId(){
            return id;
        }

        int getQuantity(){
            return quantity;
        }

        void setQuantity(int pQuantity){
            quantity = pQuantity;
        }

        List<Puerta>* getListaPuertas(){
            return listaPuertas;
        }

        Camara* getCamara(){
            return camara;
        }

        void printNetwork(){
            Puerta* puertaPtr = this;
            // buscamos la puerta original;
            // siempre el primer elemento va a ser la puerta que generó está puerta
            while (puertaPtr->getId()) { // mientras el id no sea igual a 0.
                puertaPtr = puertaPtr->getListaPuertas()->find(0);
            }
            string camaraStr = puertaPtr->getCamara() ? "c" : ""; // si tiene cámara, le ponemos una c.
            cout << puertaPtr->getId() << camaraStr <<  " ( ";

            Node<Puerta>* printPtr = puertaPtr->getListaPuertas()->getFirst();
            Stack<Node<Puerta>> *printStack = new List<Node<Puerta>>(); // usamos una cola para imprimir las puertas en orden.
            int printQuantity = 0;
            while (printQuantity < totalPuertas){
                camaraStr = printPtr->getData()->getCamara() ? "c" : "";
                cout << printPtr->getData()->getId() << camaraStr <<  " ( ";
                printQuantity++;
                if (printPtr->getData()->getListaPuertas()->findNode(1)){
                    // si el segundo elemento no es nullptr. Que tiene más de una puerta adyacente.
                    printStack->push(printPtr);
                    printPtr = printPtr->getData()->getListaPuertas()->findNode(1);
                    // entramos a la segunda puerta, para no regresarse, ya que con la primera se devuelve.
                } else {   
                    while (!printPtr->getNext()){ // si llegamos a una puerta con solo una puerta adyacente.
                        cout << " ) ";      
                        printPtr = printStack->pop(); // nos devolvemos hasta poder continuar.
                        if (!printPtr) {
                            cout << " )" << endl;
                            return;
                        }
                    }
                    cout << " ) ";
                    printPtr = printPtr->getNext(); // nos corremos a la siguiente puerta en la lista de adyacencias.
                }             
            }
        }
};

int Puerta::contadorPuertas = 0;
Queue<Puerta>* Puerta::colaPuertas = new List<Puerta>();

#endif

