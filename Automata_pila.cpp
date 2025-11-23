#include<iostream>
#include<sstream>

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
        cout<<"Apilamos el operando "<<val<<endl<<endl;
        tam++;
    }

    // La función desapilar ajusta el puntero principal de la pila y elimina último nodo
    void desapilar() {
        if(puntero == nullptr) {
            cout<<"\nLa pila ya está vacía.\n";
            return;
        }
        Nodo *temp = puntero;
        double valor = puntero->var;
        puntero = puntero->sig;
        delete temp;
        cout<<"Desapilamos el valor "<<valor<<endl<<endl;
        tam--;
    }

    // Obtenemos el últimor valor apilado. Si no hay, salimos del programa, faltan operandos.
    double verTope() {
        if (puntero == nullptr) {
            cout << "Faltan operandos. La cadena no es aceptada.\n";
            exit(1);
        }
        return puntero->var;  
    }

};

int main() {

    Pila  *pila = new Pila();
    string entrada;             // String para ingresar la cadena, divida por espacios
    string cadena[100];         // Donde registraremos cada elemento de la cadena
    int indice = 0;             

    cout<<"------------------------------------------------------------\n";
    cout<<"-------- Evaluador y validador Prefijo con Autómata --------\n";
    cout<<"------------------------------------------------------------\n\n";

    cout<<"Ingrese la cadena a evaluar, separe los operandos y operadores por espacios: \n";
    getline(cin, entrada);
    cout<<endl<<endl;

    // Separamos los simbolos de la cadena
    stringstream elementos(entrada);

    // Los insertamos en un array para analizar simbolo
    while (elementos >> cadena[indice] && indice < 100) {
        indice++;
    }

    // El bucle lee de derecha a izquierda, como se requiere para el orden prefijo
    for(int i = indice - 1; i >= 0; i--) {

        double num;             
        entrada = cadena[i];

        // Comprobamos que sea un operando y si lo es lo apilamos
        if(isdigit(entrada[0])) {
            pila->apilar(stod(entrada));
        }
        // Si no es un operando, analizamos que operador es. En cada caso se aplica la operación y se inserta en la pila
        else {
            double operando1 = pila->verTope();
            pila->desapilar();
            double operando2 = pila->verTope();
            pila->desapilar();

            if(entrada == "+") {
                num = operando1 + operando2;
                pila->apilar(num);
            }
            else if(entrada == "-") {
                num = operando1 - operando2;
                pila->apilar(num);
            }
            else if(entrada == "/") {
                if(operando2 == 0) {
                    cout<<"Operanción inválida, no se puede dividir entre 0. El resultado es indefinido.";
                    exit(1);
                }
                num = operando1 / operando2;
                pila->apilar(num);
            }else if(entrada == "*") {
                num = operando1 * operando2;
                pila->apilar(num);
            }
            // Si el simbolo no está en el alfabeto, la cadena se invalida
            else {
                cout<<"Simbolo no válido. La cadena no es aceptada.";
                return 1;
            }
        }
    }
    // Evaluamos casos de la pila vacía o si sobran operandos
    if(pila->tam == 0) {
        cout<<"Cadena vacía. No se ha procesado ninguna opración.";
    }
    else if(pila->tam > 1) {
        cout<<"Sobrando operandos. La cadena no es válida";
    }
    // Si no hay errores, imprimimos el resultado
    else {
        cout<<"La cadena es aceptada.\n";
        cout<<"El resultado de la pila es: "<<pila->verTope();
    }
    return 0;
}
