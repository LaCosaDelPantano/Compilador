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

// Función para crear un nuevo libro
Libro* crearLibro(Genero tipo, const char* autor, const char* titulo, int ano, int mes, int dia) {
    Autor* modificar = buscar(autor);
    if (modificar==nullptr){
        Autor* modificar = CrearAutor(autor);
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

void agregarLibro(Libro* nuevo,  Autor* autor){
    if (autor->primer_Lib==nullptr){
        nuevo->dig=1;
        autor->primer_Lib=nuevo;
    }else{
        Libro* temp = autor->primer_Lib;
        while(temp->siguiente!=nullptr){
            temp=temp->siguiente;
        }
        nuevo->dig=temp->dig+1;
        temp->siguiente = nuevo;
    }
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

void imprimir_lib_aut(Libro* seleccionado, const char* autor){
    int sel;
    Libro* buscar=seleccionado;
    while(seleccionado!=nullptr){
        cout<<seleccionado->dig<<". "<<seleccionado->titulo<<endl;
        seleccionado=seleccionado->siguiente;
    }
    cout<<"Elige el libro: "<<endl;
    cin>>sel;
    seleccionado=bus_lib(sel, buscar);
    if (seleccionado!=nullptr){
        cout<<"Titulo: "<<seleccionado->titulo<<endl;
        cout<<"Autor: "<<autor;
        cout<<"Fecha"<<seleccionado->dia<<"/"<<seleccionado->mes<<"/"<<seleccionado->ano;
        cout<<"Genero: "<<seleccionado->tipo;
    }else{cout<<"Numero no valido";}
}

void imprimir_autores(){
    int sel;
    Autor* imprimir =listaAutor;
    while (imprimir!=nullptr)
    {
        cout<<imprimir->digito<<". "<< imprimir->autor<<endl;
        imprimir=imprimir->siguiente;
    } 
    cout<<"Elige el Autor: "<<endl;
    cin>>sel;
    imprimir=bus_aut(sel);
    if (imprimir!=nullptr){
        imprimir_lib_aut(imprimir->primer_Lib, imprimir->autor);
    }else{cout<<"Numero no valido";}
}

void por_Crit(){
    int criterio;
    char Nom_til[50];
    cout<<"0. Regresar al Menú de Autores"<<endl;
    cout<<"1. Por Autor"<<endl;
    cout<<"2. Por Titulo"<<endl;
    cout<<"3. Por Genero"<<endl;
    cout<<"4. Por Fecha"<<endl;
    cin>>criterio;
    switch (criterio)
    {
    case 0:
        menu();
        break;
    case 1:
        cout<<"Escribe el nombre del autor: "<<endl;
        cin>>Nom_til;
        break;
    case 2:
        cout<<"Escribe el titulo del libro: "<<endl;
        cin>>Nom_til;
        break;
    case 3:
        cout<<"1. CIENCIA_FICCION"<<endl;
        cout<<"2. HORROR"<<endl;
        cout<<"3. ROMANCE"<<endl;
        cout<<"4. CIENCIA"<<endl;
        cout<<"5. AVENTURA"<<endl;
        break;
    default:
        break;
    }
}

void menu(){
    cout<<"0. Buscar por criterio"<<endl;
    cout<<"-------------Lista de Autores--------------"<<endl;
}

int main() {
    menu();
    return 0;
}