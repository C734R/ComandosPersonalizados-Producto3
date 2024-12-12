#ifndef ACTUALIZARDNS_H     // Si ACTUALIZARDNS_H no ha sido definido
#define ACTUALIZARDNS_H     // Definimos

// Añadimos librerías necesarias
#include <stdbool.h>

bool actualizarAutoDNS();
bool actualizarManualDNS();
bool actualizarDHCPDNS();

#endif      // Fin definición ACTUALIZARDNS_H