#ifndef DNS_H     // Si DNS_H no ha sido definido
#define DNS_H     // Definimos

// Añadimos librerías necesarias
#include <stdbool.h>

void pedirArchivoDNS(char* rutaArchivoDNS);
bool pedirCrearArchivoDNS(char* rutaArchivoDNS);
bool mostrarArchivoDNS(char* rutaDNS);
bool mostrarDNSAdaptador(char* adaptador, char* rutaAdaptador);
bool vaciarArchivosDNS(char* rutaAdaptador, char* rutaVelocidadFichero, char* rutaVelocidadAdaptador, char* rutaResultado, char * rutaResultadoCombinado);
bool checkVelocidadDNS(char* rutaDNS, char* rutaResultado);
bool checkSaltosDNS(char* rutaDNS, char* rutaResultado);
bool combinarFicheros(char* rutaA, char* rutaB, char* rutaC);
bool compararDNS(char* rutaResultados, char* ipRapidaA, char* ipRapidaB, bool* empate);
char* saltosDNS(char* ipRapidaA, char* ipRapidaB, bool* empate);
bool compararAdaptador(char* adaptador, char* ipA, char* ipB, bool* A, bool* B, bool* orden, char* rutaAdaptador);
bool modificarDNS(char* adaptador, char* ipA, char* ipB, bool* A, bool* B, bool* orden);
bool verificarDNS(char* adaptador);
bool establecerDNS(char* adaptador, char* ipA, char* ipB);
bool adaptadorDHCP(char* adaptador);

#endif      // Fin definición DNS_H