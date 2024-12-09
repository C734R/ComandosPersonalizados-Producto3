// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menuProducto2.h"
#include "entrada.h"
#include "fecha.h"
#include "ping.h"
#include "adaptador.h"
#include "archivo.h"

// Programa Producto 2
int menuProducto2() {

    // Declaración de variables
    char opcion[10];
    FILE *archivo = NULL;
    long opcion_int = -1;  
    char buffer[1024];
    int tamBuffer = 1024;
    char entradaProcesada[1024];

    // Bucle mientras no se seleccione la opción de salir
    do {
        // Menú principal
        printf("Bienvenid@ al programa.\n");
        printf("--- Menú Principal ---\n");
        printf("1. Insertar fecha y hora.\n");
        printf("2. Comprobar IPs.\n");
		printf("3. Mostrar adaptadores de red y registrar nuevos parámetros.\n");
        printf("4. Mostrar adaptadores de red y registrar seleccionado.\n");
        printf("5. Mostrar adaptadores de red y añadir uno diferente.\n");
        printf("6. Vaciar archivo.\n");
        printf("0. Salir del programa.\n");
        printf("Selecciona una opción: ");

        // Leer la opción
        entradaConNL(opcion, sizeof(opcion));

        // Convertir la opción a un número
        opcion_int = strtol(opcion, NULL, 10);
        
        printf("\n");

        // Según la opción seleccionada
        switch (opcion_int){
            // Insertar fecha y hora
            case 1:
                if (abrirArchivo("C:/temp/producto2.txt", "rt+", &archivo)){
                    insertarFechaHora(archivo);
                    fclose(archivo);
                };
                break;
            // Comprobar IPs
            case 2:
                if (abrirArchivo("C:/temp/producto2.txt", "at+", &archivo)){
                    pingIPs(archivo, "C:/temp/producto2.txt");
                    fclose(archivo);
                }
                break;

			// Mostrar adaptadores de red y registrar nuevos parámetros
            case 3:
                if (abrirArchivo("C:/temp/adaptador.txt", "wt+", &archivo)) {
                    modificarAdaptadorRed(archivo);
                    fclose(archivo);
                }
                break;            
            // Mostrar adaptadores de red y registrar seleccionado
            case 4:
                if (abrirArchivo("C:/temp/adaptador.txt", "wt+", &archivo)){
                    copiarAdaptadorRed(archivo);
                    fclose(archivo);
                }
                break;
            // Mostrar adaptadores de red y añadir uno diferente
            case 5:
                if (abrirArchivo("C:/temp/adaptador.txt", "wt+", &archivo)){
                    addAdaptadorRed(archivo);
                    fclose(archivo);
                }
                break;
            // Vaciar archivo
            case 6:
                printf("Introduce la ruta del archivo que deseas vaciar: ");
                entradaSinNL(buffer, tamBuffer);
                strcpy(entradaProcesada, buffer);
                if (!existeArchivo(entradaProcesada)){
                    printf("El archivo no existe.\n\n");
                    break;
                }
                // Si el archivo existe, vaciamos el archivo
                else {
                    vaciarArchivo(entradaProcesada);
                }
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