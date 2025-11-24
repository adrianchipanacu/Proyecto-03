# Evaluador y Validador Prefijo con Autómata de Pila (AP)
<section>
  <h2>Integrantes</h2>
  <ul>
    <li>Cabello Villon, Denis Emilio Enrique</li>
    <li>Condo Tiquillahuanca, Jean Phool</li>
    <li>Quispe Torres, Carlos Luis</li>
    <li>Chipana Cueva Juan Adrian</li>
  </ul>
</section>
<section>
  <h2>Introducción</h2>
  <p>
    La Notación Polaca (prefija) es una forma de escribir expresiones en la cual el operador precede a sus operandos.
    A diferencia de la notación infija común, la notación prefija permite eliminar la ambigüedad de precedencia sin paréntesis
    si se procesa en el orden correcto. Para evaluar expresiones prefijas mediante una pila, es necesario recorrer la lista de
    tokens de <em>derecha a izquierda</em>: los operandos se apilan y, al encontrar un operador binario, se desapilan dos valores
    para aplicar la operación.
  </p>

  <p>Este proyecto implementa:</p>
  <ul>
    <li>Un simulador de Autómata de Pila (AP) que procesa tokens clasificados (<code>NUMERO_ENTERO</code>, <code>NUMERO_DECIMAL</code>, <code>+</code>, <code>-</code>, <code>*</code>, <code>/</code>).</li>
    <li>Validación sintáctica: detecta errores por operandos insuficientes y verifica que al finalizar la evaluación la pila contenga exactamente un valor.</li>
    <li>Manejo de errores aritméticos, como la división por cero.</li>
    <li>Soporte para números enteros y decimales, y para las cuatro operaciones binarias básicas.</li>
  </ul>

  <p>
    El objetivo final es tanto calcular el resultado numérico correcto como garantizar la correcta aceptación o rechazo de
    secuencias de tokens conforme a las reglas formales del AP.
  </p>
</section>


## Objetivos del Autómata

- Validar expresiones formadas por operadores binarios y operandos simples en notación prefija.  
- Garantizar que cada operador cuente con exactamente dos operandos, directa o recursivamente.  
- Simular la reducción estructural propia de un árbol sintáctico, resolviendo subexpresiones a medida que se completan.  
- Aceptar únicamente expresiones completamente estructuradas y sin operaciones pendientes.  
- Proveer un modelo verificable en JFLAP para fines didácticos, experimentales o de análisis formal.


## Características del Autómata (Requerimientos)

A continuación se presentan los requerimientos del autómata expresados como **secuencia lógica de pasos**, sin aludir a estados ni a detalles internos de implementación.

### 1. Interpretación inicial  
El autómata debe aceptar que una expresión pueda iniciar con un operador o con un operando, según la forma estándar de la notación prefija.

### 2. Apertura de operandos pendientes  
Cada operando leído debe apilar un número que aún debe procesarse, indicando que se crea una nueva instacia dentro de la pila.

### 3. Cierre de operaciones pendientes
Cada vez que se lea un operador binario, se disminuye los númeos pendientes por procesar. Lo que representa un avance en la construcción de la expresión.

### 5. Continuación flexible  
Después de reducir un operando, la pila puede seguir creciendo libremente.  
Pueden aparecer operadores que sigan consumiendo la pila o aparecer operandos que que apilen elementos que luego serán desapilados.

### 6. Expresión mínima válida  
Una expresión que consista únicamente en un operando simple debe considerarse válida.

### 7. Criterio de aceptación  
La expresión completa solo es válida si:
- Se han procesado todos los símbolos de entrada.  
- No quedan operaciones pendientes por completar.

### 8. Criterio de rechazo  
La expresión debe rechazarse si ocurre alguno de estos casos:
- Existen más operandos de los necesarios.  
- Falta al menos un operando para completar una operación.  
- La cadena termina con operaciones incompletas.

<h2>Conceptos importantes</h2>
<p>
  Es necesario dejar en claro algunos aspectos en relación a como se desarrolla el autómata
  buscado, en este caso necesitamos explicar las estructuras básicas que utilizaremos dentro 
  del lenguaje c++ para construir nuestro autómata.
</p>

<hr />

<h3>Pilas (Stacks)</h3>
<p>
  Una <strong>pila</strong> (o <i>stack</i>) es una estructura de datos
  lineal que opera bajo el principio <strong>LIFO</strong> (Last-In,
  First-Out), que se traduce como "Último en Entrar, Primero en Salir".
</p>
<p>
  Funciona de manera análoga a una pila de platos: solo se puede interactuar con
  el elemento que se encuentra en el "tope" (la parte superior) de la pila.
</p>

<h4>Operaciones Fundamentales:</h4>
<ul>
  <li>
    <strong>Push (Apilar):</strong> Añade un nuevo elemento al tope de la pila.
  </li>
  <li>
    <strong>Pop (Desapilar):</strong> Elimina el elemento que se encuentra en
    el tope de la pila.
  </li>
  <li>
    <strong>Top (Cima o Tope):</strong> Consulta el elemento del tope sin
    retirarlo.
  </li>
</ul>

<hr />


<h3>Representación de una pila con <code>struct</code> en C++</h3>
<p>
  En C++, una <code>struct</code> es un tipo de dato definido por el usuario que
  permite agrupar variables de diferentes tipos bajo un mismo nombre. Es una
  herramienta fundamental para crear estructuras de datos complejas.
</p>

<h4>Implementación de la Pila</h4>
<p>
  Para implementar la pila que usará el autómata, comúnmente se utiliza una
  <strong>lista enlazada</strong>. La <code>struct</code> es la base para
  construir esta lista, definiendo la estructura de un <strong>nodo</strong>.
</p>
<p>
  Cada nodo de la pila debe contener dos cosas: el dato que almacena (un símbolo
  de la pila) y un puntero al siguiente nodo.
</p>

<pre><code>
// Definición de la estructura de un Nodo para la pila
struct Nodo {
    double var;      // Dato a almacenar (Número a operar)
    Nodo *sig; // Puntero al nodo que está "debajo" en la pila
};
</code></pre>

<p>
  Tomando como base el struct de Nodo, podemos crear nuestra estructura de Pila 
  con los métodos necesarios para procesar la cadena de símbolos.
</p>

<pre><code>
struct Pila {
    Nodo *puntero;  // El puntero que nos ayudará a apilar y desapilar
    int tam;        // El tamaño útil para comprobar si la pila está vacía

    Pila() {
        puntero = nullptr;
        tam = 0;
    }

    void apilar(double val) {
        Nodo *nuevo = new Nodo(val);
        nuevo->sig = puntero;
        puntero = nuevo;
        cout<<"Apilamos el operando "<<val<<endl<<endl;
        tam++;
    }

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

    double verTope() {
        if (puntero == nullptr) {
            cout << "Faltan operandos. La cadena no es aceptada.\n";
            exit(1);
        }
        return puntero->var;  
    }
};
</code></pre>

<p>
  Usando los conceptos mencionados, podemos crear un algoritmo
  que simule un autómata dentro de C++, donde cada operando ocupa
  un espacio en la pila. De forma que se deben apilar partiendo 
  del final de la cadena y desapilar cuando un operador los necesite.
</p>

<p>
  El autómata, por lo tanto, manejará un arreglo que será analizado
  desde el final de la cadena, es decir, de derecha a izquierda. Este
  es el criterio que se requiere para procesar expresiones en notación
  polaca.
</p>

<h4>Autómata de referencia</h4>
<p>
  A continuación, se muestra un autómata que representa la misma 
  lógica que buscamos construir, pero debido a las limitaciones del programa
  Jflap, se entiende que la cadena ha sido volteada antes de ser
  ingresas en el autómata. Dentro del código en C++ la cadena se leerá de
  derecha a izquierda.
</p>

![image alt](https://github.com/adrianchipanacu/Proyecto-03/blob/78dfda91ef895266d8b34ca4345c6b5fd75baab0/Aut%C3%B3mata_Referencia.png)

<h4>Implementación de la estructura y algoritmo de Autómata dentro de C++</h4>

<p>
  Tratando de simular en anterior autómata, el siguiente código lee un string
  que separa sus tokens por espacios y convierte los operandos válidos a en datos
  tipo Dobule. Apila cada uno de los operandos en una pila dinámica y si se llega 
  a leer algún operando desapila 2 valores y evalúa cada caso de los 4 
  operadores binarios aceptados.
</p>

<pre><code>
#include<iostream>
#include<sstream>
using namespace std;

struct Nodo {
    double var;
    Nodo *sig;
    Nodo(double val){ var = val; sig = nullptr; }
};

struct Pila {
    Nodo *puntero;
    int tam;
    Pila(){ puntero = nullptr; tam = 0; }

    void apilar(double val){
        Nodo *nuevo = new Nodo(val);
        nuevo->sig = puntero;
        puntero = nuevo;
        cout<<"Apilamos el operando "<<val<<endl<<endl;
        tam++;
    }

    void desapilar(){
        if(!puntero){ cout<<"\nLa pila ya está vacía.\n"; return; }
        Nodo *temp = puntero;
        double valor = puntero->var;
        puntero = puntero->sig;
        delete temp;
        cout<<"Desapilamos el valor "<<valor<<endl<<endl;
        tam--;
    }

    double verTope(){
        if(!puntero){ cout<<"Faltan operandos. La cadena no es aceptada.\n"; exit(1); }
        return puntero->var;
    }
};

int main(){
    Pila *pila = new Pila();
    string entrada, cadena[100];
    int indice = 0;

    cout<<"------------------------------------------------------------\n";
    cout<<"-------- Evaluador y validador Prefijo con Autómata --------\n";
    cout<<"------------------------------------------------------------\n\n";

    cout<<"Ingrese la cadena a evaluar, separe los operandos y operadores por espacios: \n";
    getline(cin, entrada);
    cout<<endl<<endl;

    stringstream elementos(entrada);
    while(elementos >> cadena[indice] && indice < 100) indice++;

    for(int i = indice - 1; i >= 0; i--){
        double num;
        entrada = cadena[i];

        if(isdigit(entrada[0])){
            pila->apilar(stod(entrada));
        } else {
            double operando1 = pila->verTope(); pila->desapilar();
            double operando2 = pila->verTope(); pila->desapilar();

            if(entrada == "+") num = operando1 + operando2;
            else if(entrada == "-") num = operando1 - operando2;
            else if(entrada == "/"){
                if(operando2 == 0){ cout<<"Operanción inválida, no se puede dividir entre 0. El resultado es indefinido."; exit(1); }
                num = operando1 / operando2;
            }
            else if(entrada == "*") num = operando1 * operando2;
            else { cout<<"Simbolo no válido. La cadena no es aceptada."; return 1; }

            pila->apilar(num);
        }
    }

    if(pila->tam == 0) cout<<"Cadena vacía. No se ha procesado ninguna opración.";
    else if(pila->tam > 1) cout<<"Sobrando operandos. La cadena no es válida";
    else {
        cout<<"La cadena es aceptada.\n";
        cout<<"El resultado de la pila es: "<<pila->verTope();
    }
    return 0;
}
</code></pre>






