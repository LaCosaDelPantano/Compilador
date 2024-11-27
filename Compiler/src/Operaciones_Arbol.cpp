#include <iostream>
#include <string>

using namespace std;
bool verif;

struct Nodito // Se define un struct que crea un tipo de dato "Nodito"
{
    int valor;          // Valor almacenado en el nodo
    Nodito* izquierdo;  // Apuntador al subárbol izquierdo
    Nodito* derecho;    // Apuntador al subárbol derecho
};

Nodito* crearNodo(int valor) // Función para crear un nuevo nodo
{
    Nodito* nuevo = new Nodito();
    nuevo->valor = valor;
    nuevo->izquierdo = nullptr;
    nuevo->derecho = nullptr;
    return nuevo;
}

Nodito* recorrer(Nodito* raiz, int valor) // Función para localizar la posición de inserción
{
    if (raiz->valor == valor) // Si se encuentra el nodo o se llega a un nodo nulo
        verif=true;
    if (valor < raiz->valor) // Si el valor es menor, busca en el subárbol izquierdo
    {
        if (raiz->izquierdo == nullptr) // Si el subárbol izquierdo está vacío, retorna la raíz actual
            return raiz;
        return recorrer(raiz->izquierdo, valor); // Continúa recorriendo
    }
    else // Si el valor es mayor o igual, busca en el subárbol derecho
    {
        if (raiz->derecho == nullptr) // Si el subárbol derecho está vacío, retorna la raíz actual
            return raiz;
        return recorrer(raiz->derecho, valor); // Continúa recorriendo
    }
}

Nodito* insertar(Nodito* raiz, int valor) // Función para insertar un valor en el árbol binario
{
    if (raiz == nullptr) // Si el árbol está vacío, crea un nuevo nodo como raíz
        return crearNodo(valor);

    Nodito* posicion = recorrer(raiz, valor); // Encuentra la posición donde insertar el nuevo nodo

    if (valor < posicion->valor) // Si el valor es menor, se inserta a la izquierda
        posicion->izquierdo = crearNodo(valor);
    else // Si el valor es mayor o igual, se inserta a la derecha
        posicion->derecho = crearNodo(valor);
    return raiz; // Retorna la raíz del árbol
}

bool buscar(Nodito* raiz, int valor) 
 {
    if (raiz==NULL)
	  {
        return false; // Si el arbol esta vacio o se llega a un nodo nulo
      }
    recorrer(raiz, valor);
    return verif;
 }

void recorridoInorden(Nodito* raiz) // funcion para mostrar el arbol en orden (inorden)
  {
    if(raiz!=NULL) 
	 {
        recorridoInorden(raiz->izquierdo);    // Recorrer subarbol izquierdo
        cout<<raiz->valor<<" ";              // Se muestran los valor de la raíz
        recorridoInorden(raiz->derecho);    // Se recorre el subarbol derecho
     }
  }

int main() 
{
    system("color f0");   //Funcion para cambiar el fondo y color 
    Nodito* raiz = NULL; // Inicializar un arbol que esta vacio
    int opcion, valor;
    cout<<"\n\t\t\t\t\t      METODOS DE ORDENAMIENTO"<<endl;
    cout<<"\t\t\tElaborado por Bautista Mendoza Saul y Velazquez Caudillo Mauricio Emiliano"<<endl;  //Datos de los alumnos
    cout<<"\t\t\t\t\t\t  GRUPO: 5CM23"<<endl;
    cout<<"\t\t\t\t\t     Analisis de Algoritmos\n"<<endl;
    cout<<"------------------------------------------------------------------------------------------------------------------------"<<endl;
    do{   //Se usa un do-while, para que se pueda regresar al menu las veces que el usuario quiera 
        cout<<"1. Insertar un valor\n";   //Menu de operaciones basicas con arboles de busqueda binarios
        cout<<"2. Busqueda de un valor\n";
        cout<<"3. Eliminacion de un valor\n";
        cout<<"4. Mostrar recorrido inorden\n";
        cout<<"0. Salir\n";
        cout<<"Selecciona una opcion: ";   //Se ingresa el valor de la opcion
        cin>>opcion;
        switch (opcion) //Inicio del switch y declaracion de los casos
		{
            case 1:
                cout<<"Ingresa un dato a insertar: ";
                cin>>valor;
                raiz=insertar(raiz, valor); // Insertar el valor dado por el usuario en la raiz
                cout<<"Se inserto el dato correctamente.\n";
            break;
                
            case 2:
            	cout<<"Ingresa el dato a buscar: ";
                cin>>valor;
                  if(buscar(raiz, valor))
                    cout<<"El dato "<<valor<<" SI se encuentra en el arbol.\n";
                  else
                    cout<<"El dato "<<valor<<" NO se encuentra en el arbol.\n";
            break;
            
            case 3:
                cout<<"Ingresa el valor a eliminar: ";
                cin>>valor;
                //raiz=eliminacion(raiz, valor);
            break;
            
            case 4:
                cout<<"Recorrido inorden: ";
                recorridoInorden(raiz); // Mostrar el arbol en inorden
                cout<<endl;
            break;
            
            case 0:
                cout<<"PULSA ENTER PARA FINALIZAR\n";
            break;
            
            default:
                cout<<"No seleccionaste una opcion valida.";
        }
    } 
while(opcion!=0);
return 0;
}