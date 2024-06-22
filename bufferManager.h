#include "pageTable.h"
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
