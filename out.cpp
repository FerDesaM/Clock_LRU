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
            pagina=Pagina(nombre_archivo);
        }
    }

    void mostrarPagina() {
        if (!estaVacio()) {
            std::cout << pagina.mostrarContenido();
        } else {
            std::cout << "El frame está vacío." << std::endl;
        }
    }
     void limpiar() {
        pagina = Pagina(); // Crea una nueva instancia de Pagina vacía para borrar el contenido actual
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
        int BirtyBit;
        int LastUsed;
        bool Pinned;
        PageEntry(int frameId, int pageId) : FrameId(frameId), PageId(pageId), DirtyBit(0), PinCount(0), BirtyBit(0),LastUsed(0) {}
    };

    
    int manecilla = 0; // puntero para el algoritmo del reloj


public:
vector<PageEntry> entries;
    // Constructor
    PageTable() {}

    // Destructor
    ~PageTable() {}

    // Mostrar la tabla 
    void mostrarTabla() {
        cout << "Frame id\tPageId\tDirtyBit\tPin Count\tBirtiBit" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 << entries[i].BirtyBit << endl;
        }
        cout << endl;
    }
    void mostrarTabla2() {
        cout << "Frame id\tPageId\tDirtyBit\tPin Count\tLastUsed" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 << entries[i].LastUsed << endl;
        }
        cout << endl;
    }
    void mostrarTablaLRU(){
        cout << "Frame id\tPageId\tDirtyBit\tPin Count\tLastUsed\tPinned" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 << entries[i].LastUsed <<"\t"<< entries[i].Pinned<<endl;
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
    void insertarPinned(int pageId) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == pageId) {
                entries[i].Pinned = true;
                cout << "Página con PageId " << pageId << " marcada como Pinned." << endl;
                return;
            }
        }
        cout << "No se encontró ninguna página con PageId " << pageId << "." << endl;
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
                entries[i].DirtyBit=1;
                break;
            }
        }
    }
    void decrementarDirty(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].DirtyBit=0;
                break;
            }
        }
    }
    //En caso de que el usuario ya ha
    
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

    void incrementaBirtyBit(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].BirtyBit++;
                break;
            }
        }
    }

    // Decrementar el valor de BirtyBit de una página dada
    void decrementaBirtyBit(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                if (entries[i].BirtyBit > 0) {
                    entries[i].BirtyBit--;
                }
                break;
            }
        }
    }

    // Actualizar los datos en la tabla de páginas cuando se solicita una página
    
    void replaceReloj(int numFrames, int pageId) {
        if (entries.empty()) {
            // If the table is empty, simply add the new entry
            agregarEntrada(numFrames, pageId);
            return;
        }

        bool allFramesInUse = true;

        for (const auto& entry : entries) {
            if (entry.PinCount == 0) {
                allFramesInUse = false;
                break;
            }
        }

        if (allFramesInUse) {
            // Print all frames in use
            cout << "Todos los frames están en uso:" << endl;
            for (size_t i = 0; i < entries.size(); ++i) {
                cout << "Frame " << i << ": PageId=" << entries[i].PageId << ", PinCount=" << entries[i].PinCount << endl;
            }
            return;
        }

        while (true) {
            // If we find a frame with BirtyBit as 0 and PinCount as 0, replace the page
            if (entries[manecilla].BirtyBit == 0 && entries[manecilla].PinCount == 0) {
                entries[manecilla].PageId = pageId;
                entries[manecilla].BirtyBit = 1; // Set BirtyBit to 1 because it's now recently used
                manecilla = (manecilla + 1) % entries.size(); // Move the pointer to the next frame
                break;
            } else {
                // If BirtyBit is 1 or PinCount is greater than 0, set BirtyBit to 0 and move the pointer
                entries[manecilla].BirtyBit = 0;
                manecilla = (manecilla + 1) % entries.size();
            }
        }
    }


     void mostrarManecilla() {
        if (entries.empty()) {
            cout << "La tabla de páginas está vacía." << endl;
        } else {
            cout << "La manecilla está apuntando al frame: " << entries[manecilla].FrameId << endl;
        }
    }

    int getManecilla(){
        return manecilla;
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
    for (size_t i=0 ;i<entries.size(); ++i) {
        if (entries[i].PinCount == 0 && entries[i].LastUsed < lruValue
            &entries[i].DirtyBit==0&&entries[i].Pinned==false) {
            lruValue = entries[i].LastUsed;
            lruIndex = i;
        }
    }
    return lruIndex;
}


};



class BufferManager {
private:
    BufferPool bufferPool;
    PageTable pageTable;
    std::unordered_map<std::string, int> archivo_a_numero;

public:
    
    BufferManager(size_t num_frames) : bufferPool(num_frames) {
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
            pageTable.incrementaBirtyBit(numPagina);
            pageTable.aumentarPinCount(numPagina);
            return;
        }

        size_t frame_num = bufferPool.buscarFrameVacio();
        if (frame_num == bufferPool.getNumFrames()) {
            // No empty frames, replacement is required
            pageTable.replaceReloj(bufferPool.getNumFrames(), numPagina);
        } else {
            // There's an empty frame, load the page into that frame
            string nombre_archivo = obtenerNombreArchivo(numPagina);
            bufferPool.cargarPaginaAlFrame(frame_num, nombre_archivo);
            pageTable.agregarEntrada(frame_num, numPagina);
            pageTable.incrementaBirtyBit(numPagina);
            pageTable.aumentarPinCount(numPagina);
        }
    }

    void consultarPagina2(int numPagina) {
    if (pageTable.verificarExistenciaDePagina(numPagina)) {
        // Si la página ya está en la tabla de páginas
        pageTable.aumentarPinCount(numPagina);
        pageTable.incrementarLastUsed(numPagina);
        std::cout << "El contador Pin Count de la página " << numPagina << " se ha aumentado en 1." << std::endl;
    } else {
        // Si la página no está en la tabla de páginas
        size_t frame_num = bufferPool.buscarFrameVacio();
        if (frame_num != bufferPool.getNumFrames()) {
            // Si hay un frame vacío disponible en el buffer pool
            std::string nombre_archivo = obtenerNombreArchivo(numPagina);
            bufferPool.cargarPaginaAlFrame(frame_num, nombre_archivo);
            pageTable.agregarEntrada(frame_num, numPagina);
            pageTable.aumentarPinCount(numPagina);
            std::cout << "La página " << numPagina << " ha sido cargada en el frame " << frame_num << " y el contador de PinCount se ha incrementado." << std::endl;
        } else {
            // Si no hay frames vacíos disponibles, se aplica el algoritmo LRU
            std::cout << "No hay frames disponibles. Aplicando LRU..." << std::endl;
            size_t lruFrame = pageTable.obtenerFrameLRU();
            if (lruFrame != -1) {
                // Se obtiene el número de página asociado al frame LRU
                int lruPageId = pageTable.entries[lruFrame].PageId;
                // Actualizar la entrada existente en lugar de eliminarla
                std::string nombre_archivo = obtenerNombreArchivo(numPagina);
                bufferPool.cargarPaginaAlFrame(lruFrame, nombre_archivo);
                // Actualizar la entrada en la tabla de páginas
                pageTable.entries[lruFrame].PageId = numPagina;
                pageTable.aumentarPinCount(numPagina);
                std::cout << "La página " << numPagina << " ha sido cargada en el frame " << lruFrame << " y el contador de PinCount se ha incrementado." << std::endl;
            } else {
                std::cout << "No hay frames disponibles ni páginas LRU para reemplazar." << std::endl;
                // Aquí podrías considerar algún manejo adicional, como liberar espacio en el buffer pool o manejar un error.
            }
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
        pageTable.incrementaBirtyBit(numPagina);
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

    void mostrarManecilla() {
        pageTable.mostrarManecilla();
    }

    void mostrarContenidoFrame(int numFrame) {
        bufferPool.mostrarContenidoFrame(numFrame);
    }

    void mostrarTablaDePaginas() {
        pageTable.mostrarTabla();
    }
    void mostrarTablaDePaginas2() {
        pageTable.mostrarTablaLRU();
    }
    void InsertarPinned(int pagina){
        pageTable.insertarPinned(pagina);
    }
};

int main() {
    BufferManager* bufferManager = nullptr;

    int elegirAlgoritmo;
    cout << "presione 1 si quiere usar el algoritmo de clow o 2 si desea el lru: "; cin >> elegirAlgoritmo;

    if(elegirAlgoritmo == 1){

        int opcion = 0;
        while (opcion != 6) {
            std::cout << "\nMenu:\n";
            std::cout << "1. Crear BufferPool\n";
            std::cout << "2. Consultar pagina en BufferPool\n";
            std::cout << "3. Lectura/Escritura de una pagina en BufferPool\n";
            std::cout << "4. Mostrar tabla\n";
            std::cout << "5. Mostrar contenido de frame\n";
            std::cout << "6. Salir\n";
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
                        bufferManager->mostrarManecilla();
                    }
                    break;
                }

                case 5: {
                    int frame;
                    cout << "ingrese numero de frame "; cin >> frame;
                    bufferManager->mostrarContenidoFrame(frame);
                    break;
                }

                case 6: {
                    cout << "saliedo del programa..." << endl;
                    break;
                }
            }
        }

    }

    else if (elegirAlgoritmo ==  2){
        int opcion = 0;
        while (opcion != 7) {
            std::cout << "\nMenu:\n";
            std::cout << "1. Crear BufferPool\n";
            std::cout << "2. Consultar pagina en BufferPool\n";
            std::cout << "3. Lectura/Escritura de una pagina en BufferPool\n";
            std::cout << "4. Mostrar tabla\n";
            std::cout << "5. Mostrar contenido de frame\n";
            std::cout<<  "6. Pinned pagina\n";
            std::cout << "7. Salir\n";
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
                    cout << "saliedo del programa..." << endl;
                    break;
                }
            }
        }
    }
    
}
