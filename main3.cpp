#include "hardDisk.h"
#include "bufferManager.h"
#include <iostream>
#include "heapfile.h"
using namespace std;

int main() {
    int num_platos, num_pistas, num_sectores, tam_sector;
    bool continuar = true;
    int operacion;

    while (continuar) {
        cout << "Ingrese 1 si desea operación con el disco" << endl;
        cout << "Ingrese 2 si desea operaciones con el bufferManager" << endl;
        cout << "Ingrese 3 para cerrar el programa" << endl;
        cin >> operacion;

        switch (operacion) {
            case 1: {
                bool subContinuar = true;
                while (subContinuar) {
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

                    // Leer el archivo de texto y guardar su contenido en los bloques
                    string archivoTxt;
                    cout << "Ingrese el nombre del archivo de texto para insertar en los bloques: ";
                    cin >> archivoTxt;
                    
                    //disco.guardarTextoEnBloque(archivoTxt);

                    // Crear los bloques
                    //disco.crearBloques();
                    //disco.mapearDireccionesBloques("map.txt");

                    heapFile heap;
                    disco.CrearBloques3();
                    disco.guardarContenidoBloque();
                    std::vector<Bloque>& bloques = disco.obtenerBloques();
                    heap.almacenarInformacionBloques(bloques);
                    heap.mostrarInformacionBloques();
                    // Opciones adicionales
                    int opcion;
                    cout << "Seleccione una opción:" << endl;
                    cout << "1. Agregar más texto a un bloque" << endl;
                    cout << "2. Eliminar registro de un bloque" << endl;
                    cout << "3. Leer un sector específico" << endl;
                    cout << "4. Mostrar capacidad del disco" << endl;
                    cout << "5. Regresar al menú principal" << endl;
                    cin >> opcion;

                    switch (opcion) {
                        case 1:
                            cout << "Ingrese el nombre del archivo de texto adicional: ";
                            cin >> archivoTxt;
                            disco.guardarTextoEnBloque(archivoTxt);
                            break;
                        case 2:
                            // Lógica para eliminar registro de un bloque
                            break;
                        case 3:
                            disco.leerSectorDinamico();
                            break;
                        case 4:
                            disco.capacidad();
                            break;
                        case 5:
                            subContinuar = false;
                            break;
                        default:
                            cout << "Opción no válida. Por favor, seleccione una opción válida." << endl;
                            break;
                    }
                }
                break;
            }
        
            case 2: {
                cout << "Operaciones con el bufferManager" << endl;
                BufferManager* bufferManager = nullptr;

                int elegirAlgoritmo;
                cout << "Presione 1 si quiere usar el algoritmo de clock o 2 si desea el LRU: ";
                cin >> elegirAlgoritmo;

                if (elegirAlgoritmo == 1) {
                    int opcion = 0;
                    while (opcion != 9) {
                        cout << "\nMenú:\n";
                        cout << "1. Crear BufferPool\n";
                        cout << "2. Consultar página en BufferPool\n";
                        cout << "3. Lectura/Escritura de una página en BufferPool\n";
                        cout << "4. Insertar Pinned en una página\n";
                        cout << "5. Insertar texto en página\n";
                        cout << "6. Guardar en bloque\n";
                        cout << "7. Mostrar tabla\n";
                        cout << "8. Mostrar contenido de frame\n";
                        cout << "9. Regresar al menú principal\n";
                        cout << "Seleccione una opción: ";
                        cin >> opcion;

                        switch (opcion) {
                            case 1: {
                                size_t num_frames;
                                cout << "Ingrese el número de frames para el BufferPool: ";
                                cin >> num_frames;
                                bufferManager = new BufferManager(num_frames);
                                cout << "BufferPool creado con " << num_frames << " frames." << endl;
                                break;
                            }
                            case 2: {
                                if (!bufferManager) {
                                    cout << "Primero debe crear el BufferPool." << endl;
                                } else {
                                    int sub_opcion;
                                    int pagina;
                                    cout << "Ingrese 1 si desea solicitar o 2 para cancelar solicitud de una página: ";
                                    cin >> sub_opcion;
                                    if (sub_opcion == 1) {
                                        cout << "Ingrese el número de página que desea consultar: ";
                                        cin >> pagina;
                                        bufferManager->consultarPagina(pagina);
                                    } else if (sub_opcion == 2) {
                                        cout << "Ingrese el número de página que desea cancelar solicitud: ";
                                        cin >> pagina;
                                        bufferManager->cancelarSolicitud(pagina);
                                    } else {
                                        cout << "Opción no válida." << endl;
                                    }
                                }
                                break;
                            }
                            case 3: {
                                if (!bufferManager) {
                                    cout << "Primero debe crear el BufferPool." << endl;
                                } else {
                                    int pagina;
                                    cout << "Ingrese el número de página que desea leer o modificar: ";
                                    cin >> pagina;
                                    bufferManager->LecturaEscritura1(pagina);
                                }
                                break;
                            }
                            case 4: {
                                if (!bufferManager) {
                                    cout << "Primero debe crear el BufferPool." << endl;
                                } else {
                                    int pagina;
                                    cout << "Ingrese el número de página que desea pinear: ";
                                    cin >> pagina;
                                    bufferManager->InsertarPinned(pagina);
                                }
                                break;
                            }
                            case 5: {
                                if (!bufferManager) {
                                    cout << "Primero debe crear el BufferPool." << endl;
                                } else {
                                    int pagina;
                                    cout << "Ingrese el número de página: ";
                                    cin >> pagina;
                                    bufferManager->GuardarValorerDisco(pagina);
                                }
                                break;
                            }
                            case 6: {
                                if (!bufferManager) {
                                    cout << "Primero debe crear el BufferPool." << endl;
                                } else {
                                    int pagina;
                                    cout << "Ingrese el número de página: ";
                                    cin >> pagina;
                                    //bufferManager->GuardarBloque(pagina);
                                }
                                break;
                            }
                            case 7: {
                                if (!bufferManager) {
                                    cout << "Primero debe crear el BufferPool." << endl;
                                } else {
                                    bufferManager->mostrarTablaDePaginas();
                                    bufferManager->mostrarManecilla();
                                }
                                break;
                            }
                            case 8: {
                                if (!bufferManager) {
                                    cout << "Primero debe crear el BufferPool." << endl;
                                } else {
                                    int frame;
                                    cout << "Ingrese el número de frame: ";
                                    cin >> frame;
                                    bufferManager->mostrarContenidoFrame(frame);
                                }
                                break;
                            }
                            case 9: {
                                cout << "Regresando al menú principal..." << endl;
                                break;
                            }
                            default: {
                                cout << "Opción no válida. Por favor, intente de nuevo." << endl;
                                break;
                            }
                        }
                    }
                }
                break;
            }

            case 3: {
                cout << "Cerrando el programa..." << endl;
                continuar = false;
                break;
            }

            default: {
                cout << "Opción no válida. Por favor, intente de nuevo." << endl;
                break;
            }
        }
    }

    return 0;
}