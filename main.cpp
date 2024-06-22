#include "disksproub.h"
#include "bufferManager.h"
using namespace std;
int main() {/*
    int num_platos, num_pistas, num_sectores, tam_sector;
    bool continuar = true;

    do {
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
        disco.guardarTextoEnBloque(archivoTxt);

        // Crear los bloques
        disco.crearBloques();

        // Opciones adicionales
        int opcion;
        cout << "Seleccione una opción:" << endl;
        cout << "1. Agregar más texto a un bloque" << endl;
        cout << "2. Eliminar registro de un bloque" << endl;
        cout << "3. Leer un sector específico" << endl;
        cout << "4. Mostrar capacidad del disco" << endl;
        cout << "5. Salir" << endl;
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
                continuar = false;
                break;
            default:
                cout << "Opción no válida. Por favor, seleccione una opción válida." << endl;
                break;
        }

    } while (continuar);

    cout << "Fin del programa." << endl;
*/
BufferManager* bufferManager = nullptr;

    int elegirAlgoritmo;
    cout << "Presione 1 si quiere usar el algoritmo de clock o 2 si desea el LRU: ";
    cin >> elegirAlgoritmo;

    if (elegirAlgoritmo == 1) {
        int opcion = 0;
        while (opcion != 9) {  // Cambié el número de la opción de salida a 7, como indicado en el menú
            cout << "\nMenu:\n";
            cout << "1. Crear BufferPool\n";
            cout << "2. Consultar pagina en BufferPool\n";
            cout << "3. Lectura/Escritura de una pagina en BufferPool\n";
            cout << "4. Insertar Pinned en una pagina\n";
            cout << "5. Insertar texto en pagina\n";
            cout << "6. Guardar en bloque\n";
            cout << "7. Mostrar tabla\n";
            cout << "8. Mostrar contenido de frame\n";
            cout << "9. Salir\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
                case 1: {
                    size_t num_frames;
                    cout << "Ingrese el numero de frames para el BufferPool: ";
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
                        cout << "Ingrese 1 si desea solicitar o 2 para cancelar solicitud de una pagina: ";
                        cin >> sub_opcion;
                        if (sub_opcion == 1) {
                            cout << "Ingrese el numero de pagina que desea consultar: ";
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
                        cout << "Ingrese el numero de pagina que desea leer o modificar: ";
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
                        cout << "Ingrese el numero de pagina que desea pinear: ";
                        cin >> pagina;
                        bufferManager->InsertarPinned(pagina);
                    }
                    break;
                }

                case 5: {
                    int pagina;
                    cout << "ingrese numero de pagina: "; cin >> pagina;
                    bufferManager->GuardarValorerDisco(pagina);
                    break;
                }

                case 6: {
                    int pagina;
                    cout << "Ingrese numero de pagina: "; cin >> pagina;
                    bufferManager->GuardarBloque(pagina);
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
                        cout << "Ingrese el numero de frame: ";
                        cin >> frame;
                        bufferManager->mostrarContenidoFrame(frame);
                    }
                    break;
                }
                case 9: {
                    cout << "Saliendo del programa..." << endl;
                    break;
                }
                default: {
                    cout << "Opción no válida. Por favor, intente de nuevo." << endl;
                    break;
                }
            }
        }
    } else if (elegirAlgoritmo ==  2){
        int opcion = 0;
        while (opcion != 9) {
            std::cout << "\nMenu:\n";
            std::cout << "1. Crear BufferPool\n";
            std::cout << "2. Consultar pagina en BufferPool\n";
            std::cout << "3. Lectura/Escritura de una pagina en BufferPool\n";
            std::cout << "4. Mostrar tabla\n";
            std::cout << "5. Mostrar contenido de frame\n";
            std::cout<<  "6. Pinned pagina\n";
            std::cout<<  "7. Llenas registros con longitud fija\n";
            std::cout<<  "8. Llenas registros con longitud variable\n";
            std::cout << "9. Salir\n";
            std::cout << "Seleccione una opcion: ";
            std::cin >> opcion;

            switch (opcion) {
                case 1: {
                    size_t num_frames;
                    std::cout << "Ingrese el numero de frames para el BufferPool: ";
                    std::cin >> num_frames;
                    bufferManager = new BufferManager(num_frames);
                    std::cout << "BufferPool creado con " << num_frames << " frames." << std::endl;
                    break;
                }
                case 2: {
                    int opcion;
                    int pagina;
                    cout << "ingrese 1 si desea solicitud o 2 para cancelar solicitud de una pagina: "; cin >> opcion;
                    if(opcion == 1){
                        cout << "Ingrese el numero de pagina que desea consultar: ";
                        cin >> pagina;
                        bufferManager->consultarPagina2(pagina);
                    }
                    else{
                        cout << "Ingrese el número de página que desea cancelar solicitud ";
                        cin >> pagina;
                        bufferManager->cancelarSolicitud(pagina);
                    }
                
                    break;
                }

                case 3: {
                    int pagina;
                    cout << "ingrese el numero de pagina que desea leer o modificar: ";
                    cin >> pagina;
                    bufferManager->LecturaEscritura(pagina);
                    break;
                }

                case 4: {
                    if (!bufferManager) {
                        std::cout << "Primero debe crear el BufferPool." << std::endl;
                    } else {
                        bufferManager->mostrarTablaDePaginas2();
                    }
                    break;
                }

                case 5: {
                    int frame;
                    cout << "ingrese numero de frame "; cin >> frame;
                    bufferManager->mostrarContenidoFrame(frame);
                    break;
                }
                case 6:{
                    int pagina;
                    cout<<"Que pagina deseas pinned"; cin>>pagina;
                    bufferManager->InsertarPinned(pagina);
                }

                case 7: {
                    string archivoTxt = "titanic.txt";
                    string archivoEsquema = "esquemas.txt";
                    string esquema = getSchema(archivoEsquema, "titanic");

                    if (!esquema.empty()) {
                        bufferManager->llenarLongitudFija(archivoTxt, esquema);
                    }
                    
                }

                case 8 : {
                    string archivoTxt = "titanic.txt";
                    string archivoEsquema = "esquemas.txt";
                    string esquema = getSchema(archivoEsquema, "titanic");

                    if (!esquema.empty()) {
                        bufferManager->llenarLongitudVariable(archivoTxt, esquema);
                    }
                }

                case 9: {
                    cout << "saliedo del programa..." << endl;
                    break;
                }
            }
        }
    }
    return 0;
}