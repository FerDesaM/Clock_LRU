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

queue<Requerimiento> Requerimientos;


bool hayEscrituraPendiente(int pagina) {
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
