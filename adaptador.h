#ifndef ADAPTADOR_H     // Si ADAPTADOR_H no ha sido definido
#define ADAPTADOR_H     // Definimos

// Añadir librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "entrada.h"

// Declaración de funciones
bool mostrarAdaptadores(void);
bool mostrarSoloAdaptadores(void);
bool encontrarAdaptador(char *nAdaptador);
bool pedirDatos(FILE* archivoParam);
char* pedirAdaptador();
bool pedirAdaptadorReintentos(char* adaptador, bool exista);
bool registrarDatosAdaptador(FILE* archivoParam, char* adaptador);

#endif      // Fin definición ADAPTADOR_H