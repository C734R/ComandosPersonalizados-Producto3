#ifndef PING_H      // Si PING_H no ha sido definido
#define PING_H      // Definimos PING_H

// -- Comprobar e informar por pantalla sobre cuáles de una lista de máquinas referenciadas 
// por su IP escrita en un archivo de texto, contestan positivamente a un ping lanzado por la 
// aplicación que estás programando. Se recomienda generar un submenú donde se solicite el 
// nombre y ubicación de dicho archivo.*/
// -- Diseñar una función que lea el contenido de un archivo de texto que consiste en una 
// serie de direcciones IP cada una de las cuales ocupa una línea de éste. La ruta y nombre 
// del archivo de texto se preguntarán al usuario e inicialmente se mostrará el contenido de 
// este por pantalla. Finalmente lanzará un ping para cada una de las IP’s y determinará e 
// informará sobre cuáles de las anteriores responden afirmativamente a este comando. (Menú punto 2)*/
// -- Se almacenará el resultado del paso anterior debajo de la fecha y la hora en el archivo 
// producto2.txt, las IPs que han dado una respuesta positiva. Guardar el archivo y no borrar. (Menú punto 2)

// Añadimos librerías necesarias
#include <stdbool.h>

//Definimos funciones
void pingIPs(FILE *archivoParam, char *rutaParam);
bool validarIP(const char *ip);

#endif      // Fin definición PING_H