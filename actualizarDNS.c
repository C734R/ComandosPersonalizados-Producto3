// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actualizarDNS.h"
#include "entrada.h"
#include "dns.h"
#include "adaptador.h"
#include "archivo.h"
#include "ping.h"

// Definición de las rutas de los archivos temporales
#define RUTA_ADAPTADOR "c:/temp/adaptadorDNS.txt"
#define RUTA_VELOCIDAD_FICHERO "c:/temp/resultadoFichero.txt"
#define RUTA_VELOCIDAD_ADAPTADOR "c:/temp/resultadoAdaptador.txt"
#define RUTA_RESULTADO_COMBINADO "c:/temp/resultadoCombinado.txt"
#define RUTA_RESULTADO "c:/temp/resultadoDNS.txt"


// Función para actualizar las DNS de forma automática mediante fichero direcciones
bool actualizarAutoDNS() {
	// Declaración de variables
	char buffer[1024];
	int tamBuffer = 1024;
	char adaptador[1024];
	char rutaDNS[1024];
	FILE* archivo = NULL;
	int contador = 0;
	char ipRapidaA[16] = { "" };
	char ipRapidaB[16] = { "" };
	bool empate = false;
	bool A = false;
	bool B = false;
	bool orden = false;

	printf("*** Solicitar nombre de adaptador ***\n\n");
	
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
	} while (1 && contador < 2);

	// Si se han superado los intentos
	if (contador > 2) {
		printf("Demasiados intentos fallidos. Volviendo...\n");
		return false;
	}

	// Mostrar el contenido del archivo de DNS
	if (!mostrarArchivoDNS(rutaDNS)) {
		return false;
	}

	pausaEnter();

	// Pedir el nombre del adaptador de red hasta que se elija existente o se decida salir
	do {
		// Mostrar los adaptadores de red
		if (!mostrarSoloAdaptadores()) {
			return false;
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

	printf("*** Vaciar archivos temporales ***\n\n");

	// Vaciar los archivos temporales
	if (!vaciarArchivosDNS(RUTA_ADAPTADOR, RUTA_VELOCIDAD_FICHERO, RUTA_VELOCIDAD_ADAPTADOR, RUTA_RESULTADO, RUTA_RESULTADO_COMBINADO)) {
		printf("Error al vaciar los archivos temporales. Volviendo...\n");
		return false;
	}

	printf("*** Mostrar los DNS configurados en el adaptador seleccionado ***\n\n");

	// Mostrar las DNS del adaptador de red seleccionado y guardar en archivo
	if (!mostrarDNSAdaptador(adaptador, RUTA_ADAPTADOR)) {
		printf("Error al mostrar las DNS del adaptador de red. Volviendo...\n");
		return false;
	}

	printf("*** Comprobar velocidad DNSs fichero %s ***\n\n", rutaDNS);

	// Comprobar velocidad y saltos DNS del fichero y guardar en fichero temporal
	if (!checkVelocidadDNS(rutaDNS, RUTA_VELOCIDAD_FICHERO)) {
		printf("Error al comprobar la velocidad de las DNS del archivo. Volviendo...\n");
		return false;
	}

	printf("*** Comprobar velocidad DNSs fichero %s ***\n\n", RUTA_ADAPTADOR);

	// Comprobar velocidad y saltos DNS del adaptador y guardar en fichero temporal
	if (!checkVelocidadDNS(RUTA_ADAPTADOR, RUTA_VELOCIDAD_ADAPTADOR)) {
		printf("Error al comprobar la velocidad de las DNS del adaptador. Volviendo...\n");
		return false;
	}

	printf("*** Combinar resultados ***\n\n--> Se omiten coincidencias <--\n\n");

	// Combinar los ficheros de velocidad
	if (!combinarFicheros(RUTA_VELOCIDAD_FICHERO, RUTA_VELOCIDAD_ADAPTADOR, RUTA_RESULTADO_COMBINADO)) {
		printf("Error al combinar los ficheros de velocidad. Volviendo...\n");
		return false;
	}

	printf("*** Comparar resultados para obtener el DNS más rápido. ***\n\n");

	// Comprobar las DNS más rápidas y guardar en variables
	if (!compararDNS(RUTA_RESULTADO_COMBINADO, ipRapidaA, ipRapidaB, &empate)) {
		printf("Error al comparar las DNS. Volviendo...\n");
		return false;
	}

	pausaEnter();

	// Si hay empate en las DNS se comprueba el número de saltos
	if (empate) {
		printf("*** Comprobar saltos tras empate de velocidad ***\n\n"); 
		// Comprobar los saltos de las DNS y seleccionar la más rápida
		char* ipSaltos = saltosDNS(ipRapidaA, ipRapidaB, &empate);
		
		// Si no se ha devuelto ninguna IP
		if (ipSaltos == NULL) {
			printf("Error al comprobar los saltos de las DNS. Volviendo...\n");
			return false;
		}

		// Si no hay empate, se intercambian las IPs
		if (!empate) {
			// Si la IP seleccionada es la B, se intercambian las IPs
			if (strcmp(ipSaltos, ipRapidaB) == 0) {
				strcpy(ipRapidaB, ipRapidaA);
				strcpy(ipRapidaA, ipSaltos);
			}
		}
		pausaEnter();
	}

	printf("*** Comparar DNSs finalistas con las del adaptador ***\n\n");

	// Comparar las DNS finalistas con las del adaptador
	if (!compararAdaptador(adaptador, ipRapidaA, ipRapidaB, &A, &B, &orden, RUTA_ADAPTADOR)) {
		printf("Error al comparar las DNS con las del adaptador. Volviendo...\n");
		return 1;
	}

	printf("*** Aplicar modificaciones ***\n\n");

	// Modificar la configuración de las DNS
	if (!modificarDNS(adaptador, ipRapidaA, ipRapidaB, &A, &B, &orden)) {
		printf("Error al comparar las DNS. Volviendo...\n");
		return false;
	}

	printf("*** Verificar configuración aplicada ***\n\n");

	// Verificar la configuración de las DNS
	if (!verificarDNS(adaptador)) {
		printf("Error al verificar la configuración del adaptador. Volviendo...");
	}

	// Devolver éxito
	return true;
}	


// Función para actualizar las DNS mediante introducción manual
bool actualizarManualDNS() {
	// Declaración de variables
	char adaptador[1024] = { "" };
	char ipA[16] = { "" };
	char ipB[16] = { "" };

	printf("*** Solicitar nombre de adaptador ***\n\n");

	// Pedir el nombre del adaptador de red hasta que se elija existente o se decida salir
	if (!pedirAdaptadorReintentos(adaptador)) {
		return false;
	}

	pausaEnter();

	printf("*** Solicitar DNSs a establecer ***\n\n");

	// Pedir dos DNS al usuario y validarlas
	if (!pedirDosIP(ipA, ipB)) {
		printf("Error en la petición de DNSs. Volviendo...\n\n");
		return false;
	}

	pausaEnter();

	printf("*** Aplicar modificaciones ***\n\n");

	// Establecer los DNS introducidos y validados en el adaptador
	if (!establecerDNS(adaptador, ipA, ipB)) {
		printf("Error al establecer las DNS en el adaptador %s. Volviendo...\n\n", adaptador);
		return false;
	}

	pausaEnter();

	printf("*** Verificar configuración aplicada ***\n\n");

	// Verificar la configuración de las DNS
	if (!verificarDNS(adaptador)) {
		printf("Error al verificar la configuración del adaptador. Volviendo...");
	}

	// Devolver éxito
	return true;
}

bool actualizarDHCPDNS() {
	// Declaración de variables
	char adaptador[1024] = { "" };

	printf("*** Petición de adaptador ***\n\n");

	// Pedir el nombre del adaptador de red hasta que se elija existente o se decida salir
	if (!pedirAdaptadorReintentos(adaptador)) {
		return false;
	}

	printf("*** Aplicar configuración DHCP al adaptador %s ***\n\n", adaptador);

	// Aplicar configuración DHCP al adaptador seleccionado
	if (!adaptadorDHCP(adaptador)) {
		return false;
	}

	printf("*** Verificar configuración aplicada ***\n\n");

	// Verificar configuración DHCP
	if (!verificarDNS(adaptador)) {
		return false;
	}

	return true;
}
