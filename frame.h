#include "pagina.h"
class Frame {
private:
    Pagina pagina; // Solo una página por frame

public:
    bool estaVacio() const {
        return pagina.mostrarContenido2().empty();
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
            std::cout << pagina.mostrarContenido2();
        } else {
            std::cout << "El frame está vacío." << std::endl;
        }
    }

    void agregarPaginaConContenido(const std::string& contenido) {
        pagina = Pagina(contenido);
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

    void reemplazarContenido(const std::string& nuevo_contenido) {
        pagina.reemplazarContenido(nuevo_contenido);
    }

};