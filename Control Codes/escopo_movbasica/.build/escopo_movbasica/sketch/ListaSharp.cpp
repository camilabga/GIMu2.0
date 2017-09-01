#include "ListaSharp.h"

ListaSharp::ListaSharp(){
    sharps = NULL;
    n = 0;
}

ListaSharp::ListaSharp(unsigned num){
    if (num != 0) {
        n = num;
        sharps = new SharpSensor[n];
    } else {
        sharps = NULL;
        n = 0;
    }
}

ListaSharp::ListaSharp(unsigned tam, int portas[]){
    n = tam;
    sharps = new SharpSensor[n];

    for(unsigned i = 0; i < n; i++){
        sharps[i].setPorta(portas[i]);
    }
}

void ListaSharp::operator=(const ListaSharp &LS){
    n = LS.n;
    for (unsigned i = 0; i < n; i++) {
        sharps[i] = LS.sharps[i];
    }
}