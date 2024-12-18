// Añadir librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "adaptador.h"
#include "entrada.h"
#include "ping.h"

bool registrarDatosAdaptador(FILE* archivoParam, char* adaptador) {
    char buffer[1024];
    int tamBuffer = sizeof(buffer);

    // Registrar el nombre del adaptador en el archivo adaptador.txt
    fprintf(archivoParam, "-----------------------------------\n");
    fprintf(archivoParam, "Adaptador de Red %s\n", adaptador);
    fprintf(archivoParam, "-----------------------------------\n");
    // Mostrar un mensaje de éxito
    printf("Nombre del adaptador registrado con éxito.\n\n");

    // Pedir datos del adaptador
    if (!pedirDatos(archivoParam)) {
        printf("Error en la introducción de datos");
        fclose(archivoParam);
        return false;
    }

    printf("\n");
    // Volver al inicio del archivo
    rewind(archivoParam);
    // Mostar el contenido del archivo adaptador.txt
    while (fgets(buffer, tamBuffer, archivoParam) != NULL) {
        // Mostrar el contenido del archivo
        printf("%s", buffer);
    }

    // Mostrar un mensaje de éxito
    printf("Se han registrado con éxito los datos del adaptador introducidos.\n\n");
    return true;
}

// Función para pedir datos de un adaptador de red
bool pedirDatos(FILE* archivoParam) {
    
    char buffer[1024];
    int tamBuffer = sizeof(buffer);
    char entradaProcesada[1024];
    int intentos = 0;

    // Mientras los carácteres introducidos no cumplan con el formato de una IP
    while (1 && intentos < 3) {
        // Solicitar la IP
        printf("Introduce la IP: ");
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);
        // Si la IP no es válida
        if (!validarIP(entradaProcesada)) {
            // Mostrar un mensaje de error
            printf("La IP introducida no es válida. Introduce una IP válida.\n");
            intentos++;
        }
        // Si la IP es válida
        else {
            // Registrar la IP en el archivo adaptador.txt
            fprintf(archivoParam, "Dirección IPv4: %s\n", entradaProcesada);
            // Salir del bucle
            break;
        }
    }

    if (intentos < 3) intentos = 0;

    // Mientras los carácteres introducidos no cumplan con el formato de una máscara
    while (1 && intentos < 3) {
        // Solicitar la máscara
        printf("Introduce la máscara de subred: ");
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);
        // Si la máscara no es válida
        if (!validarIP(entradaProcesada)) {
            // Mostrar un mensaje de error
            printf("La máscara introducida no es válida. Introduce una máscara válida.\n");
            intentos++;
        }
        // Si la máscara es válida
        else {
            // Registrar la máscara en el archivo adaptador.txt
            fprintf(archivoParam, "Máscara: %s\n", entradaProcesada);
            // Salir del bucle
            break;
        }
    }

    if (intentos < 3) intentos = 0;

    // Mientras los carácteres introducidos no cumplan con el formato de una puerta de enlace
    while (1 && intentos < 3) {
        // Solicitar la puerta de enlace
        printf("Introduce la puerta de enlace: ");
        // Leer la puerta de enlace
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);
        // Si la puerta de enlace no es válida
        if (!validarIP(entradaProcesada)) {
            // Mostrar un mensaje de error
            printf("La puerta de enlace introducida no es válida. Introduce una puerta de enlace válida.\n");
            intentos++;
        }
        // Si la puerta de enlace es válida
        else {
            // Registrar la puerta de enlace en el archivo adaptador.txt
            fprintf(archivoParam, "Puerta de enlace: %s\n", entradaProcesada);
            // Salir del bucle
            break;
        }
    }
    if (intentos < 3) fprintf(archivoParam, "-----------------------------------\n\n");

    // Si superamos los intentos el fichero se vacía
    if (intentos > 2) {
        printf("Has superado el número máximo de intentos permitidos. Volviendo...\n\n");
        // Abrir el archivo en modo escritura para vaciarlo
        freopen(NULL, "w", archivoParam);
        if (archivoParam == NULL) {
            perror("Error al intentar vaciar el fichero.\n\n");
            return false;
        }
        return false;
    }

    // Devolver éxito
	return true;
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
bool encontrarAdaptador(char* nAdaptador) {

    char comando[2048];
    FILE* consola = NULL;
    char buffer[1024];
    size_t tamBuffer = sizeof(buffer);
    char* ptrInicio = NULL;
    size_t longitud = strlen(nAdaptador);
    char* coincidencia = (char*)malloc(longitud+1);
    if (!coincidencia) {
        perror("Error al reservar memoria");
        return false;
    }

    // Crear el comando para obtener la información del adaptador de red seleccionado
    sprintf(comando, "ipconfig | findstr /C:\"%s\" /i", nAdaptador);
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
            // Detectar el inicio de un bloque de adaptador ethernet
            if ((ptrInicio = strstr(buffer, "Adaptador de Ethernet ")) != NULL) {
                ptrInicio += strlen("Adaptador de Ethernet ");
            }
            // Detectar el inicio de un bloque de adaptador de LAN inalámbrica
            else if ((ptrInicio = strstr(buffer, "Adaptador de LAN inal")) != NULL) {
                ptrInicio += strlen("Adaptador de LAN inal") + 8;
            }
            else continue;

            // Extraer los carácteres hasta la lingitud del adaptador introducido e introducir caracter final
			strncpy(coincidencia, ptrInicio, longitud);
            coincidencia[longitud] =  '\0';

            // Comparar el nombre del adaptador con el solicitado
            if (strcmp(coincidencia, nAdaptador) == 0) {
                _pclose(consola);
                printf("El adaptador '%s' existe.\n\n", nAdaptador);
                free(coincidencia);
                return true;
            }
        }
        // Cerrar la conexión con el comando ipconfig
        _pclose(consola);
        free(coincidencia);
        // Registramos que no existe el adaptador
        return false;
    }
}

// Función para pedir el nombre de un adaptador de red
char* pedirAdaptador() {
    // Declaración de variables
    char buffer[1024];
    int tamBuffer = 1024;
    char adaptador[1024] = { "" };

    // Pedir el adaptador de red 
    printf("Introduce el nombre del adaptador de red sobre el que quieres operar: ");
    entradaSinNL(buffer, tamBuffer);
    strcpy(adaptador, buffer);

    //printf("Adaptador de red introducido: %s\n", adaptador);
    return adaptador;
}

// Función para pedir adaptador existente con reintentos
bool pedirAdaptadorReintentos(char * adaptador, bool exista) {
    // Declaración de variables
    char buffer[1024];
    int tamBuffer = 1024;
    bool encontrado = false;
    
    // Pedir el nombre del adaptador de red hasta que se elija existente o se decida salir
    do {
        // Mostrar los adaptadores de red
        if (!mostrarSoloAdaptadores()) {
            return false;
        }
        // Pedir el nombre del adaptador de red
        strcpy(adaptador, pedirAdaptador());
        // Buscar adaptador
        encontrado = encontrarAdaptador(adaptador);
        // Comprobar elección de adaptador
        if (encontrado && *&exista) break;
        else if (!encontrado && !*&exista) break;
        if (!encontrado) printf("Adaptador no existe. ¿Desea introducir otro adaptador? (si = sí / otro = salir): ");
        if (encontrado) printf("Adaptador ya existe. ¿Desea introducir otro adaptador? (si = sí / otro = salir): ");
        entradaSinNL(buffer, tamBuffer);
        if (strcmp(buffer, "si") == 0) {
            continue;
        }
        printf("Volviendo...\n");
        return false;
    } while (1);

    // Devolver éxito
    return true;
}