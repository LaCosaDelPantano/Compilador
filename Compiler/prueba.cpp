#include <iostream>   //Bibliotecas usadas
#include <string>

using namespace std;

    struct Nodito //Se define un struct que va a crear un tipo de dato "Nodito"
	{
    int valor;            // Valor almacenado en el nodo
    Nodito* izquierdo;      // Apuntador al subárbol derecho e izquierdo
    Nodito* derecho;        
    };
    
Nodito* crearNodo(int valor) // Se define una funcion para crear un nuevo nodo
   {
      Nodito* nuevo=new Nodito();
      nuevo->valor=valor;
      nuevo->izquierdo=NULL;
      nuevo->derecho=NULL;
      return nuevo;
   }

Nodito* insertar(Nodito* raiz, int valor)  // Se define la funcion para insertar un valor en el arbol binario
  {
    if (raiz == NULL) // Si el arbol esta vacio, se crea un nuevo nodo
    return crearNodo(valor);

    if (valor<raiz->valor) // Se decide hacia donde se va insertar el valor
	 {
        raiz->izquierdo=insertar(raiz->izquierdo, valor); // Se inserta en el subarbol izquierdo
     } 
	else 
	 {
        raiz->derecho=insertar(raiz->derecho, valor); // Se inserta en el subarbol derecho
     }
    return raiz; // Se regresa la raiz actualizada
}

bool buscar(Nodito* raiz, int valor) 
 {
    if (raiz==NULL)
	  {
        return false; // Si el arbol esta vacio o se llega a un nodo nulo
      }
    if (raiz->valor==valor) 
	  {
        return true; // Valor encontrado
      } 
	     else if(valor<raiz->valor) 
	      {
           return buscar(raiz->izquierdo, valor); // Se busca en el subarbol izquierdo
          } 
	else 
     {
        return buscar(raiz->derecho, valor); // Se busca en el subarbol derecho
     }
 }

void recorridoInorden(Nodito* raiz) // funcion para mostrar el arbol en orden (inorden)
  {
    if(raiz!=NULL) 
	 {
        recorridoInorden(raiz->izquierdo);    // Recorrer subarbol izquierdo
        cout<<raiz->valor<<" ";              // Se muestran los valor de la raiz
        recorridoInorden(raiz->derecho);    // Se recorre el subarbol derecho
     }
  }
  
  
Nodito* encontrarMinimo(Nodito* raiz) // Se define una funcion para encontrar el nodo con el menor valor
{
    while (raiz->izquierdo != NULL) 
	  {
        raiz = raiz->izquierdo;
      }
    return raiz;
}
  
  Nodito* eliminacion(Nodito* raiz, int valor) 
  {
    if (raiz == NULL) 
	   {
        return raiz; // Caso base: el árbol está vacío o el valor no existe
       }

    if (valor < raiz->valor) 
	  {
        raiz->izquierdo = eliminacion(raiz->izquierdo, valor); // Buscar en el subárbol izquierdo
      } 
	else if (valor > raiz->valor) 
	  {
        raiz->derecho = eliminacion(raiz->derecho, valor); // Buscar en el subárbol derecho
      } 
	else 
	{
        // Caso 1: Nodo sin hijos
        if (raiz->izquierdo == NULL && raiz->derecho == NULL) 
		 {
            delete raiz;
            return NULL;
         }
        // Caso 2: Nodo con un hijo
        else if (raiz->izquierdo == NULL) 
		 {
            Nodito* temp = raiz->derecho;
            delete raiz;
            return temp;
         } 
		else if (raiz->derecho == NULL) 
		 {
            Nodito* temp = raiz->izquierdo;
            delete raiz;
            return temp;
         }
        // Caso 3: Nodo con dos hijos
        else 
		 {
            Nodito* temp = encontrarMinimo(raiz->derecho); // Encontrar sucesor en inorden
            raiz->valor = temp->valor; // Reemplazar el valor
            raiz->derecho = eliminacion(raiz->derecho, temp->valor); // Eliminar el sucesor
         }
    }
    return raiz;
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
                raiz=eliminacion(raiz, valor);
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