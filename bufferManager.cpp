#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;

// Clase Pagina para manejar el contenido de cada página
class Pagina {
private:
    std::string ruta;
    std::vector<std::string> contenido;

public:
    // Constructor con argumentos para cargar el contenido de un archivo
    Pagina(const std::string& nombre_archivo) {
        ruta = nombre_archivo;
        cargarContenido();
    }

    // Constructor sin argumentos para manejar la inicialización de una instancia vacía
    Pagina() {}

    void cargarContenido() {
        std::ifstream archivo(ruta);
        std::string linea;
        if (archivo.is_open()) {
            while (getline(archivo, linea)) {
                contenido.push_back(linea);
            }
            archivo.close();
        } else {
            contenido.push_back("El archivo no fue encontrado.");
        }
    }

    std::string mostrarContenido() const {
        std::string contenido_str;
        for (const auto& linea : contenido) {
            contenido_str += linea + "\n";
        }
        return contenido_str;
    }
};

// Clase Frame para manejar cada frame del buffer pool
class Frame {
private:
    Pagina pagina; // Solo una página por frame

public:
    bool estaVacio() const {
        return pagina.mostrarContenido().empty();
    }

    void agregarPagina(const std::string& nombre_archivo) {
        if (estaVacio()) {
            pagina = Pagina(nombre_archivo);
        } else {
            std::cout << "El frame ya tiene una página cargada. No se puede agregar más páginas." << std::endl;
        }
    }

    void mostrarPagina() {
        if (!estaVacio()) {
            std::cout << pagina.mostrarContenido();
        } else {
            std::cout << "El frame está vacío." << std::endl;
        }
    }
};

// Clase BufferPool para manejar los frames   l
class BufferPool {
private:
    std::vector<Frame> frames;

public:
    BufferPool(size_t num_frames) {
        frames.reserve(num_frames);
        for (size_t i = 0; i < num_frames; ++i) {
            frames.emplace_back();
        }
    }

    void cargarPaginaAlFrame(size_t numero_frame, const std::string& nombre_archivo) {
        if (numero_frame >= 0 && numero_frame < frames.size()) {
            frames[numero_frame].agregarPagina(nombre_archivo);
        } else {
            std::cout << "Número de frame inválido." << std::endl;
        }
    }

    void mostrarContenidoFrame(int numero_frame) {
        if (numero_frame >= 0 && numero_frame < frames.size()) {
            std::cout << "Contenido del Frame " << numero_frame << ":" << std::endl;
            frames[numero_frame].mostrarPagina();
        } else {
            std::cout << "Número de frame inválido." << std::endl;
        }
    }

    size_t buscarFrameVacio() const {
        for (size_t i = 0; i < frames.size(); ++i) {
            if (frames[i].estaVacio()) {
                return i;
            }
        }
        return frames.size(); // No hay frames vacíos
    }

    size_t getNumFrames() const {
        return frames.size();
    }

    Frame* obtenerFrame(size_t numero_frame) {
        if (numero_frame >= 0 && numero_frame < frames.size()) {
            return &frames[numero_frame];
        } else {
            std::cout << "Número de frame inválido." << std::endl;
            return nullptr;
        }
    }

    
};



class PageTable {
private:
    struct PageEntry {
        int FrameId;
        int PageId;
        int DirtyBit;
        int PinCount;
        int LastUsed;
        bool ReferenceBit;

        PageEntry(int frameId, int pageId) : FrameId(frameId), PageId(pageId), DirtyBit(0), PinCount(0), LastUsed(0),ReferenceBit(true) {}
        
    };

    vector<PageEntry> entries;
        int clockHand;
public:
    // Constructor
    PageTable(): clockHand(0) {}
    
    // Destructor
    ~PageTable() {}

    // Mostrar la tabla 
    void mostrarTabla() {
        cout << "Frame id\tPageId\tDirtyBit\tPin Count\tLast Used" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 << entries[i].LastUsed << endl;
        }
        cout << endl;
    }

    // Verificar si una página dada existe en la tabla de páginas
    bool verificarExistenciaDePagina(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                return true;
            }
        }
        return false;
    }

    // Obtener el número de marco asociado a una página dada
    int getNumFrame(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                return entries[i].FrameId;
            }
        }
        return -1; // Si la página no se encuentra
    }

    // Disminuir el contador de pines (PinCount) de una página dada
    void descontarPinCount(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].PinCount--;
                break;
            }
        }
    }

    // Aumentar el contador de pines (PinCount) de una página dada
    void aumentarPinCount(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].PinCount++;
                break;
            }
        }
    }

    // Cambiar el bit de suciedad (DirtyBit) de una página dada entre 0 y 1
    void cambiarDirty(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].DirtyBit++;
                break;
            }
        }
    }

    void incrementarLastUsed(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].LastUsed++;
                break;
            }
        }
    }

    // Decrementar el valor de LastUsed de una página dada
    void decrementarLastUsed(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                if (entries[i].LastUsed > 0) {
                    entries[i].LastUsed--;
                }
                break;
            }
        }
    }

    // Actualizar los datos en la tabla de páginas cuando se solicita una página
    void actualizarDatos(int numPaginaActualizar, int numFilaFrameId) {
        entries.push_back(PageEntry(numFilaFrameId, numPaginaActualizar));
        aumentarPinCount(numPaginaActualizar); // Incrementar el contador de pines
    }

    void agregarEntrada(int frameId, int pageId) {
        entries.emplace_back(frameId, pageId);
    }

    bool verificarExistenciaDePagina(int numPagina) const {
        for (const auto& entry : entries) {
            if (entry.PageId == numPagina) {
                return true;
            }
        }
        return false;
    }
    int obtenerFrameLRU() const {
    int lruIndex = -1;
    int lruValue = INT_MAX;
    for (const auto& entry : entries) {
        if (entry.PinCount == 0 && entry.LastUsed < lruValue) {
            lruValue = entry.LastUsed;
            lruIndex = entry.FrameId;
        }
    }
    return lruIndex;
}
    int obtenerFrameClock() {
    while (true) {
        PageEntry& entry = entries[clockHand];
        if (entry.ReferenceBit) {
            entry.ReferenceBit = false;
            clockHand = (clockHand + 1) % entries.size();
        } else {
            int frameId = entry.FrameId;
            clockHand = (clockHand + 1) % entries.size();
            return frameId;
        }
    }
}

};



class BufferManager {

public:
    enum class ReplacementPolicy { LRU, CLOCK };
    BufferManager(size_t num_frames,ReplacementPolicy policy) : bufferPool(num_frames),policy(policy) {
        // Asignar números de página a archivos
        for (int i = 1; i <= 16; ++i) {
            std::string nombre_archivo = "bloque_" + std::to_string(i) + ".txt";
            std::string ruta = "Bloques/" + nombre_archivo;
            archivo_a_numero[ruta] = i;
        }

    }
    
    std::string obtenerNombreArchivo(int numPagina) {
        // Buscar el nombre del archivo correspondiente al número de página
        for (const auto& pair : archivo_a_numero) {
            if (pair.second == numPagina) {
                return pair.first;
            }
        }
        return ""; // Devolver una cadena vacía si no se encuentra el archivo
    }

    void cargarPaginaEnBufferPool(size_t frame_num, const string& nombre_archivo) {
        bufferPool.cargarPaginaAlFrame(frame_num, nombre_archivo);

    }

    void consultarPagina(int numPagina) {
        if (pageTable.verificarExistenciaDePagina(numPagina)) {
            pageTable.aumentarPinCount(numPagina);
            cout << "el contador Pin Count aumento en 1" << endl;
        } else {
            size_t frame_num = bufferPool.buscarFrameVacio();
            if (frame_num != bufferPool.getNumFrames()) {
                std::string nombre_archivo = obtenerNombreArchivo(numPagina);
                cargarPaginaEnBufferPool(frame_num, nombre_archivo);
                pageTable.agregarEntrada(frame_num, numPagina);
                pageTable.aumentarPinCount(numPagina);
                std::cout << "La página " << numPagina << " ha sido cargada en el frame " << frame_num << " y el contador de PinCount ha sido incrementado." << std::endl;
            } else {
                std::cout << "No hay frames vacíos disponibles para cargar la página." << std::endl;
            }
        }
    }

    void cancelarSolicitud(int numPagina){
        if (pageTable.verificarExistenciaDePagina(numPagina)) {
            pageTable.descontarPinCount(numPagina);
            cout << "el contador Pin Count disminuyo en 1" << endl;
        }
        else{
            cout << "la pagina no esta siendo solicitado" << endl;
        }
    }

    void LecturaEscritura(int numPagina){
    // Verificar la existencia de la página en la tabla de páginas
        if (!pageTable.verificarExistenciaDePagina(numPagina)) {
            // Si la página no está en la tabla de páginas, imprimir un mensaje de error y salir de la función
            cout << "La página " << numPagina << " no está en la memoria." << endl;
            return;
        }

        int opcion;
        cout << "Ingrese 1 para lectura o 2 para escritura: "; 
        cin >> opcion;

        // Incrementar el registro "Last Used" independientemente de si es una operación de lectura o escritura
        pageTable.incrementarLastUsed(numPagina);

        // Manejar la operación según la opción ingresada por el usuario
        switch(opcion){
            case 1: // Lectura
                cout << "Realizando operación de lectura en la página " << numPagina << "." << endl;
                break;
            case 2: // Escritura
                cout << "Realizando operación de escritura en la página " << numPagina << "." << endl;
                // Marcar la página como modificada (Dirty Bit)
                pageTable.cambiarDirty(numPagina);
                break;
            default:
                cout << "Opción no válida." << endl;
                break;
        }
    }



    void mostrarContenidoFrame(int numFrame) {
        bufferPool.mostrarContenidoFrame(numFrame);
    }

    void mostrarTablaDePaginas() {
        pageTable.mostrarTabla();
    }
    size_t reemplazarPagina() {
    int frame_num = (policy == ReplacementPolicy::LRU) ? pageTable.obtenerFrameLRU() : pageTable.obtenerFrameClock();
    if (frame_num == -1) {
        std::cerr << "Error: no se encontró un frame para reemplazar." << std::endl;
        std::exit(1);
    }
    return frame_num;
}
private:
    ReplacementPolicy policy;
    BufferPool bufferPool;
    PageTable pageTable;
    std::unordered_map<std::string, int> archivo_a_numero;

};

int main() {
    BufferManager* bufferManager = nullptr;
    int numero = 4;
    int opcion = 0;

    while (opcion != 6) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Crear BufferPool\n";
        std::cout << "2. Consultar página en BufferPool\n";
        std::cout << "3. Lectura/Escritura de una página en BufferPool\n";
        std::cout << "4. Mostrar contenido de un frame\n";
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1: {
                size_t num_frames,policy_choice;
                std::cout << "Ingrese el número de frames para el BufferPool: ";
                std::cin >> num_frames;
                std::cout << "Seleccione política de reemplazo (1. LRU, 2. CLOCK): ";
                std::cin >> policy_choice;
                auto policy = (policy_choice == 1) ? BufferManager::ReplacementPolicy::LRU
                                                   : BufferManager::ReplacementPolicy::CLOCK;
                bufferManager = new BufferManager(num_frames, policy);
                std::cout << "BufferPool creado con " << num_frames << " frames." << std::endl;
                break;
            }
            case 2: {
                int opcion;
                int pagina;
                cout << "ingrese 1 si desea solicitud o 2 para cancelar solicitud de una pagina"; cin >> opcion;
                if(opcion == 1){
                    cout << "Ingrese el número de página que desea consultar: ";
                    cin >> pagina;
                    bufferManager->consultarPagina(pagina);
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
                    bufferManager->mostrarTablaDePaginas();
                }
                break;
            }

            case 5: {
                cout << "saliedo del programa..." << endl;
                break;
            }
        }
    }
}