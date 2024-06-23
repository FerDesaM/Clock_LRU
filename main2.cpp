#include "hardDisk.h"
#include "bufferManager.h"
using namespace std;

int main() {
    std::vector<Bloque> bloques;
    int num_platos, num_pistas, num_sectores, tam_sector;
    bool continuar = true;

    // Solicitar los parámetros del disco solo una vez al inicio
    cout << "Ingrese el número de platos: ";
    cin >> num_platos;

    cout << "Ingrese el número de pistas por superficie: ";
    cin >> num_pistas;

    cout << "Ingrese el número de sectores por pista: ";
    cin >> num_sectores;

    cout << "Ingrese el tamaño de los sectores (en bytes): ";
    cin >> tam_sector;

    DiscoDuro disco(num_platos, num_pistas, num_sectores, tam_sector);

    // Crear la estructura del disco
    disco.crearEstructuraDisco();

    do {
        int opcion;
        cout << "Seleccione una opción:" << endl;
        cout << "1. Agregar más texto a un bloque" << endl;
        cout << "2. Eliminar registro de un bloque" << endl;
        cout << "3. Leer un sector específico" << endl;
        cout << "4. Mostrar capacidad del disco" << endl;
        cout << "5. Crear Bloques" << endl;
        cout << "6. Ver las direcciones de los bloques" << endl;
        cout << "7. Salir" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string archivoTxt;
                cout << "Ingrese el nombre del archivo de texto adicional: ";
                cin >> archivoTxt;
                disco.guardarTextoEnBloque(archivoTxt);
                break;
            }
            case 2:
                // Lógica para eliminar registro de un bloque
                break;
            case 3:
                disco.leerSectorDinamico();
                break;
            case 4:
                disco.capacidad();
                break;
            case 5: {
                int numeroSectores;
                cout << "Cuántos sectores quiere por cada bloque: ";
                cin >> numeroSectores;
                CrearBloques2(numeroSectores, disco, bloques);
                asignarDirecciones(bloques, bloques.size(), disco);
                cout << "Bloques totales: " << bloques.size() << endl;
                break;
            }
            case 6:
                for (int i = 0; i < bloques.size(); i++) {
                    cout << bloques[i].imprimirDirecion() << endl;
                }
                break;
            case 7:
                continuar = false;
                break;
            default:
                cout << "Opción no válida. Por favor, seleccione una opción válida." << endl;
                break;
        }
    } while (continuar);

    cout << "Fin del programa." << endl;
    return 0;
}
