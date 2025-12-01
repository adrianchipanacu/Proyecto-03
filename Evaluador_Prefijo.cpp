#include<iostream>
#include<sstream>
#include<fstream>

using namespace std;

// Creamos un struct que será el nodo de nuestra pila
struct Nodo {
    double var;     // Almacena los operandos
    Nodo *sig;      // Apunta al siguiente nodo, necesario para unirlos

    // Este es el constructor del nodo, lo usaremos para apilar nuevos elementos en la pila
    Nodo(double val){
        var = val;
        sig = nullptr;
    }
};

// Tomando como base el struct Nodo, creamos la Pila que apuntará a la cabeza de nuestra pila, así podremos apilar o desapilar elementos
struct Pila {
    Nodo *puntero;
    int tam;

    // Su constructo, donde también definimos su tamaño inicial
    Pila() {
        puntero = nullptr;
        tam = 0;
    }

    // La función apilar, donde crearemos un nuevo nodo para cada nuevo elemento
    void apilar(double val) {
        
        Nodo *nuevo = new Nodo(val);
        nuevo->sig = puntero;
        puntero = nuevo;
        tam++;
    }

    // La función desapilar ajusta el puntero principal de la pila y elimina último nodo
    void desapilar() {
        if(puntero == nullptr) {
            cout<<"\nLa pila ya está vacía.\n";
            return;
        }
        Nodo *temp = puntero;
        puntero = puntero->sig;
        delete temp;
        tam--;
    }

    // Obtenemos el últimor valor apilado. Si no hay, salimos del programa, faltan operandos.
    double verTope() {
        if (puntero == nullptr) {
            exit(1);
        }
        return puntero->var;  
    }

    string verPila(){
        string lista;
        if(tam == 0){
            return "";
        }
        for(int i = 0; i < tam; i++) {
            lista += "X";
        }
        lista += "Z";
        return lista;
    }

};

int main() {

    Pila  *pila = new Pila();
    string token;             // String para ingresar la cadena, divida por espacios
    string cadena[100];         // Donde registraremos cada elemento de la cadena
    int indice = -1;             

    ifstream entrada("documento.txt");
    ofstream pasos("resultado.txt", ios::out | ios::trunc);

    do {
        indice++;
        getline(entrada, cadena[indice]);
        if(indice == 100) {
            pasos<<"Cadena sobrepadasa, inserte una pila más corta.";
            return 1;
        }
    } while (cadena[indice].length() > 0);


    // El bucle lee de derecha a izquierda, como se requiere para el orden prefijo
    for(int i = indice - 1; i >= 0; i--) {

        double num;             
        token = cadena[i];

        // Comprobamos que sea un operando y si lo es lo apilamos
        if(isdigit(token[0])) {
            pila->apilar(stod(token));

            if(i == indice - 1) {
                pasos<<token<<"\tZ\tq0\t\t"<<"q1\tXZ\t"<<pila->verPila();
            }
            else {
                pasos<<token<<"\tX\tq1\t\t"<<"q1\tXX\t"<<pila->verPila();
            }
            pila->verPila();
            pasos<<endl;
        }
        // Si no es un operando, analizamos que operador es. En cada caso se aplica la operación y se inserta en la pila
        else {
            double operando1 = pila->verTope();
            pila->desapilar();
            double operando2 = pila->verTope();
            pila->desapilar();

            if(token == "+") {
                num = operando1 + operando2;
                pila->apilar(num);
            }
            else if(token == "-") {
                num = operando1 - operando2;
                pila->apilar(num);
            }
            else if(token == "/") {
                if(operando2 == 0) {
                    cout<<"Operanción inválida, no se puede dividir entre 0. El resultado es indefinido.";
                    exit(1);
                }
                num = operando1 / operando2;
                pila->apilar(num);
            }else if(token == "*") {
                num = operando1 * operando2;
                pila->apilar(num);
            }
            // Si el simbolo no está en el alfabeto, la cadena se invalida
            else {
                cout<<"Simbolo no válido. La cadena no es aceptada.";
                return 1;
            }
            pasos<<token<<"\tXX\tq1\t\t"<<"q1\tX\t"<<pila->verPila();
            pila->verPila();
            pasos<<endl;
        }
    }
    // Evaluamos casos de la pila vacía o si sobran operandos
    if(pila->tam == 0) {
        cout<<"Cadena vacía. No se ha procesado ninguna operación.";
    }
    else if(pila->tam > 1) {
        cout<<"Sobrando operandos. La cadena no es válida";
    }
    // Si no hay errores, imprimimos el resultado
    else {
        pasos<<"λ\tXZ\tq1\t\t"<<"q2\tZ\tZ"<<endl;
        pasos<<"λ\tZ\tq2\t\t"<<"q2\tλ\tλ"<<endl;
        pasos<<pila->verTope();
        pila->desapilar();
    }
    return 0;
}
