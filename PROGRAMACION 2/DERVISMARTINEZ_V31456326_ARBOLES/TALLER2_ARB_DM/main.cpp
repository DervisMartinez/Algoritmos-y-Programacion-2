#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>  //útil para manejar límites numéricos, prevenir desbordamientos

//Librerias Propias 
#include "../TALLER2_ARB_DM/node/node.hpp"
#include "../TALLER2_ARB_DM/arbbin/arbBin.hpp"

using namespace std;


//verifico si el subconjunto es valido
bool validarSubconjunto(ArbolBinario<string> arbol, vector<string> S) {
    for (size_t i = 0; i < S.size(); ++i) { //que recorre todos los elementos del vector S.
        if (!arbol.buscar(S[i])) return false;  /*Llama al método buscar() del árbol binario para verificar si el elemento S[i] existe en el árbol. 
        ahora Si el método buscar() devuelve false, significa que el elemento no está en el árbol, por lo que el subconjunto es inválido. La función retorna false inmediatamente.*/
    }
    return true;
}


//calculo el Métrica Universal de Padecimiento
int calcularMUP(ArbolBinario<string> arbol, vector<string> S) {
    int mup = 0;
    
    for (size_t i = 0; i < S.size(); ++i) {
        for (size_t j = i + 1; j < S.size(); ++j) { //recorre los elementos restantes del subconjunto a partir del índice i + 1. Esto asegura que se calculen combinaciones únicas de pares de elementos.
            // Verificamos que ambos nodos existen antes de operar
            if (!arbol.buscar(S[i]) || !arbol.buscar(S[j])) {
                continue; // Saltar si alguno no está
            }
            int D = arbol.distanciaNodos(S[i], S[j]);
            int Nd = arbol.distanciaNiveles(S[i], S[j]);
            mup += D * Nd;
        }
    }
    return mup;
}

void procesarEntrada() {
    int t; //Casos de prueba 
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia buffer

    for (int caso = 0; caso < t; ++caso) { /*bucle for que itera desde caso = 0 hasta que caso < t.  significa que el código dentro del bucle se ejecutará t veces, donde t representa el número de casos de prueba o escenarios que se desean procesar.*/
        int n; // cantidad de grupos de estudiantes (es decir, árboles). 
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia el buffer y indica el número máximo de caracteres que se pueden ignorar.

        vector<ArbolBinario<string>> arboles(n); /*Declara un vector llamado arboles que contiene n elementos del tipo ArbolBinario<string>.*/
        vector<vector<string>> subconjuntos(n); /*Declara un vector llamado subconjuntos que contiene n elementos, y cada elemento es a su vez un vector de tipo string. */

        for (int i = 0; i < n; ++i) {  /*bucle externo que se ejecutara n veces, donde n representa la cantidad de árboles o conjuntos de datos que deben procesarse.*/
            vector<string> inorden;  /*vector de cadenas vacío llamado inorden. Este se llenará con los valores asociados al recorrido "inorden" de un árbol.*/
            vector<string> otroRecorrido; /*vector de cadenas vacío que lo eh llamado otroRecorrido. Este se usará para almacenar los valores de otro recorrido del árbo*/
            string tipoOtro = ""; /*cadena vacía que almacenará el nombre del recorrido alternativo (como "PREORDEN" o "POSTORDEN").*/

            for (int j = 0; j < 2; ++j) { /*ucle interno que itera dos veces porque hay exactamente dos recorridos que procesar por cada elemento: "INORDEN" y otro (como "PREORDEN" o "POSTORDEN").*/
                string linea; /*adena linea que se usará para almacenar una línea completa de entrada.*/
                getline(cin, linea); /*Lee una línea completa desde la entrada estándar (como el teclado) y la almacena en linea.*/
                istringstream iss(linea);/*flujo de entrada iss a partir de la línea leída. Esto permite procesar la línea palabra por palabra.*/
                string tipo; /*una cadena tipo que almacenará el primer token (o palabra) de la línea. Este token indica el tipo de recorrido*/
                iss >> tipo; /*Extraigo el primer token de iss y lo almacena en la variable tipo.*/

                if (tipo != "INORDEN" && tipo != "PREORDEN" && tipo != "POSTORDEN") { /*Compruebo que el valor de tipo sea uno de los tres valores válidos: "INORDEN", "PREORDEN" o "POSTORDEN".*/

                    cerr << "Formato inválido. Se esperaba INORDEN/PREORDEN/POSTORDEN" << endl; /*Si el valor de tipo no es válido, imprime un mensaje de error a la salida de errores estándar.*/
                    exit(1); /*Finaliza el programa inmediatamente con un código de error 1. Esto asegura que no se procesen entradas inválidas.*/
                } 
                vector<string> valores; /*vector vacío llamado valores, que se usará para almacenar las palabras restantes de la línea (los valores del recorrido).*/
                string palabra; /*variable palabra para procesar cada palabra individual en la línea.*/
                while (iss >> palabra) { //lee cada palabra restante de la línea y la agrega al vector valores.
                    valores.push_back(palabra);
                }
                //Si el recorrido es "INORDEN", asigna el vector de valores leídos al vector inorden
                if (tipo == "INORDEN") {
                    inorden = valores;
                } else {
                    otroRecorrido = valores;
                    tipoOtro = tipo;
                } //Si el recorrido es otro (por ejemplo, "PREORDEN" o "POSTORDEN"), guarda los valores en el vector otroRecorrido y asigna el tipo a la variable tipoOtro.
            }

            if (inorden.empty() || otroRecorrido.empty()) { // inorden.empty() comprueba si el vector inorden está vacío. y otroRecorrido.empty() verifica si el vector otroRecorrido también está vacío.
                cerr << "Error: faltan recorridos válidos (INORDEN y otro tipo) en el grupo " << (i + 1) << endl; //Impresiones de error por terminal para verificar el procedimiento del algoritmo ]
                exit(1); //detiene la ejecución del programa inmediatamente con un código de error 1.
            }

            bool esPreorden = (tipoOtro == "PREORDEN"); //tipoOtro almacena el tipo de recorrido alternativo (por ejemplo, "PREORDEN" o "POSTORDEN"). y esPreorden será true si tipoOtro es "PREORDEN", lo que indica cómo se debe proceder en la reconstrucción del árbol.
            arboles[i].reconstruir(inorden, otroRecorrido, esPreorden);

            // Leelo los subconjunto S 
            string lineaS;
            getline(cin, lineaS);
            istringstream issS(lineaS);
            vector<string> S;
            string elem;
            while (issS >> elem) { //extraer cada elemento y agregarlo al vector S.
                S.push_back(elem);
            }
            subconjuntos[i] = S;
        }

        int maxMUP = -1; //maxMUP indicaa el máximo valor de MUP como -1.
        int indiceGanador = 0;//almacena el índice del grupo con el mayor valor de MUP.

        for (int i = 0; i < n; ++i) {
            int mup = calcularMUP(arboles[i], subconjuntos[i]);
            if (mup > maxMUP) {
                maxMUP = mup;
                indiceGanador = i;
            }
        }

        cout << (indiceGanador + 1) << endl;
    }
}

int main() {
    procesarEntrada(); // porcedimiento encargado de toda la operacion 
    /*system("pause"); //pausa el programa*/
    return 0; //termina el programa
}
