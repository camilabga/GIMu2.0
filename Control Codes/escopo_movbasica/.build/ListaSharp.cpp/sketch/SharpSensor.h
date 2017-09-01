#ifndef SharpSensor_h
#define SharpSensor_h

#include "Arduino.h"

class SharpSensor{
    private:
        int porta;
        int distancia;

        void getSharp();
        

    public:
        SharpSensor();
        SharpSensor(int porta);
        inline int getDistancia(){getSharp(); return distancia;}

        inline setPorta(int porta){this->porta = porta;}

        void operator=(const SharpSensor &SS);
};

#endif