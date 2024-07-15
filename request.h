#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "calcular.h"
#include <climits>
#include <queue>
using namespace std;


struct Requerimiento
{
    int pagina;
    int accion;
};

queue<Requerimiento> Requerimientos;        // JOSE ALEJANDRO MACHACA MUÑIZ 


bool hayEscrituraPendiente(int pagina) {            // JOSE ALEJANDRO MACHACA MUÑIZ 
    // Creamos una copia de la cola original para no modificarla
    queue<Requerimiento> colaTemporal = Requerimientos;

    // Bandera para indicar si encontramos una escritura pendiente
    bool escrituraPendiente = false;

    // Iteramos sobre la cola temporal
    while (!colaTemporal.empty()) {
        Requerimiento req = colaTemporal.front();
        colaTemporal.pop();

        if (req.pagina == pagina && req.accion == 2) {
            escrituraPendiente = true;
            break; // Terminamos la iteración si encontramos una escritura pendiente
        }
    }

    return escrituraPendiente;
}

void imprimirCola(queue<Requerimiento> cola) {          // JOSE ALEJANDRO MACHACA MUÑIZ 
    cout << "Contenido de la cola Requerimientos:" << endl;

    // Creamos una copia de la cola para no modificar la original
    queue<Requerimiento> colaTemporal = cola;

    // Iteramos sobre la cola temporal
    while (!colaTemporal.empty()) {
        Requerimiento req = colaTemporal.front();
        colaTemporal.pop();

        cout << "Página: " << req.pagina << ", Acción: " << req.accion << endl;
    }
}

