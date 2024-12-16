// Dise�ar una funci�n cuya funcionalidad que muestre los adaptadores de red de la m�quina local. 
// Se mostrar� y almacenar� en un archivo adaptador.txt la informaci�n para un adaptador de red; 
// �l cual, se preguntar� previamente al usuario (elegido por el usuario) su IP, m�scara, y puerta 
// de enlace. El resto de configuraciones de red no se han de mostrar esta informaci�n. Para 
// realizar lo anterior la funci�n lanzar� un comando de dos que le proporcionar� la informaci�n 
// que necesita junto con otra que no se considera relevante, y ser� capaz de extraer y mostrar 
// la que se ha detallado. (men� punto3)*/

// A�adir librer�as necesarias
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


// Funci�n para copiar la informaci�n de un adaptador de red en un archivo
void copiarAdaptadorRed(FILE* archivoParam) {

    char buffer[1024];
    int tamBuffer = sizeof(buffer);
    char entradaProcesada[1024];
    char comando[2048];
    bool dentroBloque = false;
    FILE* consola = NULL;
    int i = 0;

    // Si no se han podido mostrar los adaptadores, salir de la funci�n
    if (!mostrarSoloAdaptadores()) {
        printf("No se puede mostrar la lista de adaptadores. Volviendo... \n\n");
        return;
    }
    // Pedir el nombre del adaptador de red
    printf("Introduce el nombre del adaptador de red del que quieres guardar su informaci�n: ");
    entradaSinNL(buffer, tamBuffer);
    strcpy(entradaProcesada, buffer);
    printf("Adaptador de red introducido: %s\n", entradaProcesada);
    // Si no se ha encontrado el adaptador, salir de la funci�n
    if (!encontrarAdaptador(entradaProcesada)) {
        printf("Adaptador de red no encontrado. Volviendo... \n\n");
        return;
    }

    // Crear el comando para obtener la informaci�n del adaptador de red 
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
            // Escribir la informaci�n en el archivo adaptador.txt y mostrar
            fprintf(archivoParam, "%s", buffer);
            printf("%s", buffer);
        }

        // Si estamos dentro del bloque del adaptador
        if (dentroBloque) {
            // Si la l�nea contiene informaci�n relevante
            if (strstr(buffer, "IPv4") != NULL || strstr(buffer, "subred") != NULL || strstr(buffer, "enlace") != NULL) {
                // Escribir la informaci�n en el archivo adaptador.txt y mostrar
                fprintf(archivoParam, "%s", buffer);
                printf("%s", buffer);
                // Incrementar el contador de l�neas
                i++;
                // Si se han mostrado las 3 l�neas, salir bucle
                if (i == 3) {
                    break;
                }
            }
        }
    }
    fprintf(archivoParam, "-----------------------------------\n\n");

    printf("-----------------------------------\n\n");
    printf("Informaci�n del adaptador de red guardada en adaptador.txt.\n\n");

    // Cerrar el archivo
    fclose(archivoParam);

    // Cerrar la conexi�n con el comando ipconfig
    _pclose(consola);
}

// Funci�n para a�adir los datos de un nuevo adaptador de red en un archivo
void addAdaptadorRed(FILE* archivoParam) {

    char adaptador[1024] = { "" };

    // Pedir el adaptador a a�adir
    if (pedirAdaptadorReintentos(adaptador, false));
    // Si existe adaptador o algo no ha ido bien, volvemos
    else return;

    // Pedir y registrar los datos del adaptador a a�adir
    registrarDatosAdaptador(archivoParam, adaptador);

    // Cerrar el archivo
    fclose(archivoParam);

}