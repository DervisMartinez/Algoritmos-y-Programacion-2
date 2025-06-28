#ifndef STACK_H
#define STACK_H

#include <cstddef>  /* para null */
#include "../Node/SinglyNode.hpp"

template <typename T>
class Stack {

    /* atributos */
    SinglyNode<T>* top;   /* apuntador al elemento superior de la pila (es NULL si es vacia) */
    int length;             /* numero de elementos en la pila */

    /* metodos privados auxiliares */
    bool isNullNode(SinglyNode<T>* node) const {
        return node == NULL; /* verifica si un nodo es nulo */
    } /*O(1)*/

    void addUpdateTop(SinglyNode<T>* newTop) {
        this->top = newTop; /* actualiza el puntero al nuevo tope */
        this->length++; /* actualiza el puntero al nuevo tope y aumenta el tamaño */
    } /*O(1)*/

    void removeUpdateTop(SinglyNode<T>* newTop) {
        this->top = newTop; /* actualiza el puntero al nuevo tope */
        this->length= (this->length > 0) ? this->length - 1: 0; /* disminuye el tamaño, añade seguridad en caso que length sea menor a cero */
    } /* O(1) */

    void uniqueElementUpdate(SinglyNode<T>* newTop) {
        this->top = newTop; /* actualiza el puntero al nuevo tope */
        this->length = 1; /* establece el tamaño a 1 */
    } /* O(1) */

    void emptyElementsUpdate() {
        this->top = NULL; /* actualiza el puntero a NULL */
        this->length = 0; /* actualiza el tamaño a 0 */
    } /* O(1) */

public:
    /* constructor */
    Stack() : top(NULL), length(0) {} /* inicializa una pila vacia O(1)*/

    /* constructor copia */
    Stack(const Stack<T>& originalStack) : top(NULL), length(0) {
        if (!originalStack.isEmpty()) {
            /* pila temporal para mantener el orden */
            Stack<T> tempStack;
            SinglyNode<T>* current = originalStack.top;
            
            /* copia los elementos a la pila temporal (orden invertido) */
            while (current != NULL) {
                tempStack.push(current->getData());
                current = current->getNext();
            }
            
            /* copia al stack actual (revirtiendo el orden nuevamente) */
            while (!tempStack.isEmpty()) {
                this->push(tempStack.getTop());
                tempStack.pop();
            }
        } /* crea una nueva pila con distintos apuntadores de memoria */
    } /* O(n) */

    /* operador de asignacion:
       permite la operacion de asignacion directa entre dos pilas (miPila = otraPila) 
       contiene proteccion contra autoasignacion y libera la memoria de los nodos actuales antes de copiar los nuevos elementos
       ademas de usar una pila temporal para preservar el orden de los elementos
    */
    Stack<T>& operator=(const Stack<T>& originalStack) {
        if (this != &originalStack) { /* esta es la proteccion contra autoasignacion */
            clear(); /* liberamos los nodos actuales */
            
            if (!originalStack.isEmpty()) {
                /*pila temporal para preservar el orden */
                Stack<T> tempStack;
                SinglyNode<T>* current = originalStack.top;
                
                /* copia los elementos a la pila temporal (orden invertido) */
                while (current != NULL) {
                    tempStack.push(current->getData());
                    current = current->getNext();
                }
                
                /* copia al stack actual (revirtiendo el orden nuevamente) */
                while (!tempStack.isEmpty()) {
                    this->push(tempStack.getTop());
                    tempStack.pop();
                }
            }
        }
        return *this; /* permite el encadenamiento (a = b = c) */
    } /* O(n) */

    /* destructor */
    ~Stack() {
        clear(); /* libera el espacio reservado de memoria de los nodos */
    } /* O(n) */

    /* getters */
    int getLength() const {
        return length; /* obtiene el tamaño actual de la pila */
    } /* O(1) */

    bool isEmpty() const {
        return length == 0; /* verifica si la pila esta vacia */
    } /* O(1) */

    T getTop() const {
        if (isEmpty()) {
            /*retorna un valor por defecto si la pila esta vacia */
            return T();
        }
        return top->getData(); /* obtiene el elemento en el tope sin removerlo */
    }

    /* operaciones basicas */

    /* apila un elemento en el tope de la pila */
    void push(T element) {
        SinglyNode<T>* newNode = new SinglyNode<T>(element);
        if (isEmpty()) { /* si la pila es vacia, llama al metodo privado de actualizacion unica */
            uniqueElementUpdate(newNode);
        } else {
            newNode->setNext(top); /* enlaza el nuevo nodo con el tope actual */
            addUpdateTop(newNode); /* actualiza el tope y el tamaño */
        }
    } /* O(1) */

    /* desapila el elemento del tope de la pila */
    void pop() {
        if (!isEmpty()) {  /* solo opera si la pila tiene elementos */
            SinglyNode<T>* nodeToRemove = top; /* guarda el nodo tope actual */
            removeUpdateTop(top->getNext()); /* actualiza el tope al siguiente nodo y decrementa el tamaño */
            delete nodeToRemove; /* libera la memoria del nodo eliminado */
        }
        /* si la pila esta vacia, no hace nada */
    } /* O(1) */

    void clear() {
        while (!isEmpty()) {
            pop();  /* usa el metodo pop() para liberar memoria */
        }
        emptyElementsUpdate(); /* actualiza los valores a su estandar vacio (esto solo lo hace por seguridad de los datos, ya que pop() por si misma los actualiza tambien) */
    } /* O(n) */

    /* operaciones extras */

    /* clona la pila actual y retorna una nueva pila clonada 
    el clon es independiente de la pila original, por lo que se puede modificar sin afectar a la original */
    Stack<T> clone() const {
        Stack<T> newStack; /* crea una pila vacia local */
        if (!isEmpty()) {
            Stack<T> tempStack; /* pila auxiliar */
            
            /* primer traspaso (genera orden invertido) */
            SinglyNode<T>* current = top;
            while (current != NULL) {
                tempStack.push(current->getData());
                current = current->getNext();
            }
            
            /* segundo traspaso copia al clon (revierte el orden) */
            while (!tempStack.isEmpty()) {
                newStack.push(tempStack.getTop());
                tempStack.pop();
            }
        }
        return newStack; /* retorna por valor */
    } /* O(n) */

    void reverse() {
        if (isEmpty() || length == 1) {    
        Stack<T> tempStack;
        while (!isEmpty()) {
            tempStack.push(getTop());  /* vacia la pila actual en tempstack (orden invertido) */
            pop();  /* desapilamos el elemento superior */
        }
        
        /* copiamos de vuelta (ahora en orden inverso al original) */
        *this = tempStack; /* invierte el orden de los elementos en la pila */
        }
        /* si la pila es vacia o tiene un solo elemento, no hace nada */
    } /* O(n) */
};

#endif  /* STACK_H */
