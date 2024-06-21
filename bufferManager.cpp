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


// Clase Pagina para manejar el contenido de cada página
class Pagina {
private:
    int tamanioTotal = 512;
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

    void insertarTexto(const std::string& texto) {
        contenido.push_back(texto);
    }

    void GuardarBloque(const std::string& ruta) {
        std::ofstream archivo(ruta);
        if (archivo.is_open()) {
            for (const auto& linea : contenido) {
                archivo << linea << "\n";
            }
            archivo.close();
            std::cout << "Contenido guardado en " << ruta << std::endl;
        } else {
            std::cerr << "No se pudo abrir el archivo para escritura: " << ruta << std::endl;
        }
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

    void reemplazarPagina(const std::string& nombre_archivo) {
        pagina = Pagina(nombre_archivo);
    }

    void insertarTexto(const std::string& texto) {
        pagina.insertarTexto(texto);
    }

    void GuardarBloque(string ruta){
        pagina.GuardarBloque(ruta);
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

    void insertarTextoEnPagina(size_t numero_frame) {
        if (numero_frame < frames.size()) {
            std::string texto;
            std::cout << "Ingrese el texto que desea insertar en el frame " << numero_frame << ": ";
            cin >> texto; // Leer texto desde la entrada estándar

            frames[numero_frame].insertarTexto(texto);
        } else {
            std::cout << "Número de frame inválido." << std::endl;
        }
    }

    void GuardarBloque(int numero_pagina, int frame){
        std::string nombre_archivo = "bloque_" + std::to_string(numero_pagina) + ".txt";
        std::string ruta = "Bloques/" + nombre_archivo;

        frames[frame].GuardarBloque(ruta);
    }
    

};


class PageTable {
private:
    int manecilla = 0; // puntero para el algoritmo del reloj

    struct PageEntry {
        int FrameId;
        int PageId;
        int DirtyBit;
        int PinCount;
        int BirtyBit;
        int LastUsed;
        bool Pineed;

        PageEntry(int frameId, int pageId) : FrameId(frameId), PageId(pageId), DirtyBit(0), PinCount(0), BirtyBit(0),LastUsed(0), Pineed(false){}
    };

    


public:
vector<PageEntry> entries;
    // Constructor
    PageTable(){}

    // Destructor
    ~PageTable() {}

    int obtenerFrameId(int pageId) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == pageId) {
                return entries[i].FrameId;
            }
        }
        cout << "No se encontró ninguna página con PageId " << pageId << "." << endl;
        return -1; // Indica que no se encontró la página
    }


    bool isDirty(int pageId) {
        for (const auto& entry : entries) {
            if (entry.PageId == pageId) {
                return entry.DirtyBit == 1;
            }
        }
        // Si no se encuentra la página, podríamos lanzar una excepción o devolver false
        return false;
    }

    // Insertar pinned
    void insertarPinned(int pageId) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == pageId) {
                entries[i].Pineed = true;
                cout << "Página con PageId " << pageId << " marcada como Pinned." << endl;
                return;
            }
        }
        cout << "No se encontró ninguna página con PageId " << pageId << "." << endl;
    }

    // Mostrar la tabla 
    void mostrarTabla() {
        cout << "Frame id\tPageId\tDirtyBit\tPin Count\tReferenceBit\tPinned" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 << entries[i].BirtyBit << "\t\t" << (entries[i].Pineed ? 1 : 0) << endl;
        }
        cout << endl;
    }

    void mostrarTablaLRU(){
        cout << "Frame id\tPageId\tPin Count\tLastUsed\tPinned" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 <<"\t\t" <<entries[i].LastUsed << endl;
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
    void AumentarDirty(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].DirtyBit = 1;
                break;
            }
        }
    }

    void DecrementarDirty(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].DirtyBit = 0;
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

    void incrementaBirtyBit(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].BirtyBit = 1;
                break;
            }
        }
    }

    // Decrementar el valor de BirtyBit de una página dada
    void decrementaBirtyBit(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                if (entries[i].BirtyBit > 0) {
                    entries[i].BirtyBit = 0;
                }
                break;
            }
        }
    }

  /*   void aumentarManecilla(){
        manecilla = manecilla + 1;
    }
 */
    // Actualizar los datos en la tabla de páginas cuando se solicita una página
    
    void replaceReloj(int frameId, int pageId) {
        if (entries.empty()) {
            // Si la tabla está vacía, simplemente añade la nueva entrada
            agregarEntrada(frameId, pageId);
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
            // Imprimir todos los frames en uso
            cout << "Todos los frames están en uso:" << endl;
            for (size_t i = 0; i < entries.size(); ++i) {
                cout << "Frame " << i << ": PageId=" << entries[i].PageId << ", PinCount=" << entries[i].PinCount << endl;
            }
            return;
        }

        while (true) {
            // Si encontramos un frame con BirtyBit en 0, PinCount en 0 y Pineed en false, reemplazamos la página
            if (!entries[manecilla].Pineed && entries[manecilla].BirtyBit == 0 && entries[manecilla].PinCount == 0 && entries[manecilla].DirtyBit == 0) {
                entries[manecilla].PageId = pageId;
                entries[manecilla].BirtyBit = 1; // Establecer BirtyBit en 1 porque ahora está recién usado
                manecilla = (manecilla + 1) % entries.size(); // Mover el puntero al siguiente frame
                break;
            } 
            
            else if(entries[manecilla].Pineed == true){
                manecilla = (manecilla + 1) % entries.size();
            }
            
            else if(entries[manecilla].Pineed == false){
                if(entries[manecilla].BirtyBit == 1){
                    entries[manecilla].BirtyBit = 0;
                }
                else{
                    entries[manecilla].BirtyBit = 1;
                }
                manecilla = (manecilla + 1) % entries.size();
            }

            
           
        }
    }



    void aumentarManecilla(int numeroFrames){
        manecilla = (manecilla + 1) % numeroFrames;
    }

     void mostrarManecilla() {
       cout << "la manecilla esta apuntado al frame: " << manecilla << endl;
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
            &&entries[i].DirtyBit==0&&entries[i].Pineed==false) {
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
            pageTable.aumentarPinCount(numPagina);
            return;
        }

        size_t frame_num = bufferPool.buscarFrameVacio();
        if (frame_num == bufferPool.getNumFrames()) {
            // No hay marcos vacíos, es necesario reemplazarlos
            pageTable.replaceReloj(bufferPool.getNumFrames(), numPagina);
            string nombre_archivo = obtenerNombreArchivo(numPagina);

            int numeroFrameReloj = pageTable.getManecilla() - 1;

            if(numeroFrameReloj == 0){
                numeroFrameReloj = bufferPool.getNumFrames() - 1;
            }


            bufferPool.cargarPaginaAlFrame(numeroFrameReloj, nombre_archivo);
            pageTable.aumentarPinCount(numPagina);

        } else {
            // Hay un marco vacío, carga la página en ese marco
            string nombre_archivo = obtenerNombreArchivo(numPagina);
            bufferPool.cargarPaginaAlFrame(frame_num, nombre_archivo);
            pageTable.agregarEntrada(frame_num, numPagina);
            pageTable.incrementaBirtyBit(numPagina);
            pageTable.aumentarPinCount(numPagina);
            pageTable.aumentarManecilla(bufferPool.getNumFrames());
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
            cout << "el contadozr Pin Count disminuyo en 1" << endl;
        }
        else{
            cout << "la pagina no esta siendo solicitado" << endl;
        }
    }

    void LecturaEscritura1(int numPagina){
    // Verificar la existencia de la página en la tabla de páginas
        bool paginaExiste = pageTable.verificarExistenciaDePagina(numPagina);
        
        if (!paginaExiste) {
            // La página no existe, proceder con la carga y reemplazo si es necesario
            size_t frame_num = bufferPool.buscarFrameVacio();
            
            if (frame_num == bufferPool.getNumFrames()) {
                // No hay marcos vacíos, es necesario reemplazarlos
                pageTable.replaceReloj(bufferPool.getNumFrames(), numPagina);
                string nombre_archivo = obtenerNombreArchivo(numPagina);

                int numeroFrameReloj = pageTable.getManecilla() - 1;
                if (numeroFrameReloj == 0) {
                    numeroFrameReloj = bufferPool.getNumFrames() - 1;
                }

                bufferPool.cargarPaginaAlFrame(numeroFrameReloj, nombre_archivo);
            } else {
                // Hay un marco vacío, carga la página en ese marco
                string nombre_archivo = obtenerNombreArchivo(numPagina);
                bufferPool.cargarPaginaAlFrame(frame_num, nombre_archivo);
                pageTable.agregarEntrada(frame_num, numPagina);
                pageTable.incrementaBirtyBit(numPagina);
                pageTable.aumentarManecilla(bufferPool.getNumFrames());
            }

        }

         else{
            if(pageTable.entries[numPagina].BirtyBit == 0){
                pageTable.incrementaBirtyBit(numPagina);
            }
        }
        

        // Solicitar operación de lectura o escritura
        int opcion;
        cout << "Ingrese 1 para lectura o 2 para escritura: "; 
        cin >> opcion;

        Requerimiento req = {numPagina, opcion};
        Requerimientos.push(req);
        // Manejar la operación según la opción ingresada por el usuario
        switch(opcion){
            case 1: // Lectura
                if(pageTable.isDirty(numPagina) == true){
                    cout << "Error en el proceso de requerimineto , el dity bit es 1" << endl;
                    cout << "Para procesar el proceso se debe guardar el archivo" << endl;
                    cout << "1. Guardar contenido" << endl;
                    cout << "2. No guardar contenido" << endl;
                    cout << "opcion : ";
                    int opcion;
                    cin >> opcion;
                    if(opcion == 1){
                        cout << "El contenido de la pagina ha sido guardado" << endl;
                        pageTable.aumentarPinCount(numPagina);
                        pageTable.DecrementarDirty(numPagina);
                    }
                    else{
                        cout << "El requerimiento sera guardado en la cola de espera" << endl;
                        Requerimiento requerimiento = {numPagina, 2};
                        //Espera.push(requerimiento);
                    }

                }
                else{
                    cout << "Realizando operación de lectura en la página " << numPagina << "." << endl;
                    pageTable.aumentarPinCount(numPagina);
                }
                break;
            case 2: // Escritura
                cout << "Realizando operación de escritura en la página " << numPagina << "." << endl;
                // Marcar la página como modificada (Dirty Bit)
                pageTable.AumentarDirty(numPagina);
                pageTable.aumentarPinCount(numPagina);


                break;
            default:
                cout << "Opción no válida." << endl;
                break;
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
        // Manejar la operación según la opción ingresada por el usuario
        switch(opcion){
            case 1: // Lectura
                cout << "Realizando operación de lectura en la página " << numPagina << "." << endl;
                pageTable.incrementarLastUsed(numPagina);

                break;
            case 2: // Escritura
                cout << "Realizando operación de escritura en la página " << numPagina << "." << endl;
                // Marcar la página como modificada (Dirty Bit)
                pageTable.AumentarDirty(numPagina);
                pageTable.incrementarLastUsed(numPagina);

                break;
            default:
                cout << "Opción no válida." << endl;
                break;
        }
    }

    void GuardarValorerDisco(int pagina){

        cout << pageTable.entries[pagina].DirtyBit << endl;

        if(pageTable.entries[pagina].DirtyBit == 1){
            bufferPool.insertarTextoEnPagina(pagina);
            pageTable.DecrementarDirty(pagina);
            return;
        }
        cout << "No se puede insertar , porque el dirtyBit no es 1" << endl;
        
            
        
    }

    void GuardarBloque(int pagina){
      
            bufferPool.GuardarBloque(pagina,pageTable.getNumFrame(pagina));
            pageTable.DecrementarDirty(pagina);
    }

       

    void InsertarPinned(int pagina){
        pageTable.insertarPinned(pagina);
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

    void llenarLongitudFija(const string& archivoTxt, const string& esquema) {
        ifstream inFile(archivoTxt);
        if (!inFile.is_open()) {
            cerr << "Error al abrir el archivo de registros" << endl;
            return;
        }

        string line;
        vector<vector<string>> blocks(16);
        int tamanioTotal = 1024;

        while (getline(inFile, line)) {
            int tamanioRegistro = calcularTamanioFijoLinea(esquema);
            bool inserted = false;

            cout << "tamanio registro: " << tamanioRegistro << endl;

            for (int i = 0; i < 16; ++i) {
                int currentBlockSize = 0;
                for (const auto& registro : blocks[i]) {
                    currentBlockSize += calcularTamanioFijoLinea(esquema);
                }

                if (currentBlockSize + tamanioRegistro <= tamanioTotal) {
                    //cout << "tamanio registro: " << current_exception << endl;
                    blocks[i].push_back(line);
                    inserted = true;
                    break;
                }
            }
         


            if (!inserted) {
                cerr << "No hay espacio suficiente para insertar el registro: " << line << endl;
            }
        }

        inFile.close();

        // Escribir los bloques en archivos
        for (int i = 0; i < 16; ++i) {
            std::string nombre_archivo = "bloque_" + std::to_string(i) + ".txt";
            std::string ruta = "Bloques/" + nombre_archivo;
            string blockFileName = ruta;
            ofstream outFile(blockFileName);
            if (!outFile.is_open()) {
                cerr << "Error al abrir el archivo de salida para el bloque " << blockFileName << endl;
                continue;
            }

            for (const auto& registro : blocks[i]) {
                outFile << registro << endl;
            }

            outFile.close();
        }
    }

    void llenarLongitudVariable(const string& archivoTxt, const string& esquema) {
        ifstream inFile(archivoTxt);
        if (!inFile.is_open()) {
            cerr << "Error al abrir el archivo de registros" << endl;
            return;
        }

        string line;
        vector<vector<string>> blocks(16);
        int tamanioTotal = 512;

        // Ignorar la primera línea del archivo (cabecera)
        getline(inFile, line);

        while (getline(inFile, line)) {
            int tamanioRegistro = calcularTamanioLineaVariable(line, esquema);
            bool inserted = false;

            for (int i = 0; i < 16; ++i) {
                int currentBlockSize = 0;
                for (const auto& registro : blocks[i]) {
                    currentBlockSize += calcularTamanioLineaVariable(registro, esquema);
                }

                if (currentBlockSize + tamanioRegistro <= tamanioTotal) {
                    blocks[i].push_back(line);
                    inserted = true;
                    cout << "Registro insertado en el bloque " << i + 1 << ": " << line << endl; // Mensaje de depuración
                    break;
                }
            }

            if (!inserted) {
                cerr << "No hay espacio suficiente para insertar el registro: " << line << endl;
            }
        }

        inFile.close();

        // Escribir los bloques en archivos
        for (int i = 0; i < 16; ++i) {
            std::string nombre_archivo = "bloque_" + std::to_string(i) + ".txt";
            std::string ruta = "Bloques/" + nombre_archivo;
            string blockFileName = ruta;
            ofstream outFile(blockFileName);
            if (!outFile.is_open()) {
                cerr << "Error al abrir el archivo de salida para el bloque " << blockFileName << endl;
                continue;
            }

            for (const auto& registro : blocks[i]) {
                outFile << registro << endl;
            }

            outFile.close();
        }
    }
};

int main() {
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