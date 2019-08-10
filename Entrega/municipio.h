#ifndef MUNICIPIO_H
#define MUNICIPIO_H
#include "sitio.h"

void mapea_tipo_basural( tipologia_t, char**, char**);

void proceso_sin_municipio(FILE*, FILE*);

int proceso_con_municipio(FILE*, FILE*, int);

int basura_acumulada(const void*, const void*);

void leer_linea(char ** ,FILE*, int * );

void agregar_a_vector(sitio_t,sitio_t** , int*);	

char* mapea_grado_relativo(int, int);



#endif
