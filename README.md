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

### 2. Apertura de operaciones pendientes  
Cada vez que se lea un operador binario, se incrementa la cantidad de operandos pendientes por completar. Esto indica que la expresión está abriendo una nueva subestructura.

### 3. Cierre de operandos pendientes  
Cada operando leído debe disminuir la cantidad de operandos que aún deben procesarse, representando el avance en la construcción de la expresión.

### 4. Reducción inmediata de subexpresiones  
Cuando una operación obtiene todos sus operandos necesarios, debe considerarse “resuelta”.  
Si esta resolución permite finalizar otras operaciones, deben cerrarse de inmediato y de manera sucesiva, sin requerir más símbolos de entrada.

### 5. Continuación flexible  
Después de reducir una operación, la expresión puede seguir creciendo libremente.  
Pueden aparecer operadores que abran subexpresiones adicionales o aparecer operandos que completen estructuras existentes.

### 6. Expresión mínima válida  
Una expresión que consista únicamente en un operando simple debe considerarse válida.

### 7. Criterio de aceptación  
La expresión completa solo es válida si:
- Se han consumido todos los símbolos de entrada.  
- No quedan operaciones pendientes por completar.

### 8. Criterio de rechazo  
La expresión debe rechazarse si ocurre alguno de estos casos:
- Existen más operandos de los necesarios.  
- Falta al menos un operando para completar una operación.  
- La expresión se prolonga tras haber sido completamente resuelta.  
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
  <li>
    <strong>isEmpty (Está Vacía):</strong> Verifica si la pila no contiene
    elementos.
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
    char simbolo;      // Dato a almacenar (ej. 'Z', 'A', 'B'...)
    Nodo* siguiente; // Puntero al nodo que está "debajo" en la pila
};
</code></pre>

<p>
  El autómata, por lo tanto, no maneja un arreglo, sino que simplemente
  mantiene un puntero al nodo <code>tope</code> (el nodo superior). Las
  operaciones <code>push</code> y <code>pop</code> se encargan de crear nuevos
  nodos (con <code>new Nodo()</code>) o eliminarlos (con <code>delete</code>),
  gestionando los punteros <code>siguiente</code> para mantener la integridad de
  la pila.
</p>
<p>
  De esta manera, el <code>struct</code> actúa como el "ladrillo" fundamental
  para construir la memoria LIFO que el autómata de pila necesita para
  funcionar.
</p>

## Autómata de referencia

Este es el autómata (por ahora) que usaremos para crear un validador de operaciones en prefijo. Desarrollado en jflap, sigue los requerimientos anteriormente indicados

/image/diagrama.png

