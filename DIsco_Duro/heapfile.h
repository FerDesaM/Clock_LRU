#include "hardDisk.h"
class heapFile {
public:
    struct BloqueInfo {//FERNANDO DEZA SOTOMAYOR
        int nroBloque;
        int espacioOcupado;
        int espacioLibre;
        std::string direccion;
    };

    std::vector<BloqueInfo> bloquesInfo;

    void agregarBloque(int nroBloque, int espacioOcupado, int espacioLibre, const std::string& direccion){ //FERNANDO DEZA SOTOMAYOR
        BloqueInfo info = { nroBloque, espacioOcupado, espacioLibre, direccion };
        bloquesInfo.push_back(info);
    }

    void mostrarInformacionBloques() {//FERNANDO DEZA SOTOMAYOR
        //cout<<"que es esto"<<endl;
        for (const auto& info : bloquesInfo) {
            std::cout << "Bloque: " << info.nroBloque 
                      << ", Espacio Libre: " << info.espacioOcupado 
                      << ", Espacio Ocupado: " << info.espacioLibre 
                      << ", Direccion: " << info.direccion << std::endl;
        }
    }

    void almacenarInformacionBloques(const std::vector<Bloque>& bloques) {//FERNANDO DEZA SOTOMAYOR
        for (int i = 0; i < bloques.size(); i++) {
            int espacioOcupado = bloques[i].tamanio();
            int espacioLibre=tamanioMaximo-bloques[i].tamanio();
            std::string direccion = "bloque_" + std::to_string(i) + ".txt";
            agregarBloque(i+1, espacioOcupado, espacioLibre, direccion);
        }
    }
    void actualizarBloque(int nroBloque, int espacioOcupado, int espacioLibre) {
        for (auto& info : bloquesInfo) {
            if (info.nroBloque == nroBloque) {
                info.espacioOcupado = espacioOcupado;
                info.espacioLibre = espacioLibre;
                return;
            }
        }
        // Si el bloque no se encuentra, podrías agregar uno nuevo
    }
};
