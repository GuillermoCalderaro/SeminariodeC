#include "municipio.h"
#include "sitio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


/***********************************************************
esta funcion recibe el tipo de basural y segun el mismo asigna los strings correspondientes a 
las variables que recibe por referencia
  ******************************************************/
void mapea_tipo_basural(tipologia_t num, char ** tipo, char** marcador){
	switch (num) {
		
				case 0: *tipo = strcpy(*tipo,"punto de arrojo");
					*marcador = strcpy(*marcador,"tan-dot");
					break;
				case 1: *tipo = strcpy(*tipo,"microbasural");
				        *marcador = strcpy(*marcador,"yellow-dot");
					break;
				case 2: *tipo = strcpy(*tipo,"basural");
					*marcador = strcpy(*marcador,"orange-dot");
					break;
				case 3: *tipo = strcpy(*tipo,"macrobasural");	
					*marcador = strcpy(*marcador,"default-dot");
					break;
	}
}


/****************************************************
esta funcion realiza el punto 1 del TP
recibe dos FILE* ya abiertos. No verifica su apertura ni realiza su cierre.Ambas se realizan en el programa principal
En caso de detectar un error al tratar de convertir el contenido de una linea 
a un struct (usando la funcion proporcionada) imprime el mismo por salida standar. La misma puede ser redireccionada.
En la guia de TP nunca se especifica si es necesario discriminar entre los tipos de errores. Esto es posible debido a ser el tipo de retorno de tipo enum.
Aqui no se hizo por simplicidad.
La funcion podria modularizarse mas  <por ejemplo se podria realizar un procedimiento leer_linea y otro procesar_linea>. 


  *****************************************************/
void proceso_sin_municipio(FILE* in, FILE* out){
	sitio_t sitio;
	char *mensaje_error = malloc(20);
	int cant = 5 , cont = 0;
	char* buffer;
	char *tipo = malloc (20);
        char *marcador = malloc (20);
	error_t error;

	while ( ! feof(in)){		// mientras no se llegue al final del archivo
		leer_linea(&buffer, in, &cant);			//leo una linea
		error = linea_csv_a_sitio_t(buffer, &sitio);		//la convierto en un struct
		if ( (cont != 0) && (strlen(buffer) != 0 )  ) { 


			if ( !error) {			//se imprime el basural leido correctamente	
				mapea_tipo_basural(sitio.sitios_tipologia, &tipo, &marcador);  //determino que tipologia tiene
				fprintf(out,"%10s,%10s {%s:%s} <%s>\n", sitio.sitios_latitud ,sitio.sitios_longitud, tipo, sitio.sitios_denominacion,marcador);
			} else {	//aqui se captura el tipo de error para mostrarlo por salida standar
				if (error == 1){
					mensaje_error = strcpy(mensaje_error,"Campo muy largo");
				}else if (error == 2){
					mensaje_error =  strcpy(mensaje_error,"Faltan campos");
				}else if (error == 3){
					mensaje_error =  strcpy(mensaje_error,"Campo no numerico");
				}else {
					mensaje_error =  strcpy(mensaje_error,"Tipologia no valida");
				}
				fprintf(stderr,"Se produjo un error al leer la linea %d. Tipo de error: %s\n",cont, mensaje_error);
			}

		}
		free(buffer);						
		cont ++;
	}
	free(tipo);
	free(marcador);
	free(mensaje_error);

}

/*********************************************************
Esta funcion recibe los punteros a los archivos a utilizar y procesa el archivo de entrada totalizando los basurales por cada municipio.
El resultado de dicho procesamiento es almacenado en un vector.
Recibe como parametro un valor entero (numero) el cual es el valor ingresado junto con el argumento -M|--mayores
Toda la amemoria alocada dinamicamente por la funcion es liberada antes de su finalizacion.
La funcion no abre ni cierra archivos. Los mismos deben abrirse/cerrarse antes de la invicacion. 
El valor de retorno de la funcion indica si el procesamiento se produjo con exito (0) o si hubo un error debido a cantidad de datos insusficientes para generar la salida esperada(ERROR 101).
************************************************************/

int proceso_con_municipio(FILE* in, FILE* out, int numero){

	error_t error;
	char* buffer = NULL;
	int i, dimL = 0, maximo = 0;
        int cant = 5;
	sitio_t sitio,*vector_recu, *vector = NULL;
	/* a continuacion se recorre el archivo totalizando los basuralos por municipio. El resultado de dicho proceso se guarda en un vector alocado dinamicamente*/
	while (!feof(in)){
		leer_linea(&buffer,in, &cant);
		error= linea_csv_a_sitio_t(buffer, &sitio);
		free(buffer);
		if (!error){
			i = 0;
			while ( (i < dimL)&&(sitio.municipios_id != vector[i].municipios_id)){
				i++;
			}
			sitio.sitios_tipologia++;
			if(i >= dimL){

				vector = realloc(vector, (dimL + 1) * sizeof(sitio_t));
				vector[dimL]=sitio;
				dimL++;
			}else{
				vector[i].sitios_tipologia += sitio.sitios_tipologia;
			}
		}
	}
	/*Se hubica el maximo*/
	for(int i = 0; i < dimL; i++){
		if (maximo <= vector[i].sitios_tipologia){
			maximo = vector[i].sitios_tipologia;
		}
	}

	maximo = maximo / 4;

	if (numero == 0){			//solo se ingreso -m sin -M
		//a continuacion se recorre la estructura generada en el while anterior y se imprime a el archivo de salida
		//no se verifican errores ya que los mismos fueron considerados en el procesamiento previo.
		char *grado_relativo;
		for (int i = 0; i < dimL; i++){
			grado_relativo = mapea_grado_relativo( vector[i].sitios_tipologia ,maximo);
			fprintf(out, "%10s, %10s Municipio: %s <%s>\n",vector[i].sitios_latitud, vector[i].sitios_longitud,vector[i].municipios_descripcion, grado_relativo);
			free(grado_relativo);
		}	
	}else 	{//se ingreso la opcion -M con un valor valido--resolucion de parte de recuperatorio	
		if (dimL<numero) {		//si no hay suficientes elementos para realizar la salida ->ERROR
			free(vector);
			return 101;
		}else {				//en caso contrario se comienza a procesar los datos
			qsort(vector, dimL, sizeof(sitio_t), basura_acumulada);		//ordeno de manera decreciente el vector
			vector_recu = malloc(numero * sizeof(sitio_t));
			int i = 0, ok = 1;
			/*recorro el vector selleccionando los municipios que cumplan el criterio y los almaceno en otra estructura*/
			while ((i < numero) && (ok)){
				if(vector[i].sitios_tipologia > numero){
					vector_recu[i] = vector[i];
				}else {
					ok = 0;
				}
				i++;
			}
			if (!ok){		//si no hay N elementos mayores a N -> ERROR
				free(vector_recu);
				free(vector);
				return 101; 
			}else {			//se pudo hacer el procesamiento adecuadamente
				fprintf(out, "Se listan los primeros %d municipios cuya suma de basurales es mayor a %d\n",numero, numero);
				char *grado_relativo;
				for (int j = 0; j < i; j++){
					grado_relativo = mapea_grado_relativo( vector_recu[j].sitios_tipologia ,maximo);

					fprintf(out, "%10s, %10s Municipio: %s <%s>\n",vector_recu[j].sitios_latitud, vector_recu[j].sitios_longitud,vector_recu[j].municipios_descripcion, grado_relativo);

				free(grado_relativo);
				}	
				free(vector_recu);
			}	
		}
	}
	free(vector);
	return 0;
}
/******************************************************
 *Este funcion es la que usa qsort para realizar el ordenamientro decreciente del vector de basurales
 * *****************************************************/

int basura_acumulada(const void* a, const void* b){
	int ai = ((sitio_t*)a)->sitios_tipologia;
	int bi = ((sitio_t*)b)->sitios_tipologia;
	return bi - ai;
}




/******************************************
Esta funcion lee una linea del archivo. Aloca memoria dinamicamente para la concatenacion de los distintos caracteres de la linea
no libera la memoria reservada. La misma debe realizarce luego de la llamada a la funcion.
*******************************************/
void leer_linea(char** buffer, FILE* input,int *valor){
	*buffer = malloc(*valor);
	int c, cant = 0;
	while(((c=getc(input))!=EOF) && (c!='\n')){
		if(cant>=(*valor)){
			(*valor) *=2;
			*buffer = realloc(*buffer,(*valor));
		}
		(*buffer)[cant++]=(char)c;
	}
	*buffer = realloc(*buffer, ++(*valor));
	(*buffer)[cant]='\0';
}
/******************************************************************************
Esta funcon recibe el grado relativo, calculado con anterioridad, y el valor totalizado con los basurales de ese municipio. En funcion a ambos
asigna los strings necesarios para los colores del punto a marcar. 
mapea_grado_relativo no libera memoria. La liberacion de la misma deve ser realizada por la funcion que llama.
***********************************************************************/
char * mapea_grado_relativo(int valor, int grado_relativo){
	char* buffer = malloc(20);
	if(valor < grado_relativo) buffer = strcpy(buffer,"tan-dot");
	else if (valor < grado_relativo * 2) buffer =  strcpy(buffer,"yellow-dot");
	else if (valor < grado_relativo * 3) buffer =  strcpy(buffer,"orange-dot");
	else buffer =  strcpy(buffer,"default-dot");
	return buffer;
}
