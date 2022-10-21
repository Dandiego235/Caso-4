#ifndef SUBCAMARA

#define SUBCAMARA 1

#include <mutex>

using namespace std;

class SubCamara{
    private:
        SubCamara *left;
        SubCamara *right;
        int height;
        SubCamara *parent;

        int distanciaTotal;
        int potencial;
        int distancia;
        int mineral; // cantidad de mineral al inicio de la generación de las cámaras.
        int mineralAzul;
        int mineralRojo;
        mutex *mtx;

    public:
        SubCamara(SubCamara* par, int pDistancia, int pMineral){
            left = nullptr;
            right = nullptr;
            height = 1;
            parent = par;
            distancia = pDistancia;
            mineral = pMineral;
            mineralAzul = pMineral;
            mineralRojo = pMineral;
            potencial = pMineral * pDistancia;
            if (par){
                distanciaTotal = par->getDistanciaTotal() + distancia;
            } else {
                distanciaTotal = 0;
            }
            bool empty = false;
            mtx = new mutex();
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

        int decMineralAzul(int number){
            mtx->lock();
            if (number > mineralAzul){
                number = mineralAzul;
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

        int decMineralRojo(int number){
            mtx->lock();
            if (number > mineralRojo){
                number = mineralRojo;
            }
            mineralRojo -= number;
            mtx->unlock();
            return number;
        }

        bool isEmptyRojo(){
            return !mineralRojo;
        }

        int getHeight(){
            return height;
        }

        void setHeight(int pHeight){
            height = pHeight;
        }

        int getDistanciaTotal(){
            if (parent == nullptr){ // si es la raiz
                return distancia;
            }
            return distancia + parent->getDistanciaTotal();
        }

        void setDistanciaTotal(int pDistanciaTotal){
            distanciaTotal = pDistanciaTotal;
        }

        void setRight(SubCamara *pNode){
            right = pNode;
        }

        SubCamara* getRight(){
            return right;
        }

        void setLeft(SubCamara *pNode){
            left = pNode;
        }

        SubCamara* getLeft(){
            return left;
        }

        void setParent(SubCamara *pNode){
            parent = pNode;
        }

        SubCamara* getParent(){
            return parent;
        }
};

#endif