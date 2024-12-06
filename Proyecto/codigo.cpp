#include <iostream>
#include <cstring>

using namespace std;

typedef enum {
    CIENCIA_FICCION,
    HORROR,
    ROMANCE,
    CIENCIA,
    AVENTURA
} Genero;

typedef struct Autor{
    char autor[50];
    int digito;
    struct Libro* primer_Lib;
    struct Autor* siguiente;
}Autor;

typedef struct Libro {
    Genero tipo;
    char titulo[100];
    int ano, mes, dia, dig;
    struct Libro* siguiente; // Puntero al siguiente libro en la lista
    struct Libro* anterior;
} Libro;

// Lista principal de libros organizada por autor
Autor* listaAutor = nullptr;

Autor* CrearAutor(const char* nombreAutor) {
    // Crear un nuevo nodo Autor
    Autor* nuevoAutor = new Autor();
    strncpy(nuevoAutor->autor, nombreAutor, sizeof(nuevoAutor->autor) - 1);
    nuevoAutor->autor[sizeof(nuevoAutor->autor) - 1] = '\0'; // Garantizar terminación nula
    nuevoAutor->primer_Lib = nullptr; // Sin libros inicialmente
    nuevoAutor->siguiente = nullptr;

    // Insertar el nuevo autor en la lista
    if (listaAutor == nullptr) {
        // Si la lista está vacía
        nuevoAutor->digito=1;
        listaAutor = nuevoAutor;
    } else {
        // Insertar al final de la lista
        Autor* temp = listaAutor;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        nuevoAutor->digito=temp->digito+1;
        temp->siguiente = nuevoAutor;
    }
    return nuevoAutor;
}

Autor*  buscar(const char* nombreAutor){
    Autor* temp = listaAutor;
    while (temp != nullptr){
        if (strcmp(temp->autor, nombreAutor) == 0){
            return temp;
        }
        temp = temp -> siguiente;
    }
    return nullptr;
}

void agregarLibro(Libro* nuevo,  Autor* autor){
    if (autor->primer_Lib==nullptr){
        nuevo->dig=1;
        autor->primer_Lib=nuevo;
    }else{
        Libro* temp = autor->primer_Lib;
        while(temp->siguiente!=nullptr){
            temp=temp->siguiente;
        }
        nuevo->anterior = temp;
        nuevo->dig=temp->dig+1;
        temp->siguiente = nuevo;
    }
}

// Función para crear un nuevo libro
Libro* crearLibro(Genero tipo, const char* autor, const char* titulo, int ano, int mes, int dia) {
    Autor* modificar = buscar(autor);
    if (modificar == nullptr) {
        modificar = CrearAutor(autor); // Actualizamos correctamente la variable
    }

    Libro* nuevoLibro = new Libro();
    nuevoLibro->tipo = tipo;
    strncpy(nuevoLibro->titulo, titulo, sizeof(nuevoLibro->titulo) - 1);
    nuevoLibro->titulo[sizeof(nuevoLibro->titulo) - 1] = '\0';
    nuevoLibro->ano = ano;
    nuevoLibro->mes = mes;
    nuevoLibro->dia = dia;
    nuevoLibro->siguiente = nullptr;

    agregarLibro(nuevoLibro, modificar);
    return nuevoLibro;
}

Autor* bus_aut(int a){
    Autor* buscar_a = listaAutor;
    while(buscar_a != nullptr){
        if (buscar_a->digito==a){
            return buscar_a;
        }
        buscar_a=buscar_a->siguiente;
    }
    return nullptr;
}

Libro* bus_lib(int a, Libro* buscar){
    while(buscar!=nullptr){
        if(buscar->dig==a){
            return buscar;
        }
        buscar=buscar->siguiente;
    }
    return nullptr;
}

void borrar(Libro* eliminar){
    Libro* aux= eliminar->anterior;
    eliminar->siguiente->dig=eliminar->dig;
    aux->siguiente=eliminar->siguiente;
    delete eliminar;
}

int imprimir_libro(Libro* seleccionado, const char* autor) {
    if (seleccionado == nullptr) {
        cout << "El libro seleccionado no es válido.\n";
        return 0; // Regresar al menú superior
    }

    int sel;
    do {
        // Mostrar detalles del libro
        cout << "----------------------------------\n";
        cout << "Titulo: " << seleccionado->titulo << endl;
        cout << "Autor: " << autor << endl;
        cout << "Fecha: " << seleccionado->dia << "/" << seleccionado->mes << "/" << seleccionado->ano << endl;
        cout << "Genero: " << seleccionado->tipo << endl;
        cout << "----------------------------------\n";
        cout << "Opciones:\n";
        cout << "0. Regresar\n";
        cout << "1. Modificar\n";
        cout << "2. Eliminar\n";
        cout << "3. Abrir\n";
        cout << "Elige una opción: ";
        cin >> sel;

        switch (sel) {
            case 0:
                return 0; // Regresar al menú de libros
            case 1:
                cout << "Función para modificar no implementada.\n";
                break;
            case 2:
                cout << "Eliminando libro...\n";
                borrar(seleccionado);
                return 0; // Regresar tras eliminar el libro
            case 3:
                cout << "Función para abrir no implementada.\n";
                break;
            default:
                cout << "Opción no válida. Intenta de nuevo.\n";
                break;
        }
    } while (true);
}

void imprimir_lib_aut(Libro* seleccionado, const char* autor) {
    if (seleccionado == nullptr) {
        cout << "No hay libros para el autor: " << autor << ".\n";
        return;
    }

    int sel;
    do {
        cout << "------------- " << autor << " -------------\n";
        cout << "0. Regresar\n";

        // Mostrar todos los libros del autor
        Libro* temp = seleccionado;
        while (temp != nullptr) {
            cout << temp->dig << ". " << temp->titulo << endl;
            temp = temp->siguiente;
        }

        cout << "Elige el libro: ";
        cin >> sel;

        if (sel == 0) {
            // Regresar al menú principal
            return;
        }

        // Buscar y mostrar detalles del libro seleccionado
        Libro* libroSeleccionado = bus_lib(sel, seleccionado);
        if (libroSeleccionado != nullptr) {
            int resultado = imprimir_libro(libroSeleccionado, autor);
            if (resultado == 0) {
                // Regresar al menú de libros
                continue;
            }
        } else {
            cout << "Número no válido. Intenta de nuevo.\n";
        }
    } while (true);
}

void imprimir_autores() {
    int sel;
    cout<<"------------- Menú Principal -------------"<< endl;
    while (true) { // Bucle para mantener al usuario en la lista de autores
        Autor* imprimir = listaAutor;

        cout << "0. Regresar al menú principal" << endl;
        while (imprimir != nullptr) {
            cout << imprimir->digito << ". " << imprimir->autor << endl;
            imprimir = imprimir->siguiente;
        }

        cout << "Elige el Autor: ";
        cin >> sel;

        if (sel == 0) {
            // Regresar al menú principal
            return;
        }

        // Buscar el autor seleccionado
        imprimir = bus_aut(sel);
        if (imprimir != nullptr) {
            imprimir_lib_aut(imprimir->primer_Lib, imprimir->autor);
        } else {
            cout << "Número no válido. Intenta de nuevo.\n";
        }
    }
}

void por_Crit() {
    while (true) { // Bucle para buscar por criterio
        int criterio;
        char Nom_til[50];
        cout << "------------- Buscar por Criterio -------------" << endl;
        cout << "0. Regresar al menú principal\n1. Por Autor\n2. Por Titulo\n3. Por Genero\n4. Por Fecha\n";
        cout << "Elige una opción: ";
        cin >> criterio;

        switch (criterio) {
        case 0:
            return; // Regresar al menú principal
        case 1:
            cout << "Escribe el nombre del autor: ";
            cin >> Nom_til;
            // Implementa búsqueda por autor aquí
            cout << "Búsqueda por autor no implementada.\n";
            break;
        case 2:
            cout << "Escribe el título del libro: ";
            cin >> Nom_til;
            // Implementa búsqueda por título aquí
            cout << "Búsqueda por título no implementada.\n";
            break;
        case 3:
            cout << "1. CIENCIA_FICCION\n2. HORROR\n3. ROMANCE\n4. CIENCIA\n5. AVENTURA\n";
            // Implementa búsqueda por género aquí
            cout << "Búsqueda por género no implementada.\n";
            break;
        case 4:
            int ano, mes, dia;
            cout << "Introduce la fecha en formato numérico.\nAño: ";
            cin >> ano;
            cout << "Mes: ";
            cin >> mes;
            cout << "Día: ";
            cin >> dia;
            // Implementa búsqueda por fecha aquí
            cout << "Búsqueda por fecha no implementada.\n";
            break;
        default:
            cout << "Opción no válida. Intenta de nuevo.\n";
        }
    }
}

void menu() {
    while (true) { // Bucle principal del menú
        cout << "------------- Menú Principal -------------" << endl;
        cout << "0. Salir\n1. Buscar por criterio\n2. Lista de Autores\n";
        cout << "Elige una opción: ";
        int opcion;
        cin >> opcion;

        switch (opcion) {
        case 0:
            cout << "Saliendo del programa...\n";
            return; // Salir del programa
        case 1:
            por_Crit();
            break;
        case 2:
            imprimir_autores();
            break;
        default:
            cout << "Opción no válida. Intenta de nuevo.\n";
        }
    }
}

int main() {
    crearLibro(AVENTURA,"Robert Jordan", "El ojo del mundo", 1990,1,15);
    crearLibro(AVENTURA,"Robert Jordan", "El despertar de los heroes", 1990,11,15);
    crearLibro(AVENTURA,"Robert Jordan", "El dragon renacido", 1991,9,15);
    crearLibro(HORROR, "Stephen King", "It",1986,9,15);
    menu();
    return 0;
};