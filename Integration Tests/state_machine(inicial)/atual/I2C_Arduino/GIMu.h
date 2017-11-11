#ifndef GIMu_h
#define GIMu_h

#include "Arduino.h"
#include "Motor.h"
#include "SharpIR.h"
#include "Pins.cpp"
#include "variables.cpp"
#include "BracoCopo.h"
#include "Elevador.h"

class GIMu{
  public:
    GIMu(Motor d, Motor e);
    GIMu(BracoCopo b);
    GIMu(Motor d, Motor e, BracoCopo b);
    GIMu(Motor d, Motor e, BracoCopo b, Elevador l);
    GIMu(Elevador e);
    GIMu(Motor d, Motor e, BracoCopo b, Elevador l, Motor s);    

    void moveFrente(int velocidade);
    void moveTras(int velocidade);
    void moveTank(int pwm_esquerdo, int pwm_direito);
    inline void stop(){moveFrente(0);}
    void taxearDireita();
    void taxearEsquerda();

    void taxearDireitaRe();

    void getSharps();
    inline int getDistancia(int i){return sharpsBase[i];}
    int getSharp(int porta);
    inline int getMSharp(){return analogRead(MSH_ORDENHADOR);}
    
    void follow_wall_to_terrine_areaE();
    void follow_wall_to_terrine_areaD();
    void adjust_to_get_cupE();
    void adjust_to_get_cupD();
    void follow_wall_to_little_gate();

    void getTerrineE();
    void getTerrineD();

    void ordenhar01();
    void ordenhar02();
    void ordenhar03();
    void ordenhar04();

    void adjust_to_derramar_leite();

    void dropMilk();
    void milkTeta();

    int sharpsBase[6];
    

  private:
    Motor Mright;
    Motor Mleft;
    Motor sucker;
    BracoCopo bracoCopo;
    Elevador elevador;
    Servo SM_Ordenhador;

    
};

#endif
