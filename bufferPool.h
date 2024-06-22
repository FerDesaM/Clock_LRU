#include "frame.h"
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
