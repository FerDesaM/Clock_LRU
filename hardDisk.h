#ifndef DISK_H
#define DISK_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>

using namespace std;

struct BlockHeader {
    int plato;
    int superficie;
    int pista;
    int sector;
};

class Block {
public:
    BlockHeader header;
    vector<string> sectores;

    Block(int plato, int superficie, int pista, int sector, int tamanoSector, int numSectoresPorBloque) {
        header.plato = plato;
        header.superficie = superficie;
        header.pista = pista;
        header.sector = sector;

        // Crear sectores vacíos
        for (int i = 0; i < numSectoresPorBloque; i++) {
            sectores.push_back(string(tamanoSector, ' '));
        }
    }

    string getSectorData(int index) {
        if (index >= 0 && index < sectores.size()) {
            return sectores[index];
        }
        return "";
    }

    void setSectorData(int index, const string &data) {
        if (index >= 0 && index < sectores.size()) {
            sectores[index] = data;
        }
    }

    void addRegistro(const string &registro, int index) {
        if (index >= 0 && index < sectores.size()) {
            sectores[index] += registro + "\n";
        }
    }

    void eliminarRegistro(int index) {
        if (index >= 0 && index < sectores.size()) {
            sectores[index] = "";
        }
    }
};

class DiscoDuro {
private:
    int num_platos;
    int num_pist;
    int sectoresPorPista;
    int tam_sect;
    int sectoresporbloque = 4;

public:
    DiscoDuro() : num_platos(0), num_pist(0), sectoresPorPista(0), tam_sect(0) {}

    DiscoDuro(int num_plat, int num_pistas, int num_sect, int tam_sector)
        : num_platos(num_plat), num_pist(num_pistas), sectoresPorPista(num_sect), tam_sect(tam_sector) {}

    void setNumPlatos(int num) {
        num_platos = num;
    }

    void setPistasPorSuperficie(int num) {
        num_pist = num;
    }

    void setSectoresPorPista(int num) {
        sectoresPorPista = num;
    }

    void setBytesPorSector(int num) {
        tam_sect = num;
    }

    bool discoExiste() {
        struct stat info;
        return stat("discoDuro", &info) == 0 && S_ISDIR(info.st_mode);
    }

    void crearEstructuraDisco() {
        string carpeta = "discoDuro";
        mkdir(carpeta.c_str(), 0777);

        for (int plato = 0; plato < num_platos; plato++) {
            string carpetaPlato = carpeta + "/plato_" + to_string(plato + 1);
            mkdir(carpetaPlato.c_str(), 0777);

            for (int superficie = 0; superficie < 2; superficie++) {
                string carpetaSuperficie = carpetaPlato + "/superficie_" + to_string(superficie + 1);
                mkdir(carpetaSuperficie.c_str(), 0777);

                for (int pista = 0; pista < num_pist; pista++) {
                    string carpetaPista = carpetaSuperficie + "/pista_" + to_string(pista + 1);
                    mkdir(carpetaPista.c_str(), 0777);

                    for (int sector = 0; sector < sectoresPorPista; sector++) {
                        string archivoSector = carpetaPista + "/sector_" + to_string(sector + 1) + ".txt";
                        crearArchivo(archivoSector);
                    }
                }
            }
        }
        cout << "La estructura del disco ha sido creada exitosamente." << endl;
    }

    int calcularLineasPorSector() {
        int tamano_sector = tam_sect;
        int tamano_linea = 256;
        return tamano_sector / tamano_linea;
    }

    void crearArchivo(const string &rutaArchivo) {
        ofstream archivo(rutaArchivo);
        cout << "Creando archivo: " << rutaArchivo << endl;
    }

    bool sectorDisponible(const std::string &sector) {
        std::ifstream archivo(sector);
        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo " << sector << std::endl;
            return false; // No se puede abrir el archivo, por lo tanto, se considera no disponible
        }

        // Verificar si el archivo está vacío
        archivo.seekg(0, std::ios::end);
        if (archivo.tellg() == 0) {
            std::cout << "Sector disponible: " << sector << std::endl;
            archivo.close();
            return true; // El archivo está vacío, por lo tanto está disponible
        } else {
            std::cout << "Sector ocupado: " << sector << std::endl;
            archivo.close();
            return false; // El archivo contiene contenido y por lo tanto está ocupado
        }
    }

    void guardarTextoEnBloque(const std::string &archivoTxt) {
        if (!discoExiste()) {
            std::cerr << "El disco no existe." << std::endl;
            return;
        }

        std::ifstream archivo(archivoTxt);
        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo " << archivoTxt << std::endl;
            return;
        }

        std::vector<std::string> contenido;
        std::string linea;
        while (std::getline(archivo, linea)) {
            contenido.push_back(linea);
        }
        archivo.close();

        int lineas_por_sector = calcularLineasPorSector();

        int sectorIndex = 0;
        for (int plato = 1; plato <= num_platos; ++plato) {
            for (int superficie = 1; superficie <= 2; ++superficie) {
                for (int pista = 1; pista <= num_pist; ++pista) {
                    for (int sector = 1; sector <= sectoresPorPista; ++sector) {
                        std::string archivoSector = "discoDuro/plato_" + std::to_string(plato) + "/superficie_" + std::to_string(superficie) + "/pista_" + std::to_string(pista) + "/sector_" + std::to_string(sector) + ".txt";

                        if (sectorDisponible(archivoSector)) {
                            std::ofstream archivoEscritura(archivoSector);
                            if (archivoEscritura.is_open()) {
                                int lineasEscritas = 0;
                                while (sectorIndex < contenido.size() && lineasEscritas < lineas_por_sector) {
                                    archivoEscritura << contenido[sectorIndex] << std::endl;
                                    ++lineasEscritas;
                                    ++sectorIndex;
                                }
                                archivoEscritura.close();
                                std::cout << "Texto guardado en " << archivoSector << std::endl;
                            } else {
                                std::cerr << "No se pudo abrir el archivo " << archivoSector << " para escritura." << std::endl;
                            }
                        } else {
                            std::cerr << "El sector no está disponible: " << archivoSector << std::endl;
                        }
                    }
                }
            }
        }
    }

    void guardarTextoEnSectores(const std::string &archivoTxt) {
        if (!discoExiste()) {
            std::cerr << "El disco no existe." << std::endl;
            return;
        }

        std::ifstream archivo(archivoTxt);
        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo " << archivoTxt << std::endl;
            return;
        }

        std::vector<std::string> contenido;
        std::string linea;
        while (std::getline(archivo, linea)) {
            contenido.push_back(linea);
        }
        archivo.close();

        int lineas_por_sector = calcularLineasPorSector();

        int sectorIndex = 0;
        for (int plato = 1; plato <= num_platos; ++plato) {
            for (int superficie = 1; superficie <= 2; ++superficie) {
                for (int pista = 1; pista <= num_pist; ++pista) {
                    for (int sector = 1; sector <= sectoresPorPista; ++sector) {
                        std::string archivoSector = "discoDuro/plato_" + std::to_string(plato) + "/superficie_" + std::to_string(superficie) + "/pista_" + std::to_string(pista) + "/sector_" + std::to_string(sector) + ".txt";

                        if (sectorDisponible(archivoSector)) {
                            std::ofstream archivoEscritura(archivoSector);
                            if (archivoEscritura.is_open()) {
                                int lineasEscritas = 0;
                                while (sectorIndex < contenido.size() && lineasEscritas < lineas_por_sector) {
                                    archivoEscritura << contenido[sectorIndex] << std::endl;
                                    ++lineasEscritas;
                                    ++sectorIndex;
                                }
                                archivoEscritura.close();
                                std::cout << "Texto guardado en " << archivoSector << std::endl;
                            } else {
                                std::cerr << "No se pudo abrir el archivo " << archivoSector << " para escritura." << std::endl;
                            }
                        } else {
                            std::cerr << "El sector no está disponible: " << archivoSector << std::endl;
                        }
                    }
                }
            }
        }
    }

    bool leerSector(const std::string &sector, std::string &contenido) {
        std::ifstream archivo(sector);
        if (archivo.is_open()) {
            std::stringstream buffer;
            buffer << archivo.rdbuf();
            contenido = buffer.str();
            archivo.close();
            return true;
        }
        return false;
    }

    void leerSectorDinamico() {
        int PlatoAmostrar, SuperficieAMostrar, PistaaMostrar, SectorAmostrar;
        cout << "Plato: ";
        cin >> PlatoAmostrar;
        cout << "Superficie: ";
        cin >> SuperficieAMostrar;
        cout << "Pista: ";
        cin >> PistaaMostrar;
        cout << "Sector: ";
        cin >> SectorAmostrar;
        string sectorALeer = "discoDuro/plato_" + to_string(PlatoAmostrar) + "/superficie_" + to_string(SuperficieAMostrar) + "/pista_" + to_string(PistaaMostrar) + "/sector_" + to_string(SectorAmostrar) + ".txt";
        string contenido;
        if (leerSector(sectorALeer, contenido)) {
            cout << "Contenido del sector P: " << PlatoAmostrar << " S: " << SuperficieAMostrar << " P: " << PistaaMostrar << " Se: " << SectorAmostrar << ":" << endl;
            cout << contenido << endl;
        } else {
            cout << "No se pudo leer el sector " << PlatoAmostrar << " S: " << SuperficieAMostrar << " P: " << PistaaMostrar << " Se: " << SectorAmostrar << endl;
        }
    }

    void capacidad() {
        int capacidad = num_platos * 2 * num_pist * sectoresPorPista * tam_sect;
        cout << "Capacidad del disco:  " << capacidad << " bytes" << endl;
    }

    void crearBloques() {
    for (int plato = 1; plato <= num_platos; ++plato) {
        for (int superficie = 1; superficie <= 2; ++superficie) {
            for (int pista = 1; pista <= num_pist; ++pista) {
                // Ruta base del bloque y carpeta de la pista
                string rutaBase = "discoDuro/plato_" + to_string(plato) + "/superficie_" + to_string(superficie);
                string carpetaPista = rutaBase + "/pista_" + to_string(pista);

                // Iterar sobre los sectores por bloque
                for (int sector = 1; sector <= sectoresPorPista; sector += sectoresporbloque) {
                    // Número de bloque actual
                    int numBloque = (sector - 1) / sectoresporbloque + 1;

                    // Nombre del archivo del bloque
                    string archivoBloque = carpetaPista + "/bloque_" + to_string(numBloque) + ".txt";

                    ofstream archivo(archivoBloque);
                    if (archivo.is_open()) {
                        archivo << "Plato: " << plato << " Superficie: " << superficie << " Pista: " << pista << " bloque " << numBloque << "\n";
                        archivo << "===================\n";

                        // Iterar sobre los sectores del bloque
                        for (int i = 0; i < sectoresporbloque; ++i) {
                            int sectorActual = sector + i;
                            if (sectorActual <= sectoresPorPista) {
                                // Ruta del archivo del sector
                                string archivoSector = carpetaPista + "/sector_" + to_string(sectorActual) + ".txt";
                                string contenido;
                                if (leerSector(archivoSector, contenido)) {
                                    archivo << "Plato: " << plato << " Superficie: " << superficie << " Pista: " << pista << " Sector: " << sectorActual << " --\n";
                                    archivo << contenido << "\n";
                                } else {
                                    archivo << "Plato: " << plato << " Superficie: " << superficie << " Pista: " << pista << " Sector: " << sectorActual << " --\n";
                                    archivo << "Sector vacío\n";
                                }
                                archivo << "-------------------\n";
                            }
                        }
                        archivo.close();
                        cout << "Bloque creado: " << archivoBloque << endl;
                    } else {
                        cerr << "No se pudo crear el archivo del bloque: " << archivoBloque << endl;
                    }
                }
            }
        }
    }
}
};
#endif // DISK_H
/*
Comparar por esquema
int main() {
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

        // Leer el archivo de texto y guardar su contenido en los sectores del disco
        string archivoTxt;
        cout << "Ingrese el nombre del archivo de texto para insertar en los sectores: ";
        cin >> archivoTxt;
        disco.guardarTextoEnSectores(archivoTxt);

        // Opciones adicionales
        int opcion;
        cout << "Seleccione una opción:" << endl;
        cout << "1. Agregar más texto" << endl;
        cout << "2. Leer un sector" << endl;
        cout << "3. Salir" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Ingrese el nombre del archivo de texto adicional: ";
                cin >> archivoTxt;
                disco.guardarTextoEnSectores(archivoTxt);
                break;
            case 2:
                disco.leerSectorDinamico();
                break;
            case 3:
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
*/