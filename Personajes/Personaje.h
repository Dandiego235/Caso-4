#ifndef PERSONAJE

#define PERSONAJE 1

#include "../Puerta.cpp"
#include "../Camara.cpp"
#include "../Estrategias/Estrategia.h"
#include <thread>

class Estrategia;

class Personaje{
    protected:
        string name;
        float speed;
        int capacity;
        bool jugador; // false para azul y true para rojo.
        Puerta *puerta;
        Camara *camara;
        SubCamara *subcamara;
        SubCamara *raiz;
        SubCamara *subcamaraPtr;
        int mineralAcumulado;

    public:
        virtual void play(thread *pThread, Estrategia *pEstrategia) = 0;

        string getName(){
            return name;
        }

        void setName(string pName){
            name = pName;
        }

        void setPuerta(Puerta *pPuerta){
            puerta = pPuerta;
        }

        Puerta* getPuerta(){
            return puerta;
        }

        float getSpeed(){
            return speed;
        }

        void setSpeed(float pSpeed){
            speed = pSpeed;
        }

        int getCapacity(){
            return capacity;
        }

        void setCapacity(int pCapacity){
            capacity = pCapacity;
        }

        bool getJugador(){
            return jugador;
        }

        void setJugador(bool pJug){
            jugador = pJug;
        }

        Camara* getCamara(){
            return camara;
        }

        void setCamara(Camara* pCamara){
            camara = pCamara;
            setRaiz(dynamic_cast<SubCamara*>(camara->getRaiz()->getData()));
            setSubCamara(raiz);
        }

        SubCamara* getSubCamara(){
            return subcamara;
        }

        void setSubCamara(SubCamara* pSubCamara){
            subcamara = pSubCamara;
        }

        // función que retorna la cantidad de mineral en una subcámara dependiendo del color del jugador
        int readMineral(SubCamara *pSubCamara){
            if (pSubCamara == nullptr){
                return -1;
            }
            if (jugador){ // si es rojo.
                return pSubCamara->getMineralRojo();
            } else {
                return pSubCamara->getMineralAzul();
            }
        }

        // función que coge mineral de una subcámara dependiendo del color del jugador
        int takeMineral(SubCamara *pSubCamara, int number, int total, int capacity){
            if (jugador){ // si es rojo.
                return pSubCamara->decMineralRojo(number, total, capacity);
            } else {
                return pSubCamara->decMineralAzul(number, total, capacity);
            }
        }

        int leaveOne(SubCamara* pSubCamara, int pMineralRecogido){
            int mineralAgarrado;
            if (readMineral(pSubCamara) <= capacity){ // si la cantidad es menor o igual al capacity.
                mineralAgarrado = takeMineral(pSubCamara, readMineral(pSubCamara)-1, mineralRecogido, capacity);
                // agarramos los suficientes para que quede en 1.
            } else {
                mineralAgarrado = takeMineral(pSubCamara, capacity, pMineralRecogido, capacity);
                // agaramos todos los que se puede.
            }
            return mineralAgarrado;
        }

        SubCamara* getRaiz(){
            return raiz;
        }

        void setRaiz(SubCamara* pRaiz){
            raiz = pRaiz;
        }

        int getMineralAcumulado(){
            return mineralAcumulado;
        }

        void setMineralAcumulado(int pMineral){
            mineralAcumulado = pMineral;
        }

        void incMineralAcumulado(int number){
            mineralAcumulado += number;
        }

};

#endif