#include "pageTable.h"
#include "hardDisk.h"
#include "calcular.h"
#include "heapfile.h"

heapFile heap;



class BufferManager {                       // JOSE ALEJANDRO MACHACA MUÑIZ                        
private:
    BufferPool bufferPool;
    PageTable pageTable;
    //std::unordered_map<std::string, int> archivo_a_numero;

public:
   
    BufferManager(size_t num_frames) : bufferPool(num_frames) {}    // JOSE ALEJANDRO MACHACA MUÑIZ  
    
      /* std::string obtenerNombreArchivo(int numPagina) {                   // JOSE ALEJANDRO MACHACA MUÑIZ
        // Buscar el nombre del archivo correspondiente al número de página
        for (const auto& pair : archivo_a_numero) {
            if (pair.second == numPagina) {
                return pair.first;
            }
        }
        return ""; // Devolver una cadena vacía si no se encuentra el archivo
    } 
  */
    void cargarPaginaEnBufferPool(size_t frame_num, const string& nombre_archivo) {  // JOSE ALEJANDRO MACHACA MUÑIZ  
        bufferPool.cargarPaginaAlFrame(frame_num, nombre_archivo);

    }

    
    
    void consultarPagina(int numPagina) {               // JOSE ALEJANDRO MACHACA MUÑIZ 
        
        if(numPagina - 1 < 0 || numPagina - 1 >= bloques.size()){
            cout << "fuera de limites de los bloques" << endl;
            return;
        }


        if (pageTable.verificarExistenciaDePagina(numPagina)) {
            pageTable.aumentarPinCount(numPagina);
            return;
        }

        size_t frame_num = bufferPool.buscarFrameVacio();
        if (frame_num == bufferPool.getNumFrames()) {
            // No hay marcos vacíos, es necesario reemplazarlos
            pageTable.replaceReloj(bufferPool.getNumFrames(), numPagina);
            //string nombre_archivo = obtenerNombreArchivo(numPagina); // cambiar

            int numeroFrameReloj = pageTable.getManecilla() - 1;

            if(numeroFrameReloj == 0){
                numeroFrameReloj = bufferPool.getNumFrames() - 1;
            }


            bufferPool.cargarPaginaAlFrame2(numeroFrameReloj, bloques[numPagina - 1].imprimirDireccion()); // cambiar
            pageTable.aumentarPinCount(numPagina);
            

        } else {
            // Hay un marco vacío, carga la página en ese marco
           // string nombre_archivo = obtenerNombreArchivo(numPagina);     
            bufferPool.cargarPaginaAlFrame2(frame_num, bloques[numPagina - 1].imprimirDireccion());   // cambiar 
            pageTable.agregarEntrada(frame_num, numPagina);
            pageTable.incrementaBirtyBit(numPagina);
            pageTable.aumentarPinCount(numPagina);
            pageTable.aumentarManecilla(bufferPool.getNumFrames());
        }
    }

    void consultarPagina2(int numPagina) {      //FERNANDO DEZA SOTOMAYOR                      
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
            //std::string nombre_archivo = obtenerNombreArchivo(numPagina);
            bufferPool.cargarPaginaAlFrame(frame_num, bloques[numPagina - 1].imprimirDireccion());
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
                //std::string nombre_archivo = obtenerNombreArchivo(numPagina);
                bufferPool.cargarPaginaAlFrame(lruFrame, bloques[numPagina - 1].imprimirDireccion());
                // Actualizar la entrada en la tabla de páginas
                pageTable.entries[lruFrame].PageId = numPagina;
                pageTable.aumentarPinCount(numPagina);
                std::cout << "La página " << numPagina << " ha sido cargada en el frame " << lruFrame << " y el contador de PinCount se ha incrementado." << std::endl;
            } else {
                std::cout << "No hay frames disponibles ni páginas LRU para reemplazar." << std::endl;
            }
        }
    }
}


    void cancelarSolicitud(int numPagina){                          // JOSE ALEJANDRO MACHACA MUÑIZ 
        if (pageTable.verificarExistenciaDePagina(numPagina)) {
            pageTable.descontarPinCount(numPagina);
            cout << "el contadozr Pin Count disminuyo en 1" << endl;
        }
        else{
            cout << "la pagina no esta siendo solicitado" << endl;
        }
    }

    void LecturaEscritura1(int numPagina){                          // JOSE ALEJANDRO MACHACA MUÑIZ 
    // Verificar la existencia de la página en la tabla de páginas
        bool paginaExiste = pageTable.verificarExistenciaDePagina(numPagina);
        
        if (!paginaExiste) {
            // La página no existe, proceder con la carga y reemplazo si es necesario
            size_t frame_num = bufferPool.buscarFrameVacio();
            
            if (frame_num == bufferPool.getNumFrames()) {
                // No hay marcos vacíos, es necesario reemplazarlos
                pageTable.replaceReloj(bufferPool.getNumFrames(), numPagina);
                //string nombre_archivo = obtenerNombreArchivo(numPagina);

                int numeroFrameReloj = pageTable.getManecilla() - 1;
                if (numeroFrameReloj == 0) {
                    numeroFrameReloj = bufferPool.getNumFrames() - 1;
                }

                bufferPool.cargarPaginaAlFrame2(numeroFrameReloj, bloques[numPagina - 1].imprimirDireccion()); // cambiar
            } else {
                // Hay un marco vacío, carga la página en ese marco
                //string nombre_archivo = obtenerNombreArchivo(numPagina);
                bufferPool.cargarPaginaAlFrame2(frame_num, bloques[numPagina - 1].imprimirDireccion()); // cambiar
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
                    cout << "Para procesar el proceso se debe MODIFICAR LA PAGINA" << endl;
                    cout << "1. MODIFICAR PAGINA" << endl;
                    cout << "2. NO MODIFICAR PAGINA" << endl;
                    cout << "opcion : ";
                    int opcion;
                    cin >> opcion;
                    if(opcion == 1){
                        int opcion2;
                        cout << "1. si desea insertar un nuevo registro" << endl;
                        cout << "2. si desea modificar un registro" << endl;
                        cout << "3. si desea eliminar un registro" << endl;
                        cout << "opcion: "; cin >> opcion2;

                        switch (opcion2)
                        {
                        case 1:
                            int opcion3;
                            cout << "1. si desea insertar el rigistro con longitud fija" << endl;
                            cout << "2. si desea insertar el rigistro con longitud variable" << endl;
                            cout << "opcion: "; cin >> opcion3;
                            if (opcion3 == 1) {
                                ifstream archivoEsquemas("esquemas.txt");
                                if (!archivoEsquemas.is_open()) {
                                    cerr << "No se pudo abrir el archivo esquemas.txt" << endl;
                                    return;
                                }

                                string lineaEsquema;
                                while (getline(archivoEsquemas, lineaEsquema)) {
                                    // Aquí podrías agregar lógica para elegir un esquema específico
                                    // Por simplicidad, usaremos el primer esquema que encontremos
                                    if (lineaEsquema.find("titanic#") != string::npos) {
                                        // Ignorar la parte del nombre del esquema
                                        size_t pos = lineaEsquema.find('#');
                                        if (pos != string::npos) {
                                            string esquema = lineaEsquema.substr(pos + 1);
                                            int tamanioRegistro = calcularTamanioFijoLinea(esquema);
                                            cout << "El tamaño del nuevo registro es: " << tamanioRegistro << " bytes" << endl;
                                            
                                            // Ingresar datos según el esquema
                                            string registroNuevo;
                                            ingresarDatosSegunEsquema(esquema, registroNuevo);
                                            cout << "El registro es: " << registroNuevo << endl;
                                            if(bloques[numPagina - 1].tamanio() - tamanioRegistro >= 0){
                                                int frame = pageTable.obtenerFrameId(numPagina);
                                                cout << "frame" << numPagina << endl;
                                                bufferPool.insertarTextoEnPagina(frame, registroNuevo);
                                                bloques[numPagina - 1].RestarTamanio(tamanioRegistro);
                                                break;
                                            }
                                            else{
                                                cout << "la pagina no tiene suficiente espacio" << endl;
                                                break;
                                            }

                                        }
                                        heap.mostrarInformacionBloques();
                                        break;
                                    }
                                }

                                archivoEsquemas.close();
                            }
                            else if(opcion3 == 2){
                                 ifstream archivoEsquemas("esquemas.txt");
                                if (!archivoEsquemas.is_open()) {
                                    cerr << "No se pudo abrir el archivo esquemas.txt" << endl;
                                    return;
                                }

                                string lineaEsquema;
                                while (getline(archivoEsquemas, lineaEsquema)) {
                                    // Aquí podrías agregar lógica para elegir un esquema específico
                                    // Por simplicidad, usaremos el primer esquema que encontremos
                                    if (lineaEsquema.find("titanic#") != string::npos) {
                                        // Ignorar la parte del nombre del esquema
                                        size_t pos = lineaEsquema.find('#');
                                        if (pos != string::npos) {
                                            string esquema = lineaEsquema.substr(pos + 1);
                                            //int tamanioRegistro = calcularTamanioFijoLinea(esquema);
                                            //cout << "El tamaño del nuevo registro es: " << tamanioRegistro << " bytes" << endl;
                                            
                                            // Ingresar datos según el esquema
                                            string registroNuevo;
                                            ingresarDatosSegunEsquema(esquema, registroNuevo);
                                            int tamanioRegistro = calcularTamanioLineaVariable(registroNuevo, esquema);

                                            cout << "El nuevo registro es: " << registroNuevo << endl;
                                            if(bloques[numPagina - 1].tamanio() - tamanioRegistro >= 0){
                                                int frame = pageTable.obtenerFrameId(numPagina);
                                                cout << "frame" << numPagina << endl;
                                                bufferPool.insertarTextoEnPagina(frame, registroNuevo);
                                                bloques[numPagina - 1].RestarTamanio(tamanioRegistro);
                                                break;
                                            }
                                            else{
                                                cout << "la pagina no tiene suficiente espacio" << endl;
                                                break;
                                            }

                                        }
                                        heap.mostrarInformacionBloques();
                                        break;
                                    }
                                }

                                archivoEsquemas.close();
                            }

                            break;
                        
                        case 2:
                            cout << "modificar registro" << endl;
                            cout << bloques[numPagina - 1].imprimirDireccion();
                            cout << "1. si desea insertar el rigistro con longitud fija" << endl;
                            cout << "2. si desea insertar el rigistro con longitud variable" << endl;
                            cout << "opcion: "; cin >> opcion3;
                            if (opcion3 == 1) {
                                ifstream archivoEsquemas("esquemas.txt");
                                if (!archivoEsquemas.is_open()) {
                                    cerr << "No se pudo abrir el archivo esquemas.txt" << endl;
                                    return;
                                }

                                string lineaEsquema;
                                while (getline(archivoEsquemas, lineaEsquema)) {
                                    // Aquí podrías agregar lógica para elegir un esquema específico
                                    // Por simplicidad, usaremos el primer esquema que encontremos
                                    if (lineaEsquema.find("titanic#") != string::npos) {
                                        // Ignorar la parte del nombre del esquema
                                        size_t pos = lineaEsquema.find('#');
                                        if (pos != string::npos) {
                                            string esquema = lineaEsquema.substr(pos + 1);
                                            int tamanioRegistro = calcularTamanioFijoLinea(esquema);
                                            cout << "El tamaño del nuevo registro es: " << tamanioRegistro << " bytes" << endl;
                                            
                                            // Ingresar datos según el esquema
                                            string registroNuevo;
                                            ingresarDatosSegunEsquema(esquema, registroNuevo);
                                            cout << "El nuevo registro es: " << registroNuevo << endl;
                                            if(bloques[numPagina - 1].tamanio() >= 0){
                                                int frame = pageTable.obtenerFrameId(numPagina);
                                                cout << "frame" << numPagina << endl;
                                                int registro;
                                                cout << "que registro desea modificar: "; cin >> registro;

                                                bufferPool.modificarRegistro(registro, registroNuevo, bloques[numPagina - 1].imprimirDireccion(), frame);
                                                heap.mostrarInformacionBloques();
                                                break;
                                            }
                                            else{
                                                cout << "no hay ningun contenido en la pagina para modififcar" << endl;
                                                break;
                                            }

                                        }
                                        heap.mostrarInformacionBloques();
                                        break;
                                    }
                                }

                                archivoEsquemas.close();
                            }
                            else if(opcion3 == 2){
                                 ifstream archivoEsquemas("esquemas.txt");
                                if (!archivoEsquemas.is_open()) {
                                    cerr << "No se pudo abrir el archivo esquemas.txt" << endl;
                                    return;
                                }

                                string lineaEsquema;
                                while (getline(archivoEsquemas, lineaEsquema)) {
                                    // Aquí podrías agregar lógica para elegir un esquema específico
                                    // Por simplicidad, usaremos el primer esquema que encontremos
                                    if (lineaEsquema.find("titanic#") != string::npos) {
                                        // Ignorar la parte del nombre del esquema
                                        size_t pos = lineaEsquema.find('#');
                                        if (pos != string::npos) {
                                            string esquema = lineaEsquema.substr(pos + 1);
                                            //int tamanioRegistro = calcularTamanioFijoLinea(esquema);
                                            //cout << "El tamaño del nuevo registro es: " << tamanioRegistro << " bytes" << endl;
                                            
                                            // Ingresar datos según el esquema
                                            string registroNuevo;
                                            ingresarDatosSegunEsquema(esquema, registroNuevo);

                                            int tamanioRegistro2 = calcularTamanioFijoLinea(esquema);    

                                            bloques[numPagina - 1].AumentarTamanio(tamanioRegistro2);    // se resta el tamaño del registro como si fuera longitud fija

                                            int tamanioRegistro = calcularTamanioLineaVariable(registroNuevo, esquema);

                                            cout << "El nuevo registro es: " << registroNuevo << endl;



                                            if(bloques[numPagina - 1].tamanio() - tamanioRegistro >= 0){
                                                int registro;
                                                cout << "que registro desea modificar: "; cin >> registro;
                                                int frame = pageTable.obtenerFrameId(numPagina);
                                                cout << "frame" << numPagina << endl;
                                                bufferPool.modificarRegistro(registro, registroNuevo, bloques[numPagina - 1].imprimirDireccion(), frame);
                                                bloques[numPagina - 1].RestarTamanio(tamanioRegistro);
                                                cout  << "el contenido del nuevo registro ha sido guardado" << endl;
                                                break;
                                            }
                                            else{
                                                cout << "la pagina no tiene suficiente espacio" << endl;
                                                break;
                                            }

                                        }
                                        break;
                                    }
                                }

                                archivoEsquemas.close();
                            }

                            heap.mostrarInformacionBloques();

                            break;
                        
                      
                        
                        
                        case 3:
                            int registro;
                            cout << "ingrese numero de registro: "; cin >> registro;
                            int frame = pageTable.obtenerFrameId(numPagina);

                            bufferPool.eliminarRegistro(registro, bloques[numPagina - 1].imprimirDireccion(), frame);
                            
                            break;
                        }



                        //cout << "El contenido de la pagina ha sido guardado" << endl;
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
            case 2: // Escritura  dirti bit 0
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

    /* void GuardarValorerDisco(int pagina){

        cout << pageTable.entries[pagina].DirtyBit << endl;

        if(pageTable.entries[pagina].DirtyBit == 1){
            //bufferPool.insertarTextoEnPagina(pagina);
            pageTable.DecrementarDirty(pagina);
            return;
        }
        cout << "No se puede insertar , porque el dirtyBit no es 1" << endl;
        
            
        
    } */

    /* void GuardarBloque(int pagina){
      
            bufferPool.GuardarBloque(pagina,pageTable.getNumFrame(pagina));
            pageTable.DecrementarDirty(pagina);
    }
 */
       

    void InsertarPinned(int pagina){            // JOSE ALEJANDRO MACHACA MUÑIZ 
        pageTable.insertarPinned(pagina);
    }

    void mostrarManecilla() {                   // JOSE ALEJANDRO MACHACA MUÑIZ 
        pageTable.mostrarManecilla();
    }

    void mostrarContenidoFrame(int numFrame) {          // JOSE ALEJANDRO MACHACA MUÑIZ 
        bufferPool.mostrarContenidoFrame2(numFrame);
    }

    void mostrarTablaDePaginas() {          // JOSE ALEJANDRO MACHACA MUÑIZ 
        pageTable.mostrarTabla();
    }
    void mostrarTablaDePaginas2() { //FERNANDO DEZA SOTOMAYOR
        pageTable.mostrarTablaLRU();
    }

    /* void llenarLongitudFija(const string& archivoTxt, const string& esquema) {
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
    } */

    /* void llenarLongitudVariable(const string& archivoTxt, const string& esquema) {
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
    } */
};
