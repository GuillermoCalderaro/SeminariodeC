#ifndef MY_GETOPT_H
#define MY_GETOPT_H
#include <stdio.h>


// las funciones a continuacion son para el parseo de argumentos
int es_ayuda(char*);

int es_leer(char*);

int es_salida(char*);

int es_municipio(char*);

int es_mayores(char*);

int es_arg(char);

int my_getopt_func(int*, FILE**, FILE**, int, char**, int*);

void imprimir_ayuda(void);

#endif
