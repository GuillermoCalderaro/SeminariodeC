
/*****************************************************************
 * TP integrador Seminario de Lenguajes - Opcion C
 * Alumno Calderaro Guillermo
 * Primera Entrega 
 * 24 de Junio del 2019
 * ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "sitio.h"
#include "my_getopt.h"
#include "municipio.h"




int main(int argc, char ** argv){
	
	int flag_m, error, numero, error_muni;				
	flag_m = numero = 0;
	
	FILE *in = NULL;
        FILE *out = NULL;

	error = my_getopt_func(&flag_m,&in, &out, argc, argv,&numero);

	if(!error){		

		if (!flag_m){
			proceso_sin_municipio(in, out);			
		}else{
			error_muni = proceso_con_municipio(in, out, numero);
			if (error_muni){
				fclose(in);
				fclose(out);
				fprintf(stderr, "ERROR 101: No hay suficientes elementos para generar la salida.\n");
				return error_muni;	
			}
		}
		fclose(in);
		fclose(out);
	}else {
		return error;	
	}
	return 0;
}







