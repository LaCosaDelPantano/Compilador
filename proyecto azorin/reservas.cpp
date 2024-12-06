#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

#define MAX_RESERVAS 100

// Estructura para representar una reserva
struct Reserva {
    string nombreCliente;
    string tipoHabitacion; // Ej: "Individual", "Doble", "Suite"
    int dia;
    int mes;
    int anio;
};

// Declaraciones globales
Reserva reservas[MAX_RESERVAS];
int totalReservas = 0;

// Prototipos de funciones
void agregarReserva();
void mostrarReservas();
void ordenarPorFecha();
void ordenarPorNombre();
void ordenarPorTipoHabitacion();
int particion(Reserva arr[], int inicio, int fin, bool (*criterio)(const Reserva&, const Reserva&));
void quickSort(Reserva arr[], int inicio, int fin, bool (*criterio)(const Reserva&, const Reserva&));
bool esMenorFecha(const Reserva& a, const Reserva& b);
bool esMenorNombre(const Reserva& a, const Reserva& b);
bool esMenorTipoHabitacion(const Reserva& a, const Reserva& b);

// Menú principal
int main() {
    int opcion;
    do {
        cout << "\n--- Sistema de Reservas de Hotel ---\n";
        cout << "1. Agregar nueva reserva\n";
        cout << "2. Mostrar todas las reservas\n";
        cout << "3. Ordenar reservas por fecha\n";
        cout << "4. Ordenar reservas por nombre del cliente\n";
        cout << "5. Ordenar reservas por tipo de habitacion\n";
        cout << "6. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregarReserva();
                break;
            case 2:
                mostrarReservas();
                break;
            case 3:
                ordenarPorFecha();
                break;
            case 4:
                ordenarPorNombre();
                break;
            case 5:
                ordenarPorTipoHabitacion();
                break;
            case 6:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion != 6);
    return 0;
}

// Función para agregar una nueva reserva
void agregarReserva() {
    if (totalReservas >= MAX_RESERVAS) {
        cout << "No se pueden agregar más reservas. Capacidad máxima alcanzada.\n";
        return;
    }

    Reserva nuevaReserva;
    cin.ignore(); // Limpiar buffer antes de capturar cadenas

    cout << "Ingrese el nombre del cliente: ";
    getline(cin, nuevaReserva.nombreCliente);

    cout << "Ingrese el tipo de habitacion (Ej: Individual, Doble, Suite): ";
    getline(cin, nuevaReserva.tipoHabitacion);

    cout << "Ingrese la fecha de la reserva (dd mm aaaa): ";
    cin >> nuevaReserva.dia >> nuevaReserva.mes >> nuevaReserva.anio;
    if (nuevaReserva.dia > 31)
    {
        cout<<"Debes ingresar un dia valido, al menos que vengas del futuro y ya hay mas de 31 dias ";
        return;
    }
    if (nuevaReserva.mes > 12)
    {
        cout<<"Ah perro no sabia que habian mas de 12 meses ";
        return;
    }

    reservas[totalReservas++] = nuevaReserva;
    cout << "Reserva agregada exitosamente.\n";
}

// Función para mostrar todas las reservas
void mostrarReservas() {
    if (totalReservas == 0) {
        cout << "No hay reservas registradas.\n";
        return;
    }

    cout << "\n--- Lista de Reservas ---\n";
    for (int i = 0; i < totalReservas; i++) {
        cout << "Reserva " << i + 1 << ":\n";
        cout << "Nombre del Cliente: " << reservas[i].nombreCliente << "\n";
        cout << "Tipo de Habitacion: " << reservas[i].tipoHabitacion << "\n";
        cout << "Fecha: " << (reservas[i].dia < 10 ? "0" : "") << reservas[i].dia << "/"
             << (reservas[i].mes < 10 ? "0" : "") << reservas[i].mes << "/"
             << reservas[i].anio << "\n";
        cout << "--------------------------\n";
    }
}

// Funciones de comparación
bool esMenorFecha(const Reserva& a, const Reserva& b) {
    if (a.anio != b.anio) return a.anio < b.anio;
    if (a.mes != b.mes) return a.mes < b.mes;
    return a.dia < b.dia;
}

bool esMenorNombre(const Reserva& a, const Reserva& b) {
    return a.nombreCliente < b.nombreCliente;
}

bool esMenorTipoHabitacion(const Reserva& a, const Reserva& b) {
    return a.tipoHabitacion < b.tipoHabitacion;
}

// Función para particionar el array según un criterio
int particion(Reserva arr[], int inicio, int fin, bool (*criterio)(const Reserva&, const Reserva&)) {
    Reserva pivote = arr[fin];
    int i = inicio - 1;

    for (int j = inicio; j < fin; j++) {
        if (criterio(arr[j], pivote)) {
            i++;
            Reserva temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    Reserva temp = arr[i + 1];
    arr[i + 1] = arr[fin];
    arr[fin] = temp;

    return i + 1;
}

// QuickSort genérico
void quickSort(Reserva arr[], int inicio, int fin, bool (*criterio)(const Reserva&, const Reserva&)) {
    if (inicio < fin) {
        int pi = particion(arr, inicio, fin, criterio);
        quickSort(arr, inicio, pi - 1, criterio);
        quickSort(arr, pi + 1, fin, criterio);
    }
}

// Funciones para ordenar por criterios específicos
void ordenarPorFecha() {
    quickSort(reservas, 0, totalReservas - 1, esMenorFecha);
    cout << "Reservas ordenadas por fecha exitosamente.\n";
    mostrarReservas();
}

void ordenarPorNombre() {
    quickSort(reservas, 0, totalReservas - 1, esMenorNombre);
    cout << "Reservas ordenadas por nombre del cliente exitosamente.\n";
    mostrarReservas();
}

void ordenarPorTipoHabitacion() {
    quickSort(reservas, 0, totalReservas - 1, esMenorTipoHabitacion);
    cout << "Reservas ordenadas por tipo de habitacion exitosamente.\n";
    mostrarReservas();
}
