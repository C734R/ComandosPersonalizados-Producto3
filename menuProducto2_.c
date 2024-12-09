#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada.h"
#include "fecha.h"
#include "ping.h"
#include "adaptador.h"
#include "archivo.h"

// Programa principal
int menuProducto2() {

    // Declaraci�n de variables
    char opcion[10];
    FILE* archivo = NULL;
    int opcion_int = -1;  // Inicializamos la opci�n a -1
    char buffer[1024];
    int tamBuffer = 1024;
    char entradaProcesada[1024];

    // Bucle mientras no se seleccione la opci�n de salir
    do {
        // Men� principal
        printf("--- Men� Producto 2 ---\n");
        printf("1. Insertar fecha y hora.\n");
        printf("2. Comprobar IPs.\n");
        printf("3. Mostrar adaptadores de red y registrar seleccionado.\n");
        printf("4. Mostrar adaptadores de red y a�adir uno diferente.\n");
        printf("5. Vaciar archivo.\n");
        printf("0. Salir del programa.\n");
        printf("Selecciona una opci�n: ");

        // Leer la opci�n
        entradaConNL(opcion, sizeof(opcion));

        // Convertir la opci�n a un n�mero
        opcion_int = atoi(opcion);

        // Vaciar entrada
        vaciarEntrada(opcion);

        // Mientras la opci�n no sea un n�mero y no est� vac�a registrar error
        while ((opcion_int < 0 || opcion_int > 5)) {
            opcion_int = -1;
            break;
        }

        // Seg�n la opci�n seleccionada
        switch (opcion_int) {
            // Insertar fecha y hora
        case 1:
            // Si se ha podido abrir el archivo en modo escritura y lectura al inicio del archivo
            if (abrirArchivo("C:/temp/producto2.txt", "rt+", &archivo)) {
                // Llamamos a la funci�n para insertar la fecha y hora en el archivo
                insertarFechaHora(archivo);
                // Cerramos el archivo
                fclose(archivo);
            };
            // Salimos del bucle         
            break;
            // Comprobar IPs
        case 2:
            // Si se ha podido abrir el archivo en modo escritura y lectura al inicio del archivo
            if (abrirArchivo("C:/temp/producto2.txt", "at+", &archivo)) {
                // Llamamos a la funci�n para comprobar las IPs
                pingIPs(archivo, "C:/temp/producto2.txt");
                // Cerramos el archivo
                fclose(archivo);
            }
            // Salimos del bucle
            break;
            // Mostrar adaptadores de red y registrar seleccionado
        case 3:
            // Si se ha podido abrir el archivo en modo escritura y lectura al inicio del archivo
            if (abrirArchivo("C:/temp/adaptador.txt", "wt", &archivo)) {
                // Llamamos a la funci�n para mostrar los adaptadores de red, pasando el archivo como par�metro
                copiarAdaptadorRed(archivo);
                fclose(archivo);
            }
            break;
            // Mostrar adaptadores de red y a�adir uno diferente
        case 4:
            // Si existe el archivo, lo abrimos, mostramos los adaptadores de red,
            // pedimos seleccionar uno y cerramos el archivo
            if (abrirArchivo("C:/temp/adaptador.txt", "wt", &archivo)) {
                addAdaptadorRed(archivo);
                fclose(archivo);
            }
            break;
            // Vaciar archivo
        case 5:
            // Solicitamos la ruta del archivo a vaciar
            printf("Introduce la ruta del archivo que deseas vaciar: ");
            strcpy(entradaProcesada, entradaSinNL(buffer, stdin, tamBuffer));
            // Si no existe el fichero, mostramos error y salimos 
            if (!existeArchivo(entradaProcesada) && vaciarEntrada(buffer)) {
                printf("El archivo no existe.\n\n");
                break;
            }
            // Si el archivo existe, vaciamos el archivo
            else {
                vaciarArchivo(entradaProcesada);
            }
            // Librerar la memoria
            free(entradaProcesada);
            free(buffer);
            break;
            // Salir del programa. Informamos de la salida de la aplicaci�n y esperamos enter.
        case 0:
            printf("Saliendo de la aplicaci�n...\n");
            pausaEnter();
            break;
            // Opci�n inv�lida. Informamos de que la opci�n seleccionada no es v�lida
        default:
            printf("Opci�n inv�lida. Por favor, seleccionz una opci�n v�lida.\n\n");
        }
    } while (opcion_int != 0);

    return 0;
}