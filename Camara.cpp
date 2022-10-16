#include "SubCamara.h"
#include "RandomGen.cpp"
#include <iostream>

#define MAX_DISTANCE 720

using namespace std;

class Camara{
    private:
        SubCamara* raiz;
        int totalMineral;
        int maxDistancia;
        int totalDistLeft;
        int totalDistRight;
        int distanciaGen;
        int cantMineral;

    public:
        Camara(){
            raiz = nullptr;
            totalMineral = 200; //Random::rand_num(200, 600); // generamos un número máximo de minerales que puede haber en la 
            maxDistancia = 0;
            totalDistLeft = 0;
            totalDistRight = 0;
            int mineralGenerado = 0;
            

            while (mineralGenerado < totalMineral && maxDistancia < MAX_DISTANCE){
                cantMineral = Random::rand_num(1, 20); //generamos la cantidad de mineral en la camara.
                distanciaGen = Random::rand_num(10, 80);
                if (mineralGenerado + cantMineral > totalMineral){
                    cantMineral = totalMineral - mineralGenerado;
                    // si nos pasamos de la cantidad de mineral, simplemente cogemos lo que falta para llegar al límite
                }
                SubCamara* parentNode = findParent(cantMineral * distanciaGen); // buscamos el que sería el padre.
                if (parentNode){
                    int distanciaPadre = parentNode->getDistanciaTotal();
                    if (distanciaPadre + distanciaGen > MAX_DISTANCE){
                        distanciaGen = MAX_DISTANCE - distanciaPadre; 
                    }
                    if (distanciaPadre + distanciaGen > maxDistancia){
                        maxDistancia = distanciaPadre + distanciaGen;
                    }
                }
                AVL_insert(parentNode, raiz, distanciaGen * cantMineral);
                mineralGenerado += cantMineral;
                printTree();
                cin;
            }
        }
        /*
            Function to get the height of the tree
        */

        SubCamara* getRaiz(){
            return raiz;
        }
        
        int height(SubCamara *N){
            if (N == NULL)
                return 0;
            return N->getHeight();
        }

        /*
            Function to create a new SubCamara 
            Adding the newly created node as leaf node
        */

        SubCamara* newSubCamara(SubCamara* par, int key){
            //distanciaGen = 1;
            //cantMineral = key;
            SubCamara* node = new SubCamara(par, distanciaGen, cantMineral);
            if (!raiz){
                raiz = node;
            }
            return(node);
        }

        /*
            Function to update the heights
        */
        void update_height(SubCamara* root){
            if (root != NULL) {
                int val = 1;
                if (root->getLeft() != NULL)
                    val = root->getLeft()->getHeight() + 1;
                if (root->getRight() != NULL)
                    val = max(val, root->getRight()->getHeight() + 1);
                root->setHeight(val);
            }
        }

        /*
            Function for right rotation
        */
        SubCamara *rightRotate(SubCamara *y){
            SubCamara *x = y->getLeft();
            SubCamara *T2 = x->getRight();

            if (x->getRight() != NULL)
                x->getRight()->setParent(y);

        /*
            Perform rotation
        */
            y->setLeft(T2);
            x->setRight(y);

            x->setParent(y->getParent());
            y->setParent(x);

            if (x->getParent() != NULL && y->getPotencial() < x->getParent()->getPotencial()) {
                x->getParent()->setLeft(x);
            }
            else{
                if (x->getParent() != NULL)
                    x->getParent()->setRight(x);
                else {
                    raiz = x;
                }
            }

            y = x;

        /*
            Update the heights
        */    
            update_height(y->getLeft());
            update_height(y->getRight());
            update_height(y);
            update_height(y->getParent());

        /*
            Return new root
        */
            return y;
        }

        /*
            Function for left rotation
        */
        SubCamara *leftRotate(SubCamara *x){
            SubCamara *y = x->getRight();
            SubCamara *T2 = y->getLeft();

        /*
            Perform rotation
        */
            x->setRight(T2);

            if (y->getLeft() != NULL)
                y->getLeft()->setParent(x);
            y->setLeft(x);

            y->setParent(x->getParent());
            x->setParent(y);

            if (y->getParent() != NULL && x->getPotencial() < y->getParent()->getPotencial()) {
                y->getParent()->setLeft(y);
            }
            else{
                if (y->getParent() != NULL)
                    y->getParent()->setRight(y);
                else {
                    raiz = y;
                }
            }

        /*
            Update heights
        */

            update_height(x->getLeft());
            update_height(x->getRight());
            update_height(x);
            update_height(x->getParent());

        /*
            Return new root
        */
            return y;
        }

        /*
            Function to find the Balance factor of Nodes
        */
        int getBalance(SubCamara *N){
            if (N == NULL)
                return 0;
            return height(N->getLeft()) - height(N->getRight());
        }


        /*
            Function to construct a tree.
        */
        SubCamara* AVL_insert(int key){
            return AVL_insert(NULL, raiz, key);
        }

        SubCamara* AVL_insert(SubCamara *par, SubCamara* root, int key){

        /*
            Perform the normal BST insertion
        */
            if (root == NULL)
                return(newSubCamara(par, key));

            if (key < root->getPotencial())
                root->setLeft(AVL_insert(root, root->getLeft(), key));

            else if (key > root->getPotencial())
                root->setRight(AVL_insert(root, root->getRight(), key));
            else 
                return root;

        /*
            Step 1: Find the balance factor of parent\
        */
            int balance = getBalance(root);
        /*
            If this Node becomes unbalanced, all four cases are:
        */
        /*
            1. Left Left Case
        */
            if (balance > 1 && key < root->getLeft()->getPotencial())
                return rightRotate(root);

        /*
            2. Right Right Case
        */
            if (balance < -1 && key > root->getRight()->getPotencial())
                return leftRotate(root);

        /*
            3. Left Right Case
        */
            if (balance > 1 && key > root->getLeft()->getPotencial()){
                root->setLeft(leftRotate(root->getLeft()));
                return rightRotate(root);
            }

        /* 
            4. Right Left Case
        */
            if (balance < -1 && key < root->getRight()->getPotencial()){
                root->setRight(rightRotate(root->getRight()));
                return leftRotate(root);
            }

        /*
            Update the heights
        */
            update_height(root);

        /*
            Return the (unchanged) Node pointer
        */
            return root;
        }

        /*
            AVL Tree Traversal 
        */
        void PREORDER(SubCamara *root){
            cout << "Node: " << root->getPotencial() << ", Parent Node: ";

            if (root->getParent() != NULL)
                cout << root->getParent()->getPotencial() << endl;
            else
                cout << "NULL" << endl;
            /* 
                Recur to the left subtree
            */
            if (root->getLeft() != NULL) {
                PREORDER(root->getLeft());
            }

            /*
                Recur to the right subtree
            */
            if (root->getRight() != NULL) {
                PREORDER(root->getRight());
            }
        }

        /*
            Function to find the SubCamara with minimum key value
        */
        SubCamara * minValueSubCamara(SubCamara* node){
            SubCamara* current = node;
            /*
                Going to the left most side
            */
            while (current->getLeft() != NULL)
                current = current->getLeft();
            return current;
        }

        SubCamara* AVL_delete(int key){
            return AVL_delete(raiz, key);
        }
        /*
            Function to delete an SubCamara with the given key from the subtree 
        */
        SubCamara* AVL_delete(SubCamara* root, int key){
        /*
            Perform normal BST deletion
        */
            if (root == NULL)
                return root;
        /*
            Find the node to be deleted
        */

        /*
            Left Side
        */
            if ( key < root->getPotencial() )
                root->setLeft(AVL_delete(root->getLeft(), key));

        /*
            Right Side
        */
            else if( key > root->getPotencial() )
                root->setRight(AVL_delete(root->getRight(), key));

        /*
            Root Node
        */
            else{
                if( (root->getLeft() != NULL) && (root->getRight() == NULL) ){
                    SubCamara *temp = root->getLeft();

                    if (root->getParent() != NULL) {
                        if (root->getParent()->getPotencial() < root->getPotencial())
                            root->getParent()->setRight(root->getLeft());
                        else
                            root->getParent()->setLeft(root->getLeft());

                        /*
                            Update the height of root's parent
                        */
                        update_height(root->getParent());
                    }
                    root->getLeft()->setParent(root->getParent());
                /*
                    One child case
                */
                *root = *temp; /*
                    Copy the contents of the non-empty child
                */
                delete(temp);

            } else if((root->getLeft() == NULL) && (root->getRight() != NULL)){
                SubCamara *temp = root->getRight();
                
                if (root->getParent() != NULL) {
                    if (root->getParent()->getPotencial() < root->getPotencial())
                        root->getParent()->setRight(root->getRight());
                    else
                        root->getParent()->setLeft(root->getRight());

                        /*
                            Update the height of root's parent
                        */
                    update_height(root->getParent());
                }
                root->getRight()->setParent(root->getParent());
                /*
                    One child case
                */
                *root = *temp; /*
                    Copy the contents of the non-empty child
                */
                delete(temp);
            } else if ((root->getLeft() == NULL) && (root->getRight() == NULL)){ // case with no children
                cout << root->getParent()->getPotencial() << " " << root->getPotencial() << endl;
                if (root->getParent()->getPotencial() < root->getPotencial()) { // si estamos borrando uno de la derecha
                    root->getParent()->setRight(NULL);
                } else if (root->getParent()->getPotencial() > root->getPotencial()) { // si estamos borrando uno de la izquierda
                    root->getParent()->setLeft(NULL);
                } else { //if (root->getParent()->getPotencial() == root->getPotencial()){
                    if (root->getParent()->getLeft() == root){
                        cout << "Equals2" << endl;
                        root->getParent()->setLeft(NULL);
                    } else {
                        cout << "Equals3" << endl;
                        root->getParent()->setRight(NULL);
                    }
                }

                if (root->getParent() != NULL)
                    update_height(root->getParent());
                SubCamara *temp = root;
                root = NULL;
            } else{
                /*
                    SubCamara with two children: Get the inorder successor (smallest in the right subtree)
                */
                SubCamara* temp = minValueSubCamara(root->getRight());
                /*
                    Copy the inorder successor's data to this SubCamara
                */
                root->setDistancia(temp->getDistancia());
                root->setMineral(temp->getMineral());  
                /*
                    Delete the inorder successor
                */
                root->setRight(AVL_delete(root->getRight(),temp->getPotencial()));
            }
        }

        /*
            If the tree had only one SubCamara then return
        */
        if (root == NULL)
            return root;

        /*
            UPDATE HEIGHT OF THE CURRENT SubCamara
        */
        update_height(root);

        /*
            GET THE BALANCE FACTOR OF THIS SubCamara (to check whether this SubCamara became unbalanced)
        */
        int balance = getBalance(root);

        /*
            If this SubCamara becomes unbalanced, then there are 4 cases
        */

        /*
            Left Left Case
        */
        if (balance > 1 && getBalance(root->getLeft()) >= 0)
            return rightRotate(root);

        /*
            Left Right Case
        */
        if (balance > 1 && getBalance(root->getLeft()) < 0){
            root->setLeft(leftRotate(root->getLeft()));
            return rightRotate(root);
        }

        /*
            Right Right Case
        */
        if (balance < -1 && getBalance(root->getRight()) <= 0)
            return leftRotate(root);

        /*
            Right Left Case
        */
        if (balance < -1 && getBalance(root->getRight()) > 0){
            root->setRight(rightRotate(root->getRight()));
            return leftRotate(root);
        }

        return root;
        }

        // Print the tree
        void printTree(){
            printTree(raiz, "", true);
        }
        void printTree(SubCamara *pRoot, string indent, bool last) {
            if (pRoot != nullptr) {
                cout << indent;
                if (last) {
                cout << "R----";
                indent += "   ";
                } else {
                cout << "L----";
                indent += "|  ";
                }
                cout << "Potencial: " << pRoot->getPotencial() << " Distancia: " << pRoot->getDistancia() << " Mineral: " << pRoot->getMineral()
                 << " Distancia Total: " << pRoot->getDistanciaTotal() << endl;
                printTree(pRoot->getLeft(), indent, false);
                printTree(pRoot->getRight(), indent, true);
            }
        }

        SubCamara* searchI(int key){
            SubCamara* nodePtr = raiz;
            while(nodePtr){
                if (key == nodePtr->getPotencial()){ // si son iguales, no debemos realizar nada, entonces salimos
                    return nodePtr;
                } else if (key < nodePtr->getPotencial()){ // si es menor, vamos al subárbol izquierdo
                    nodePtr = nodePtr->getLeft(); // nos movemos el subarbol izquierdo;
                } else { 
                    nodePtr = nodePtr->getRight(); // nos movemos el subarbol derecho;
                }
            }
            return NULL; // si no lo encontró, retorna NULL.
        }

        SubCamara* searchR(SubCamara *root, int key){
            if (root == NULL || root->getPotencial() == key){
                return root; // casos base: Si la raíz es nula o si ya es la llave.
            }

            // Si la valor a buscar es mayor que el valor dado, nos movemos al subárbol derecho
            if (key > root->getPotencial()){
                return searchR(root->getRight(), key);
            }

            // Si el valor a buscar es menor que el valor dado, nos movemos al subárbol izquierdo
            return searchR(root->getLeft(), key);
        }

        // Función que busca el que sería el padre de un nuevo nodo que vamos a insertar.
        SubCamara* findParent(int key){
            SubCamara* nodePtr = raiz;
            while(nodePtr){
                if (key == nodePtr->getPotencial()){ // si son iguales, no debemos realizar nada, entonces salimos
                    return nodePtr;
                } else if (key < nodePtr->getPotencial()){ // si es menor, vamos al subárbol izquierdo
                    if (nodePtr->getLeft()){
                        nodePtr = nodePtr->getLeft(); // nos movemos el subarbol izquierdo;
                    } else {
                        return nodePtr; // si debe ir a la izquierda del nodo y esto no tiene hijo, encontramos el padre del nuevo nodo.
                    }
                } else { 
                    if (nodePtr->getRight()){
                        nodePtr = nodePtr->getRight(); // nos movemos el subarbol izquierdo;
                    } else {
                        return nodePtr; // si debe ir a la derecha del nodo y esto no tiene hijo, encontramos el padre del nuevo nodo.
                    }
                }
            }
            return nodePtr; // si no lo encontró, retorna NULL.
        }
};


int main(){
    Camara *arbol = new Camara();

    //arbol->printTree();


/*    //Constructing tree 

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
    cout << nodo->getHeight() << " " << nodo->getPotencial()  << " " << nodo->getParent()->getPotencial() << endl;*/

}
