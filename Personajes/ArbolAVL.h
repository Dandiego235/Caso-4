#ifndef ARBOLAVL

#define ARBOLAVL 1

#include "NodeAVL.h"
#include <iostream>

using namespace std;

class ArbolAVL {
    private:
        NodeAVL<int>* root;
    public:
        ArbolAVL(){
            root = NULL;
        }

        bool isEmpty(){
            return !root; // si no hay un hay un nodo raíz, se retorna verdadero.
        }

        NodeAVL<int>* getRoot(){
            return root;
        }

        void updateFE(NodeAVL<int>* pNode){
            NodeAVL<int>* nodePtr = pNode;
            NodeAVL<int>* parent = nodePtr->getParent();
            while(parent){ // mientras el puntero no sea nulo
                if (*nodePtr->getData() < *parent->getData()){ // si el hijo es a la izquierda
                    parent->decFE();
                    if (parent->getFE() == -2){
                        //rotacion
                        if (nodePtr->getFE() <= 0){
                            rightRotate(parent);
                            //rotacion a la derecha
                        } else {
                            leftRotate(nodePtr);
                            printTree(root, "", true);
                            rightRotate(parent);
                            //rotacion izquierda-derecha
                        }
                        return;
                    } else if (parent->getFE() == 0){
                        return; // si quedó en 0, no se aumentó la altura de los subárboles, entonces solo hizo falta cambiar el FE del parent.
                    }
                } else {
                    parent->incFE();
                    if (parent->getFE() == 2){
                        //rotacion
                        if (nodePtr->getFE() >= 0){
                            leftRotate(parent);
                            //rotacion a la izquierda
                        } else {
                            rightRotate(nodePtr);
                            printTree(root, "", true);
                            leftRotate(parent);
                            //rotacion dercha-izquierda
                        }
                        return;
                    } else if (parent->getFE() == 0){
                        return; // si quedó en 0, no se aumentó la altura de los subárboles, entonces solo hizo falta cambiar el FE del parent.
                    }
                }
                nodePtr = parent;
                parent = nodePtr->getParent();
            }
        }

        void rightRotate(NodeAVL<int> *pNode){
            NodeAVL<int> *parent = pNode->getParent();
            NodeAVL<int> *child = pNode->getLeft();
            if (parent){
                if (pNode == parent->getLeft()){
                    parent->setLeft(child);
                } else {
                    parent->setRight(child);
                }
            } else {
                root = child;  
            }
            child->setParent(parent);
            NodeAVL<int> *tempNode = child->getRight();
            child->setRight(pNode);
            pNode->setParent(child);
            pNode->setLeft(tempNode);
            if (tempNode){ // si temp no es nulo
                tempNode->setParent(pNode); // hacemos el pNode el padre de temp
            }
            calculateFE(pNode); // recalculamos los factores de equilibrio
            calculateFE(pNode->getParent());
            calculateFE(child);
        }

        void leftRotate(NodeAVL<int> *pNode){
            NodeAVL<int> *parent = pNode->getParent();
            NodeAVL<int> *child = pNode->getRight();
            if (parent){
                if (pNode == parent->getLeft()){
                    parent->setLeft(child);
                } else {
                    parent->setRight(child);
                }
            } else {
                root = child;  
            }
            child->setParent(parent);
            NodeAVL<int> *tempNode = child->getLeft();
            child->setLeft(pNode);
            pNode->setParent(child);
            pNode->setRight(tempNode);
            if (tempNode){
                tempNode->setParent(pNode);
            }
            calculateFE(pNode); // recalculamos los factores de equilibrio
            calculateFE(pNode->getParent());
            calculateFE(child);
        }

        void calculateFE(NodeAVL<int> *pNode){
            int left = 0;
            int right = 0;
            if (pNode->getLeft()){
                left = pNode->getLeft()->getFE();
            }
            if (pNode->getRight()){
                right = pNode->getRight()->getFE();
            }
            pNode->setFE(right - left);
        }

        // Print the tree
        void printTree(NodeAVL<int> *root, string indent, bool last) {
            if (root != nullptr) {
                cout << indent;
                if (last) {
                cout << "R----";
                indent += "   ";
                } else {
                cout << "L----";
                indent += "|  ";
                }
                cout << *root->getData() << endl;
                printTree(root->getLeft(), indent, false);
                printTree(root->getRight(), indent, true);
            }
        }

        void insert(int pNum){
            if (!isEmpty()){
                bool inserted = false;
                NodeAVL<int> *nodePtr = root;

                while (!inserted){
                    if (pNum < *nodePtr->getData()){ // si es menor, vamos al subárbol izquierdo
                        if (!nodePtr->getLeft()){ // si no tiene nodo hijo izquierdo
                            int* data = (int*) malloc(sizeof(int));
                            *data = pNum;
                            NodeAVL<int>* newNode = new NodeAVL<int>(data);
                            newNode->setParent(nodePtr);
                            nodePtr->setLeft(newNode); // ponemos el hijo izquierdo como el nuevo nodo.
                            // si ingresamos un nodo a la izquierda, el FE del camino se incrementa
                            updateFE(newNode); // recalculamos los factores de equilibrio y realizamos la rotación
                            inserted = true;
                        } else {
                            nodePtr = nodePtr->getLeft(); // nos movemos el subarbol izquierdo;
                        }
                    } else if (pNum > *nodePtr->getData()) { // si es mayor, vamos al subárbol derecho
                        if (!nodePtr->getRight()){ // si no tiene nodo hijo derecho
                            int* data = (int*) malloc(sizeof(int));
                            *data = pNum;
                            NodeAVL<int>* newNode = new NodeAVL<int>(data);
                            newNode->setParent(nodePtr);
                            nodePtr->setRight(newNode); // ponemos el hijo derecho como el nuevo nodo.
                            // si ingresamos un nodo a la derecha, el FE del camino se incrementa
                            updateFE(newNode); // recalculamos los factores de equilibrio y realizamos la rotación
                            inserted = true;
                        } else {
                            nodePtr = nodePtr->getRight(); // nos movemos el subarbol derecho;
                        }
                    } else { // si son iguales, no debemos realizar nada, entonces salimos
                        return;
                    }
                }

            } else {
                int* data = (int*) malloc(sizeof(int));
                *data = pNum;
                root = new NodeAVL<int>(data);
            }
        }

        /* La búsqueda en un árbol AVL funciona igual que en un árbol de búsqueda binaria.
           Nos vamos a mover por el árbol comparando el valor que vamos a buscar con un nodo.
           Si es mayor, nos movemos al nodo hijo de la derecha, y si es menor, nos movemos al
           nodo hijo de la izquierda.
           Esta función recibe el nodo raíz del árbol y el valor a buscar.
           Retorna el nodo que contiene el elemento buscado. Retorna nulo si no lo encuentra.
        */
        NodeAVL<int>* searchI(int key){
            NodeAVL<int>* nodePtr = root;
            while(nodePtr){
                if (key == *nodePtr->getData()){ // si son iguales, no debemos realizar nada, entonces salimos
                    return nodePtr;
                } else if (key < *nodePtr->getData()){ // si es menor, vamos al subárbol izquierdo
                    nodePtr = nodePtr->getLeft(); // nos movemos el subarbol izquierdo;
                } else { 
                    nodePtr = nodePtr->getRight(); // nos movemos el subarbol derecho;
                }
            }
            return NULL; // si no lo encontró, retorna NULL.
        }

        NodeAVL<int>* searchR(NodeAVL<int> *root, int key){
            if (root == NULL || *root->getData() == key){
                return root; // casos base: Si la raíz es nula o si ya es la llave.
            }

            // Si la valor a buscar es mayor que el valor dado, nos movemos al subárbol derecho
            if (key > *root->getData()){
                return searchR(root->getRight(), key);
            }

            // Si el valor a buscar es menor que el valor dado, nos movemos al subárbol izquierdo
            return searchR(root->getLeft(), key);
        }
};
#endif