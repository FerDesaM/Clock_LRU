#include "request.h"
class Pagina {
private:
    //int tamanioTotal = 512;
    std::string ruta;
    std::vector<std::string> contenido;

public:
    // Constructor con argumentos para cargar el contenido de un archivo
    /* Pagina(const std::string& nombre_archivo) {
        ruta = nombre_archivo;
        cargarContenido();
    } */

    // Constructor sin argumentos para manejar la inicialización de una instancia vacía
    Pagina() {}


    Pagina(const std::string& contenido) {          // JOSE ALEJANDRO MACHACA MUÑIZ 
        this->contenido.push_back(contenido);
    }

    void cargarContenido() {                    // JOSE ALEJANDRO MACHACA MUÑIZ 
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

    

    std::string mostrarContenido2() const {         // JOSE ALEJANDRO MACHACA MUÑIZ 
        std::string contenido_str;
        for (const auto& linea : contenido) {
            contenido_str += linea + "\n";
        }
        return contenido_str;
    }

    void insertarTexto(const std::string& texto) {      // JOSE ALEJANDRO MACHACA MUÑIZ 
        contenido.push_back(texto);
    }

    void GuardarBloque(const std::string& ruta) {       // JOSE ALEJANDRO MACHACA MUÑIZ 
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

    void reemplazarContenido(const std::string& nuevo_contenido) {
        contenido.clear(); // Limpiar el contenido actual
        contenido.push_back(nuevo_contenido); // Agregar el nuevo contenido
    }
    
};