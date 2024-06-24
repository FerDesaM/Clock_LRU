#include "hardDisk.h"
class heapFile {
public:
    struct BloqueInfo {
        int nroBloque;
        int espacioOcupado;
        int espacioLibre;
        std::string direccion;
    };

    std::vector<BloqueInfo> bloquesInfo;

    void agregarBloque(int nroBloque, int espacioOcupado, int espacioLibre, const std::string& direccion) {
        BloqueInfo info = { nroBloque, espacioOcupado, espacioLibre, direccion };
        bloquesInfo.push_back(info);
    }

    void mostrarInformacionBloques() {
        cout<<"que es esto"<<endl;
        for (const auto& info : bloquesInfo) {
            std::cout << "Bloque: " << info.nroBloque 
                      << ", Espacio Ocupado: " << info.espacioOcupado 
                      << ", Espacio Libre: " << info.espacioLibre 
                      << ", Direccion: " << info.direccion << std::endl;
        }
    }

    void almacenarInformacionBloques(const std::vector<Bloque>& bloques) {
        for (int i = 0; i < bloques.size(); i++) {
            int espacioLibre = bloques[i].tamanio();
            int espacioOcupado = bloques[i].tamanio() - espacioOcupado;
            std::string direccion = "bloque_" + std::to_string(i) + ".txt";
            agregarBloque(i+1, espacioOcupado, espacioLibre, direccion);
        }
    }
};
