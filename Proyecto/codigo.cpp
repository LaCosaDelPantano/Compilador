#include <iostream>
#include <cstring>
#include <fstream>

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

void agregarLibro(Libro* nuevo, Autor* autor) {
    if (autor->primer_Lib == nullptr) {
        nuevo->dig = 0;
        Libro* nuevoLibro = new Libro();
        autor->primer_Lib = nuevoLibro;
        agregarLibro(nuevo, autor);
    } else {
        Libro* temp = autor->primer_Lib;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        nuevo->anterior = temp;
        nuevo->dig = temp->dig + 1;
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
    nuevoLibro->anterior = nullptr;

    agregarLibro(nuevoLibro, modificar);
    return nuevoLibro;
}

Autor* bus_aut(int a) {
    Autor* buscar_a = listaAutor;
    while (buscar_a != nullptr) {
        if (buscar_a->digito == a) {
            return buscar_a;
        }
        buscar_a = buscar_a->siguiente;
    }
    return nullptr;
}

Libro* bus_lib(int a, Libro* buscar) {
    while (buscar != nullptr) {
        if (buscar->dig == a) {
            return buscar;
        }
        buscar = buscar->siguiente;
    }
    return nullptr;
}

void borrarLibro(Libro* libro, Autor* autor) {
    Libro* aux= libro->anterior;
    libro->siguiente->dig=libro->dig;
    aux->siguiente=libro->siguiente;
    delete libro;
    while(aux->siguiente!=nullptr){
        aux->siguiente->dig=aux->dig+1;
        aux=aux->siguiente;
    }
    return;
}

void cambiar_autor(const char* nuevoAutorNombre, Libro* libroMover) {
    // Buscar el autor actual del libro
    Autor* autorActual = nullptr;
    for (Autor* temp = listaAutor; temp != nullptr; temp = temp->siguiente) {
        Libro* libroTemp = temp->primer_Lib;
        while (libroTemp != nullptr) {
            if (libroTemp == libroMover) {
                autorActual = temp;
                Libro* aux =libroTemp;
                while(aux!=nullptr){
                    aux->dig=aux->dig-1;
                    aux=aux->siguiente;
                }
                break;
            }
            libroTemp = libroTemp->siguiente;
        }
        if (autorActual != nullptr) break;
    }

    // Buscar o crear el nuevo autor
    Autor* nuevoAutor = buscar(nuevoAutorNombre);
    if (nuevoAutor == nullptr) {
        nuevoAutor = CrearAutor(nuevoAutorNombre);
    }

    Libro* libroTemp = autorActual->primer_Lib;
    while (libroTemp != nullptr && libroTemp->siguiente != libroMover) {
        libroTemp = libroTemp->siguiente;
    }
    if (libroTemp != nullptr) {
        libroTemp->siguiente = libroMover->siguiente;
    }

    // Actualizar punteros del libro
    libroMover->siguiente = nullptr;
    libroMover->anterior = nullptr;

    // Agregar el libro al nuevo autor
    agregarLibro(libroMover, nuevoAutor);

    cout << "El libro \"" << libroMover->titulo << "\" fue reasignado al autor \"" << nuevoAutorNombre << "\".\n";
    return;
}

void modificar(Libro* modificar, const char* autor) {
    int sel;
    char dato[100];

    do {
        // Mostrar menú de opciones
        cout << "----------------------------------\n";
        cout << "0. Regresar" << endl;
        cout << "1. Titulo: " << modificar->titulo << endl;
        cout << "2. Autor: " << autor << endl;
        cout << "3. Fecha: " << modificar->dia << "/" << modificar->mes << "/" << modificar->ano << endl;
        cout << "4. Genero: " << modificar->tipo << endl;
        cout << "Elige lo que quieres modificar: ";
        cin >> sel;

        switch (sel) {
            case 0: // Salir del menú de modificación
                return;

            case 1: // Modificar título
                cout << "Escribe el nuevo título: ";
                cin.ignore(); // Limpiar el buffer
                cin.getline(dato, sizeof(dato));
                strncpy(modificar->titulo, dato, sizeof(modificar->titulo) - 1);
                modificar->titulo[sizeof(modificar->titulo) - 1] = '\0';
                cout << "Título modificado exitosamente.\n";
                break;

            case 2: // Modificar autor
                cout << "Escribe el nuevo autor: " << endl;
                cin.ignore(); // Limpiar el buffer antes de usar getline
                cin.getline(dato, sizeof(dato));
                cambiar_autor(dato, modificar);  // Cambiar el autor
                sel=0;
                break;
            case 3: { // Modificar fecha
                int ano, mes, dia;
                cout << "Escribe la nueva fecha.\n";
                cout << "Año: ";
                cin >> ano;
                if (ano > 0) modificar->ano = ano;

                cout << "Mes (1-12): ";
                cin >> mes;
                if (mes >= 1 && mes <= 12) modificar->mes = mes;
                else cout << "Mes inválido.\n";

                cout << "Día (1-31): ";
                cin >> dia;
                if (dia >= 1 && dia <= 31) modificar->dia = dia;
                else cout << "Día inválido.\n";

                cout << "Fecha modificada exitosamente.\n";
                break;
            }

            case 4: { // Modificar género
                int genero;
                cout << "Géneros disponibles:\n";
                cout << "1. CIENCIA_FICCION\n2. HORROR\n3. ROMANCE\n4. CIENCIA\n5. AVENTURA\n";
                cout << "Elige un género: ";
                cin >> genero;

                switch (genero) {
                    case 1: modificar->tipo = CIENCIA_FICCION; break;
                    case 2: modificar->tipo = HORROR; break;
                    case 3: modificar->tipo = ROMANCE; break;
                    case 4: modificar->tipo = CIENCIA; break;
                    case 5: modificar->tipo = AVENTURA; break;
                    default:
                        cout << "Opción de género no válida.\n";
                        continue;
                }
                cout << "Género modificado exitosamente.\n";
                break;
            }

            default: // Manejo de opciones no válidas
                cout << "Opción no válida. Intenta de nuevo.\n";
                break;
        }
    } while (sel!=0); // Repetir el bucle hasta que el usuario seleccione "Regresar"
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
        cout << "3. Ver siguiente libro\n";
        cout << "4. Ver libro anterior\n";
        cout << "Elige una opción: ";
        cin >> sel;

        switch (sel) {
            case 0: return 0; // Regresar al menú superior
            case 1: modificar(seleccionado, autor); return 0; break;
            case 2: borrarLibro(seleccionado, buscar(autor)); return 0; // Eliminar y regresar
            case 3: if (seleccionado->siguiente != nullptr) seleccionado = seleccionado->siguiente; break;
            case 4: if (seleccionado->anterior != nullptr) seleccionado = seleccionado->anterior; break;
            default: cout << "Opción no válida.\n"; break;
        }
    } while (sel!=0); // Repetir hasta que el usuario elija regresar
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
                continue; // Regresar al menú de libros
            }
        } else {
            cout << "Número no válido. Intenta de nuevo.\n";
        }
    } while (true);
}

void imprimir_autores() {
    int sel;
    while (true) { // Bucle para mantener al usuario en la lista de autores
        cout<<"------------- Menú Autores -------------"<< endl;
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
            imprimir_lib_aut(imprimir->primer_Lib->siguiente, imprimir->autor);
        } else {
            cout << "Número no válido. Intenta de nuevo.\n";
        }
    }
}

Autor* buscar_por_lib(const char* libro) {
    for (Autor* temp = listaAutor; temp != nullptr; temp = temp->siguiente) {
        Libro* libroTemp = temp->primer_Lib;
        while (libroTemp != nullptr) {
            if (strcmp(libroTemp->titulo, libro) == 0) { // Comparar cadenas correctamente
                return temp; // Retornar el autor al que pertenece el libro
            }
            libroTemp = libroTemp->siguiente;
        }
    }
    return nullptr; // Retornar nullptr si no se encuentra el libro
}

Libro* buscar_libro(Autor* d_libro, const char* libro) {
    Libro* temp = d_libro->primer_Lib;
    while (temp != nullptr) {
        if (strcmp(temp->titulo, libro) == 0) { // Comparar cadenas correctamente
            return temp; // Retornar el libro encontrado
        }
        temp = temp->siguiente;
    }
    return nullptr; // Retornar nullptr si no se encuentra el libro
}

void imprimir_p_tipo(Genero gen) {
    for (Autor* temp = listaAutor; temp != nullptr; temp = temp->siguiente) {
        Libro* libroTemp = temp->primer_Lib;
        bool tieneLibros = false;

        // Verificar si el autor tiene libros del género especificado
        while (libroTemp != nullptr) {
            if (libroTemp->tipo == gen) {
                if (!tieneLibros) { 
                    // Imprimir el autor solo si tiene libros del género
                    cout << "------------- " << temp->autor << " -------------\n";
                    tieneLibros = true;
                }
                cout << libroTemp->titulo << endl; // Imprimir el título del libro
            }
            libroTemp = libroTemp->siguiente;
        }
    }
}

void por_Crit() {
    while (true) { // Bucle para buscar por criterio
        int criterio;
        char Nom_til[50];

        cout << "------------- Buscar por Criterio -------------" << endl;
        cout << "0. Regresar al menú principal\n1. Por Autor\n2. Por Titulo\n3. Por Genero\n";
        cout << "Elige una opción: ";
        cin >> criterio;

        switch (criterio) {
        case 0: // Regresar al menú principal
            return;

        case 1: { // Buscar por autor
            cout << "Escribe el nombre del autor: ";
            cin.ignore(); // Limpiar el buffer en caso de caracteres residuales
            cin.getline(Nom_til, sizeof(Nom_til));

            Autor* local = buscar(Nom_til);
            if (local != nullptr) {
                imprimir_lib_aut(local->primer_Lib->siguiente, local->autor);
            } else {
                cout << "No se encontró el autor.\n";
            }
            break;
        }

        case 2: { // Buscar por título
            cout << "Escribe el título del libro: ";
            cin.ignore(); // Limpiar el buffer en caso de caracteres residuales
            cin.getline(Nom_til, sizeof(Nom_til));

            Autor* local = buscar_por_lib(Nom_til); // Buscar el autor por el título del libro
            if (local != nullptr) {
                Libro* lib = buscar_libro(local, Nom_til); // Buscar el libro dentro del autor encontrado
                if (lib != nullptr) {
                    imprimir_libro(lib, local->autor); // Imprimir detalles del libro
                } else {
                cout << "El libro existe, pero no se pudo acceder correctamente.\n";
                }
            } else {
                cout << "No se encontró un autor con un libro titulado '" << Nom_til << "'.\n";
            }
            break;
        }

        case 3: { // Buscar por género
            int genero;
            cout << "Géneros disponibles:\n";
            cout << "1. CIENCIA_FICCION\n2. HORROR\n3. ROMANCE\n4. CIENCIA\n5. AVENTURA\n";
            cout << "Elige un género: ";
            cin >> genero;

            if (genero < 1 || genero > 5) {
                cout << "Opción no válida. Intenta de nuevo.\n";
                break;
            }

            Genero tipo;
            switch (genero) {
            case 1: tipo = CIENCIA_FICCION; break;
            case 2: tipo = HORROR; break;
            case 3: tipo = ROMANCE; break;
            case 4: tipo = CIENCIA; break;
            case 5: tipo = AVENTURA; break;
            }
            imprimir_p_tipo(tipo);
            break;
        }

        default: // Opción no válida
            cout << "Opción no válida. Intenta de nuevo.\n";
            break;
        }
    }
};

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
//La hice pero no se que hacer con ella
bool traer_libros(){
    string linea;
    char Libro_bus[50]="";
    ifstream fs("Libros.txt");
    while (getline(fs, linea))
    {
        int i=0;
        for(; i < linea.length();i++){
            if (linea[i]==','){break;}
            Libro_bus[i]=linea[i];
        }
        Libro_bus[i]='\0';
        memset(Libro_bus, 0, sizeof(Libro_bus));
    }
    fs.close();
    return true;
}

void agregar(){
    Genero tipo;
    char titulo[100], autor[50];
    int ano, mes, dia, dig;
}

int main() {
    crearLibro(AVENTURA,"Robert Jordan", "El ojo del mundo", 1990,1,15);
    crearLibro(AVENTURA,"Robert Jordan", "El despertar de los heroes", 1990,11,15);
    crearLibro(AVENTURA,"Robert Jordan", "El dragon renacido", 1991,9,15);
    crearLibro(AVENTURA,"Robert Jordan", "El aumento de la sombra", 1992,9,15);
    crearLibro(HORROR, "Stephen King", "It",1986,9,15);
    menu();
    return 0;
};