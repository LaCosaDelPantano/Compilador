#include <iostream>
using namespace std;

struct Nodito {
    int valor;
    Nodito* izquierdo = nullptr;
    Nodito* derecho = nullptr;
};

Nodito* crearNodo(int valor) { return new Nodito{valor}; }

Nodito* recorrer(Nodito* raiz, int valor) {
    if (!raiz || raiz->valor == valor) return raiz;
    return valor < raiz->valor ? (raiz->izquierdo ? recorrer(raiz->izquierdo, valor) : raiz)
                               : (raiz->derecho ? recorrer(raiz->derecho, valor) : raiz);
}

Nodito* insertar(Nodito* raiz, int valor) {
    if (!raiz) return crearNodo(valor);
    Nodito* pos = recorrer(raiz, valor);
    if (valor < pos->valor) pos->izquierdo = crearNodo(valor);
    else pos->derecho = crearNodo(valor);
    return raiz;
}

Nodito* encontrarMinimo(Nodito* raiz) {
    while (raiz && raiz->izquierdo) raiz = raiz->izquierdo;
    return raiz;
}

Nodito* eliminacion(Nodito* raiz, int valor) {
    if (!raiz) return nullptr;
    if (valor < raiz->valor) raiz->izquierdo = eliminacion(raiz->izquierdo, valor);
    else if (valor > raiz->valor) raiz->derecho = eliminacion(raiz->derecho, valor);
    else {
        if (!raiz->izquierdo || !raiz->derecho) {
            Nodito* temp = raiz->izquierdo ? raiz->izquierdo : raiz->derecho;
            delete raiz;
            return temp;
        }
        Nodito* sucesor = encontrarMinimo(raiz->derecho);
        raiz->valor = sucesor->valor;
        raiz->derecho = eliminacion(raiz->derecho, sucesor->valor);
    }
    return raiz;
}

void recorridoInorden(Nodito* raiz) {
    if (!raiz) return;
    recorridoInorden(raiz->izquierdo);
    cout << raiz->valor << " ";
    recorridoInorden(raiz->derecho);
}

bool buscar(Nodito* raiz, int valor) { return recorrer(raiz, valor) != nullptr; }

int main() {
    system("color f0");   //Funcion para cambiar el fondo y color 
    Nodito* raiz = NULL; // Inicializar un arbol que esta vacio
    int opcion, valor;
    cout<<"\n\t\t\t\t\t      METODOS DE ORDENAMIENTO"<<endl;
    cout<<"\t\t\tElaborado por Bautista Mendoza Saul y Velazquez Caudillo Mauricio Emiliano"<<endl;  //Datos de los alumnos
    cout<<"\t\t\t\t\t\t  GRUPO: 5CM23"<<endl;
    cout<<"\t\t\t\t\t     Analisis de Algoritmos\n"<<endl;
    cout<<"------------------------------------------------------------------------------------------------------------------------"<<endl;
    Nodito* raiz = nullptr;
    int opcion, valor;
    do {
        cout << "\n1. Insertar valor\n2. Buscar valor\n3. Eliminar valor\n4. Mostrar inorden\n0. Salir\nSelecciona: ";
        cin >> opcion;
        switch (opcion) {
            case 1:
                cout << "Dato a insertar: "; cin >> valor;
                raiz = insertar(raiz, valor);
                cout << "Dato insertado.\n";
                break;
            case 2:
                cout << "Dato a buscar: "; cin >> valor;
                cout << "El dato " << valor << (buscar(raiz, valor) ? " SI " : " NO ") << "se encuentra en el árbol.\n";
                break;
            case 3:
                cout << "Dato a eliminar: "; cin >> valor;
                raiz = eliminacion(raiz, valor);
                cout << "Dato eliminado.\n";
                break;
            case 4:
                cout << "Recorrido inorden: "; recorridoInorden(raiz); cout << endl;
                break;
            case 0:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }
    } while (opcion != 0);
    return 0;
}