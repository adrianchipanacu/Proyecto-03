#include<iostream>
#include<vector>
#include<string.h>

using namespace std;

struct Nodo {
    char var;
    Nodo *sig;

    Nodo(int val){
        var = val;
        sig = nullptr;
    }
};

struct Pila {
    Nodo *puntero;
    int tam;

    Pila() {
        puntero = nullptr;
        tam = 0;
    }

    void apilar(char val) {
        
        Nodo *nuevo = new Nodo(val);
        nuevo->sig = puntero;
        puntero = nuevo;
        
    }

    void desapilar(int &valor) {
        Nodo *temp = puntero;
        valor = puntero->var;
        puntero = puntero->sig;

        delete temp;
    }

};

struct Transicion {
    int estadoActual;
    char entrada;
    string retirar;

    int estadoSiguiente;
    string insertar;

    Transicion(int Ea, char En, string Re, int Es, string In) {
        estadoActual = Ea;
        entrada = En;
        retirar = Re;
        estadoSiguiente = Es;
        insertar = In;
    }
};


int main() {



    return 0;
}