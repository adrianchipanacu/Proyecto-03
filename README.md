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

<hr />
<h3>Implementación de la estructura y algoritmo de Autómata dentro de C++</h3>

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
</code></pre>

<hr />
<h2>📘 Guía de Uso</h2>

<h3>🔧 Pasos para la compilación y ejecución correcta (Linux y Windows)</h3>

<h4>1. Verificar instalación del compilador C++</h4>
<ul>
  <li><strong>Linux:</strong> Puede instalarse con:
    <pre><code>sudo apt install g++</code></pre>
  </li>
  <li><strong>Windows:</strong> Instalar MinGW-w64 o MSYS2. Luego verificar con:
    <pre><code>g++ --version</code></pre>
  </li>
</ul>

<h4>2. Crear el archivo <code>.cpp</code></h4>

<p><strong>Linux:</strong></p>
<pre><code>nano [nombre_archivo].cpp</code></pre>

<p><strong>Windows:</strong></p>
<pre><code>notepad [nombre_archivo].cpp</code></pre>

<p>Pegar el código del programa y guardar. También puede usar editores de código como VSCode y guardar los cambios.</p>

<h4>3. Compilar el programa</h4>

<p><strong>Linux:</strong></p>
<pre><code>g++ [nombre_archivo].cpp -o [nombre_archivo]</code></pre>

<p><strong>Windows:</strong></p>
<pre><code>g++ [nombre_archivo].cpp -o [nombre_archivo].exe</code></pre>

<h4>4. Crear el archivo de entrada <code>.txt</code></h4>

<p><strong>Linux:</strong></p>
<pre><code>nano documento.txt</code></pre>

<p><strong>Windows:</strong></p>
<pre><code>notepad documento.txt</code></pre>

<p>Escribir los operadores y operandos separados por saltos de línea. Ejemplo:</p>

<pre><code>+
-
5
3
1
</code></pre>

<h4>5. Ejecutar el programa</h4>

<p><strong>Linux:</strong></p>
<pre><code>./[nombre_archivo]</code></pre>

<p><strong>Windows:</strong></p>
<pre><code>.\[nombre_archivo].exe</code></pre>

<h4>6. Verificar resultados</h4>

<p>Se generará automáticamente un archivo <code>resultado.txt</code> con:</p>
<ul>
  <li>Las transiciones realizadas por el autómata</li>
  <li>El resultado de la operación</li>
</ul>
<p>
  El formato del resultado impreso en "resultado.txt" tiene un formato que se compone de 6 columnas.
  Las leemos de derecas a izquierda y explicamos que signifca cada una:
</p>
<ul>
  <li>1. Simbolo leído de la cadena</li>
  <li>2. Tope de pila actual, se desapila. Pueden ir uno o más símbolos.</li>
  <li>3. Estado actual del autómata.</li>
  <li>4. Nuevo estado actual.</li>
  <li>5. Simbolos apilados antes de terminar la transición.</li>
  <li>6. Vista general de la pila tras cada transición. Los últimos símbolos apilados están a la izquierda</li>
</ul>
<p>
  Cabe aclarar que los símbolos usados para la Pila representativa son la "Z" como el fondo y la "X" para cada número
  entero o decimal.
</p>


<hr />
<h3>Casos de Prueba</h3>
<p>
  Probaremos una 3 casos, uno correcto y 2 incorrectos.
  
  CASO CORRECTO:
  + * 5 6 3
</p>

<pre><code>
  3	Z	q0		q1	XZ	XZ
  6	X	q1		q1	XX	XXZ
  5	X	q1		q1	XX	XXXZ
  *	XX	q1		q1	X	XXZ
  +	XX	q1		q1	X	XZ
  λ	XZ	q1		q2	Z	Z
  λ	Z	q2		q2	λ	λ
  33
</code></pre>

<p>
  Pila vacía, cadena aceptada y resultado mostrado.
  
  
  CASO INCORRECTO (FALTA OPERANDOS):
  + 5
</p>
<pre><code>
  5	Z	q0		q1	XZ	XZ
</code></pre>
<p>
  Pila no vacía, cadena NO aceptada y NO se muestra resultado.

  
  CASO INCORRECTO (SOBRAN OPERANDOS):
  + 5 5 5
</p>
<pre><code>
  5	Z	q0		q1	XZ	XZ
  5	X	q1		q1	XX	XXZ
  5	X	q1		q1	XX	XXXZ
  +	XX	q1		q1	X	XXZ
</code></pre>
<p>
  Pila no vacía, cadena NO aceptada y NO se muestra resultado.
</p>

<hr />
<h3>Conclusiones</h3>

<p>
  Eficacia de los Autómatas de Pila en el Parsing: Se demostró que los Autómatas de Pila (PDA) son la herramienta computacional idónea para validar Gramáticas Libres de Contexto (GLC), como las expresiones matemáticas. A diferencia de un Autómata Finito Determinista (DFA) simple que no tiene memoria, el PDA utiliza la pila para "recordar" operandos pendientes, asegurando que cada operador tenga sus dos argumentos correspondientes antes de proceder. 

Ventajas de la Notación Prefija: La implementación evidenció por qué la notación prefija (y postfija) es preferida en computación sobre la notación infija tradicional. Al eliminar la necesidad de paréntesis y reglas de precedencia complejas, el algoritmo de evaluación se simplifica linealmente, reduciendo la complejidad computacional y el uso de memoria. 

Importancia de las Estructuras de Datos Dinámicas: La construcción manual de la estructura Pila (utilizando struct y punteros) permitió un control total sobre la gestión de memoria. Esto valida la importancia de comprender las estructuras de datos fundamentales para manipular flujos d
</p>




