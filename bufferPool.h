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

    void cargarPaginaAlFrame2(size_t numero_frame, const std::string& contenido) {
        if (numero_frame < frames.size()) {
            frames[numero_frame].agregarPaginaConContenido(contenido);
        } else {
            std::cout << "Número de frame inválido." << std::endl;
        }
    } 

    

    void mostrarContenidoFrame2(int numero_frame) {
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

    void insertarTextoEnPagina(size_t numero_frame, string texto) {
        if (numero_frame < frames.size()) {

            frames[numero_frame].insertarTexto(texto);
        } else {
            std::cout << "Número de frame inválido." << std::endl;
        }
    }

    /* void GuardarBloque(int numero_pagina, int frame){
        std::string nombre_archivo = "bloque_" + std::to_string(numero_pagina) + ".txt";
        std::string ruta = "Bloques/" + nombre_archivo;

        frames[frame].GuardarBloque(ruta);
    } */

    void reemplazarContenido(size_t numero_frame, const std::string& nuevo_contenido) {
        if (numero_frame < frames.size()) {
            frames[numero_frame].reemplazarContenido(nuevo_contenido);
        } else {
            std::cout << "Número de frame inválido." << std::endl;
        }
    }

    void modificarRegistro(int numero_registro, const std::string& nuevo_contenido, string registros, int frame) {
        string RegistroNuevo = registros;
        std::stringstream ss(RegistroNuevo);
        std::string linea;
        std::stringstream nuevos_registros;

        bool encontrado = false;
        while (std::getline(ss, linea, '\n')) {
            // Obtener el número de registro del inicio de la línea
            size_t pos = linea.find('#');
            if (pos != std::string::npos) {
                int num_registro = std::stoi(linea.substr(0, pos));
                if (num_registro == numero_registro) {
                    // Reemplazar el registro completo por el nuevo contenido
                    nuevos_registros << nuevo_contenido << "\n";
                    encontrado = true;
                    std::cout << "Registro modificado exitosamente." << std::endl;
                } else {
                    nuevos_registros << linea << "\n";
                }
            } else {
                nuevos_registros << linea << "\n";  // Conservar líneas que no siguen el formato esperado
            }
        }

        if (!encontrado) {
            std::cout << "Registro no encontrado." << std::endl;
        }

        // Actualizar la cadena de registros
        RegistroNuevo = nuevos_registros.str();

        reemplazarContenido(frame, RegistroNuevo);

    }

    void eliminarRegistro(int numero_registro, const std::string& registros, int frame) {
        std::string registrosActualizados = registros;
        std::stringstream ss(registrosActualizados);
        std::string linea;
        std::stringstream nuevos_registros;

        bool encontrado = false;
        while (std::getline(ss, linea, '\n')) {
            // Obtener el número de registro del inicio de la línea
            size_t pos = linea.find('#');
            if (pos != std::string::npos) {
                int num_registro = std::stoi(linea.substr(0, pos));
                if (num_registro == numero_registro) {
                    encontrado = true;
                    std::cout << "Registro eliminado exitosamente." << std::endl;
                } else {
                    nuevos_registros << linea << "\n";
                }
            } else {
                nuevos_registros << linea << "\n";  // Conservar líneas que no siguen el formato esperado
            }
        }

        if (!encontrado) {
            std::cout << "Registro no encontrado." << std::endl;
        }

        // Actualizar la cadena de registros
        registrosActualizados = nuevos_registros.str();

        reemplazarContenido(frame, registrosActualizados);
        
    }


    
    

};
