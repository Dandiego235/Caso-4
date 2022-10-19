#ifndef PERSONAJE

#define PERSONAJE 1

#include <thread>
#include "../Estrategia.h"
#include "../Puerta.cpp"
#include "../Camara.cpp"

class Personaje{
    protected:
        string name;
        int speed;
        int capacity;
        bool jugador;
        Puerta *puerta;
        Camara *camara;
        SubCamara *raiz;
        SubCamara *subcamaraPtr;
        int mineralAcumulado;

    public:
        virtual void play(thread *pThread, Estrategia *pEstrategia) = 0;

        String getName(){
            return name;
        }

        void setName(string pName){
            name = pName;
        }

        void setPuerta(Puerta pPuerta){
            puerta = pPuerta;
        }

        Puerta* getPuerta(){
            return puerta;
        }

        int getSpeed(){
            return speed;
        }

        void setSpeed(int pSpeed){
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
            setRaiz(camara->getRaiz());
            setSubCamara(raiz);
        }

        SubCamara* getSubCamara(){
            return subcamara;
        }

        void setSubCamara(SubCamara* pSubCamara){
            subcamara = pSubCamara;
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