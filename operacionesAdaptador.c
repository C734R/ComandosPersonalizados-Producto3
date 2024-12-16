// Diseñar una función cuya funcionalidad que muestre los adaptadores de red de la máquina local. 
// Se mostrará y almacenará en un archivo adaptador.txt la información para un adaptador de red; 
// él cual, se preguntará previamente al usuario (elegido por el usuario) su IP, máscara, y puerta 
// de enlace. El resto de configuraciones de red no se han de mostrar esta información. Para 
// realizar lo anterior la función lanzará un comando de dos que le proporcionará la información 
// que necesita junto con otra que no se considera relevante, y será capaz de extraer y mostrar 
// la que se ha detallado. (menú punto3)*/

// Añadir librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "adaptador.h"
#include "entrada.h"
#include "ping.h"

void modificarAdaptadorRed(FILE* archivoParam) {
    char adaptador[1024] = { "" };

    // Pedir el adaptador a modificar
    if (pedirAdaptadorReintentos(adaptador, true));
    // Si no existe adaptador o algo no ha ido bien, volvemos
    else return;

    // Pedir y registrar los datos del adaptador a modificar
    registrarDatosAdaptador(archivoParam, adaptador);

    // Cerrar el archivo
    fclose(archivoParam);
}


// Función para copiar la información de un adaptador de red en un archivo
void copiarAdaptadorRed(FILE* archivoParam) {

    char buffer[1024];
    int tamBuffer = sizeof(buffer);
    char entradaProcesada[1024];
    char comando[2048];
    bool dentroBloque = false;
    FILE* consola = NULL;
    int i = 0;

    // Si no se han podido mostrar los adaptadores, salir de la función
    if (!mostrarSoloAdaptadores()) {
        printf("No se puede mostrar la lista de adaptadores. Volviendo... \n\n");
        return;
    }
    // Pedir el nombre del adaptador de red
    printf("Introduce el nombre del adaptador de red del que quieres guardar su información: ");
    entradaSinNL(buffer, tamBuffer);
    strcpy(entradaProcesada, buffer);
    printf("Adaptador de red introducido: %s\n", entradaProcesada);
    // Si no se ha encontrado el adaptador, salir de la función
    if (!encontrarAdaptador(entradaProcesada)) {
        printf("Adaptador de red no encontrado. Volviendo... \n\n");
        return;
    }

    // Crear el comando para obtener la información del adaptador de red 
    sprintf(comando, "ipconfig | findstr /C:\"%s\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i", entradaProcesada);
    // Ejecutar el comando definido
    consola = _popen(comando, "r");

    // Inicializar contador
    i = 0;
    // Mientras haya respuesta del comando ipconfig
    while (fgets(buffer, tamBuffer, consola) != NULL) {

        // Verificar si es el bloque del adaptador solicitado
        if (strstr(buffer, entradaProcesada) != NULL && !dentroBloque) {
            printf("--- Datos del adaptador guardado ---\n");
            // Establecer que estamos dentro del bloque del adaptador
            dentroBloque = true;
            fprintf(archivoParam, "-----------------------------------\n");
            fprintf(archivoParam, "--- Datos del adaptador guardado ---\n");
            // Escribir la información en el archivo adaptador.txt y mostrar
            fprintf(archivoParam, "%s", buffer);
            printf("%s", buffer);
        }

        // Si estamos dentro del bloque del adaptador
        if (dentroBloque) {
            // Si la línea contiene información relevante
            if (strstr(buffer, "IPv4") != NULL || strstr(buffer, "subred") != NULL || strstr(buffer, "enlace") != NULL) {
                // Escribir la información en el archivo adaptador.txt y mostrar
                fprintf(archivoParam, "%s", buffer);
                printf("%s", buffer);
                // Incrementar el contador de líneas
                i++;
                // Si se han mostrado las 3 líneas, salir bucle
                if (i == 3) {
                    break;
                }
            }
        }
    }
    fprintf(archivoParam, "-----------------------------------\n\n");

    printf("-----------------------------------\n\n");
    printf("Información del adaptador de red guardada en adaptador.txt.\n\n");

    // Cerrar el archivo
    fclose(archivoParam);

    // Cerrar la conexión con el comando ipconfig
    _pclose(consola);
}

// Función para añadir los datos de un nuevo adaptador de red en un archivo
void addAdaptadorRed(FILE* archivoParam) {

    char adaptador[1024] = { "" };

    // Pedir el adaptador a añadir
    if (pedirAdaptadorReintentos(adaptador, false));
    // Si existe adaptador o algo no ha ido bien, volvemos
    else return;

    // Pedir y registrar los datos del adaptador a añadir
    registrarDatosAdaptador(archivoParam, adaptador);

    // Cerrar el archivo
    fclose(archivoParam);

}