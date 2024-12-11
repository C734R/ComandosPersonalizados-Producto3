
// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "entrada.h"

// Función para leer una entrada y eliminar el carácter de nueva línea
 char * entradaSinNL(char *destino, int tamBuffer) {
     if (fgets(destino, tamBuffer, stdin) == NULL) {
         printf("Error al procesar cadena");
         return NULL;
     }
    // Eliminar el carácter de nueva línea
    eliminarNL(destino);
    return destino;
 }

// Función para leer una entrada y no eliminar el carácter de nueva línea
char * entradaConNL(char *destino, int tamBuffer) {
    if (fgets(destino, tamBuffer, stdin) == NULL) {
        printf("Error al procesar cadena");
		return NULL;
    }    
    return destino;
}

// Función para vaciar el buffer de entrada
bool vaciarEntrada (const char *entrada) {
	// Si la entrada es nula o está vacía
	if (entrada == NULL || strlen(entrada)==0) return false;

    // Si el último carácter de la cadena no es un salto de línea
    if(entrada[strlen(entrada) - 1] != '\n'){
      //Limpiamos el búfer
      while(getchar() != '\n');
    }
    // Devolver verdadero
    return true;
}

// Función para eliminar el carácter de nueva línea
bool eliminarNL (char *entrada) {
    // Si la entrada es nula o está vacía
    if (entrada == NULL || strlen(entrada) == 0) return false;
    // Eliminar el carácter de nueva línea
    if (entrada[strlen(entrada) - 1] == '\n') {
        entrada[strlen(entrada) - 1] = '\0';
    }
    // Devolver verdadero
    return true;
}

// Función para esperar a pulsar enter
void pausaEnter() {
    printf("Pulsa Enter para continuar...");
    while (getchar() != '\n');
    printf("\n");
}