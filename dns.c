// Colección de funciones para actualizar DNS de adaptador

// Inclusión de las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "dns.h"
#include "entrada.h"
#include "archivo.h"

// Función para pedir la ruta del archivo de DNS
char * pedirArchivoDNS() {
	// Declaración de variables
	char buffer[1024];
	int tamBuffer = 1024;
	char rutaDNS[1024];

	// Pedir la ruta del archivo de DNS
	printf("Introduce la ruta del archivo de DNS: ");
	entradaSinNL(buffer, tamBuffer);
	strcpy(rutaDNS, buffer);
	// Devolver la ruta del archivo de DNS
	return rutaDNS;
}

// Función para mostrar el contenido de un archivo de DNS
bool mostrarArchivoDNS(char * rutaDNS) {
	// Declaración de variables
	char buffer[1024];
	int tamBuffer = 1024;
	FILE* archivoDNS = NULL;

	// Intentar abrir el archivo y mostrar un mensaje de éxito o error
	if(!abrirArchivo(rutaDNS, "r", &archivoDNS)){
		printf("Error al abrir el archivo.\n");
		return false;
	}
	printf("Archivo abierto con éxito.\n\n");

	// Mostrar el contenido del archivo de DNS
	printf("--- Contenido del archivo de DNS ---\n");
	while (fgets(buffer, tamBuffer, archivoDNS) != NULL) {
		printf("%s", buffer);
	}
	printf("-----------------------------------\n\n");

	// Cerrar el archivo
	fclose(archivoDNS);

	// Devolver éxito
	return true;
}

char * pedirAdaptador() {
	// Declaración de variables
	char buffer[1024];
	int tamBuffer = 1024;
	char adaptador[1024];

	// Pedir el adaptador de red del que se quiere actualizar la DNS
	printf("Introduce el nombre del adaptador de red del que quieres actualizar el DNS: ");
	entradaSinNL(buffer, tamBuffer);
	strcpy(adaptador, buffer);
	printf("Adaptador de red introducido: %s\n", adaptador);
	return adaptador;
}

bool mostrarDNSAdaptador(char* adaptador, char * rutaAdaptador) {
	// Declaración de variables
	char buffer[1024];
	int tamBuffer = 1024;
	char comando[1024];
	FILE* consola = NULL;
	FILE* adaptadorDNS = NULL;

	// Crear el comando para obtener las DNS del adaptador de red seleccionado
	sprintf(comando, "netsh interface ipv4 show dnsservers \"%s\"", adaptador);

	// Ejecutar el comando definido
	consola = _popen(comando, "r");

	// Si no se ha podido ejecutar el comando netsh
	if (consola == NULL) {
		// Mostrar un mensaje de error
		printf("Error al ejecutar el comando netsh.\n");
		return false;
	}

	// Mostrar un mensaje de éxito
	printf("Comando netsh ejecutado con éxito.\n\n");
	
	// Abrir el archivo para guardar las DNS del adaptador
	if (!abrirArchivo(rutaAdaptador, "w", &adaptadorDNS)) {
		printf("Error al abrir el archivo %s.\n", rutaAdaptador);
		return false;
	}

	// Mostrar las DNS del adaptador de red seleccionado
	printf("--- DNS del adaptador de red ---\n");
	// Recorrer las líneas de la salida del comando ipconfig
	while (fgets(buffer, tamBuffer, consola) != NULL) {
		printf("%s", buffer);
		// Guardar las DNS en el archivo adaptadorDNS.txt
		fprintf(adaptadorDNS, "%s", buffer);
	}
	printf("-----------------------------------\n\n");

	// Cerrar la conexión con el comando ipconfig
	_pclose(consola);

	// Cerrar el archivo adaptadorDNS.txt
	fclose(adaptadorDNS);

	// Devolver éxito
	return true;
}

// Función para vaciar los archivos de DNS
bool vaciarArchivosDNS(char* rutaAdaptador, char* rutaVelocidadFichero, char* rutaVelocidadAdaptador, char* rutaResultado, char* rutaResultadoCombinado) {
	if (!vaciarArchivo(rutaAdaptador)) return false;
	if (!vaciarArchivo(rutaVelocidadFichero)) return false;
	if (!vaciarArchivo(rutaVelocidadAdaptador)) return false;
	if (!vaciarArchivo(rutaResultado)) return false;
	if (!vaciarArchivo(rutaResultadoCombinado)) return false;
	return true;
}

bool checkVelocidadDNS(char * rutaDNS, char * rutaResultado) {
	// Declaración de variables
	FILE* archivoDNS = NULL;
	FILE* archivoResultado = NULL;
	char buffer[1024];
	int tamBuffer = 1024;
	char comando[1024];
	FILE* pingDNS = NULL;
	int contador = 0;
	char ip[16] = "";
	int numIPs = 0;
	char ipPositivas[10][16];
	char tiempo[10][16];
	char* coincidencia_ip;
	char* coincidencia_tiempo;
	char busqueda[1024];

	// Abrir el archivo de DNS
	if (!abrirArchivo(rutaDNS, "r", &archivoDNS)) {
		printf("Error al abrir el archivo.\n");
		return false;
	}
	printf("Archivo abierto con éxito.\n\n");

	// Abrir el archivo de resultados
	if (!abrirArchivo(rutaResultado, "w", &archivoResultado)) {
		printf("Error al abrir el archivo de resultados.\n");
		return false;
	}

	// Informamos de la comprobación de las DNS
	printf("--- Testeando velocidad DNSs ---\n");
	while (fscanf(archivoDNS, "%15s", ip) != EOF) {

		printf("Testeando IP: %s ...\n", ip);

		// Comando para ejecutar el ping a la IP del DNS con carga de 1500 bytes
		sprintf(comando, "ping -n 1 -l 1500 %s", ip);
		FILE* pingDNS = _popen(comando, "r");
		if (pingDNS == NULL) {
			printf("Error al ejecutar el comando ping.\n");
			return false;
		}

		// Leer la salida del comando ping
		while (fgets(buffer, tamBuffer, pingDNS) != NULL) {

			// Componer el texto a buscar
			sprintf(busqueda, "ping para %s", ip);
			// Buscar la coincidencia de la IP en la respuesta del ping
			coincidencia_ip = strstr(buffer, busqueda);
			// Si la respuesta es positiva
			if (coincidencia_ip != NULL) {
				// Guardar la IP en el array de IPs positivas
				strcpy(ipPositivas[numIPs], ip);
			}

			// Buscar la coincidencia del tiempo de respuesta en la respuesta del ping
			coincidencia_tiempo = strstr(buffer, "Media = ");
			if (coincidencia_tiempo != NULL) {
				// Guardar el tiempo de respuesta en la variable tiempo
				strcpy(tiempo[numIPs], coincidencia_tiempo + 8);
				// Quitar el texto "ms" del final del tiempo
				strcpy(tiempo[numIPs], strtok(tiempo[numIPs], "ms"));
				// Mostrar el tiempo de respuesta para la IP
				printf("Tiempo de respuesta para IP %s: %s ms\n", ip, tiempo[numIPs]);
			}

			// Incrementar el contador de IPs positivas
			if (coincidencia_ip != NULL) {
				numIPs++;
				break;
			}
		}

		// Cerrar la conexión con el comando ping
		_pclose(pingDNS);
	}

	// Cerrar archivo
	fclose(archivoDNS);

	// Mostrar resumen resultados
	printf("\n--- Resumen de resultados ---\n");
	if (numIPs == 0) {
		printf("No hubo respuestas positivas.\n");
		return false;
	}
	printf("IPs que respondieron al ping:\n");
	for (int i = 0; i < numIPs; i++) {
		printf("Tiempo de respuesta para IP %s: %s ms\n", ipPositivas[i], tiempo[i]);
	}
	printf("-----------------------------------\n\n");

	// Actualizar archivo de resultados
	// Insertar ip y tiempo en el archivo de resultados
	for (int i = 0; i < numIPs; i++) {
		fprintf(archivoResultado, "%s %s\n", ipPositivas[i], tiempo[i]);
	}
	// Cerrar el archivo
	fclose(archivoResultado);
		
	// Devolver éxito
	return true;
}


// Función para comprobar los saltos de las DNS
bool checkSaltosDNS(char* rutaDNS, char* rutaResultado) {
	// Declaración de variables
	char buffer[1024];
	int tamBuffer = 1024;
	char comando[1024];
	FILE* archivoDNS = NULL;
	FILE* consola = NULL;
	FILE* archivoResultado = NULL;
	int contador = 0;
	char ip[16] = { "" };
	int numIPs = 0;
	char ipPositivas[10][16] = { "" };
	int saltos[10] = { 0 };

	// Abrir el archivo de DNS
	if (!abrirArchivo(rutaDNS, "r", &archivoDNS)) {
		printf("Error al abrir el archivo.\n");
		return false;
	}

	// Informamos de la comprobación de las DNS
	printf("--- Testeando saltos DNSs ---\n");
	while (fscanf(archivoDNS, "%15s", ip) != EOF) {

		printf("Testeando IP: %s ...\n", ip);
		
		// Comando para ejecutar el tracert a la IP del DNS sin resolución de nombres, con limitación a 30 saltos y tiempo de espera de 500 ms
		sprintf(comando, "tracert -d -h 30 -w 500 %s", ip);

		// Ejecutar el comando
		FILE* consola = _popen(comando, "r");
		if (consola == NULL) {
			printf("Error al ejecutar el comando tracert.\n");
			return false;
		}

		// Leer la salida del comando tracert
		while (fgets(buffer, tamBuffer, consola) != NULL) {
			// Si la línea comienza con un número, registrar ip y saltos
			if (isdigit(buffer[0])) {
				strcpy(ipPositivas[numIPs], ip);
				saltos[numIPs]++;
			}
		}

		// Mostrar el número de saltos para la IP
		printf("Número de saltos para IP %s: %i\n", ip, saltos[numIPs]);

		// Cerrar la conexión con el comando tracert
		_pclose(consola);
	}

	// Cerrar archivo
	fclose(archivoDNS);

	// Abrir el archivo de resultados
	if (!abrirArchivo(rutaResultado, "w", &archivoResultado)) {
		printf("Error al abrir el archivo de resultados.\n");
		return false;
	}
	
	// Insertar ip y saltos en el archivo de resultados
	for (int i = 0; i < numIPs; i++) {
		fprintf(archivoResultado, "%s %i\n", ipPositivas[i], saltos[i]);
	}

	// Cerrar el archivo
	fclose(archivoResultado);

	// Devolver éxito
	return true; 
}

bool combinarFicheros(char* rutaA, char* rutaB, char * rutaC) {

	// Declaración de variables
	FILE* archivoA = NULL;
	FILE* archivoB = NULL;
	FILE* archivoC = NULL;
	char buffer[1024];
	int tamBuffer = 1024;

	// Abrir archivo A, origen de la combinación
	if (!abrirArchivo(rutaA, "r", &archivoA)) {
		printf("Error al abrir el archivo %s.\n", rutaA);
		return false;
	}

	// Abrir archivo B, origen de la combinación
	if (!abrirArchivo(rutaB, "r", &archivoB)) {
		printf("Error al abrir el archivo %s.\n", rutaB);
		return false;
	}

	// Abrir archivo C, destino de la combinación
	if (!abrirArchivo(rutaC, "w", &archivoC)) {
		printf("Error al abrir el archivo %s.\n", rutaC);
		return false;
	}

	// Recorrer el archivo A y copiar su contenido en el archivo C
	while (fgets(buffer, tamBuffer, archivoA) != NULL) {
		fprintf(archivoC, "%s", buffer);
	}

	// Recorrer el archivo B y copiar su contenido en el archivo C
	while (fgets(buffer, tamBuffer, archivoB) != NULL) {
		fprintf(archivoC, "%s", buffer);
	}

	// Cerrar los archivos
	fclose(archivoA);
	fclose(archivoB);
	fclose(archivoC);

	// Devolver éxito
	return true;
}


// Función para comparar y elegir la DNS más rápida
bool compararDNS(char * rutaResultados, char * ipRapidaA, char * ipRapidaB, bool * empate) {
	// Declaración de variables
	FILE* archivoResultados = NULL;
	int tamBuffer = 1024;
	char ip[16] = { "" };
	char tiempo[16] = { "" };
	char tiempoRapidoA = 9999;
	char tiempoRapidoB = 9999;
	empate = false;

	// Abrir los archivos de resultados de resultados
	if (!abrirArchivo(rutaResultados, "r", &archivoResultados)) {
		printf("Error al abrir el archivo %s.\n", rutaResultados);
		return false;
	}

	// Leer los resultados de velocidad
	while (fscanf(archivoResultados, "%15s %15s", ip, tiempo) != EOF) {
		
		// Si el tiempo es menor que el tiempo más rápido, actualizar la IP más rápida y el tiempo más rápido
		if (atoi(tiempo) < tiempoRapidoA) {
			strcpy(ipRapidaB, ipRapidaA);
			strcpy(ipRapidaA, ip);
			tiempoRapidoB = tiempoRapidoA;
			tiempoRapidoA = atoi(tiempo);
		}
	}

	// Si no se han encontrado IPs con respuesta al ping
	if (tiempoRapidoA == 9999) {
		printf("No se han encontrado IPs con respuesta al ping.\n");
		return false;
	}

	// Si las velocidades son diferentes
	if (!tiempoRapidoA == tiempoRapidoB) {
		// Mostrar la DNS más rápida
		printf("La DNS más rápida es %s con un tiempo de respuesta de %i ms.\n", ipRapidaA, tiempoRapidoA);
		// Cerrar el archivo
		fclose(archivoResultados);
		empate = false;
		// Devolver éxito
		return true;
	}

	// Si no lo son, mostrar las IPs más rápidas
	printf("Hay dos IPs con el mismo tiempo de respuesta más rápido: %s y %s con %i ms.\n", ipRapidaA, ipRapidaB, tiempoRapidoA);
	empate = true;

	// Cerrar el archivo
	fclose(archivoResultados);

	// Devolver éxito
	return true;
}

// Función para comprobar los saltos de las DNS en disputa
char * saltosDNS (char * ipRapidaA, char * ipRapidaB, bool * empate) {
	// Declaración de variables
	FILE* consola = NULL;
	char comando[1024];
	char buffer[1024];
	int tamBuffer = 1024;
	int saltosA = 0;
	int saltosB = 0;
	empate = false;

	// Informamos de la comprobación de las DNS
	printf("--- Testeando saltos DNSs ---\n");

	printf("Testeando IP: %s ...\n", ipRapidaA);
	// Comando para ejecutar el tracert a la IP del DNS sin resolución de nombres, con limitación a 30 saltos y tiempo de espera de 500 ms
	sprintf(comando, "tracert -d -h 30 -w 500 %s", ipRapidaA);

	// Ejecutar el comando
	consola = _popen(comando, "r");
	if (consola == NULL) {
		printf("Error al ejecutar el comando tracert.\n");
		return false;
	}

	// Leer la salida del comando tracert
	while (fgets(buffer, tamBuffer, consola) != NULL) {
		// Si la línea comienza con un número
		if (isdigit(buffer[0])) {
			saltosA++;
		}
	}

	// Mostrar el número de saltos para la IP
	printf("Número de saltos para IP %s: %s\n", ipRapidaA, saltosA);

	// Cerrar la conexión con el comando tracert
	_pclose(consola);

	printf("Testeando IP: %s ...\n", ipRapidaB);

	// Comando para ejecutar el tracert a la IP del DNS sin resolución de nombres, con limitación a 30 saltos y tiempo de espera de 500 ms
	sprintf(comando, "tracert -d -h 30 -w 500 %s", ipRapidaB);

	// Ejecutar el comando
	consola = _popen(comando, "r");
	if (consola == NULL) {
		printf("Error al ejecutar el comando tracert.\n");
		return false;
	}

	// Leer la salida del comando tracert
	while (fgets(buffer, tamBuffer, consola) != NULL) {
		// Si la línea comienza con un número
		if (isdigit(buffer[0])) {
			saltosB++;
		}
	}

	// Mostrar el número de saltos para la IP
	printf("Número de saltos para IP %s: %s\n", ipRapidaB, saltosB);

	// Cerrar la conexión con el comando tracert
	_pclose(consola);

	// Comparar los saltos de las DNS y seleccionar según resultados
	// Si las DNS tienen el mismo número de saltos
	if (ipRapidaA == ipRapidaB) {
		printf("Las IPs tienen el mismo número de saltos. Por defecto se selecciona la primera IP.\n");
		empate = true;
		return ipRapidaA;
	}
	// Si la IP A tiene menos saltos que la IP B
	else if (ipRapidaA < ipRapidaB) {
		printf("La IP %s tiene menos saltos que la IP %s. Se selecciona la IP %s.\n", ipRapidaA, ipRapidaB, ipRapidaA);
		empate = false;
		return ipRapidaA;
	}
	// Si la IP B tiene menos saltos que la IP A
	else {
		printf("La IP %s tiene menos saltos que la IP %s. Se selecciona la IP %s.\n", ipRapidaB, ipRapidaA, ipRapidaB);
		empate = false;
		return ipRapidaB;
	}
}

// Comparar IPs con la del adaptador
bool compararAdaptador(char * adaptador, char * ipA, char * ipB, bool * A, bool * B) {
	// Declaración de variables
	char buffer[1024];
	int tamBuffer = 1024;
	char comando[1024];
	FILE* consola = NULL;
	A = false;
	B = false;

	// Crear el comando para obtener las DNS del adaptador de red seleccionado
	sprintf(comando, "netsh interface ipv4 show dnsservers \"%s\"", adaptador);

	// Ejecutar el comando definido
	consola = _popen(comando, "r");
	// Si no se ha podido ejecutar el comando netsh
	if (consola == NULL) {
		// Mostrar un mensaje de error
		printf("Error al ejecutar el comando netsh.\n");
		return false;
	}

	// Mostrar un mensaje de éxito
	printf("Comando ipconfig ejecutado con éxito.\n\n");

	// Recorrer las líneas de la salida del comando netsh
	while (fgets(buffer, tamBuffer, consola) != NULL) {
			if (strstr(buffer, ipA) != NULL || strstr(buffer, ipB) != NULL) {
				if (strstr(buffer, ipA) != NULL) A = true;
				if (strstr(buffer, ipB) != NULL) B = true;
			}
	}

	// Cerrar la conexión con el comando netsh
	_pclose(consola);

	// Si la IP A está en el adaptador
	if (A) printf("La IP %s ya está configurada en el adaptador %s.\n", ipA, adaptador);
	// Si la IP B está en el adaptador
	if (B) printf("La IP %s ya está configurada en el adaptador %s.\n", ipB, adaptador);
	// Si ninguna de las IPs está en el adaptador
	if (!A && !B) printf("Ninguna de las IPs está configurada en el adaptador %s.\n", adaptador);

	// Devolver éxito
	return true;
}

// Función para actualizar las DNS de un adaptador de red
bool modificarDNS(char * adaptador, char * ipA, char * ipB, bool * A, bool * B) {
	// Declaración de variables
	char comando[1024];
	FILE* consola = NULL;
	char buffer[1024];
	int tamBuffer = 1024;

	// Informamos de la actualización de las DNS
	printf("Actualizando DNS ...\n");
	// Comando para establecer la primera DNS
	if (!A) {
		sprintf(comando, "netsh interface ip set dns \"%s\" static %s", adaptador, ipA);

		// Ejecutar el comando
		consola = _popen(comando, "r");
		if (consola == NULL) {
			printf("Error al ejecutar el comando netsh.\n");
			return false;
		}

		// Cerrar la conexión con el comando netsh
		_pclose(consola);

		// Mostrar un mensaje de éxito
		printf("DNS primaria actualizada con éxito.\n");
	}

	if (!B) {
		// Comando para establecer la segunda DNS
		sprintf(comando, "netsh interface ip add dns \"%s\" %s index=2", adaptador, ipB);

		// Ejecutar el comando
		consola = _popen(comando, "r");
		if (consola == NULL) {
			printf("Error al ejecutar el comando netsh.\n");
			return false;
		}

		// Cerrar la conexión con el comando netsh
		_pclose(consola);

		// Mostrar un mensaje de éxito
		printf("DNS secundaria actualizada con éxito.\n");
	}

	// Mostrar un mensaje de éxito
	printf("DNS actualizadas con éxito.\n");

	// Devolver éxito
	return true;

}

bool verificarDNS(char* adaptador) {
	// Declaración de variables
	char comando[1024];
	FILE* verificarDNS = NULL;
	char buffer[1024];
	int tamBuffer = 1024;

	// Comando para mostrar las DNS del adaptador de red seleccionado
	sprintf(comando, "netsh interface ipv4 show dnsservers \"%s\"", adaptador, "DNS");

	// Ejecutar el comando
	verificarDNS = _popen(comando, "r");
	if (verificarDNS == NULL) {
		printf("Error al ejecutar el comando netsh.\n");
		return false;
	}

	// Mostrar un mensaje de éxito
	printf("Comando netsh ejecutado con éxito.\n\n");

	// Mostrar las DNS del adaptador de red actualizado
	printf("--- Adaptador con DNS actualizada ---\n");
	while (fgets(buffer, tamBuffer, verificarDNS) != NULL) {
		printf("%s", buffer);
	}
	printf("-----------------------------------\n\n");

	// Cerrar la conexión con el comando netsh
	_pclose(verificarDNS);

	// Devolver éxito
	return true;
}