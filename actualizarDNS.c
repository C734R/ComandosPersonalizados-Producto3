// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actualizarDNS.h"
#include "entrada.h"
#include "dns.h"
#include "adaptador.h"
#include "archivo.h"

// Definición de las rutas de los archivos temporales
#define RUTA_ADAPTADOR "adaptadorDNS.txt"
#define RUTA_VELOCIDAD_FICHERO "resultadoFichero.txt"
#define RUTA_VELOCIDAD_ADAPTADOR "resultadoAdaptador.txt"
#define RUTA_RESULTADO_COMBINADO "resultadoCombinado.txt"
#define RUTA_RESULTADO "resultadoDNS.txt"


// Función para actualizar las DNS
int actualizarDNS() {
	// Declaración de variables
	char buffer[1024];
	int tamBuffer = 1024;
	char adaptador[1024];
	char rutaDNS[1024];
	FILE* archivo = NULL;
	int contador = 0;
	char ipRapidaA[16];
	char ipRapidaB[16];
	bool empate = false;
	bool A = false;
	bool B = false;
	
	// Pedir la ruta del archivo de DNS hasta que se introduzca una válida o se superen los intentos
	do {
		// Pedir la ruta del archivo de DNS
		strcpy(rutaDNS,pedirArchivoDNS());

		// Comprobar si el archivo existe y es accesible
		if (existeArchivo(rutaDNS)) {
			printf("Archivo accesible.\nArchivo de DNS introducido: %s\n", rutaDNS);
			break;
		}
		printf("Error al abrir el archivo. Introduce una ruta válida.\n");
		contador++;
		fclose(archivo);
	} while (1 || contador > 2);

	// Si se han superado los intentos
	if (contador > 2) {
		printf("Demasiados intentos fallidos. Volviendo...\n");
		return 1;
	}

	// Mostrar el contenido del archivo de DNS
	if (!mostrarArchivoDNS(rutaDNS)) {
		return 1;
	}

	pausaEnter();

	// Pedir el nombre del adaptador de red hasta que se elija existente o se decida salir
	do {
		// Mostrar los adaptadores de red
		if (!mostrarAdaptadores()) {
			return 1;
		}
		// Pedir el nombre del adaptador de red
		strcpy(adaptador,pedirAdaptador());
		// Comprobar elección de adaptador
		if (encontrarAdaptador(adaptador)) break;
		printf("Adaptador no encontrado. ¿Desea introducir otro adaptador? (s/otro = salir): ");
		entradaSinNL(buffer, tamBuffer);
		if (strcmp(buffer, "s") == 0) {
			continue;
		}
		printf("Volviendo...\n");
		return 1;
	} while (1);

	// Vaciar los archivos temporales
	if (!vaciarArchivosDNS(RUTA_ADAPTADOR, RUTA_VELOCIDAD_FICHERO, RUTA_VELOCIDAD_ADAPTADOR, RUTA_RESULTADO, RUTA_RESULTADO_COMBINADO)) {
		printf("Error al vaciar los archivos temporales. Volviendo...\n");
		return 1;
	}

	// Mostrar las DNS del adaptador de red seleccionado y guardar en archivo
	if (!mostrarDNSAdaptador(adaptador, RUTA_ADAPTADOR)) {
		printf("Error al mostrar las DNS del adaptador de red. Volviendo...\n");
		return 1;
	}

	// Comprobar velocidad y saltos DNS del fichero y guardar en fichero temporal
	if (!checkVelocidadDNS(rutaDNS, RUTA_VELOCIDAD_FICHERO)) {
		printf("Error al comprobar la velocidad de las DNS del archivo. Volviendo...\n");
		return 1;
	}

	// Comprobar velocidad y saltos DNS del adaptador y guardar en fichero temporal
	if (!checkVelocidadDNS(RUTA_ADAPTADOR, RUTA_VELOCIDAD_ADAPTADOR)) {
		printf("Error al comprobar la velocidad de las DNS del adaptador. Volviendo...\n");
		return 1;
	}

	// Combinar los ficheros de velocidad
	if (!combinarFicheros(RUTA_VELOCIDAD_FICHERO, RUTA_VELOCIDAD_ADAPTADOR, RUTA_RESULTADO_COMBINADO)) {
		printf("Error al combinar los ficheros de velocidad. Volviendo...\n");
		return 1;
	}

	// Comprobar las DNS más rápidas y guardar en variables
	if (!compararDNS(RUTA_RESULTADO_COMBINADO, ipRapidaA, ipRapidaB, &empate)) {
		printf("Error al comparar las DNS. Volviendo...\n");
		return 1;
	}

	// Si hay empate en las DNS se comprueba el número de saltos
	if (empate) {
		// Comprobar los saltos de las DNS y seleccionar la más rápida
		char* ipSaltos = saltosDNS(ipRapidaA, ipRapidaB, &empate);
		
		if (!empate) {
			strcpy(ipRapidaB, ipRapidaA);
			strcpy(ipRapidaA, ipSaltos);
			printf("Se han intercambiado la prioridad de IPs debido al número de saltos.\n");
		}
	}

	// Comparar las DNS finalistas con las del adaptador
	if (!compararAdaptador(adaptador, ipRapidaA, ipRapidaB, &A, &B)) {
		printf("Error al comparar las DNS con las del adaptador. Volviendo...\n");
		return 1;
	}

	// Si la IP A y B están en el adaptador, no se modifica
	if (A && B) {
		printf("Ambas IP ya están configuradas en el adaptador. No se modificarán.\n");
		return 0;
	}
	// Si la IP A está en el adaptador, se modifica la B
	else if (A) {
		printf("La IP %s ya está configurada en el adaptador %s. Solo se modificará la segunda.\n", ipRapidaA, adaptador);
		return 0;
	}
	// Si la IP B está en el adaptador, se modifica la A
	else if (B) {
		printf("La segunda IP más rápida %s está configurada en el adaptador %s.\n Se configurará como secundaria.\n", ipRapidaB, adaptador);
		return 0;
	}

	// Modificar la configuración de las DNS
	if (!modificarDNS(adaptador, ipRapidaA, ipRapidaB, &A, &B)) {
		printf("Error al comparar las DNS. Volviendo...\n");
		return 1;
	}


}	