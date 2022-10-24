#ifndef SUBCAMARA

#define SUBCAMARA 1

#include "Estructuras/AVL_Node.h"
#include "Estructuras/IData.h"
#include <mutex>
#include <string>

using namespace std;

class SubCamara : public IData{
    private:
        int potencial;
        int distancia;
        int mineral; // cantidad de mineral al inicio de la generación de las cámaras.
        int mineralAzul;
        int mineralRojo;
        mutex *mtx;
        AVL_Node *position; // puntero a la posición en el árbol de esta subcámara
        SubCamara* parent;

    public:
        SubCamara(int pDistancia, int pMineral){
            distancia = pDistancia;
            mineral = pMineral;
            mineralAzul = pMineral;
            mineralRojo = pMineral;
            potencial = pMineral * pDistancia;
            mtx = new mutex();
            position = nullptr;
            parent = nullptr;
        }

        int compareTo(IData *pData){ // función para comparar dos cámaras. Se va a basar en su potencial de minado.
            SubCamara* compare = dynamic_cast<SubCamara*>(pData); // convertimos el IData a SubCamara para poder compararlo
            if (potencial == compare->getPotencial()){
                return 0;
            } else if (potencial > compare->getPotencial()){
                return 1;
            }
            return -1;
        }

        string toString(){ // imprime la información de la subcámara.
            string str = "Potencial: " + to_string(potencial) + " Distancia: " + to_string(distancia) + " Mineral: " + to_string(mineral) + " Distancia Total: " + to_string(getDistanciaTotal());
            return str;
        }

        int getPotencial() {
            return potencial;
        }

        void setPotencial(int pPotencial) {
            this->potencial = pPotencial;
        }

        int getDistancia(){
            return distancia;
        }

        void setDistancia(int pDistancia){
            distancia = pDistancia;
        }

        void setMineral(int pMineral){ // establecemos los dos contadores de mineral.
            mineral = pMineral;
            setMineralAzul(pMineral);
            setMineralRojo(pMineral);
        }

        int getMineral(){
            return mineral;
        }

        int getMineralAzul(){
            return mineralAzul;
        }

        void setMineralAzul(int pMineral){ // usamos mutex para proteger que dos threads la intenten acceder y modifiar.
            mtx->lock();
            mineralAzul = pMineral;
            mtx->unlock();
        }

        int decMineralAzul(int number, int totalRecogido, int capacity){
            // función para decrementar la cantidad de mineral azul.
            mtx->lock();
            if (number > mineralAzul){
                number = mineralAzul; // si intentamos coger más de lo que hay, cogemos todo lo que se puede.
            }
            if ((totalRecogido + number) > capacity){
                // si intenta recoger mas de lo que puede cargar, recoge solo lo que puede.
                number = capacity - totalRecogido;
            }
            mineralAzul -= number;
            mtx->unlock();
            return number;
        }

        bool isEmptyAzul(){
            return !mineralAzul;
        }

        int getMineralRojo(){
            return mineralRojo;
        }

        void setMineralRojo(int pMineral){
            mtx->lock();
            mineralRojo = pMineral;
            mtx->unlock();
        }

        int decMineralRojo(int number, int totalRecogido, int capacity){
            mtx->lock();
            if (number > mineralRojo){
                number = mineralRojo;  // si intentamos coger más de lo que hay, cogemos todo lo que se puede.
            }
            if ((totalRecogido + number) > capacity){
                // si intenta recoger mas de lo que puede cargar, recojo solo lo que se pueda.
                number = capacity - totalRecogido;
            }
            mineralRojo -= number;
            mtx->unlock();
            return number;
        }

        bool isEmptyRojo(){
            return !mineralRojo;
        }

        int getDistanciaTotal(){
            if (position->getParent() == nullptr){ // si es la raiz
                return distancia;
            }
            SubCamara* subCamaraParent = dynamic_cast<SubCamara*>(position->getParent()->getData()); // obtenemos la subcamara padre de esta.
            return distancia + subCamaraParent->getDistanciaTotal();
        }

        void setPosition(AVL_Node *pPosition){
            position = pPosition;
        }

        AVL_Node* getPosition(){
            return position;
        }

        SubCamara* getParent(){ // función para encontrar la subcámara padre directamente.
            AVL_Node* parentNode = position->getParent();
            if (parentNode){
                // si hay parent Node, casteamos su IData para obtener la subcámara padre.
                SubCamara* parentSubCamara = dynamic_cast<SubCamara*>(parentNode->getData());
                return parentSubCamara;
            }
            // si no hay parent
            return nullptr;
        }
        
        SubCamara* getLeft(){ // función para obtener la subcámara hija izquierda directamente.
            if(position->getLeft()){ // si hay un hijo izquierdo, retornamos el contenido del nodo como una subcamara
                return dynamic_cast<SubCamara*>(position->getLeft()->getData());
            }
            return nullptr; // si no hay hijo izquierdo, retornamos nulo.
        }

        SubCamara* getRight(){ // función para obtener la subcámara hija derecha directamente.
            if(position->getRight()){ // si hay un hijo derecho, retornamos el contenido del nodo como una subcamara
                return dynamic_cast<SubCamara*>(position->getRight()->getData());
            }
            return nullptr; // si no hay hijo derecho, retornamos nulo.
        }
        
};

#endif