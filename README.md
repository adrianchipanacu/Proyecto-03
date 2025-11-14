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

<h2>Marco Teórico: Pilas, Autómatas de Pila y C++</h2>
<p>
  Este documento describe los conceptos fundamentales de las estructuras de datos de
  tipo Pila, su aplicación en los Autómatas de Pila y cómo pueden ser
  representados en C++ usando <code>struct</code>.
</p>

<hr />

<h3>1. Pilas (Stacks)</h3>
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

<h3>2. Autómatas de Pila (Pushdown Automata)</h3>
<p>
  Un <strong>autómata de pila</strong> (PDA, por sus siglas en inglés) es un
  modelo de computación teórico. Es más capaz que un autómata finito (que no
  tiene memoria), pero menos capaz que una máquina de Turing (que tiene memoria
  de acceso aleatorio).
</p>
<p>
  La característica distintiva de un PDA es que utiliza una
  <strong>pila</strong> (la estructura de datos LIFO) como memoria auxiliar.
</p>
<p>
  Las transiciones de un PDA (es decir, sus "decisiones") se basan en tres
  factores:
</p>
<ol>
  <li>El estado actual en el que se encuentra.</li>
  <li>El símbolo de entrada que está leyendo.</li>
  <li>El símbolo que se encuentra en el <b>tope</b> de la pila.</li>
</ol>
<p>
  Al realizar una transición, el autómata puede cambiar de estado y,
  crucialmente, puede realizar operaciones <code>push</code> o
  <code>pop</code> en su pila.
</p>
<p>
  Los autómatas de pila son el mecanismo formal para reconocer
  <strong>Lenguajes Libres de Contexto</strong> (CFL), como por ejemplo, el
  lenguaje de paréntesis balanceados <code>( ( ) ( ) )</code>.
</p>

<hr />

<h3>3. Representación con <code>struct</code> en C++</h3>
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


<div style="background:#272822; padding:15px; border-radius:10px; color:white;">
  
  <pre>
<code>
def sumar(a, b):
    return a + b
</code>
  </pre>

</div>
