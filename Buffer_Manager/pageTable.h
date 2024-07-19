#include "bufferPool.h"
class PageTable {
private:
    int manecilla = 0; // puntero para el algoritmo del reloj

    struct PageEntry {          // JOSE ALEJANDRO MACHACA MUÑIZ 
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

    int obtenerFrameId(int pageId) {    // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == pageId) {
                return entries[i].FrameId;
            }
        }
        cout << "No se encontró ninguna página con PageId " << pageId << "." << endl;
        return -1; // Indica que no se encontró la página
    }


    bool isDirty(int pageId) {          // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (const auto& entry : entries) {
            if (entry.PageId == pageId) {
                return entry.DirtyBit == 1;
            }
        }
        // Si no se encuentra la página, podríamos lanzar una excepción o devolver false
        return false;
    }

    // Insertar pinned
    void insertarPinned(int pageId) {           // JOSE ALEJANDRO MACHACA MUÑIZ 
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
    void mostrarTabla() {                           // JOSE ALEJANDRO MACHACA MUÑIZ 
        cout << "Frame id\tPageId\tDirtyBit\tPin Count\tReferenceBit\tPinned" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 << entries[i].BirtyBit << "\t\t" << (entries[i].Pineed ? 1 : 0) << endl;
        }
        cout << endl;
    }

    void mostrarTablaLRU(){ //FERNANDO DEZA SOTOMAYOR
        cout << "Frame id\tPageId\tPin Count\tLastUsed\tPinned" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 <<"\t\t" <<entries[i].LastUsed << endl;
        }
        cout << endl;
    }

    // Verificar si una página dada existe en la tabla de páginas
    bool verificarExistenciaDePagina(int numPagina) {           // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                return true;
            }
        }
        return false;
    }

    // Obtener el número de marco asociado a una página dada
    int getNumFrame(int numPagina) {                                // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                return entries[i].FrameId;
            }
        }
        return -1; // Si la página no se encuentra
    }

    // Disminuir el contador de pines (PinCount) de una página dada
    void descontarPinCount(int numPagina) {                         // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].PinCount--;
                break;
            }
        }
    }

    // Aumentar el contador de pines (PinCount) de una página dada
    void aumentarPinCount(int numPagina) {                          // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].PinCount++;
                break;
            }
        }
    }

    // Cambiar el bit de suciedad (DirtyBit) de una página dada entre 0 y 1
    void AumentarDirty(int numPagina) {                             // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].DirtyBit = 1;
                break;
            }
        }
    }

    void DecrementarDirty(int numPagina) {                      // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].DirtyBit = 0;
                break;
            }
        }
    }


    void incrementarLastUsed(int numPagina) {                   // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].LastUsed++;
                break;
            }
        }
    }

    // Decrementar el valor de LastUsed de una página dada
    void decrementarLastUsed(int numPagina) {                   // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                if (entries[i].LastUsed > 0) {
                    entries[i].LastUsed--;
                }
                break;
            }
        }
    }

    void incrementaBirtyBit(int numPagina) {                // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].BirtyBit = 1;
                break;
            }
        }
    }

    // Decrementar el valor de BirtyBit de una página dada
    void decrementaBirtyBit(int numPagina) {                // JOSE ALEJANDRO MACHACA MUÑIZ 
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
    
    void replaceReloj(int frameId, int pageId) {                    // JOSE ALEJANDRO MACHACA MUÑIZ 
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



    void aumentarManecilla(int numeroFrames){           // JOSE ALEJANDRO MACHACA MUÑIZ 
        manecilla = (manecilla + 1) % numeroFrames;
    }

     void mostrarManecilla() {                          // JOSE ALEJANDRO MACHACA MUÑIZ 
       cout << "la manecilla esta apuntado al frame: " << manecilla << endl;
    }

    int getManecilla(){                 // JOSE ALEJANDRO MACHACA MUÑIZ 
        return manecilla;
    }

    void agregarEntrada(int frameId, int pageId) {          // JOSE ALEJANDRO MACHACA MUÑIZ 
        entries.emplace_back(frameId, pageId);
    }

    bool verificarExistenciaDePagina(int numPagina) const {     // JOSE ALEJANDRO MACHACA MUÑIZ 
        for (const auto& entry : entries) {
            if (entry.PageId == numPagina) {
                return true;
            }
        }
        return false;
    }
    int obtenerFrameLRU() const {//FERNANDO DEZA SOTOMAYOR
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
