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
    char buffer[1024];
    int tamBuffer = sizeof(buffer);
    char entradaProcesada[1024];
    FILE* consola = NULL;
    int i = 0;
    bool bAdaptador = false;

    // Mientras el nombre de adaptador introducido exista
    while (1) {
        // Si no se han podido mostrar los adaptadores, salir
        if (!mostrarAdaptadores()) {
            printf("No se puede mostrar la lista de adaptadores. Volviendo... \n\n");
            break;
        }
        // Pedir el nombre del adaptador de red
        printf("Introduce el nombre del adaptador de red que quieres modificar: ");
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);
        printf("Adaptador de red introducido: %s\n", entradaProcesada);

        // Si se encuentra el adaptador
        if (encontrarAdaptador(entradaProcesada)) {
            bAdaptador = true;
            break;
        }
        // Si no se encuentra el adaptador
        else {
            printf("El adaptador no existe. Introduce otro nombre.\n\n");
            bAdaptador = false;
        }
    }

    // Si existe el adaptador
    if (bAdaptador) {

        // Registrar el nombre del adaptador en el archivo adaptador.txt
        fprintf(archivoParam, "-----------------------------------\n");
        fprintf(archivoParam, "Adaptador de Red %s\n", entradaProcesada);
        fprintf(archivoParam, "-----------------------------------\n");
        // Mostrar un mensaje de éxito
        printf("Nombre del adaptador registrado con éxito.\n\n");

        // Pedir datos del adaptador
        if (!pedirDatos(archivoParam)) {
            printf("Error en la introducción de datos");
            fclose(archivoParam);
            return;
        }

        // Volver al inicio del archivo
        rewind(archivoParam);
        // Mostar el contenido del archivo adaptador.txt
        while (fgets(buffer, tamBuffer, archivoParam) != NULL) {
            // Mostrar el contenido del archivo
            printf("%s", buffer);
        }

        // Mostrar un mensaje de éxito
        printf("Se han registrado con éxito los datos del adaptador introducidos.\n\n");


    }

    // Cerrar el archivo
    fclose(archivoParam);
}

// Función para pedir datos de un adaptador de red
bool pedirDatos(FILE* archivoParam) {
    
    char buffer[1024];
    int tamBuffer = sizeof(buffer);
    char entradaProcesada[1024];

    // Mientras los carácteres introducidos no cumplan con el formato de una IP
    while (1) {
        // Solicitar la IP
        printf("Introduce la IP: ");
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);
        // Si la IP no es válida
        if (!validarIP(entradaProcesada)) {
            // Mostrar un mensaje de error
            printf("La IP introducida no es válida. Introduce una IP válida.\n");
        }
        // Si la IP es válida
        else {
            // Registrar la IP en el archivo adaptador.txt
            fprintf(archivoParam, "Dirección IPv4: %s\n", entradaProcesada);
            // Salir del bucle
            break;
        }
    }

    // Mientras los carácteres introducidos no cumplan con el formato de una máscara
    while (1) {
        // Solicitar la máscara
        printf("Introduce la máscara de subred: ");
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);
        // Si la máscara no es válida
        if (!validarIP(entradaProcesada)) {
            // Mostrar un mensaje de error
            printf("La máscara introducida no es válida. Introduce una máscara válida.\n");
        }
        // Si la máscara es válida
        else {
            // Registrar la máscara en el archivo adaptador.txt
            fprintf(archivoParam, "Máscara: %s\n", entradaProcesada);
            // Salir del bucle
            break;
        }
    }

    // Mientras los carácteres introducidos no cumplan con el formato de una puerta de enlace
    while (1) {
        // Solicitar la puerta de enlace
        printf("Introduce la puerta de enlace: ");
        // Leer la puerta de enlace
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);
        // Si la puerta de enlace no es válida
        if (!validarIP(entradaProcesada)) {
            // Mostrar un mensaje de error
            printf("La puerta de enlace introducida no es válida. Introduce una puerta de enlace válida.\n");
        }
        // Si la puerta de enlace es válida
        else {
            // Registrar la puerta de enlace en el archivo adaptador.txt
            fprintf(archivoParam, "Puerta de enlace: %s\n", entradaProcesada);
            // Salir del bucle
            break;
        }
    }
    fprintf(archivoParam, "-----------------------------------\n\n");

    // Devolver éxito
	return true;
}


// Función para copiar la información de un adaptador de red en un archivo
void copiarAdaptadorRed(FILE *archivoParam) {

    char buffer[1024];
    int tamBuffer = sizeof(buffer);
    char entradaProcesada[1024];
    char comando[2048];
    bool dentroBloque = false;
    FILE* consola = NULL;
    int i = 0;

    // Si no se han podido mostrar los adaptadores, salir de la función
    if (!mostrarAdaptadores()) {
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
        // Detectar el inicio de un bloque de adaptador
        //if (strstr(_buffer, "Adaptador de Ethernet") != NULL || strstr(_buffer, "Adaptador de LAN inal") != NULL){
            
            // Verificar si es el bloque del adaptador solicitado
            if (strstr(buffer, entradaProcesada) != NULL) {
                printf("--- Datos del adaptador guardado ---\n");
                // Establecer que estamos dentro del bloque del adaptador
                dentroBloque = true;
                fprintf(archivoParam, "-----------------------------------\n");
                fprintf(archivoParam, "--- Datos del adaptador guardado ---\n");
                // Escribir la información en el archivo adaptador.txt y mostrar
                fprintf(archivoParam, "%s", buffer);
                printf("%s", buffer);
            }

            // Si no es el bloque del adaptador solicitado, false
            else if (strstr(buffer, "Adaptador") != NULL) dentroBloque = false;
        //}
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
                if (i == 3){
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
void addAdaptadorRed(FILE * archivoParam) {
    
    char buffer[1024];
    int tamBuffer = sizeof(buffer);
    char entradaProcesada[1024];
    bool bAdaptador = false;

    // Mientras el nombre de adaptador introducido exista
    while (1){
        // Si no se han podido mostrar los adaptadores, salir
        if (!mostrarAdaptadores()) {
            break;
        }
        // Si se han podido mostrar
        else {
            // Pedir el nombre del adaptador de red
            printf("Introduce el nombre del adaptador de red que quieres añadir: ");
            entradaSinNL(buffer, tamBuffer);
            strcpy(entradaProcesada, buffer);
            printf("Adaptador de red introducido: %s\n", entradaProcesada);
            
            // Si se encuentra el adaptador
            if(encontrarAdaptador(entradaProcesada)){
                printf("El adaptador ya existe. Introduce otro nombre.\n\n");
                bAdaptador = false;
            }
            // Si no se encuentra el adaptador
            else {
                bAdaptador = true;
                break;
            }   
        }
    }
    // Si no existe el adaptador
    if (bAdaptador){
        
        // Registrar el nombre del adaptador en el archivo adaptador.txt
        fprintf(archivoParam, "-----------------------------------\n");
        fprintf(archivoParam, "Adaptador de Red %s\n", entradaProcesada);
        fprintf(archivoParam, "-----------------------------------\n");
        // Mostrar un mensaje de éxito
        printf("Nombre del adaptador registrado con éxito.\n\n");

        // Pedir datos del adaptador
        if (!pedirDatos(archivoParam)) {
            printf("Error en la introducción de datos");
            fclose(archivoParam);
            return;
        }

        // Volver al inicio del archivo
        rewind(archivoParam);
        // Mostar el contenido del archivo adaptador.txt
        while (fgets(buffer, tamBuffer, archivoParam) != NULL) {
            // Mostrar el contenido del archivo
            printf("%s", buffer);
        }

        // Mostrar un mensaje de éxito
        printf("Se han registrado con éxito los datos del adaptador introducidos.\n\n");

        // Cerrar el archivo
        fclose(archivoParam);
    }
}

// Función para mostrar los adaptadores de red
bool mostrarAdaptadores(void){
    
    char comando[2048];
    FILE* consola = NULL;
    char buffer[1024];
    int tamBuffer = sizeof(buffer);

    // Definimos el comando a ejecutar para obtener los adaptadores de red
    sprintf(comando,"ipconfig | findstr /C:\"Adaptador\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i");
    consola = _popen(comando, "r");

    // Si no se ha podido ejecutar el comando ipconfig
    if (consola == NULL) {
        printf("Error al ejecutar el comando ipconfig.\n");
        return false;
    }
      
    // Mostrar un mensaje de éxito
    printf("Comando ipconfig ejecutado con éxito.\n\n");
    printf("--- Adaptadores de red ---\n");

    // Recorrer las líneas de la salida del comando ipconfig
    while (fgets(buffer, tamBuffer, consola) != NULL) {
        printf("%s", buffer);
    }
    printf("-----------------------------------\n\n");
        
    // Cerrar la conexión con el comando ipconfig
    _pclose(consola);

    // Devolver verdadero
    return true;
}

// Función para mostrar los adaptadores de red
bool mostrarSoloAdaptadores(void) {

    char comando[2048];
    FILE* consola = NULL;
    char buffer[1024];
    int tamBuffer = sizeof(buffer);

    // Definimos el comando a ejecutar para obtener los adaptadores de red
    sprintf(comando, "ipconfig | findstr /C:\"Adaptador\" /i");
    consola = _popen(comando, "r");

    // Si no se ha podido ejecutar el comando ipconfig
    if (consola == NULL) {
        printf("Error al ejecutar el comando ipconfig.\n");
        return false;
    }

    // Mostrar un mensaje de éxito
    printf("Comando ipconfig ejecutado con éxito.\n\n");
    printf("--- Adaptadores de red ---\n");

    // Recorrer las líneas de la salida del comando ipconfig
    while (fgets(buffer, tamBuffer, consola) != NULL) {
        printf("%s", buffer);
    }
    printf("-----------------------------------\n\n");

    // Cerrar la conexión con el comando ipconfig
    _pclose(consola);

    // Devolver verdadero
    return true;
}


// Función para comrpobar si existe un adaptador de red
bool encontrarAdaptador(char *nAdaptador){
    
    char comando[2048];
    FILE* consola = NULL;
    char buffer[1024];
    int tamBuffer = sizeof(buffer);

    // Crear el comando para obtener la información del adaptador de red que coincida con la IP, la máscara y la puerta de enlace introducidas
    sprintf(comando, "ipconfig | findstr /C:\"%s\" /C:\"IPv4\" /C:\"enlace\" /C:\"subred\" /i", nAdaptador);
    // Ejecutar el comando definido
    consola = _popen(comando, "r");
    // Si no se ha podido ejecutar el comando ipconfig
    if (consola == NULL) {
        // Mostrar un mensaje de error
        printf("Error al ejecutar el comando ipconfig.\n");
        return false;
    }
    else {
        // Mostrar un mensaje de éxito
        printf("Comando ipconfig ejecutado con éxito.\n\n");
        // Recorerr las líneas de la salida del comando ipconfig
        while (fgets(buffer, tamBuffer, consola) != NULL) {
            // Detectar el inicio de un bloque de adaptador
            if (strstr(buffer, "Adaptador de Ethernet") != NULL || strstr(buffer, "Adaptador de LAN inal") != NULL){
                // Verificar si encontramos el adaptador solicitado
                if (strstr(buffer, nAdaptador) != NULL) {
                    // Cerrar la conexión con el comando ipconfig
                    _pclose(consola);
                    printf("El adaptador existe.\n\n");
                    // Registramos que el adaptador ya existe
                    return true;
                }
            }
        }
        // Cerrar la conexión con el comando ipconfig
        _pclose(consola);
        // Registramos que no existe el adaptador
        return false;
    }
}