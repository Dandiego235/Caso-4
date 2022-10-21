#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>

#ifndef GAME_TIMER

#define GAME_TIMER 1

using namespace std::chrono;
using namespace std;

class GameTimer{
    private:
        system_clock::time_point startTime;
        duration<float> deltaTime; // diferencia de tiempo desde el tiempo de inicio.
        float endTime;

    public:
        GameTimer(float pEndTime){
            deltaTime = duration<float>(0.0f);
            endTime = pEndTime;
        }

        void start(){
            startTime = system_clock::now(); // obtenemos la hora actual.
            while(deltaTime.count() < endTime){
                tick();
                if (round(endTime - deltaTime.count()) <= 10){
                    cout << round(endTime - deltaTime.count()) << endl;
                }
                this_thread::sleep_for(seconds(1));
            }
        }

        float getDeltaTime(){
            return deltaTime.count();
        }

        void tick(){
            deltaTime = system_clock::now() - startTime;
        }

};

#endif