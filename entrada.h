#ifndef ENTRADA_H       // Si ENTRADA_H no ha sido definido
#define ENTRADA_H       // Definimos

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdbool.h>

// Declarar funciones
char * entradaSinNL (char * destino, int tamBuffer);
char * entradaConNL (char * destino, int tamBuffer);
bool vaciarEntrada(const char *entrada);
bool eliminarNL(char *entrada);
void pausaEnter();

#endif      // Fin definición ENTRADA_H