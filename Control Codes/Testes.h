#ifdef TESTES_H
#define TESTES_H

#include "Arduino.h"
#include "GIMu.h"

class Testes {
public:
    Testes();
    void checkMovimentacao();
    void checkSensors();
    void testElevator();
    void testBraco();
    void testOrdenha();

    void seguir_parede();
    void ajustar_pegar_copo();
    void pegar_copo();
    void ordenhar();

private:
    bool ok;
};

#endif