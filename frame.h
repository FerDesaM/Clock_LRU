#include "pagina.h"
class Frame {
private:
    Pagina pagina; // Solo una página por frame

public:
    bool estaVacio() const {                // JOSE ALEJANDRO MACHACA MUÑIZ 
        return pagina.mostrarContenido2().empty();
    }

    void agregarPagina(const std::string& nombre_archivo) { // JOSE ALEJANDRO MACHACA MUÑIZ 
        if (estaVacio()) {
            pagina = Pagina(nombre_archivo);
        } else {        
            pagina=Pagina(nombre_archivo);
        }
    }

    void mostrarPagina() {          // JOSE ALEJANDRO MACHACA MUÑIZ 
        if (!estaVacio()) {
            std::cout << pagina.mostrarContenido2();
        } else {
            std::cout << "El frame está vacío." << std::endl;
        }
    }

    void agregarPaginaConContenido(const std::string& contenido) {      // JOSE ALEJANDRO MACHACA MUÑIZ 
        pagina = Pagina(contenido);
    }

    void reemplazarPagina(const std::string& nombre_archivo) {  // JOSE ALEJANDRO MACHACA MUÑIZ 
        pagina = Pagina(nombre_archivo);
    }

    void insertarTexto(const std::string& texto) {          // JOSE ALEJANDRO MACHACA MUÑIZ 
        pagina.insertarTexto(texto);
    }

    void GuardarBloque(string ruta){        // JOSE ALEJANDRO MACHACA MUÑIZ 
        pagina.GuardarBloque(ruta);
    }

    void reemplazarContenido(const std::string& nuevo_contenido) {      // JOSE ALEJANDRO MACHACA MUÑIZ 
        pagina.reemplazarContenido(nuevo_contenido);
    }

};