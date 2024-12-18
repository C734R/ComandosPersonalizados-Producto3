// Colecci�n de funciones para actualizar DNS de adaptador

// Inclusi�n de las librer�as necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "dns.h"
#include "entrada.h"
#include "archivo.h"
#include "ping.h"

// Funci�n para pedir la ruta del archivo de DNS
void pedirArchivoDNS(char * rutaArchivoDNS) {
	// Declaraci�n de variables
	char buffer[1024];
	int tamBuffer = 1024;

	// Pedir la ruta del archivo de DNS
	printf("Introduce la ruta del archivo de DNS: ");
	entradaSinNL(buffer, tamBuffer);
	strcpy(rutaArchivoDNS, buffer);

}

// Funci�n para pedir ruta de fichero con IPs servidores DNSs o crear nuevo en ruta especificada e introducirlas
bool pedirCrearArchivoDNS(char * rutaArchivoDNS) {
	// Declaraci�n de variables
	FILE* archivoDNS = NULL;
	char entradaProcesada[100];
	char buffer[100] = { "" };
	int tamBuffer = sizeof(buffer);
	bool crear = false;

	// Pedir archivo existente o crear en ruta especificada
	while (1 && !crear) {
		// Pedir ruta del archivo de DNSs
		pedirArchivoDNS(rutaArchivoDNS);
		if (!existeArchivo(rutaArchivoDNS)) {
			printf("\nEl archivo no existe o es inaccesible.\n\n");
			printf("�Quieres volver a intentar introducir la ruta del fichero? (si = s�, otro = no) \n");
			entradaSinNL(buffer, tamBuffer);
			if (strcmp(buffer, "si") == 0) {
				continue;
			}
			printf("�Quieres crear el fichero %s e introducir las IP? (si = s�, otro = volver) \n", rutaArchivoDNS);
			entradaSinNL(buffer, tamBuffer);
			if (strcmp(buffer, "si") == 0) {
				crear = true;
				continue;
			}
			printf("Volviendo...\n\n");
			return false;
		}
		return true;
	}

	// Crear el fichero en la ruta definida
	if (!abrirArchivo(rutaArchivoDNS, "wt+", &archivoDNS)) {
		printf("Error al crear el archivo en la ruta : %s\n", rutaArchivoDNS);
		return false;
	}

	printf("Archivo creado con �xito.\n\n");

	// Solicitar IPs a introducir en el fichero
	while (1) {
		printf("Introduce la IP a testear (o 'fin' para terminar): ");
		entradaSinNL(buffer, tamBuffer);
		strcpy(entradaProcesada, buffer);

		// Si el usuario introduce 'fin', salir del bucle
		if (strcmp(entradaProcesada, "fin") == 0) break;

		// Si la IP no es v�lida, mostrar un mensaje de error y continuar con el bucle
		if (!validarIP(entradaProcesada)) {
			printf("IP no v�lida. Introduce una IP v�lida.\n\n");
			continue;
		}

		// Escribir la IP en el archivo
		fprintf(archivoDNS, "%s\n", entradaProcesada);
	}

	// Cerrar fichero DNSs
	fclose(archivoDNS);

	// Mostrar mensaje de �xito
	printf("IPs introducidas en el archivo.\n\n");
	return true;
}

// Funci�n para mostrar el contenido de un archivo de DNS
bool mostrarArchivoDNS(char * rutaDNS) {
	// Declaraci�n de variables
	char buffer[1024];
	int tamBuffer = 1024;
	FILE* archivoDNS = NULL;

	// Intentar abrir el archivo y mostrar un mensaje de �xito o error
	if(!abrirArchivo(rutaDNS, "r", &archivoDNS)){
		printf("Error al abrir el archivo.\n\n");
		return false;
	}
	printf("Archivo abierto con �xito.\n\n");

	// Mostrar el contenido del archivo de DNS
	printf("--- Contenido del archivo de DNS ---\n");
	while (fgets(buffer, tamBuffer, archivoDNS) != NULL) {
		printf("%s", buffer);
	}
	printf("-----------------------------------\n\n");

	// Cerrar el archivo
	fclose(archivoDNS);

	// Devolver �xito
	return true;
}

// Funci�n para mostrar el DNS de un adaptador y registrar los resultados en un fichero
bool mostrarDNSAdaptador(char* adaptador, char * rutaAdaptador) {
	// Declaraci�n de variables
	char buffer[1024];
	int tamBuffer = 1024;
	char comando[1024];
	FILE* consola = NULL;
	FILE* adaptadorDNS = NULL;
	char ip[16];
	bool ipOK = false;
	bool cabecera = false;

	// Variables para el m�todo num�rico
	char* puntero = NULL;
	char* inicio = NULL;
	int puntos = 0;
	__int64 longitudIP = 0;

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

	// Mostrar un mensaje de �xito
	printf("Comando netsh ejecutado con �xito.\n\n");
	
	// Abrir el archivo para guardar las DNS del adaptador
	if (!abrirArchivo(rutaAdaptador, "w+", &adaptadorDNS)) {
		printf("Error al abrir el archivo %s.\n", rutaAdaptador);
		return false;
	}


	// Recorrer las l�neas de la salida del comando ipconfig
	while (fgets(buffer, tamBuffer, consola) != NULL) {
		// Verificar contenido buffer
		//printf("%s", buffer);

		// Si la configuraci�n es DHCP, omitir
		if (strstr(buffer, "DHCP") != NULL) {
			printf("Configuraci�n DHCP. Omitiendo recogida de DNS del adaptador.\n\n");
			return true;
		}


		// Mostrar las DNS del adaptador de red seleccionado
		
		// -- M�todo avances
		//// Guardar las DNS en el archivo adaptadorDNS.txt
		//if (strstr(buffer, "Servidores DNS configurados est") != NULL) {
		//	//printf("%s", buffer + 50);
		//	fprintf(adaptadorDNS, "%s", buffer + 50);
		//}
				
		// -- M�todo num�rico
		// Definimos un puntero con la posici�n de inicio del buffer
		puntero = buffer;
		puntos = 0;
		// Recorrer el buffer hasta el final
		while (*puntero != '\0') {
			// Si el puntero apunta a un n�mero
			if (*puntero >= '0' && *puntero <= '9') {
				// Guardamos la direcci�n de inicio del n�mero
				inicio = puntero;

				// Recorrer el resto del buffer y contar puntos
				while (*puntero != '\0' && (isdigit(*puntero) || *puntero == '.')) {
					if (*puntero == '.') puntos++;
					puntero++;
				}

				// Si encontramos 3 puntos puede ser una IP. Comprobamos longitud y registramos
				if (puntos == 3) {
					longitudIP = puntero - inicio;
					if (longitudIP < 16) {
						strncpy(ip, inicio, longitudIP);
						ip[longitudIP] = '\0';
						if (!cabecera) {
							printf("--- DNS del adaptador de red ---\n");
							cabecera = true;
						}
						printf("Encontrada IP: %s\n", ip);
						fprintf(adaptadorDNS, "%s\n", ip);
						ipOK = true;
					}
				}
			}
			// Si no es un n�mero, avanzamos el puntero
			puntero++;
		}
	}
	printf("-----------------------------------\n\n");

	if (!ipOK) printf("No se encontraron IPs v�lidas.\n");

	// Cerrar la conexi�n con el comando ipconfig
	_pclose(consola);

	// Cerrar el archivo adaptadorDNS.txt
	fclose(adaptadorDNS);

	// Devolver �xito
	return true;
}

// Funci�n para vaciar los archivos temporales de DNS
bool vaciarArchivosDNS(char* rutaAdaptador, char* rutaVelocidadFichero, char* rutaVelocidadAdaptador, char* rutaResultado, char* rutaResultadoCombinado) {
	if (!vaciarArchivo(rutaAdaptador)) return false;
	if (!vaciarArchivo(rutaVelocidadFichero)) return false;
	if (!vaciarArchivo(rutaVelocidadAdaptador)) return false;
	if (!vaciarArchivo(rutaResultado)) return false;
	if (!vaciarArchivo(rutaResultadoCombinado)) return false;
	return true;
}

// Funci�n para comprobar la velocidad de direcciones DNS de un fichero y registrar los resultados 
bool checkVelocidadDNS(char * rutaDNS, char * rutaResultado) {
	// Declaraci�n de variables
	FILE* archivoDNS = NULL;
	FILE* archivoResultado = NULL;
	char buffer[1024];
	int tamBuffer = 1024;
	char comando[1024];
	FILE* pingDNS = NULL;
	int contador = 0;
	char ip[16] = "";
	int numIPs = 0;
	char ipPositivas[10][16] = { "" };
	char tiempo[10][16] = { "" };
	char* coincidencia_ip = "";
	char* coincidencia_tiempo = "";
	char busqueda[1024];

	// Abrir el archivo de DNS
	if (!abrirArchivo(rutaDNS, "r", &archivoDNS)) {
		printf("Error al abrir el archivo.\n");
		return false;
	}
	printf("Archivo abierto con �xito.\n\n");


	// Informamos de la comprobaci�n de las DNS
	printf("--- Testeando velocidad DNSs ---\n");
	while (fscanf(archivoDNS, "%15s", ip) != EOF) {

		printf("Testeando IP: %s ...\n", ip);

		// Comando para ejecutar el ping a la IP del DNS 4 veces con carga de 1000 bytes
		sprintf(comando, "ping -n 4 -l 1000 %s", ip);
		FILE* pingDNS = _popen(comando, "r");
		if (pingDNS == NULL) {
			printf("Error al ejecutar el comando ping.\n");
			return false;
		}
		bool respuesta = false;

		// Leer la salida del comando ping
		while (fgets(buffer, tamBuffer, pingDNS) != NULL) {

			// Componer el texto a buscar
			sprintf(busqueda, "Respuesta desde %s", ip);

			// Buscar la coincidencia de la IP en la respuesta del ping, guardar IP y registrar que ha habido respuesta
			coincidencia_ip = strstr(buffer, busqueda);
			if (coincidencia_ip != NULL && respuesta == false) {
				strcpy(ipPositivas[numIPs], ip);
				respuesta = true;
			}

			// Buscar la coincidencia del tiempo de respuesta en la respuesta del ping y registrar eliminando "ms"
			coincidencia_tiempo = strstr(buffer, "Media = ");
			if (coincidencia_tiempo != NULL) {
				strcpy(tiempo[numIPs], coincidencia_tiempo + 8);
				strcpy(tiempo[numIPs], strtok(tiempo[numIPs], "ms"));
				numIPs++;
			}

		}

		// Cerrar la conexi�n con el comando ping
		_pclose(pingDNS);

	}
	printf("-----------------------------------\n\n");

	// Cerrar archivo
	fclose(archivoDNS);

	// Abrir el archivo de resultados
	if (!abrirArchivo(rutaResultado, "w+", &archivoResultado)) {
		printf("Error al abrir el archivo de resultados.\n");
		return false;
	}

	// Mostrar y registrar resultados
	printf("\n--- Resumen de resultados ---\n");
	if (numIPs == 0) {
		printf("No hubo respuestas positivas.\n");
		printf("-----------------------------------\n\n");
		return true;
	}
	printf("IPs que respondieron al ping:\n");
	for (int i = 0; i < numIPs; i++) {
		printf("Tiempo de respuesta para IP %s: %s ms\n", ipPositivas[i], tiempo[i]);
		fprintf(archivoResultado, "%s %s\n", ipPositivas[i], tiempo[i]);
	}
	printf("-----------------------------------\n\n");

	// Cerrar el archivo
	fclose(archivoResultado);

	pausaEnter();
			
	// Devolver �xito
	return true;
}


// Funci�n para comprobar los saltos de las DNS
bool checkSaltosDNS(char* rutaDNS, char* rutaResultado) {
	// Declaraci�n de variables
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

	// Informamos de la comprobaci�n de las DNS
	printf("--- Testeando saltos DNSs ---\n");
	while (fscanf(archivoDNS, "%15s", ip) != EOF) {

		printf("Testeando IP: %s ...\n", ip);
		
		// Comando para ejecutar el tracert a la IP del DNS sin resoluci�n de nombres, con limitaci�n a 30 saltos y tiempo de espera de 500 ms
		sprintf(comando, "tracert -d -h 30 -w 500 %s", ip);

		// Ejecutar el comando
		FILE* consola = _popen(comando, "r");
		if (consola == NULL) {
			printf("Error al ejecutar el comando tracert.\n");
			return false;
		}

		// Leer la salida del comando tracert
		while (fgets(buffer, tamBuffer, consola) != NULL) {
			// Si la l�nea comienza con un n�mero, registrar ip y saltos
			if (isdigit(buffer[0])) {
				strcpy(ipPositivas[numIPs], ip);
				saltos[numIPs]++;
			}
		}

		// Mostrar el n�mero de saltos para la IP
		printf("N�mero de saltos para IP %s: %i\n", ip, saltos[numIPs]);

		// Cerrar la conexi�n con el comando tracert
		_pclose(consola);
	}
	printf("-----------------------------------\n\n");

	// Cerrar archivo
	fclose(archivoDNS);

	// Abrir el archivo de resultados
	if (!abrirArchivo(rutaResultado, "w+", &archivoResultado)) {
		printf("Error al abrir el archivo de resultados.\n");
		return false;
	}
	
	// Insertar ip y saltos en el archivo de resultados
	for (int i = 0; i < numIPs; i++) {
		fprintf(archivoResultado, "%s %i\n", ipPositivas[i], saltos[i]);
	}

	// Cerrar el archivo
	fclose(archivoResultado);

	// Devolver �xito
	return true; 
}

// Funci�n para combinar los resultados DNS de dos ficheros temporales en uno solo
bool combinarFicheros(char* rutaA, char* rutaB, char * rutaC) {

	// Declaraci�n de variables
	FILE* archivoA = NULL;
	FILE* archivoB = NULL;
	FILE* archivoC = NULL;
	char buffer[1024];
	int tamBuffer = 1024;
	char bufferA[1024];
	bool encontrado = false;

	// Abrir archivo A, origen de la combinaci�n
	if (!abrirArchivo(rutaA, "r", &archivoA)) {
		printf("Error al abrir el archivo %s.\n", rutaA);
		return false;
	}

	// Abrir archivo B, origen de la combinaci�n
	if (!abrirArchivo(rutaB, "r", &archivoB)) {
		printf("Error al abrir el archivo %s.\n", rutaB);
		return false;
	}

	// Abrir archivo C, destino de la combinaci�n
	if (!abrirArchivo(rutaC, "w+", &archivoC)) {
		printf("Error al abrir el archivo %s.\n", rutaC);
		return false;
	}

	// Recorrer el archivo A y copiar su contenido en el archivo C
	while (fgets(buffer, tamBuffer, archivoA) != NULL) {
		fprintf(archivoC, "%s", buffer);
	}

	// Recorrer el archivo B y copiar su contenido en el archivo C
	while (fgets(buffer, tamBuffer, archivoB) != NULL) {
		
		// Comprobar si la IP ya est� en el archivo
		rewind(archivoA);
		encontrado = false;
		while (fgets(bufferA, tamBuffer, archivoA) != NULL) {
			if (strstr(buffer, bufferA) != NULL) {
				encontrado = true;
				break;
			}
		}
		// Si no est� en el archivo, copiar la IP en el archivo C
		if (!encontrado) fprintf(archivoC, "%s", buffer);
	}

	// Cerrar los archivos
	fclose(archivoA);
	fclose(archivoB);
	fclose(archivoC);

	// Devolver �xito
	return true;
}


// Funci�n para comparar y elegir la DNS m�s r�pida
bool compararDNS(char * rutaResultados, char * ipRapidaA, char * ipRapidaB, bool * empate) {
	// Declaraci�n de variables
	FILE* archivoResultados = NULL;
	int tamBuffer = 1024;
	char ip[16] = { "" };
	char tiempo[16] = { "" };
	char tiempoRapidoA[10] = { "9999" };
	char tiempoRapidoB[10] = { "9999" };
	*empate = false;

	// Abrir los archivos de resultados de resultados
	if (!abrirArchivo(rutaResultados, "r", &archivoResultados)) {
		printf("Error al abrir el archivo %s.\n", rutaResultados);
		return false;
	}

	// Leer los resultados de velocidad
	while (fscanf(archivoResultados, "%16s %10s", ip, tiempo) != EOF) {
		
		// Si el tiempo es menor que el tiempo m�s r�pido, actualizar la IP m�s r�pida y el tiempo m�s r�pido
		if (atoi(tiempo) < atoi(tiempoRapidoA)) {
			strcpy(ipRapidaB, ipRapidaA);
			strcpy(ipRapidaA, ip);
			strcpy(tiempoRapidoB, tiempoRapidoA);
			strcpy(tiempoRapidoA, tiempo);
		}
		else if (atoi(tiempo) < atoi(tiempoRapidoB)) {
			strcpy(ipRapidaB, ip);
			strcpy(tiempoRapidoB, tiempo);
		}
	}

	// Si no se han encontrado IPs con respuesta al ping
	if (strcmp(tiempoRapidoA, "9999") == 0) {
		printf("No se han encontrado IPs con respuesta al ping.\n\n");
		return false;
	}

	// Si las velocidades son diferentes
	if (strcmp(tiempoRapidoA,tiempoRapidoB) != 0) {
		// Mostrar la DNS m�s r�pida
		printf("Las DNS m�s r�pidas son %s con un tiempo de respuesta de %s ms y %s con un tiempo de respuesta de %s ms.\n\n", ipRapidaA, tiempoRapidoA, ipRapidaB, tiempoRapidoB);
		// Cerrar el archivo
		fclose(archivoResultados);
		*empate = false;
		// Devolver �xito
		return true;
	}

	// Si no lo son, mostrar las IPs m�s r�pidas
	printf("Hay dos IPs con el mismo tiempo de respuesta m�s r�pido: %s y %s con %s ms.\n\n", ipRapidaA, ipRapidaB, tiempoRapidoA);
	*empate = true;

	// Cerrar el archivo
	fclose(archivoResultados);

	// Devolver �xito
	return true;
}

// Funci�n para comprobar los saltos de las DNS en disputa
char * saltosDNS (char * ipRapidaA, char * ipRapidaB, bool * empate) {
	// Declaraci�n de variables
	FILE* consola = NULL;
	char comando[1024];
	char buffer[1024];
	int tamBuffer = 1024;
	int saltosA = 0;
	int saltosB = 0;
	*empate = false;
	bool espacio = false;
	bool digito = false;

	// Informamos de la comprobaci�n de las DNS
	printf("--- Testeando saltos DNSs ---\n");

	printf("Testeando IP: %s ...\n", ipRapidaA);
	// Comando para ejecutar el tracert a la IP del DNS sin resoluci�n de nombres, con limitaci�n a 30 saltos y tiempo de espera de 500 ms
	sprintf(comando, "tracert -d -h 30 -w 500 %s", ipRapidaA);

	// Ejecutar el comando
	consola = _popen(comando, "r");
	if (consola == NULL) {
		printf("Error al ejecutar el comando tracert.\n");
		return NULL;
	}

	// Leer la salida del comando tracert
	while (fgets(buffer, tamBuffer, consola) != NULL) {
		// Mostrar respuesta
		printf("%s",buffer);

		// Buscamos un espacio en el inicio de la l�nea, lo que indica un salto
		if (buffer[0] == ' ' && espacio == false) {
			espacio = true;
		}

		// Buscamos un d�gito en la l�nea para registrar el salto
		if (espacio == true && buffer[2] >= '0' && buffer[2] <= '9' && digito == false) {
			saltosA++;
			digito = true;
		}

		// Una vez procesada la l�nea, reiniciamos las variables
		espacio = false;
		digito = false;
	}

	// Mostrar el n�mero de saltos para la IP
	printf("N�mero de saltos para IP %s: %i\n", ipRapidaA, saltosA);

	// Cerrar la conexi�n con el comando tracert
	_pclose(consola);

	printf("Testeando IP: %s ...\n", ipRapidaB);

	// Comando para ejecutar el tracert a la IP del DNS sin resoluci�n de nombres, con limitaci�n a 30 saltos y tiempo de espera de 500 ms
	sprintf(comando, "tracert -d -h 30 -w 500 %s", ipRapidaB);

	// Ejecutar el comando
	consola = _popen(comando, "r");
	if (consola == NULL) {
		printf("Error al ejecutar el comando tracert.\n");
		return NULL;
	}

	// Leer la salida del comando tracert
	while (fgets(buffer, tamBuffer, consola) != NULL) {
		// Comprobar respuesta
		printf("%s", buffer);

		// Buscamos un espacio en el inicio de la l�nea, lo que indica un salto
		if (buffer[0] == ' ' && espacio == false) {
			espacio = true;
		}

		// Buscamos un d�gito en la l�nea para registrar el salto
		if (espacio == true && buffer[2] >= '0' && buffer[2] <= '9' && digito == false) {
			saltosB++;
			digito = true;
		}

		// Una vez procesada la l�nea, reiniciamos las variables
		espacio = false;
		digito = false;
	}

	// Mostrar el n�mero de saltos para la IP
	printf("N�mero de saltos para IP %s: %i\n", ipRapidaB, saltosB);

	// Cerrar la conexi�n con el comando tracert
	_pclose(consola);

	// Comparar los saltos de las DNS y seleccionar seg�n resultados
	// Si las DNS tienen el mismo n�mero de saltos
	if (saltosA == saltosB) {
		printf("Las IPs tienen el mismo n�mero de saltos. Por defecto se selecciona la primera IP, %s, como la primaria.\n\n", ipRapidaA);
		*empate = true;
		return ipRapidaA;
	}
	// Si la IP A tiene menos saltos que la IP B
	else if (saltosA < saltosB) {
		printf("La IP %s tiene menos saltos que la IP %s. Se selecciona la IP %s como primaria.\n\n", ipRapidaA, ipRapidaB, ipRapidaA);
		*empate = false;
		return ipRapidaA;
	}
	// Si la IP B tiene menos saltos que la IP A
	else {
		printf("La IP %s tiene menos saltos que la IP %s. Se selecciona la IP %s como primaria.\n\n", ipRapidaB, ipRapidaA, ipRapidaB);
		*empate = false;
		return ipRapidaB;
	}
}

// Comparar IPs con la del adaptador
bool compararAdaptador(char * adaptador, char * ipA, char * ipB, bool * A, bool * B, bool * orden, char * rutaAdaptador) {
	// Declaraci�n de variables
	char buffer[1024];
	int tamBuffer = 1024;
	bool lineas = false;
	FILE* ficheroAdaptador = NULL;
	*A = false;
	*B = false;
	*orden = false;

	// Abrir fichero de DNSs del adaptador
	if (!abrirArchivo(rutaAdaptador, "r", &ficheroAdaptador)) {
		printf("Error al abrir el archivo %s.\n", rutaAdaptador);
		return false;
	}

	// Recorrer las l�neas del fichero
	while (fgets(buffer, tamBuffer, ficheroAdaptador) != NULL) {
		
		if (strstr(buffer, ipA) != NULL) {
			*A = true;
			if (!*B) *orden = true;
		}
		else if (strstr(buffer, ipB) != NULL) {
			*B = true;
			if (!*A) *orden = false;
		}
		lineas = true;
	}

	if (!lineas) return true;

	// Cerrar el fichero de DNSs del adaptador
	fclose(ficheroAdaptador);

	// Si ninguna de las IPs est�n en el adaptador
	if (!*A && !*B) printf("Ninguna de las IPs est� configurada en el adaptador %s.\n", adaptador);
	// Si ambas IPs est�n en el adaptador 
	else if (*A && *B) {
		// Y en orden correcto de prioridad
		if (orden) printf("Las dos IPs m�s r�pidas, %s y %s, ya est�n configuradas en el adaptador %s en orden correcto.\n", ipA, ipB, adaptador);
		// Pero en orden incorrecto de prioridad
		else printf("Las dos IPs m�s r�pidas, %s y %s, ya est�n configuradas en el adaptador %s, pero en orden incorrecto.\n", ipA, ipB, adaptador);
	}
	// Si la IP A est� en el adaptador
	else if (*A) printf("La IP %s ya est� configurada en el adaptador %s.\n", ipA, adaptador);
	// Si la IP B est� en el adaptador
	else if (*B) printf("La IP %s ya est� configurada en el adaptador %s.\n", ipB, adaptador);

	printf("\n");

	// Devolver �xito
	return true;
}

// Funci�n para actualizar las DNS de un adaptador de red
bool modificarDNS(char* adaptador, char* ipA, char* ipB, bool* A, bool* B, bool* orden) {
	// Declaraci�n de variables
	char comando[1024];
	FILE* consola = NULL;
	int tamBuffer = 1024;
	char ipPrimaria[16] = { "" };
	char ipSecundaria[16] = { "" };

	// Si la IP A y B est�n en el adaptador correctamente ordenadas, no se modifican
	if (*A && *B && *orden) {
		printf("Ambas IP ya est�n configuradas correctamente en el adaptador %s, por lo que no se modificar�.\n", adaptador);
	}
	// Si la IP A y B est�n en el adaptador correctamente ordenadas, no se modifican
	else if (*A && *B && !*orden){
		printf("Ambas IP ya est�n configuradas en el adaptador %s, pero en orden de prioridad incorrecto, por lo que se modificar� su orden.\n", adaptador);
		strcpy(ipPrimaria, ipB);
		strcpy(ipSecundaria, ipA);
	}
	// Si la IP A est� en el adaptador, se modifica la B
	else if (*A || *B) {
		if (*A && !*B) printf("La IP %s ya est� configurada en el adaptador %s. Se a�adir� la IP %s como secundaria.\n", ipA, adaptador, ipB);
		else if (*B && !*A) printf("La segunda IP m�s r�pida %s est� configurada en el adaptador %s.\n Se configurar� como secundaria y se a�adir� la m�s r�pida, %s, como primaria.\n", ipB, adaptador, ipA);
		strcpy(ipPrimaria, ipA);
		strcpy(ipSecundaria, ipB);
	}

	printf("\n");

	// Informamos de la actualizaci�n de las DNS
	if ((!*A || !*B) || (*A && *B && !orden)) {

		printf("Actualizando DNS ...\n\n");

		// Comando para borrar DNSs adaptador mediante la activaci�n de DHCP
		sprintf(comando, "netsh interface ip set dns \"%s\" dhcp", adaptador);

		// Ejecutar el comando
		consola = _popen(comando, "r");
		if (consola == NULL) {
			printf("Error al ejecutar el comando netsh.\n");
			return false;
		}

		// Cerrar la conexi�n con el comando netsh
		_pclose(consola);

		// Comando para establecer de nuevo configuracion
		sprintf(comando, "netsh interface ip set dns \"%s\" static none", adaptador);

		// Ejecutar el comando
		consola = _popen(comando, "r");
		if (consola == NULL) {
			printf("Error al ejecutar el comando netsh.\n");
			return false;
		}

		// Cerrar la conexi�n con el comando netsh
		_pclose(consola);

		// Comando para establecer la primera DNS
		sprintf(comando, "netsh interface ip add dns \"%s\" %s index=1", adaptador, ipA);

		// Ejecutar el comando
		consola = _popen(comando, "r");
		if (consola == NULL) {
			printf("Error al ejecutar el comando netsh.\n");
			return false;
		}

		// Cerrar la conexi�n con el comando netsh
		_pclose(consola);

		// Mostrar un mensaje de �xito
		printf("DNS primaria actualizada con �xito.\n\n");

		// Comando para establecer la segunda DNS
		sprintf(comando, "netsh interface ip add dns \"%s\" %s index=2", adaptador, ipB);

		// Ejecutar el comando
		consola = _popen(comando, "r");
		if (consola == NULL) {
			printf("Error al ejecutar el comando netsh.\n");
			return false;
		}

		// Cerrar la conexi�n con el comando netsh
		_pclose(consola);

		// Mostrar un mensaje de �xito
		printf("DNS secundaria actualizada con �xito.\n\n");
	}

	// Devolver �xito
	return true;
}

// Funci�n para actualizar las DNS de un adaptador de red
bool establecerDNS(char* adaptador, char* ipA, char* ipB) {
	// Declaraci�n de variables
	char comando[1024];
	FILE* consola = NULL;
	int tamBuffer = 1024;
	char ipPrimaria[16] = { "" };
	char ipSecundaria[16] = { "" };

	printf("Aplicando DNSs ...\n\n");

	// Comando para borrar DNSs adaptador mediante la activaci�n de DHCP
	if (!adaptadorDHCP(adaptador)) {
		printf("Error al establecer la configuraci�n del adaptador en modo DHCP. Volviendo...\n\n");
		return false;
	}

	// Comando para establecer de nuevo configuracion
	sprintf(comando, "netsh interface ip set dns \"%s\" static none", adaptador);

	// Ejecutar el comando
	consola = _popen(comando, "r");
	if (consola == NULL) {
		printf("Error al ejecutar el comando netsh.\n");
		return false;
	}

	// Cerrar la conexi�n con el comando netsh
	_pclose(consola);

	// Comando para establecer la primera DNS
	sprintf(comando, "netsh interface ip add dns \"%s\" %s index=1", adaptador, ipA);

	// Ejecutar el comando
	consola = _popen(comando, "r");
	if (consola == NULL) {
		printf("Error al ejecutar el comando netsh.\n");
		return false;
	}

	// Cerrar la conexi�n con el comando netsh
	_pclose(consola);

	// Mostrar un mensaje de �xito
	printf("DNS primaria actualizada con �xito.\n\n");

	// Comando para establecer la segunda DNS
	sprintf(comando, "netsh interface ip add dns \"%s\" %s index=2", adaptador, ipB);

	// Ejecutar el comando
	consola = _popen(comando, "r");
	if (consola == NULL) {
		printf("Error al ejecutar el comando netsh.\n");
		return false;
	}

	// Cerrar la conexi�n con el comando netsh
	_pclose(consola);

	// Mostrar un mensaje de �xito
	printf("DNS secundaria actualizada con �xito.\n\n");
	

	// Devolver �xito
	return true;
}

// Funci�n para mostrar y poder verificar la configuraci�n DNS aplicada
bool verificarDNS(char* adaptador) {
	// Declaraci�n de variables
	char comando[1024];
	FILE* verificarDNS = NULL;
	char buffer[1024];
	int tamBuffer = 1024;

	// Comando para mostrar las DNS del adaptador de red seleccionado
	sprintf(comando, "netsh interface ipv4 show dnsservers \"%s\"", adaptador);

	// Ejecutar el comando
	verificarDNS = _popen(comando, "r");
	if (verificarDNS == NULL) {
		printf("Error al ejecutar el comando netsh.\n");
		return false;
	}

	// Mostrar un mensaje de �xito
	printf("Comando netsh ejecutado con �xito.\n\n");

	// Mostrar las DNS del adaptador de red actualizado
	printf("--- Adaptador con DNS actualizada ---\n");
	while (fgets(buffer, tamBuffer, verificarDNS) != NULL) {
		printf("%s", buffer);
	}
	printf("-----------------------------------\n\n");

	// Cerrar la conexi�n con el comando netsh
	_pclose(verificarDNS);

	// Devolver �xito
	return true;
}

// Funci�n para establecer modo DHCP en adaptador seleccionado
bool adaptadorDHCP(char * adaptador) {
	// Declaraci�n de variables
	FILE* consola = NULL;
	char comando[1024];

	// Comando para establecer la activaci�n de DHCP
	sprintf(comando, "netsh interface ip set dns \"%s\" dhcp", adaptador);

	// Ejecutar el comando
	consola = _popen(comando, "r");
	if (consola == NULL) {
		printf("Error al ejecutar el comando netsh.\n");
		return false;
	}

	printf("Comando netsh ejecutado con �xito.\n\n");

	// Cerrar la conexi�n con el comando netsh
	_pclose(consola);

	// Devolver �xito
	return true;
}

