#include <iostream>
#include <string>
#include <sstream>
#include "../lib/Node/DoublyNode.hpp"
#include "../lib/List/SinglyList.hpp"
#include "../lib/List/CircularDoublyList.hpp"


string encontrarElegido(const string& nombres, int k) {
    CircularDoublyList<string> lista; //creo la lista doblemente enlazada
   /* istringstream : 
   permite tratar una cadena de texto (string) como si fuera un flujo de entrada (como cin).*/
    istringstream iss(nombres); 
    string nombre;
    
    while (iss >> nombre) {
        lista.addToEnd(nombre); //añado al final de la lista
    }

    if (lista.getSize() == 0) return "";
    
    auto* current = lista.getHead();
    bool direccionDerecha = true; // true = next, false = prev

    while (lista.getSize() > 1) {
        // Saltar k-1 personas
        for (int i = 1; i < k; ++i) {
            current = direccionDerecha ? current->getNext() : current->getPrev();
        }

        // Elimino la persona k-ésima de la lista
        auto* toRemove = current;
        current = direccionDerecha ? current->getNext() : current->getPrev();
        lista.removeNode(toRemove);

        // Invertir dirección ahora
        direccionDerecha = !direccionDerecha;
    }

    return lista.getHead()->getData();
}
int main() {
    int n;  // Casos De Prueba
    cin >> n; // Recibo los casos de Prueba 
    cin.ignore(); //manejo de la entrada

    int* valoresK = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> valoresK[i];
        cin.ignore();
    }

    string linea;
    for (int i = 0; i < n; ++i) {
        getline(cin, linea);
        string elegido = encontrarElegido(linea, valoresK[i]);
        cout << elegido << endl;
    }

    delete[] valoresK;
    return 0;  //Termina el Programa 
};