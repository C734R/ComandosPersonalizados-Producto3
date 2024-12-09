#ifndef ARCHIVO_H       // Si ARCHIVO_H no ha sido definido
#define ARCHIVO_H       // Definimos ARCHIVO_H

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdbool.h>

// Declarar funciones
bool abrirArchivo(char *rutaParam, char *modoParam, FILE **archivoParam);
bool existeArchivo(char *rutaParam);
bool vaciarArchivo(char *rutaParam);

#endif      // Fin definición ARCHIVO_H