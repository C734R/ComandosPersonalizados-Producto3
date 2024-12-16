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
        printf("--- Menú Principal Producto 3---\n");
        printf("1. Actualizar DNS de adaptador mediante archivo.\n");
        printf("2. Establecer DNS de adaptador manualmente. (Extra)\n");
        printf("3. Adaptador en modo DHCP. (Extra)\n");
        printf("4. Menú Producto 2.\n");
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
				if (!actualizarAutoDNS()) printf("Error al actualizar las DNS del adaptador. Volviendo al menú principal...\n");
				pausaEnter();
                break;
            // Establecer manualmente las DNS de un adaptador
            case 2:
                if (!actualizarManualDNS()) printf("Error al establecer manualmente las DNS en el adaptador. Volviendo al menú principal...\n");
                pausaEnter();
                break;
            // Establecer DHCP para adaptador seleccionado
            case 3:
                if (!actualizarDHCPDNS()) printf("Error al establecer el modo DHCP para el adaptador seleccionado. Volviendo al menú principal...\n");
                pausaEnter();
                break;
            // Mostrar menú Producto 2
            case 4:
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