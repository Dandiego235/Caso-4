#ifndef SUBCAMARA

#define SUBCAMARA 1

#include "Estructuras/AVL_Node.h"
#include "Estructuras/IData.h"
#include <mutex>
#include <string>

using namespace std;

class SubCamara : public IData{
    private:
        int distanciaTotal;
        int potencial;
        int distancia;
        int mineral; // cantidad de mineral al inicio de la generación de las cámaras.
        int mineralAzul;
        int mineralRojo;
        mutex *mtx;
        AVL_Node *position; // puntero a la posición en el árbol de esta subcámara

    public:
        SubCamara(int pDistancia, int pMineral){
            distancia = pDistancia;
            mineral = pMineral;
            mineralAzul = pMineral;
            mineralRojo = pMineral;
            potencial = pMineral * pDistancia;
            mtx = new mutex();
            position = nullptr;
        }

        int compareTo(IData *pData){
            SubCamara* compare = dynamic_cast<SubCamara*>(pData); // convertimos el IData a SubCamara para poder compararlo
            if (potencial == compare->getPotencial()){
                return 0;
            } else if (potencial > compare->getPotencial()){
                return 1;
            }
            return -1;
        }

        string toString(){
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

        void setMineralAzul(int pMineral){
            mtx->lock();
            mineralAzul = pMineral;
            mtx->unlock();
        }

        int decMineralAzul(int number, int total, int capacity){
            mtx->lock();
            if (number > mineralAzul){
                number = mineralAzul;
            }
            if ((total + number) > capacity){
                number = capacity - number;
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

        int decMineralRojo(int number, int total, int capacity){
            mtx->lock();
            if (number > mineralRojo){
                number = mineralRojo;
            }
            if ((total + number) > capacity){
                number = capacity - number;
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

        void setDistanciaTotal(int pDistanciaTotal){
            distanciaTotal = pDistanciaTotal;
        }

        void setPosition(AVL_Node *pPosition){
            position = pPosition;
        }

        AVL_Node* getPosition(){
            return position;
        }

        
};

#endif