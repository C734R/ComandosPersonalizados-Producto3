// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdbool.h>
#include "archivo.h"

// Función para abrir un archivo de la ruta especificada en el modo especificado y almacenar el puntero al archivo
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam) {

    // Abrir el archivo en el modo especificado
    *archivoParam = fopen(rutaParam, modoParam);
    // Si no se ha podido abrir el archivo, false
    if (*archivoParam == NULL) return false;
    // Si se ha podido abrir el archivo, true
    else return true;
}

// Función para comprobar si un archivo existe en la ruta especificada
bool existeArchivo(char *rutaParam) {
    // Intentar abrir el archivo en modo lectura
    FILE *archivo = fopen(rutaParam, "r");
    // Si no se ha podido abrir el archivo, false
    if (archivo == NULL) return false;
    // Si se ha podido abrir el archivo
    else {
        // Cerrar el archivo
        fclose(archivo);
        // Devolver verdadero
        return true;
    }
}

// Función para vaciar un archivo
bool vaciarArchivo(char *rutaParam) {
    // Abrir el archivo en modo escritura
    FILE *archivo = fopen(rutaParam, "w");
    // Si no se ha podido abrir el archivo
    if (archivo == NULL) {
        // Mostrar un mensaje de error
        printf("Error al abrir el archivo ubicado en la ruta : %s\n", rutaParam);
        // Devolver falso
        return false;
    }
    // Si se ha podido abrir el archivo
    else {
        // Mostrar un mensaje de éxito
        printf("Archivo %s vaciado con éxito.\n\n", rutaParam);
        // Cerrar el archivo
        fclose(archivo);
        // Devolver verdadero
        return true;
    }
}