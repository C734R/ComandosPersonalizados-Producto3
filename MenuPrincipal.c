// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada.h"
#include "actualizarDNS.h"
#include "menuProducto2.h"

// Programa principal
int main() {

    // Declaración de variables
    char opcion[10];
    long opcion_int = -1;  

    // Bucle mientras no se seleccione la opción de salir
    do {
        // Menú principal
        printf("Bienvenid@ al programa.\n");
        printf("--- Menú Principal ---\n");
        printf("1. Actualizar DNS de adaptador.\n");
        printf("2. Menú Producto 2.\n");
        printf("0. Salir del programa.\n");
        printf("Selecciona una opción: ");

        // Leer la opción
        entradaConNL(opcion, sizeof(opcion));

        // Si la opción está vacía, se registra como -1
		if (strcmp(opcion, "\n") == 0) opcion_int = -1;
		// Si no, se convierte a un número
        else opcion_int = strtol(opcion, NULL, 10);

        printf("\n");

        // Según la opción seleccionada
        switch (opcion_int){
			// Proceder a la actualización de DNS de adaptador por el más rápido.
            case 1:
				if (!actualizarDNS()) printf("Error al actualizar las DNS del adaptador. Volviendo al menú principal...\n");
				pausaEnter();
                break;
			// Mostrar menú Producto 2
            case 2:
                menuProducto2();
                break;
            // Salir del programa. 
            case 0:
                printf("Saliendo de la aplicación...\n");
                pausaEnter();
                break;
            // Opción inválida.
            default:
                printf("Opción inválida. Por favor, seleccionz una opción válida.\n\n");
        }
    } 
    while (opcion_int != 0);

    return 0;
}