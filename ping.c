// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "ping.h"
#include "archivo.h"
#include "entrada.h"
#include "fecha.h"

// Función para comprobar ping a IPs
void pingIPs(FILE *archivoParam, char *rutaParam) {
    
    // Declaración de variables
    char ip[16];
    FILE *archivoIPs;
    char comando[100];
    char lectura[100];
    char entradaProcesada[100];
    int totalIPs = 0;
    int numIPs = 0;
    char busqueda [200];
    char buffer[100];
	int tamBuffer = sizeof(buffer);
    char ipPositivas[10][16] = { "" };

    printf("Introduce la ruta del archivo que contiene las IPs a testear: ");
    entradaSinNL(buffer, tamBuffer);
    strcpy(entradaProcesada, buffer);

    if (!existeArchivo(entradaProcesada)){
        printf("El archivo no existe. ¿Quieres crearlo e introducir las IP? (s = sí, otro = no) ");
        int respuesta = getchar();
        while (getchar() != '\n');
        if (respuesta != 's') return;

        if (!abrirArchivo(entradaProcesada, "wt+", &archivoParam)) {
            printf("Error al crear el archivo en la ruta : %s\n", entradaProcesada);
            return;
        }

        printf("Archivo creado con éxito.\n\n");
        archivoIPs = archivoParam;

        while (1) {
            printf("Introduce la IP a testear (o 'fin' para terminar): ");
            entradaSinNL(buffer, tamBuffer);
            strcpy(entradaProcesada, buffer);

            // Si el usuario introduce 'fin', salir del bucle
            if (strcmp(entradaProcesada, "fin") == 0) break;

            // Si la IP no es válida, mostrar un mensaje de error y continuar con el bucle
            if (!validarIP(entradaProcesada)) {
                printf("IP no válida. Introduce una IP válida.\n");
                continue;
            }

            // Escribir la IP en el archivo
            fprintf(archivoIPs, "%s\n", entradaProcesada);
        }
        //fclose(archivoIPs);
        printf("IPs introducidas en el archivo.\n\n");
    }
    else {
        printf("Archivo encontrado.\n\n");
        if (!abrirArchivo(entradaProcesada, "r", &archivoIPs)) {
            return;
        }
    }

    // Volver a inicio
    rewind(archivoIPs);

    printf("--- Testearemos las siguientes IPs ---\n");
    while (fscanf(archivoIPs, "%s", ip) != EOF) {
        printf("%s\n", ip);
        totalIPs++;
    }
    printf("-----------------------------------\n\n");

    // Volver a inicio
    rewind(archivoIPs);

    printf("--- Testeando IPs ---\n");
    while (fscanf(archivoIPs, "%15s", ip) != EOF) {
        
        printf("Testeando IP: %s ...\n", ip);
		// Comando para ejecutar el ping
        sprintf(comando, "ping -n 1 %s", ip);
        FILE *ping = _popen(comando, "r");
		
        if (ping == NULL) {
			printf("Error al ejecutar el comando ping.\n");
			return;
		}

		// Leer la salida del comando ping
        while (fgets(lectura, sizeof(lectura), ping) != NULL) {
                
            sprintf(busqueda,"Respuesta desde %s",ip);
            if (strstr(lectura, busqueda) != NULL) {
                strcpy(ipPositivas[numIPs], ip);
                numIPs++;
                break;
            }
        }
        // Cerrar la conexión con el comando ping
        _pclose(ping);
    }

    // Cerrar archivo
    fclose(archivoIPs);


    // Mostrar resumen
    printf("\n--- Resumen de resultados ---\n");
    if (numIPs == 0) {
        printf("No hubo respuestas positivas.\n");
    }
    else {
        printf("IPs que respondieron al ping:\n");
        for (int i = 0; i < numIPs; i++) {
            printf("%s\n", ipPositivas[i]);
        }
    }
	printf("-----------------------------------\n\n");

    // Actualizar archivo de resultados
    if (abrirArchivo(rutaParam, "a", &archivoParam)) {
		fprintf(archivoParam, "-----------------------------------\n");
        insertarFechaHora(archivoParam);
        fprintf(archivoParam, "---- IPs que responden al ping ----\n");
        for (int i = 0; i < numIPs; i++) {
            fprintf(archivoParam, "%s\n", ipPositivas[i]);
        }
        fprintf(archivoParam, "-----------------------------------\n");

        fclose(archivoParam);
    }
     
    // Mensaje confirmación
    printf("Archivo producto2.txt actualizado con las IPs que han dado respuesta.\n\n");
}

// Función para validar una dirección IP
bool validarIP(const char *ip) {

    int num, puntos = 0;
    const char *puntero_ip = ip;

    // Si la ip es nula
    if (ip == NULL) return false;
    // Mientras haya un carácter en la dirección IP
    while (*puntero_ip) {
        // Si no es un dígito
        if (*puntero_ip < '0' || *puntero_ip > '9') {
            // Si es un punto
            if (*puntero_ip == '.') {
                puntos++;
                // Si hay más de 3 puntos o el último carácter es un punto
                if (puntos > 3 || *(puntero_ip - 1) == '.') return false;
            } 
            // Si no es ni punto ni dígito
            else return false;
        }
        // Mover el puntero al siguiente carácter
        puntero_ip++;
    }
    // Si termina en punto o si la cantidad de puntos no es exactamente 3
    if (*(puntero_ip - 1) == '.' || puntos != 3) return false;
    
    // Para cada segmento de la dirección IP
    for (int i = 0; i < 4; i++) {
        num = atoi(ip);
        // Si el número no está en el rango de 0 a 255
        if (num < 0 || num > 255) return false;
		// Si no es el último segmento, ir al siguiente
        if (i < 3) ip = strchr(ip, '.') + 1;
    }

    // Si todo es correcto evolver verdadero
    return true;
}

// Función para pedir IP válida
bool pedirDosIP(char * ipA, char * ipB) {
    // Declarar variables
    char buffer[1024];
    int tamBuffer = sizeof(buffer);
    char entradaProcesada[1024];
    int intentos = 0;

    // Mientras los carácteres introducidos no cumplan con el formato de una IP
    do {
        // Solicitar la IP
        printf("Introduce el DNS primario: ");
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);

        // Si la IP no es válida
        if (!validarIP(entradaProcesada)) {
            // Mostrar un mensaje de error
            printf("La IP introducida no es válida. Introduce una IP válida.\n\n");
            intentos++;
            continue;
        }

        // Si la IP es válida y responde a ping
        if (respuestaPing(entradaProcesada)) {
            // Registrar la IP en el archivo adaptador.txt
            strcpy(ipA, entradaProcesada);
            // Salir del bucle
            break;
        }
        intentos++;

    } while (1 && intentos < 3);

    if (intentos < 3) intentos = 0;

    // Mientras los carácteres introducidos no cumplan con el formato de una IP
    do {
        // Solicitar la IP
        printf("Introduce el DNS secundario: ");
        entradaSinNL(buffer, tamBuffer);
        strcpy(entradaProcesada, buffer);

        // Si la IP no es válida
        if (!validarIP(entradaProcesada)) {
            // Mostrar un mensaje de error
            printf("La IP introducida no es válida. Introduce una IP válida.\n\n");
            intentos++;
            continue;
        }

        // Si la IP es válida y responde a ping
        if (respuestaPing(entradaProcesada)) {
            // Registrar la IP en el archivo adaptador.txt
            strcpy(ipB, entradaProcesada);
            // Salir del bucle
            break;
        }
    } while (1 && intentos < 3);

    // Si se superan los intentos máximos, volvemos
    if (intentos > 2) {
        printf("Máximos intentos superados. Volviendo...\n\n");
        return false;
    }

    // Informamos y devolvemos éxito
    printf("DNSs correctamente introducidos y validados.\n\n");
    return true;
}

// Función para comprobar la respuesta de una IP válida
bool respuestaPing(char* IP) {
    
    // Declaración de variables
    FILE* ping = NULL;
    char comando[100];
    char lectura[100];
    bool respuesta = false;
    char busqueda[200];

    printf("\nTesteando IP: %s ...\n", IP);

    // Comando para ejecutar el ping
    sprintf(comando, "ping -n 1 %s", IP);
    ping = _popen(comando, "r");
    if (ping == NULL) {
        printf("Error al ejecutar el comando ping.\n");
        return false;
    }

    // Leer la salida del comando ping
    while (fgets(lectura, sizeof(lectura), ping) != NULL) {

        sprintf(busqueda, "Respuesta desde %s", IP);
        // Si obtenemos respuesta, registramos e informamos
        if (strstr(lectura, busqueda) != NULL) {
            respuesta = true;
            printf("¡¡ %s obtenida !!\n\n", busqueda);
            break;
        }
    }

    // Si no recibimos respuesta, informamos
    if (!respuesta) printf("No se ha recibido respuesta de la IP %s.\n\n", IP);

    // Cerrar la conexión con el comando ping
    _pclose(ping);

    // Devolver resultado
    return respuesta;
}