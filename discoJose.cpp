#include <iostream>
#include <limits>
#include <sys/stat.h> // Para mkdir en sistemas Unix-like
#include <fstream>
#include <sstream>
#include <iomanip>

#include <algorithm>
#include <cctype>
using namespace std;

class Sector {
private:
    int tamanio;

public:
    // Constructor predeterminado
    Sector() : tamanio(0) {}

    // Constructor con un argumento
    Sector(int tam) : tamanio(tam) {}

    int getTamanio() const {
        return tamanio;
    }

    void RestarPeso(int peso){
        tamanio = tamanio - peso;
    }
};

class Superficie {
private:
    int numeroSectores;
    Sector* sectores;

public:
    // Constructor predeterminado
    Superficie() : numeroSectores(0), sectores(nullptr) {}

    // Constructor con argumentos
    Superficie(int numSect, int tamSect) : numeroSectores(numSect) {
        sectores = new Sector[numeroSectores];
        for (int i = 0; i < numeroSectores; ++i) {
            sectores[i] = Sector(tamSect);
        }
    }

    // Constructor de copia
    Superficie(const Superficie& other) : numeroSectores(other.numeroSectores) {
        sectores = new Sector[numeroSectores];
        for (int i = 0; i < numeroSectores; ++i) {
            sectores[i] = other.sectores[i];
        }
    }

    // Operador de asignación
    Superficie& operator=(const Superficie& other) {
        if (this != &other) {
            delete[] sectores;
            numeroSectores = other.numeroSectores;
            sectores = new Sector[numeroSectores];
            for (int i = 0; i < numeroSectores; ++i) {
                sectores[i] = other.sectores[i];
            }
        }
        return *this;
    }

    // Destructor
    ~Superficie() {
        delete[] sectores;
    }

    int getNumeroSectores() const {
        return numeroSectores;
    }

    int getTamanioSector() const {
        return (numeroSectores > 0) ? sectores[0].getTamanio() : 0;
    }

    Sector& getSector(int index) const {
        return sectores[index];
    }
};

class Pistas {
private:
    int numeroSuperficies;
    Superficie* superficies;

public:
    Pistas() : numeroSuperficies(0), superficies(nullptr) {}

    Pistas(int numeroSuper, int numeroSector, int sizeSector) : numeroSuperficies(numeroSuper) {
        superficies = new Superficie[numeroSuper];
        for (int i = 0; i < numeroSuper; ++i) {
            superficies[i] = Superficie(numeroSector, sizeSector);
        }
    }

    // Constructor de copia
    Pistas(const Pistas& other) : numeroSuperficies(other.numeroSuperficies) {
        superficies = new Superficie[numeroSuperficies];
        for (int i = 0; i < numeroSuperficies; ++i) {
            superficies[i] = other.superficies[i];
        }
    }

    // Operador de asignación
    Pistas& operator=(const Pistas& other) {
        if (this != &other) {
            delete[] superficies;
            numeroSuperficies = other.numeroSuperficies;
            superficies = new Superficie[numeroSuperficies];
            for (int i = 0; i < numeroSuperficies; ++i) {
                superficies[i] = other.superficies[i];
            }
        }
        return *this;
    }

    // Destructor
    ~Pistas() {
        delete[] superficies;
    }

    int getNumeroSuperficies() const {
        return numeroSuperficies;
    }

    

    Superficie& getSuperficie(int index) const {
        return superficies[index];
    }
};

class Platos{
private:
    int numeroPistas;
    Pistas *pistas;
public:
    Platos() : numeroPistas(0), pistas(nullptr) {}

    Platos(int numeroPistas, int numeroSuperficies, int numeroSectores, int tamanioSector) : 
    numeroPistas(numeroPistas){
        pistas = new Pistas[numeroPistas];
        for (int i = 0; i < numeroPistas; ++i) {
            pistas[i] = Pistas(numeroSuperficies, numeroSectores, tamanioSector);
        }
    }

    // Constructor de copia
    Platos(const Platos& other) : numeroPistas(other.numeroPistas) {
        pistas = new Pistas[numeroPistas];
        for (int i = 0; i < numeroPistas; ++i) {
            pistas[i] = other.pistas[i];
        }
    }

    // Operador de asignación
    Platos& operator=(const Platos& other) {
        if (this != &other) {
            delete[] pistas;
            numeroPistas = other.numeroPistas;
            pistas = new Pistas[numeroPistas];
            for (int i = 0; i < numeroPistas; ++i) {
                pistas[i] = other.pistas[i];
            }
        }
        return *this;
    }

    Pistas& getPista(int index) const {
        return pistas[index];
    }

    int getNumeroPistas() const {
        return numeroPistas;
    }


    // Destructor
    ~Platos() {
        delete[] pistas;
    }
};

class DiscoDuro{
private:
    int tamanio;
    int numeroPlatos;
    Platos *platos;
public:

    DiscoDuro(int numeroPlatos, int numeroPistas, int numeroSuperficies, int numeroSectores, int tamanioSector ) :
    numeroPlatos(numeroPlatos), platos(new Platos[numeroPlatos]) {
        for (int i = 0; i < numeroPlatos; ++i) {
            platos[i] = Platos(numeroPistas, numeroSuperficies, numeroSectores, tamanioSector);
        }
    }

    // Constructor de copia
    DiscoDuro(const DiscoDuro& other) : tamanio(other.tamanio), numeroPlatos(other.numeroPlatos) {
        platos = new Platos[numeroPlatos];
        for (int i = 0; i < numeroPlatos; ++i) {
            platos[i] = other.platos[i];
        }
    }

    // Operador de asignación
    DiscoDuro& operator=(const DiscoDuro& other) {
        if (this != &other) {
            delete[] platos;
            tamanio = other.tamanio;
            numeroPlatos = other.numeroPlatos;
            platos = new Platos[numeroPlatos];
            for (int i = 0; i < numeroPlatos; ++i) {
                platos[i] = other.platos[i];
            }
        }
        return *this;
    }

    Platos& getPlato(int index) const {
        return platos[index];
    }

    // Función para mostrar la información del disco duro
    void mostrarInformacion() const {
        for (int i = 0; i < numeroPlatos; ++i) {
            std::cout << "Plato " << i + 1 << ":" << std::endl;
            std::cout << "Número de pistas en el plato "<< i + 1 << " : " << platos[i].getNumeroPistas() << std::endl;

            for (int j = 0; j < platos[i].getNumeroPistas(); ++j) {
                std::cout << "Pista " << j + 1 << ":" << std::endl;
                std::cout << "Número de superficies en la pista: " << platos[i].getPista(j).getNumeroSuperficies() << std::endl;

                for (int k = 0; k < platos[i].getPista(j).getNumeroSuperficies(); ++k) {
                    std::cout << "Superficie " << k + 1 << ":" << std::endl;
                    std::cout << "Número de sectores en la superficie: " << platos[i].getPista(j).getSuperficie(k).getNumeroSectores() << std::endl;
                    std::cout << "Tamaño del sector en la superficie: " << platos[i].getPista(j).getSuperficie(k).getTamanioSector() << std::endl;
                }
            }
        }
    }
    
    void calcularPeso(){
        tamanio = getNumeroPlatos() * platos[0].getNumeroPistas() * platos[0].getPista(0).getNumeroSuperficies()
        * platos[0].getPista(0).getSuperficie(0).getNumeroSectores() * platos[0].getPista(0).getSuperficie(0).getTamanioSector();
    }

    void guardarInformacion() {
        std::ofstream archivo("informacion_disco.txt", std::ofstream::out | std::ofstream::trunc);
        if (archivo.is_open()) {
            archivo << "platos\tpistas\tsuperficies\tsectores\ttamanioSector\ttamanioDisco\n";
            archivo << getNumeroPlatos() << "\t" << platos[0].getNumeroPistas() << "\t" 
                    << platos[0].getPista(0).getNumeroSuperficies() << "\t" 
                    << platos[0].getPista(0).getSuperficie(0).getNumeroSectores() << "\t" 
                    << platos[0].getPista(0).getSuperficie(0).getTamanioSector() << "\t" 
                    << getTamanio() << "\n";
            archivo.close();
            std::cout << "Se han guardado los detalles del disco en 'informacion_disco.txt'\n";
        } else {
            std::cerr << "No se pudo abrir el archivo para escribir.\n";
        }
    }

    void guardarRegistroSectores(const std::string& linea, int peso) {
        for (int i = 0; i < numeroPlatos; i++) {
            for (int j = 0; j < platos[i].getNumeroPistas(); j++) {
                for (int k = 0; k < platos[i].getPista(j).getNumeroSuperficies(); k++) {
                    for (int l = 0; l < platos[i].getPista(j).getSuperficie(k).getNumeroSectores(); l++) {
                        int tamanioSector = platos[i].getPista(j).getSuperficie(k).getSector(l).getTamanio();
                        if (peso <= tamanioSector) { // El sector tiene espacio para el registro
                            // Crear la ruta del archivo donde se guardará el registro
                            std::stringstream rutaArchivo;
                            rutaArchivo << "Plato" << std::setw(2) << std::setfill('0') << i + 1 << "/Pista" << std::setw(2) << std::setfill('0') 
                            << j + 1 << "/Superficie" << std::setw(2) << std::setfill('0') << k + 1 << "/Sector" << std::setw(2) 
                            << std::setfill('0') << l + 1 << "/registro.txt";

                            std::ofstream archivo(rutaArchivo.str(), std::ios::app);
                            archivo << linea << std::endl;
                            platos[i].getPista(j).getSuperficie(k).getSector(l).RestarPeso(peso);
                            archivo.close();
                            return; // Salir de la función después de guardar el registro
                        }
                    }
                }
            }
        }
        // Si llegamos aquí, significa que no se encontró ningún sector disponible
        std::cout << "No hay sectores disponibles para guardar el registro." << std::endl;
    }

    void cargarInformacionDesdeArchivo(const std::string& nombreArchivo, DiscoDuro& discoDuro) {
        std::ifstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            // Ignorar la primera línea
            archivo.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            int numPlatos, numPistas, numSuperficies, numSectores, tamSector, tamDisco;

            // Leer directamente los valores, sin necesidad de leer el delimitador
            archivo >> numPlatos >> numPistas >> numSuperficies 
                    >> numSectores >> tamSector >> tamDisco;

            // Actualizar el objeto discoDuro con los valores cargados
            discoDuro = DiscoDuro(numPlatos, numPistas, numSuperficies, numSectores, tamSector);
            discoDuro.setTamanio(tamDisco);

            std::cout << "Se han cargado los detalles del disco desde '" << nombreArchivo << "'.\n";
        } else {
            std::cerr << "No se pudo abrir el archivo para leer.\n";
        }
    }

   void crearCarpetas() const {
        for (int i = 0; i < numeroPlatos; ++i) {
            std::stringstream platoDir;
            platoDir << "Plato" << std::setw(2) << std::setfill('0') << i + 1;
            std::string platoDirStr = platoDir.str();

            if (mkdir(platoDirStr.c_str(), 0777) == -1) {
                std::cerr << "Error al crear el directorio '" << platoDirStr << "'\n";
                return;
            }

            for (int j = 0; j < platos[i].getNumeroPistas(); ++j) {
                std::stringstream pistaDir;
                pistaDir << platoDirStr << "/Pista" << std::setw(2) << std::setfill('0') << j + 1;
                std::string pistaDirStr = pistaDir.str();

                if (mkdir(pistaDirStr.c_str(), 0777) == -1) {
                    std::cerr << "Error al crear el directorio '" << pistaDirStr << "'\n";
                    return;
                }

                for (int k = 0; k < platos[i].getPista(j).getNumeroSuperficies(); ++k) {
                    std::stringstream superficieDir;
                    superficieDir << pistaDirStr << "/Superficie" << std::setw(2) << std::setfill('0') << k + 1;
                    std::string superficieDirStr = superficieDir.str();

                    if (mkdir(superficieDirStr.c_str(), 0777) == -1) {
                        std::cerr << "Error al crear el directorio '" << superficieDirStr << "'\n";
                        return;
                    }

                    for (int l = 0; l < platos[i].getPista(j).getSuperficie(k).getNumeroSectores(); ++l) {
                        std::stringstream sectorDir;
                        sectorDir << superficieDirStr << "/Sector" << std::setw(2) << std::setfill('0') << l + 1;
                        std::string sectorDirStr = sectorDir.str();

                        if (mkdir(sectorDirStr.c_str(), 0777) == -1) {
                            std::cerr << "Error al crear el directorio '" << sectorDirStr << "'\n";
                            return;
                        }
                    }
                }
            }
        }
        std::cout << "Estructura de carpetas creada correctamente en el directorio actual\n";
    }



    void setTamanio(int tamanio){
        this->tamanio = tamanio;
    }

    int getTamanio() const{
        return tamanio;
    }


    int getNumeroPlatos() const {
        return numeroPlatos;
    }


    // Destructor
    ~DiscoDuro() {
        delete[] platos;
    }
};

std::string getSchema(const std::string& schemaFile, std::string fileName);
int contarAtributos(const std::string& cadena);
void obtenerTiposDeDato(const std::string& cadena, std::string tiposDato[], int tamano);

int TotalRegistro = 0;

class Registro {
private:
    int id;
    int tamanioChar = 1;
    int tamanioInt = 4;
    int tamanioFloat = 4;
    int tamanioRegistro = 0;

public:
    Registro() {}


    void calcularTamanio(string campo, string tipoDato) {
        if (tipoDato == "string") {
            tamanioRegistro += tamanioChar * campo.length();
        } else if (tipoDato == "int") {
            tamanioRegistro += tamanioInt;
        } else if (tipoDato == "float") {
            tamanioRegistro += tamanioFloat;
        } else if (tipoDato == "char"){
            tamanioRegistro += tamanioChar;
        }
    }

    void setTamanioRegistro(int numero){
        this->tamanioRegistro = numero;
    }

    void setId(int id){
        this->id = id;
    }

    int getId(){
        return id;
    }

    int getTamanioRegistro() {
        return tamanioRegistro;
    }

    void reiniciarTamanio() {
        tamanioRegistro = 0;
    }

    int calcularTamanioRegistro(int numeroRegistro, string nombreArchivo) {
        ifstream archivo(nombreArchivo);
        string DatosEsquema = getSchema("esquemas.txt", nombreArchivo);

        int cantidadAtributos = contarAtributos(DatosEsquema);
        string tiposDato[cantidadAtributos];
        obtenerTiposDeDato(DatosEsquema, tiposDato, cantidadAtributos);

        string linea;
        int contadorLineas = 0; // Contador de líneas

        // Ignorar la primera línea
        getline(archivo, linea);

        // Procesar el resto del archivo
        while (getline(archivo, linea)) {
            if (contadorLineas == numeroRegistro) { // Si es el registro deseado
                istringstream ss(linea);
                string palabra;
                int contador = 0; // Reiniciar el contador en cada línea

                // Leer palabra por palabra
                while (getline(ss, palabra, '#')) {
                    if (contador >= cantidadAtributos) {
                        contador = 0;
                    }
                    calcularTamanio(palabra, tiposDato[contador]);
                    contador++;
                }
                archivo.close();
                return tamanioRegistro; // Devolver el tamaño del registro
            }
            contadorLineas++; // Incrementar el contador de líneas
        }
        archivo.close();
        return -1; // Si no se encontró el registro
    }

};

int contarLineasArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::string linea;
    int contador = 0;

    if (archivo.is_open()) {
           while (std::getline(archivo, linea)) {
                contador++;
           }
          archivo.close();
     } else {
        std::cerr << "No se pudo abrir el archivo " << nombreArchivo << std::endl;
     }

    return contador - 1;
}

int contarAtributos(const std::string& cadena) {
    std::istringstream ss(cadena);
    std::string fragmento;
    int contador = 0;
        
    while (std::getline(ss, fragmento, '#')) {
         if (!fragmento.empty() && fragmento != "string" && fragmento != "int") {
              contador++;
           }
       }
        
      return contador;
 }

 void obtenerTiposDeDato(const std::string& cadena, std::string tiposDato[], int tamano) {
    std::istringstream ss(cadena);
    std::string fragmento;
    int indice = 0;
    bool guardar = false;
    
    while (std::getline(ss, fragmento, '#')) {
        if (guardar && indice < tamano) {
            tiposDato[indice] = fragmento;
            indice++;
        }
        guardar = !guardar;
    }
}


string getSchema(const string& schemaFile, string fileName) {
    // Eliminar los últimos 4 caracteres (extensión .txt)
    fileName = fileName.substr(0, fileName.size() - 4);
    ifstream file(schemaFile);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            getline(ss, name, '#');
            if (name == fileName) {
                return line.substr(name.length() + 1);
            }
        }
        file.close();
    }
    return "";
}

string leerLineaEspecifica(const string& nombreArchivo, int numeroLinea) {
    ifstream archivo(nombreArchivo);
    string lineaDeseada;
    string linea;
    int numLineaActual = 1;
    
    // Verificar si se pudo abrir el archivo
    if (!archivo.is_open()) {
        return "No se pudo abrir el archivo.";
    }
    
    // Leer y guardar la línea deseada en un string
    while (getline(archivo, linea)) {
        if (numLineaActual >= 2 && numLineaActual == numeroLinea) {
            lineaDeseada = linea;
            break;
        }
        numLineaActual++;
    }
    
    // Cerrar el archivo
    archivo.close();
    
    if (lineaDeseada.empty()) {
        return "No se encontró la línea especificada.";
    } else {
        return lineaDeseada;
    }
}

class Bloque{
    private:
        int tamanio;
    public:
        void Espacio(){
            cout << "espacio sobrante: " << tamanio;
        }
        void setTamanio(int tamanio){
            this->tamanio = tamanio;
        }
        void CrearDicionario(DiscoDuro DiscoDuro){

        }

};


int main() {

    DiscoDuro discoDuro(0, 0, 0, 0, 0); // Se inicializa con valores predeterminados, los cuales se sobrescribirán en la opción 1
    Registro *registros = nullptr;

    int opcion;
    do {
        cout << "Ingrese 1 si quiere crear el disco" << endl;
        cout << "Ingrese 2 para cargar los datos disco de un txt" << endl;
        cout << "Ingrese 3 si quiere mostrar los datos del disco" << endl;
        cout << "Ingrese 4 para crear carpetas" << endl;
        cout << "Ingrese 5 para crear registros" << endl;
        cout << "Ingrese 6 para crear bloques" << endl;
        cout << "Opción: ";
        cin >> opcion;
        switch (opcion) {
            case 1: {
                int numPlatos;
                int numPistas;
                int numSuperficies;
                int numSectores;
                int tamSector;


                cout << "Ingrese el número de platos: "; cin >> numPlatos;
                cout << "Ingrese el número de pistas: "; cin >> numPistas;
                cout << "Ingrese el número de superficies: "; cin >> numSuperficies;
                cout << "Ingrese el número de sectores: "; cin >> numSectores;
                cout << "Ingrese el tamaño del sector: ";  cin >> tamSector;

                // Crear una instancia de DiscoDuro
                discoDuro = DiscoDuro(numPlatos, numPistas, numSuperficies, numSectores, tamSector);
                discoDuro.calcularPeso();
                discoDuro.guardarInformacion();
                break;
            }
            case 2:{
                discoDuro.cargarInformacionDesdeArchivo("informacion_disco.txt", discoDuro);
                break;
            }

            case 3:
                discoDuro.mostrarInformacion();
                cout << "Tamanio del disco: " << discoDuro.getTamanio() << endl;
                break;
            case 4:{
                discoDuro.crearCarpetas();
                break;
            }
            case 5:{
                string nombreArchivo;
                cout << "ingrese nombre de Archivo: "; cin >> nombreArchivo;
                int cantidadRegistros = contarLineasArchivo(nombreArchivo);
                cout << cantidadRegistros << endl;
                if(registros == nullptr){
                    registros = new Registro[cantidadRegistros];
                    string DatosEsquema = getSchema("esquemas.txt", nombreArchivo);
                    cout << DatosEsquema << endl;
                    for(int i = 0; i < cantidadRegistros; i++){
                        string lineaArchivo = leerLineaEspecifica(nombreArchivo, i + 2);
                        int tamañoRegistro = registros[i].calcularTamanioRegistro(i, nombreArchivo);
                        discoDuro.guardarRegistroSectores(lineaArchivo, tamañoRegistro);
                        registros[i].setId(i + 1);
                        cout << "tamanio del registro " << i + 1 << ": ";
                        cout << tamañoRegistro << endl;
                    }
                }
                else{
                    Registro *registros2 = new Registro[cantidadRegistros + TotalRegistro];
                    for(int i = 0; i < TotalRegistro; i++){
                        registros2[i].setId(registros[i].getId());
                        registros2[i].setTamanioRegistro(registros[i].getTamanioRegistro());
                    }
                    for(int i = TotalRegistro; i < TotalRegistro + cantidadRegistros; i++){
                        string lineaArchivo = leerLineaEspecifica(nombreArchivo, i + 2 - TotalRegistro);
                        int tamañoRegistro = registros2[i].calcularTamanioRegistro(i - TotalRegistro, nombreArchivo);
            
                        discoDuro.guardarRegistroSectores(lineaArchivo, tamañoRegistro);
                        registros2[i].setId(i + 1);
                    }
                    for(int i = 0; i < TotalRegistro + cantidadRegistros; i++ ){
                        cout << "id del regisro: " << registros2[i].getId() << endl;
                        cout << "tamanio del registro " << i + 1 << ": ";
                        cout << registros2[i].getTamanioRegistro() << endl;
                    }
                    registros = registros2;
                    delete[] registros2;
                }
                
                TotalRegistro = TotalRegistro + cantidadRegistros;
                
                break;
            }
            case 6:{
                int numeroBloques, tamanioBloque;
                cout << "ingrese cuantos bloques quiere: "; cin >> numeroBloques;
                Bloque *bloques = new Bloque[numeroBloques];
                for(int i = 0; i < numeroBloques; i++){
                    bloques[i].setTamanio(tamanioBloque);
                }
            }
            default:
                break;
        }
    } while (opcion != 7);
    
}