#ifndef PERSONAJE

#define PERSONAJE 1

class Personaje{
    protected:
        string name;
        int speed;
        int capacity;
        bool jugador;
        Puerta *puerta;
        Camara *camara;
    public:
        virtual void play(thread *pThread) = 0;
};

#endif