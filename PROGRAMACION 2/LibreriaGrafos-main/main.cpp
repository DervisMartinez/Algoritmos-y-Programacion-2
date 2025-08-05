#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <algorithm>
#include "grafoNoDirigido.hpp"

using namespace std;

// Función para verificar si un conjunto es dominante
bool esConjuntoDominante(const GrafoNoDirigido<string>& grafo, const unordered_set<string>& conjunto) {
    unordered_set<string> cubiertos = conjunto; //Se crea un conjunto cubiertos, iniciándolo con los vértices del conjunto dominante propuesto.
    /*
    Para cada vértice en conjunto, se obtienen sus vecinos y se agregan a cubiertos.
    Esto asegura que todos los vecinos de los vértices en el conjunto dominante sean considerados como cubiertos.
    */
    for (const string& v : conjunto) {
        list<string> vecinos = grafo.vecinos(v);
        for (const string& vecino : vecinos) {
            cubiertos.insert(vecino);
        }
    }
    list<string> todos = grafo.vertices(); //Se obtiene la lista de todos los vértices del grafo.
    for (const string& v : todos) { 
        if (cubiertos.find(v) == cubiertos.end()) { //Se recorre cada vértice y se verifica si está presente en cubiertos.
            return false; // Si algún vértice no está cubierto, la función devuelve false.
        };
    };
    return true; //Si todos los vértices están cubiertos, la función devuelve true.
};

// Función recursiva de backtracking mejorada
void encontrarMDSRecursivo(int k,const vector<string>& verticesOrdenados,vector<string>& conjuntoActual,vector<string>& mejorConjunto,unordered_set<string>& noCubiertos,const GrafoNoDirigido<string>& grafo)
{
    // si ya tenemos una solución mejor, no continuar
    if (!mejorConjunto.empty() && conjuntoActual.size() >= mejorConjunto.size()) {
        return;
    }

    // Si cubrimos todo el grafo, actualizamos la mejor solución
    if (noCubiertos.empty()) {
        mejorConjunto = conjuntoActual;
        return;
    }

    // Si hemos procesado todos los vértices
    if (k >= verticesOrdenados.size()) {
        return;
    }

    const string& verticeActual = verticesOrdenados[k]; //Se toma el vértice en la posición k dentro del conjunto ordenado.


    // Calculamos qué nodos cubriría este vértice
    unordered_set<string> nuevosCubiertos;
    if (noCubiertos.count(verticeActual)) {
        nuevosCubiertos.insert(verticeActual);
    } //Se determina qué vértices quedarían cubiertos si incluimos verticeActual en el conjunto dominante.

    list<string> vecinos = grafo.vecinos(verticeActual);
    for (const string& vecino : vecinos) {
        if (noCubiertos.count(vecino)) {
            nuevosCubiertos.insert(vecino);
        }
    }

    //Incluir el vértice actual
    if (!nuevosCubiertos.empty()) {
        conjuntoActual.push_back(verticeActual);
        for (const string& cubierto : nuevosCubiertos) {
            noCubiertos.erase(cubierto);
        }
        encontrarMDSRecursivo(k + 1, verticesOrdenados, conjuntoActual, mejorConjunto, noCubiertos, grafo); //Simplemente avanzamos sin agregar verticeActual al conjunto dominante.
        // Backtrack
        conjuntoActual.pop_back();
        for (const string& cubierto : nuevosCubiertos) {
            noCubiertos.insert(cubierto);
        }
    }

    //No incluir el vértice actual
    encontrarMDSRecursivo(k + 1, verticesOrdenados, conjuntoActual, mejorConjunto, noCubiertos, grafo);
}

// Función principal para encontrar el conjunto dominante mínimo
list<string> encontrarConjuntoDominanteMinimoExacto(GrafoNoDirigido<string>& grafo) {
    list<string> verticesList = grafo.vertices();
    if (verticesList.empty()) {
        return {};
    }

    // Ordenamos los vértices para exploración consistente
    vector<string> verticesOrdenados(verticesList.begin(), verticesList.end());
    sort(verticesOrdenados.begin(), verticesOrdenados.end());

    unordered_set<string> noCubiertos(verticesOrdenados.begin(), verticesOrdenados.end());
    vector<string> conjuntoActual;
    vector<string> mejorConjunto;

    // Empezamos con el peor caso (todos los vértices)
    mejorConjunto = verticesOrdenados;

    encontrarMDSRecursivo(0, verticesOrdenados, conjuntoActual, mejorConjunto, noCubiertos, grafo);

    // Ordenamos el resultado final
    sort(mejorConjunto.begin(), mejorConjunto.end());
    return list<string>(mejorConjunto.begin(), mejorConjunto.end());
}

void procesarEntrada(istream& entrada, GrafoNoDirigido<string>& grafo) {
    string pueblo1, pueblo2;
    while (entrada >> pueblo1 >> pueblo2) {
        grafo.agregarVertice(pueblo1);
        grafo.agregarVertice(pueblo2);
        grafo.agregarArcoND(pueblo1, pueblo2);
    }
}

/*
argc indica la cantidad de argumentos pasados al programa (incluyendo el nombre del ejecutable).
argv es un arreglo de char* que contiene los argumentos como cadenas.
*/
int main(int argc, char* argv[]) {
    GrafoNoDirigido<string> grafo; //Se crea un objeto grafo de tipo GrafoNoDirigido<string>.
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            ifstream archivo(argv[i]);
            if (!archivo) {
                cerr << "Error: No se pudo abrir el archivo '" << argv[i] << "'" << endl;
                return 1;
            }
            procesarEntrada(archivo, grafo);
            archivo.close();
        }
    } else {
        procesarEntrada(cin, grafo);
    }

    list<string> resultado = encontrarConjuntoDominanteMinimoExacto(grafo);

    // Imprimir resultado
    if (!resultado.empty()) {
        auto it = resultado.begin();
        cout << *it;
        for (++it; it != resultado.end(); ++it) {
            cout << " " << *it;
        }
    }
    cout << endl;

    return 0;
}